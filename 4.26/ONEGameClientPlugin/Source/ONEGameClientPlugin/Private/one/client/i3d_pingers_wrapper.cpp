// Copyright i3D.net, 2021. All Rights Reserved.

#include <one/client/i3d_pingers_wrapper.h>

#include <assert.h>
#include <string>
#include <cstring>

#include <one/ping/c_api.h>
#include <one/ping/c_error.h>

#include <CoreMinimal.h>

namespace i3d_ping_integration {
namespace {

// Logger to pass into One.
void pingers_log(void *userdata, I3dPingLogLevel level, const char *message) {
    if (userdata == nullptr) {
        return;
    }

    auto wrapper = reinterpret_cast<I3dPingersWrapper *>(userdata);

    if (wrapper->quiet()) {
        return;
    }

    switch (level) {
        case I3D_PING_LOG_LEVEL_INFO:
            UE_LOG(LogTemp, Log, TEXT("ONE CLIENT: %s"), *FString(message));
            break;
        case I3D_PING_LOG_LEVEL_ERROR:
            UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"), *FString(message));
            break;
        default:
            break;
    }
}

}  // namespace

I3dPingersWrapper::I3dPingersWrapper() : _pingers(nullptr), _quiet(true) {}

I3dPingersWrapper::~I3dPingersWrapper() {
    shutdown();
}

bool I3dPingersWrapper::init(I3dIpListPtr ip_list) {
    const std::lock_guard<std::mutex> lock(_wrapper);

    if (_pingers != nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: already initialized"));
        return false;
    }

    //-----------------------
    // Create the i3D Pinger.

    I3dPingError err = i3d_ping_pingers_create(&_pingers);
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    // Set custom logger - optional.
    err = i3d_ping_pingers_set_logger(_pingers, pingers_log,
                                      nullptr);  // null userdata as global log is used.
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    err = i3d_ping_pingers_init(_pingers, ip_list);
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("I3dPingersWrapper init complete"));
    return true;
}

void I3dPingersWrapper::shutdown() {
    const std::lock_guard<std::mutex> lock(_wrapper);

    if (_pingers == nullptr) {
        return;
    }

    // Free all objects created via the ONE API. This also shuts down the server
    // first, ending any active connection to it.
    i3d_ping_pingers_destroy(_pingers);
    _pingers = nullptr;
}

void I3dPingersWrapper::update(bool quiet) {
    const std::lock_guard<std::mutex> lock(_wrapper);
    assert(_pingers != nullptr);

    // Updates the server, which handles client connections, and services
    // outgoing and incoming messages.
    // Any registered callbacks will called during update, if the corresponding
    // messages are received.
    I3dPingError err = i3d_ping_pingers_update(_pingers);
    if (i3d_ping_is_error(err)) {
        if (!quiet)
            UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
                   *FString(i3d_ping_error_text(err)));
        return;
    }
}

std::string I3dPingersWrapper::status_to_string(Status status) {
    switch (status) {
        case Status::uninitialized:
            return "uninitialized";
        case Status::initialized:
            return "initialized";
        default:
            return "unknown";
    }
}

I3dPingersWrapper::Status I3dPingersWrapper::status() const {
    const std::lock_guard<std::mutex> lock(_wrapper);

    I3dPingersStatus status;
    I3dPingError err = i3d_ping_pingers_status(_pingers, &status);
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return Status::unknown;
    }
    switch (status) {
        case I3D_PINGERS_STATUS_UNINITIALIZED:
            return Status::uninitialized;
        case I3D_PINGERS_STATUS_INITIALIZED:
            return Status::initialized;
        default:
            return Status::unknown;
    }
}

bool I3dPingersWrapper::quiet() const {
    return _quiet;
}

bool I3dPingersWrapper::pingers_size(unsigned int &size) const {
    const std::lock_guard<std::mutex> lock(_wrapper);

    I3dPingError err = i3d_ping_pingers_size(_pingers, &size);
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    return true;
}

bool I3dPingersWrapper::statistics(unsigned int pos, PingStatistics &s) const {
    const std::lock_guard<std::mutex> lock(_wrapper);

    I3dPingError err = i3d_ping_pingers_statistics(
        _pingers, pos, &(s.last_time), &(s.average_time), &(s.min_time), &(s.max_time),
        &(s.median_time), &(s.ping_response_count));
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    return true;
}

bool I3dPingersWrapper::at_least_one_site_has_been_pinged(bool &result) const {
    const std::lock_guard<std::mutex> lock(_wrapper);

    I3dPingError err =
        i3d_ping_pingers_at_least_one_site_has_been_pinged(_pingers, &result);
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    return true;
}
bool I3dPingersWrapper::all_sites_have_been_pinged(bool &result) const {
    const std::lock_guard<std::mutex> lock(_wrapper);

    I3dPingError err = i3d_ping_pingers_all_sites_have_been_pinged(_pingers, &result);
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    return true;
}

}  // namespace i3d_ping_integration
