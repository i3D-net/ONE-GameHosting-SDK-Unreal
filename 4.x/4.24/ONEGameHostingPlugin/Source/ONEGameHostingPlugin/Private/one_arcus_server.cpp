// Copyright i3D.net, 2021. All Rights Reserved.

#include "one_arcus_server.h"

#include "one_arcus_array.h"
#include "one_arcus_object.h"

#include <one/server/one_parsing.h>
#include <one/arcus/c_error.h>

#include "Misc/CommandLine.h"

#include <chrono>
#include <array>

using namespace one_integration;

AOneArcusServer::AOneArcusServer() : _one_server(), _quiet(true), _is_initialized(false) {
    // Set to false to avoid duplicating the AOneArcusServer on the client side.
    bReplicates = false;
    bNetLoadOnClient = false;
}

void AOneArcusServer::BeginDestroy() {
    //------------------------------------------------------------
    // Shutdown & proper memory deallocation.

    _one_server.shutdown();
    Super::BeginDestroy();
}

void AOneArcusServer::init(int64 port, int64 players, int64 max_players, FString name,
                           FString map, FString mode, FString version) {
    //------------------------------------------------------------
    // Initialization of the server.

    if (_is_initialized) {
        if (!_quiet)
            UE_LOG(LogTemp, Log, TEXT("ONE ARCUS: is already initialized, skipping..."));
        return;
    }

    if (!_quiet) UE_LOG(LogTemp, Log, TEXT("ONE ARCUS: init"));

    //------------------------------------------------------------
    // Initialization.

    if (!_one_server.init(port)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: failed to init one server"));
        return;
    }

    //------------------------------------------------------------
    // Set initial game state.

    set_game_state(players, max_players, name, map, mode, version);

    //------------------------------------------------------------
    // Register notification callbacks.

    _one_server.set_soft_stop_callback(soft_stop_callback, this);
    _one_server.set_allocated_callback(allocated_callback, this);
    _one_server.set_metadata_callback(metadata_callback, this);
    _one_server.set_host_information_callback(host_information_callback, this);
    _one_server.set_application_instance_information_callback(
        application_instance_information_callback, this);
    _one_server.set_custom_command_callback(custom_command_callback, this);

    _is_initialized =
        (_one_server.status() == OneServerWrapper::Status::waiting_for_client);

    if (!_is_initialized) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: failed to init one server"));
        return;
    }
}

void AOneArcusServer::set_game_state(int64 players, int64 max_players, FString name,
                                     FString map, FString mode, FString version) {
    //------------------------------------------------------------
    // Set current game state.

    OneServerWrapper::GameState game_state;
    game_state.players = players;
    game_state.max_players = max_players;

    std::string name_string = std::string(TCHAR_TO_UTF8(*name));

    if (codec::value_max_size_null_terminated() < name_string.size() + 1) {
        UE_LOG(LogTemp, Error, TEXT("name length is too big"));
        return;
    }

    for (size_t i = 0; i < name_string.size(); ++i) {
        game_state.name.data()[i] = name_string[i];
    }

    game_state.name[name_string.size()] = '\0';

    std::string map_string = std::string(TCHAR_TO_UTF8(*map));

    if (codec::value_max_size_null_terminated() < map_string.size() + 1) {
        UE_LOG(LogTemp, Error, TEXT("map length is too big"));
        return;
    }

    for (size_t i = 0; i < map_string.size(); ++i) {
        game_state.map.data()[i] = map_string[i];
    }

    game_state.map[map_string.size()] = '\0';

    std::string mode_string = std::string(TCHAR_TO_UTF8(*mode));

    if (codec::value_max_size_null_terminated() < mode_string.size() + 1) {
        UE_LOG(LogTemp, Error, TEXT("mode length is too big"));
        return;
    }

    for (size_t i = 0; i < mode_string.size(); ++i) {
        game_state.mode.data()[i] = mode_string[i];
    }

    game_state.mode[mode_string.size()] = '\0';

    std::string version_string = std::string(TCHAR_TO_UTF8(*version));

    if (codec::value_max_size_null_terminated() < version_string.size() + 1) {
        UE_LOG(LogTemp, Error, TEXT("version length is too big"));
        return;
    }

    for (size_t i = 0; i < version_string.size(); ++i) {
        game_state.version.data()[i] = version_string[i];
    }

    game_state.version[version_string.size()] = '\0';

    _one_server.set_game_state(game_state);
}

void AOneArcusServer::set_application_instance_starting() {
    _one_server.set_application_instance_status(
        OneServerWrapper::ApplicationInstanceStatus::starting);
}

void AOneArcusServer::set_application_instance_online() {
    _one_server.set_application_instance_status(
        OneServerWrapper::ApplicationInstanceStatus::online);
}

void AOneArcusServer::set_application_instance_allocated() {
    _one_server.set_application_instance_status(
        OneServerWrapper::ApplicationInstanceStatus::allocated);
}

