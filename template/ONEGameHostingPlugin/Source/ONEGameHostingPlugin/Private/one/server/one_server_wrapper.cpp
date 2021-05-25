// Copyright i3D.net, 2021. All Rights Reserved.

#include <one/server/one_server_wrapper.h>

#include <one/arcus/c_api.h>
#include <one/arcus/c_error.h>
#include <one/server/one_parsing.h>

#include <assert.h>
#include <cstring>
#include <cstdlib>

#include <CoreMinimal.h>

namespace one_integration {
namespace {

// Logger to pass into One.
void log(void *userdata, OneLogLevel level, const char *message) {
    if (userdata == nullptr) {
        return;
    }

    auto wrapper = reinterpret_cast<OneServerWrapper *>(userdata);

    if (wrapper->quiet()) {
        return;
    }

    switch (level) {
        case ONE_LOG_LEVEL_INFO:
            UE_LOG(LogTemp, Log, TEXT("ONE ARCUS: %s"), *FString(message));
            break;
        case ONE_LOG_LEVEL_ERROR:
            UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(message));
            break;
        default:
            break;
    }
}

}  // namespace

OneServerWrapper::OneServerWrapper()
    : _server(nullptr)
    , _quiet(false)
    , _soft_stop_callback(nullptr)
    , _soft_stop_userdata(nullptr)
    , _allocated_callback(nullptr)
    , _allocated_userdata(nullptr)
    , _metadata_callback(nullptr)
    , _metadata_userdata(nullptr)
    , _host_information_callback(nullptr)
    , _host_information_userdata(nullptr)
    , _application_instance_information_callback(nullptr)
    , _application_instance_information_userdata(nullptr) {}

OneServerWrapper::~OneServerWrapper() {
    shutdown();
}

bool OneServerWrapper::init(unsigned int port) {
    const std::lock_guard<std::mutex> lock(_wrapper);

    if (_server != nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: already initialized"));
        return false;
    }

    //-----------------------
    // Create the one server.

    // Each game server must have one corresponding arcus server.
    OneError err = one_server_create(port, &_server);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return false;
    }

    // Set custom logger - optional.
    err = one_server_set_logger(_server, log, this);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return false;
    }

    //---------------
    // Set callbacks.

    // The default behavior of the platform will always hard stop your application, Its
    // required to configure a soft stop method for your application before this packet
    // will be send to your application.
    err = one_server_set_soft_stop_callback(_server, soft_stop, this);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return false;
    }

    err = one_server_set_allocated_callback(_server, allocated, this);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return false;
    }

    err = one_server_set_metadata_callback(_server, metadata, this);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return false;
    }

    err = one_server_set_host_information_callback(_server, host_information, this);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return false;
    }

    err = one_server_set_application_instance_information_callback(
        _server, application_instance_information, this);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("ONE ARCUS: initialization is complete"));
    return true;
}

void OneServerWrapper::shutdown() {
    const std::lock_guard<std::mutex> lock(_wrapper);

    if (_server == nullptr) {
        return;
    }

    // Free all objects created via the ONE API. This also shuts down the server
    // first, ending any active connection to it.
    one_server_destroy(_server);
    _server = nullptr;
}

void OneServerWrapper::update(bool quiet) {
    const std::lock_guard<std::mutex> lock(_wrapper);
    assert(_server != nullptr);

    _quiet = quiet;

    // Updates the server, which handles client connections, and services
    // outgoing and incoming messages.
    // Any registered callbacks will called during update, if the corresponding
    // messages are received.
    OneError err = one_server_update(_server);
    if (one_is_error(err)) {
        if (!_quiet)
            UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return;
    }
}

const char *OneServerWrapper::status_to_string(Status status) {
    switch (status) {
        case Status::uninitialized:
            return "uninitialized";
        case Status::initialized:
            return "initialized";
        case Status::waiting_for_client:
            return "waiting_for_client";
        case Status::handshake:
            return "handshake";
        case Status::ready:
            return "ready";
        case Status::error:
            return "error";
        default:
            return "unknown";
    }
}

OneServerWrapper::Status OneServerWrapper::status() const {
    const std::lock_guard<std::mutex> lock(_wrapper);

    OneServerStatus status;
    OneError err = one_server_status(_server, &status);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return Status::unknown;
    }
    switch (status) {
        case ONE_SERVER_STATUS_UNINITIALIZED:
            return Status::uninitialized;
        case ONE_SERVER_STATUS_INITIALIZED:
            return Status::initialized;
        case ONE_SERVER_STATUS_WAITING_FOR_CLIENT:
            return Status::waiting_for_client;
        case ONE_SERVER_STATUS_HANDSHAKE:
            return Status::handshake;
        case ONE_SERVER_STATUS_READY:
            return Status::ready;
        case ONE_SERVER_STATUS_ERROR:
            return Status::error;
        default:
            return Status::unknown;
    }
}

bool OneServerWrapper::quiet() const {
    return _quiet;
}

void OneServerWrapper::set_game_state(const GameState &state) {
    // If the game wishes to send and coordinate the processing of additional
    // game state to the ONE Platform, it can add that data here as an object
    // with additional keys. Note that these key names must not conflict with
    // the base key names of the Live State message.
    // OneObjectPtr object;
    // OneError err = one_object_create(&object);
    // if (one_is_error(err)) {
    //     UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err));
    // }
    // one_object_set_val_int(object, "matchLength", 123);

    // The optional object may be passed here as the last variable.
    OneError err = one_server_set_live_state(
        _server, state.players, state.max_players, state.name.data(), state.map.data(),
        state.mode.data(), state.version.data(), nullptr);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
    }

    // If custom data was added, then make sure to destroy the object.
    // one_object_destroy(object);
}

