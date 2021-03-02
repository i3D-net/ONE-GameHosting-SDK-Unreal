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

// Provides example global allocation hooks used to override the allocations
// made within the ONE Game Hosting SDK.
namespace allocation {

void *alloc(size_t bytes) {
    return std::malloc(bytes);
}

void free(void *p) {
    std::free(p);
}

void *realloc(void *p, size_t bytes) {
    return std::realloc(p, bytes);
}

}  // namespace allocation

AOneArcusServer::AOneArcusServer() : _one_server(), _quiet(false), _is_init(false) {
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

int32 AOneArcusServer::parse_command_line_management_port(int32 default_value) {
    int32 value;

    if (FParse::Value(FCommandLine::Get(), TEXT("ManagementPort"), value)) {
        return value;
    }

    return default_value;
}

void AOneArcusServer::init(int64 port, int64 players, int64 max_players, FString name,
                           FString map, FString mode, FString version) {
    //------------------------------------------------------------
    // Initialization of the server.

    if (_is_init) {
        if (!_quiet)
            UE_LOG(LogTemp, Log, TEXT("ONE ARCUS: is already initialized, skipping..."));
        return;
    }

    if (!_quiet) UE_LOG(LogTemp, Log, TEXT("ONE ARCUS: init"));

    //------------------------------------------------------------
    // Set allocator hooks and initialization.

    OneServerWrapper::AllocationHooks hooks(allocation::alloc, allocation::free,
                                            allocation::realloc);
    if (!_one_server.init(port, hooks)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: failed to init one server"));
        return;
    }

    //------------------------------------------------------------
    // Set initial game state.

    OneServerWrapper::GameState game_state;
    game_state.players = players;
    game_state.max_players = max_players;

    if (codec::value_max_size_null_terminated() < static_cast<size_t>(name.Len()) + 1) {
        UE_LOG(LogTemp, Error, TEXT("name length is too big"));
        return;
    }

    for (int32 i = 0; i < name.Len(); ++i) {
        game_state.name.data()[i] = name[i];
    }

    game_state.name[name.Len()] = '\0';

    if (codec::value_max_size_null_terminated() < static_cast<size_t>(map.Len()) + 1) {
        UE_LOG(LogTemp, Error, TEXT("map length is too big"));
        return;
    }

    for (int32 i = 0; i < map.Len(); ++i) {
        game_state.map.data()[i] = map[i];
    }

    game_state.map[name.Len()] = '\0';

    if (codec::value_max_size_null_terminated() < static_cast<size_t>(mode.Len()) + 1) {
        UE_LOG(LogTemp, Error, TEXT("mode length is too big"));
        return;
    }

    for (int32 i = 0; i < mode.Len(); ++i) {
        game_state.mode.data()[i] = mode[i];
    }

    game_state.mode[mode.Len()] = '\0';

    if (codec::value_max_size_null_terminated() <
        static_cast<size_t>(version.Len()) + 1) {
        UE_LOG(LogTemp, Error, TEXT("version length is too big"));
        return;
    }

    for (int32 i = 0; i < version.Len(); ++i) {
        game_state.version.data()[i] = version[i];
    }

    game_state.version[version.Len()] = '\0';

    _one_server.set_game_state(game_state);

    //------------------------------------------------------------
    // Register notification callbacks.

    _one_server.set_soft_stop_callback(soft_stop_callback, this);
    _one_server.set_allocated_callback(allocated_callback, this);
    _one_server.set_metadata_callback(metadata_callback, this);
    _one_server.set_host_information_callback(host_information_callback, this);
    _one_server.set_application_instance_information_callback(
        application_instance_information_callback, this);

    _is_init = (_one_server.status() == OneServerWrapper::Status::waiting_for_client);

    if (!_is_init) {
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

    if (codec::value_max_size_null_terminated() < static_cast<size_t>(name.Len()) + 1) {
        UE_LOG(LogTemp, Error, TEXT("name length is too big"));
        return;
    }

    for (int32 i = 0; i < name.Len(); ++i) {
        game_state.name.data()[i] = name[i];
    }

    game_state.name[name.Len()] = '\0';

    if (codec::value_max_size_null_terminated() < static_cast<size_t>(map.Len()) + 1) {
        UE_LOG(LogTemp, Error, TEXT("map length is too big"));
        return;
    }

    for (int32 i = 0; i < map.Len(); ++i) {
        game_state.map.data()[i] = map[i];
    }

    game_state.map[name.Len()] = '\0';

    if (codec::value_max_size_null_terminated() < static_cast<size_t>(mode.Len()) + 1) {
        UE_LOG(LogTemp, Error, TEXT("mode length is too big"));
        return;
    }

    for (int32 i = 0; i < mode.Len(); ++i) {
        game_state.mode.data()[i] = mode[i];
    }

    game_state.mode[mode.Len()] = '\0';

    if (codec::value_max_size_null_terminated() <
        static_cast<size_t>(version.Len()) + 1) {
        UE_LOG(LogTemp, Error, TEXT("version length is too big"));
        return;
    }

    for (int32 i = 0; i < version.Len(); ++i) {
        game_state.version.data()[i] = version[i];
    }

    game_state.version[version.Len()] = '\0';

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

void AOneArcusServer::update() {
    //------------------------------------------------------------
    // Updates: polling for incomming messages & sending outgoing messages.

    _one_server.update(_quiet);
}

void AOneArcusServer::shutdown() {
    //------------------------------------------------------------
    // Shutdown the server.

    if (!_quiet) UE_LOG(LogTemp, Log, TEXT("ONE ARCUS: shutdown"));
    _one_server.shutdown();
}

void AOneArcusServer::allocatedPayloadParse(UOneArcusArray *array) {
    //------------------------------------------------------------
    // Parsing the allocated payload from UOneArcusArray & translate it in Unreal basic
    // types.

    if (array == nullptr) {
        if (!_quiet) UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: array is nullptr"));
        return;
    }

    int32 players = 0;
    int32 duration = 0;

    // Optional - the game can require and read allocated keys to configure
    // the server. This is to mirror the documentation example here:
    // https://www.i3d.net/docs/one/odp/Game-Integration/Management-Protocol/Arcus-V2/request-response/#allocated
    auto callback =
        [&](const size_t total_number_of_keys,
            const std::array<char, codec::key_max_size_null_terminated()> &key,
            const std::array<char, codec::value_max_size_null_terminated()> &value) {
            // Since the allocated fields are optional, one can add and/or remove the
            // fields being parsed as needed.

            if (total_number_of_keys != 2) {
                if (!_quiet)
                    UE_LOG(LogTemp, Error,
                           TEXT("ONE ARCUS: expected 2 keys, but received a different "
                                "number instead"));
                return false;
            }

            if (std::strncmp(key.data(), "players",
                             codec::key_max_size_null_terminated()) == 0) {
                players = std::atoi(value.data());
                return true;
            }

            if (std::strncmp(key.data(), "duration",
                             codec::key_max_size_null_terminated()) == 0) {
                duration = std::atoi(value.data());
                return true;
            }

            if (!_quiet) UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: key is not handled"));
            return false;
        };

    // Extracting the payload using the previously defined callback.
    if (!Parsing::extract_key_value_payload(array->array(), callback)) {
        if (!_quiet)
            UE_LOG(LogTemp, Error,
                   TEXT("ONE ARCUS: failed to extract key/value payload"));
        return;
    }

    // Send an allocatedExtracted event with the extracted payload.
    allocatedExtracted(players, duration);
}

void AOneArcusServer::metaDataPayloadParse(UOneArcusArray *array) {
    if (array == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: array is nullptr"));
        return;
    }

    std::array<char, codec::value_max_size_null_terminated()> map;
    std::array<char, codec::value_max_size_null_terminated()> mode;
    std::array<char, codec::value_max_size_null_terminated()> type;

    // Optional - the game can require and read allocated keys to configure
    // the server. This is to mirror the documentation example here:
    // https://www.i3d.net/docs/one/odp/Game-Integration/Management-Protocol/Arcus-V2/request-response/#meta-data
    auto callback =
        [&](const size_t total_number_of_keys,
            const std::array<char, codec::key_max_size_null_terminated()> &key,
            const std::array<char, codec::value_max_size_null_terminated()> &value) {
            // Since the allocated fields are optional, one can add and/or remove the
            // fields being parsed as needed.

            if (total_number_of_keys != 3) {
                if (!_quiet)
                    UE_LOG(LogTemp, Error,
                           TEXT("ONE ARCUS: expected 3 keys, but received a different "
                                "number instead"));
                return false;
            }

            if (std::strncmp(key.data(), "map", codec::key_max_size_null_terminated()) ==
                0) {
                map = value;
                return true;
            }

            if (std::strncmp(key.data(), "mode", codec::key_max_size_null_terminated()) ==
                0) {
                mode = value;
                return true;
            }

            if (std::strncmp(key.data(), "type", codec::key_max_size_null_terminated()) ==
                0) {
                type = value;
                return true;
            }

            if (!_quiet) UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: key is not handled"));
            return false;
        };

    // Extracting the payload using the previously defined callback.
    if (!Parsing::extract_key_value_payload(array->array(), callback)) {
        if (!_quiet)
            UE_LOG(LogTemp, Error,
                   TEXT("ONE ARCUS: failed to extract key/value payload"));
        return;
    }

    FString map_string(map.data());
    FString mode_string(mode.data());
    FString type_string(type.data());

    // Send an allocatedExtracted event with the extracted payload.
    metaDataExtracted(map_string, mode_string, type_string);
}

void AOneArcusServer::hostInformationPayloadParse(UOneArcusObject *object) {
    if (object == nullptr) {
        if (!_quiet) UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: array is nullptr"));
        return;
    }

    int32 id = 0;
    int32 server_id = 0;
    std::array<char, codec::value_max_size_null_terminated()> server_name;

    // This is to mirror the documentation example here:
    // https://www.i3d.net/docs/one/odp/Game-Integration/Management-Protocol/Arcus-V2/request-response/#host-information
    // Only some field are parsed for simplicity.
    OneError err = one_object_val_int(object->object(), "id", &id);
    if (one_is_error(err)) {
        if (!_quiet) UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: failed to extract id key"));
        return;
    }

    err = one_object_val_int(object->object(), "serverId", &server_id);
    if (one_is_error(err)) {
        if (!_quiet)
            UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: failed to extract serverId key"));
        return;
    }

    if (!Parsing::extract_string(
            object->object(), "serverName",
            [&](const std::array<char, codec::value_max_size_null_terminated()> &value) {
                server_name = value;
                return true;
            })) {
        if (!_quiet)
            UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: failed to extract serverName key"));
        return;
    }

    // ... add more field parsing as needed.

    FString server_name_string(server_name.data());

    hostInformationExtracted(id, server_id, server_name_string);
}

void AOneArcusServer::applicationInstanceInformationPayloadParse(
    UOneArcusObject *object) {
    if (object == nullptr) {
        if (!_quiet) UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: array is nullptr"));
        return;
    }

    std::array<char, codec::value_max_size_null_terminated()> fleet_id;
    int32 host_id = 0;
    bool is_virtual = false;

    // This is to mirror the documentation example here:
    // https://www.i3d.net/docs/one/odp/Game-Integration/Management-Protocol/Arcus-V2/request-response/#applicationinstance-information
    // Only some field are parsed for simplicity.
    if (!Parsing::extract_string(
            object->object(), "fleetId",
            [&](const std::array<char, codec::value_max_size_null_terminated()> &value) {
                fleet_id = value;
                return true;
            })) {
        if (!_quiet)
            UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: failed to extract fleetId key"));
        return;
    }

    OneError err = one_object_val_int(object->object(), "hostId", &host_id);
    if (one_is_error(err)) {
        if (!_quiet)
            UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: failed to extract hostId key"));
        return;
    }

    err = one_object_val_bool(object->object(), "isVirtual", &is_virtual);
    if (one_is_error(err)) {
        if (!_quiet)
            UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: failed to extract isVirtual key"));
        return;
    }

    // ... add more field parsing as needed.

    FString fleet_id_string(fleet_id.data());
    FString virtual_string(is_virtual ? "true" : "false");

    // Send an applicationInstanceInformation event with the extracted payload.
    applicationInstanceInformationExtracted(fleet_id_string, host_id, is_virtual);
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