void AOneArcusServer::send_reverse_metadata(FString map, FString mode, FString type) {
    OneServerWrapper::ReverseMetadata data;

    std::string map_string = std::string(TCHAR_TO_UTF8(*map));

    if (codec::value_max_size_null_terminated() < map_string.size() + 1) {
        UE_LOG(LogTemp, Error, TEXT("map length is too big"));
        return;
    }

    for (size_t i = 0; i < map_string.size(); ++i) {
        data.map.data()[i] = map_string[i];
    }

    data.map[map_string.size()] = '\0';

    std::string mode_string = std::string(TCHAR_TO_UTF8(*mode));

    if (codec::value_max_size_null_terminated() < mode_string.size() + 1) {
        UE_LOG(LogTemp, Error, TEXT("mode length is too big"));
        return;
    }

    for (size_t i = 0; i < mode_string.size(); ++i) {
        data.mode.data()[i] = mode_string[i];
    }

    data.mode[mode_string.size()] = '\0';

    std::string type_string = std::string(TCHAR_TO_UTF8(*type));

    if (codec::value_max_size_null_terminated() < type_string.size() + 1) {
        UE_LOG(LogTemp, Error, TEXT("type length is too big"));
        return;
    }

    for (size_t i = 0; i < type_string.size(); ++i) {
        data.type.data()[i] = type_string[i];
    }

    data.type[type_string.size()] = '\0';

    _one_server.send_reverse_metadata(data);
}

void AOneArcusServer::update() {
    //------------------------------------------------------------
    // Updates: polling for incomming messages & sending outgoing messages.

    if (!_is_initialized) {
        return;
    }

    _one_server.update(_quiet);
}

void AOneArcusServer::shutdown() {
    //------------------------------------------------------------
    // Shutdown the server.

    if (!_quiet) UE_LOG(LogTemp, Log, TEXT("ONE ARCUS: shutdown"));
    _one_server.shutdown();
}

void AOneArcusServer::soft_stop_callback(int timeout, void *userdata) {
    auto server = reinterpret_cast<AOneArcusServer *>(userdata);
    if (server == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: userdata is null"));
        return;
    }

    // Send an softStop event with the extracted payload.
    // Since the payload is already an Unreal basic type, there is no
    // need to extract the payload for the needed fields.
    server->softStopReceived(timeout);
}

void AOneArcusServer::allocated_callback(OneArrayPtr data, void *userdata) {
    auto server = reinterpret_cast<AOneArcusServer *>(userdata);
    if (server == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: userdata is null"));
        return;
    }

    // Creating a UOneArcusArray containing a copy of OneArrayPtr
    // so it could be used as an Unreal event containing the data.
    UOneArcusArray *array = NewObject<UOneArcusArray>();
    OneArrayPtr new_array = array->array();

    OneError err = one_array_copy(data, new_array);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: failed to copy array"));
        return;
    }

    // Send an allocatedReceived event with the raw payload.
    server->allocatedReceived(array);
}

void AOneArcusServer::metadata_callback(OneArrayPtr data, void *userdata) {
    auto server = reinterpret_cast<AOneArcusServer *>(userdata);
    if (server == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: userdata is null"));
        return;
    }

    // Creating a UOneArcusArray containing a copy of OneArrayPtr
    // so it could be used as an Unreal event containing the data.
    UOneArcusArray *array = NewObject<UOneArcusArray>();
    OneArrayPtr new_array = array->array();

    OneError err = one_array_copy(data, new_array);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: failed to copy array"));
        return;
    }

    // Send an metaDataReceived event with the raw payload.
    server->metaDataReceived(array);
}

void AOneArcusServer::host_information_callback(OneObjectPtr data, void *userdata) {
    auto server = reinterpret_cast<AOneArcusServer *>(userdata);
    if (server == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: userdata is null"));
        return;
    }

    // Creating a UOneArcusObject containing a copy of OneObjectPtr
    // so it could be used as an Unreal event containing the data.
    UOneArcusObject *object = NewObject<UOneArcusObject>();
    OneObjectPtr new_object = object->object();

    OneError err = one_object_copy(data, new_object);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: failed to copy object"));
        return;
    }

    // Send an hostInformationReceived event with the raw payload.
    server->hostInformationReceived(object);
}

void AOneArcusServer::application_instance_information_callback(OneObjectPtr data,
                                                                void *userdata) {
    auto server = reinterpret_cast<AOneArcusServer *>(userdata);
    if (server == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: userdata is null"));
        return;
    }

    // Creating a UOneArcusObject containing a copy of OneObjectPtr
    // so it could be used as an Unreal event containing the data.
    UOneArcusObject *object = NewObject<UOneArcusObject>();
    OneObjectPtr new_object = object->object();

    OneError err = one_object_copy(data, new_object);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: failed to copy object"));
        return;
    }

    // Send an applicationInstanceInformationReceived event with the raw payload.
    server->applicationInstanceInformationReceived(object);
}

void AOneArcusServer::custom_command_callback(OneArrayPtr data, void *userdata) {
    auto server = reinterpret_cast<AOneArcusServer *>(userdata);
    if (server == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: userdata is null"));
        return;
    }

    // Creating a UOneArcusArray containing a copy of OneArrayPtr
    // so it could be used as an Unreal event containing the data.
    UOneArcusArray *array = NewObject<UOneArcusArray>();
    OneArrayPtr new_array = array->array();

    OneError err = one_array_copy(data, new_array);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: failed to copy array"));
        return;
    }

    // Send an metaDataReceived event with the raw payload.
    server->customCommandReceived(array);
}