void OneServerWrapper::set_application_instance_status(ApplicationInstanceStatus status) {
    OneError err = one_server_set_application_instance_status(
        _server, (OneApplicationInstanceStatus)status);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
    }
}

void OneServerWrapper::set_soft_stop_callback(
    std::function<void(int timeout, void *userdata)> callback, void *userdata) {

    _soft_stop_callback = callback;
    _soft_stop_userdata = userdata;
}

void OneServerWrapper::set_allocated_callback(
    std::function<void(const OneArrayPtr data, void *userdata)> callback,
    void *userdata) {

    _allocated_callback = callback;
    _allocated_userdata = userdata;
}

void OneServerWrapper::set_metadata_callback(
    std::function<void(const OneArrayPtr data, void *userdata)> callback,
    void *userdata) {
    _metadata_callback = callback;
    _metadata_userdata = userdata;
}

void OneServerWrapper::set_host_information_callback(
    std::function<void(const OneObjectPtr data, void *userdata)> callback,
    void *userdata) {
    UE_LOG(LogTemp, Log, TEXT("ONE ARCUS: set host information callbacks"));
    _host_information_callback = callback;
    _host_information_userdata = userdata;
}

void OneServerWrapper::set_application_instance_information_callback(
    std::function<void(const OneObjectPtr data, void *userdata)> callback,
    void *userdata) {
    _application_instance_information_callback = callback;
    _application_instance_information_userdata = userdata;
}

// Tell the server to shutdown at the next appropriate time for its users (e.g.,
// after a match end).
void OneServerWrapper::soft_stop(void *userdata, int timeout_seconds) {
    if (userdata == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: userdata is nullptr"));
        return;
    }

    auto wrapper = reinterpret_cast<OneServerWrapper *>(userdata);
    if (wrapper->_soft_stop_callback == nullptr) {
        if (!wrapper->quiet())
            UE_LOG(LogTemp, Log, TEXT("ONE ARCUS: soft stop callback is nullptr"));
        return;
    }

    // Leave userdata optional.
    wrapper->_soft_stop_callback(timeout_seconds, wrapper->_soft_stop_userdata);
}

void OneServerWrapper::allocated(void *userdata, void *allocated) {
    if (userdata == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: userdata is nullptr"));
        return;
    }

    if (allocated == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: allocated is nullptr"));
        return;
    }

    auto wrapper = reinterpret_cast<OneServerWrapper *>(userdata);
    assert(wrapper->_server != nullptr);

    if (wrapper->_allocated_callback == nullptr) {
        if (!wrapper->quiet())
            UE_LOG(LogTemp, Log, TEXT("ONE ARCUS: allocated callback is nullptr"));
        return;
    }

    auto array = reinterpret_cast<OneArrayPtr>(allocated);
    wrapper->_allocated_callback(array, wrapper->_allocated_userdata);
}

void OneServerWrapper::metadata(void *userdata, void *metadata) {
    if (userdata == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: userdata is nullptr"));
        return;
    }

    if (metadata == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: metadata is nullptr"));
        return;
    }

    auto wrapper = reinterpret_cast<OneServerWrapper *>(userdata);
    assert(wrapper->_server != nullptr);

    if (wrapper->_metadata_callback == nullptr) {
        if (!wrapper->quiet())
            UE_LOG(LogTemp, Log, TEXT("ONE ARCUS: meta data callback is nullptr"));
        return;
    }

    auto array = reinterpret_cast<OneArrayPtr>(metadata);
    wrapper->_metadata_callback(array, wrapper->_metadata_userdata);
}

void OneServerWrapper::host_information(void *userdata, void *information) {
    if (userdata == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: userdata is nullptr"));
        return;
    }

    if (information == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: information is nullptr"));
        return;
    }

    auto wrapper = reinterpret_cast<OneServerWrapper *>(userdata);
    assert(wrapper->_server != nullptr);

    if (wrapper->_host_information_callback == nullptr) {
        if (!wrapper->quiet())
            UE_LOG(LogTemp, Log, TEXT("ONE ARCUS: host information callback is nullptr"));
        return;
    }

    auto object = reinterpret_cast<OneObjectPtr>(information);
    wrapper->_host_information_callback(object, wrapper->_host_information_userdata);
}

void OneServerWrapper::application_instance_information(void *userdata,
                                                        void *information) {
    if (userdata == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: userdata is nullptr"));
        return;
    }

    if (information == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: information is nullptr"));
        return;
    }

    auto wrapper = reinterpret_cast<OneServerWrapper *>(userdata);
    assert(wrapper->_server != nullptr);

    if (wrapper->_application_instance_information_callback == nullptr) {
        if (!wrapper->quiet())
            UE_LOG(
                LogTemp, Log,
                TEXT("ONE ARCUS: application instance information callback is nullptr"));
        return;
    }

    auto object = reinterpret_cast<OneObjectPtr>(information);
    wrapper->_application_instance_information_callback(
        object, wrapper->_application_instance_information_userdata);
}

}  // namespace one_integration
