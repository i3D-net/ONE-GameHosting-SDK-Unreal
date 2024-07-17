// Fill out your copyright notice in the Description page of Project Settings.

#include "FakeGame.h"

#include <Misc/CommandLine.h>

#include <one_arcus_array.h>
#include <one_arcus_object.h>

#include <one/server/one_parsing.h>
#include <one/arcus/c_error.h>

using namespace one_integration;

// Sets default values
AFakeGame::AFakeGame() {
    // Set to false to avoid duplicating the AOneArcusServer on the client side.
    bReplicates = false;
    bNetLoadOnClient = false;
}

int64 AFakeGame::parse_command_line_management_port(int64 default_value) {
    int64 value;

    if (FParse::Value(FCommandLine::Get(), TEXT("ManagementPort"), value)) {
        return value;
    }

    return default_value;
}

void AFakeGame::allocatedPayloadParse(UOneArcusArray *array) {
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
                    UE_LOG(
                        LogTemp, Error,
                        TEXT("ONE ARCUS: expected 2 keys, but received %d keys instead"),
                        total_number_of_keys);
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

void AFakeGame::metaDataPayloadParse(UOneArcusArray *array) {
    if (array == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: array is nullptr"));
        return;
    }

    std::array<char, codec::value_max_size_null_terminated()> map;
    std::array<char, codec::value_max_size_null_terminated()> mode;
    std::array<char, codec::value_max_size_null_terminated()> type;

    // Optional - the game can require and read allocated keys to configure
    // the server. This is to mirror the documentation example here:
    // https://www.i3d.net/docs/one/odp/Game-Integration/Management-Protocol/Arcus-V2/request-response/#metadata
    auto callback =
        [&](const size_t total_number_of_keys,
            const std::array<char, codec::key_max_size_null_terminated()> &key,
            const std::array<char, codec::value_max_size_null_terminated()> &value) {
            // Since the allocated fields are optional, one can add and/or remove the
            // fields being parsed as needed.

            if (total_number_of_keys != 3) {
                if (!_quiet)
                    UE_LOG(
                        LogTemp, Error,
                        TEXT("ONE ARCUS: expected 3 keys, but received %d keys instead"),
                        total_number_of_keys);
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

    FString map_string(UTF8_TO_TCHAR(map.data()));
    FString mode_string(UTF8_TO_TCHAR(mode.data()));
    FString type_string(UTF8_TO_TCHAR(type.data()));

    // Send an allocatedExtracted event with the extracted payload.
    metaDataExtracted(map_string, mode_string, type_string);
}

void AFakeGame::hostInformationPayloadParse(UOneArcusObject *object) {
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

    FString server_name_string(UTF8_TO_TCHAR(server_name.data()));

    hostInformationExtracted(id, server_id, server_name_string);
}

void AFakeGame::applicationInstanceInformationPayloadParse(UOneArcusObject *object) {
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

    FString fleet_id_string(UTF8_TO_TCHAR(fleet_id.data()));
    FString virtual_string(is_virtual ? "true" : "false");

    // Send an applicationInstanceInformation event with the extracted payload.
    applicationInstanceInformationExtracted(fleet_id_string, host_id, is_virtual);
}

void AFakeGame::customCommandPayloadParse(UOneArcusArray *array) {
    if (array == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: array is nullptr"));
        return;
    }

    std::array<char, codec::value_max_size_null_terminated()> command;
    std::array<char, codec::value_max_size_null_terminated()> argument;

    // Optional - the game can require and read allocated keys to configure
    // the server. This is to mirror the documentation example here:
    // https://www.i3d.net/docs/one/odp/Game-Integration/Management-Protocol/Arcus-V2/request-response/#custom-command
    auto callback =
        [&](const size_t total_number_of_keys,
            const std::array<char, codec::key_max_size_null_terminated()> &key,
            const std::array<char, codec::value_max_size_null_terminated()> &value) {
            // Since the allocated fields are optional, one can add and/or remove the
            // fields being parsed as needed.

            if (total_number_of_keys != 2) {
                if (!_quiet)
                    UE_LOG(
                        LogTemp, Error,
                        TEXT("ONE ARCUS: expected 2 keys, but received %d keys instead"),
                        total_number_of_keys);
                return false;
            }

            if (std::strncmp(key.data(), "command",
                             codec::key_max_size_null_terminated()) == 0) {
                command = value;
                return true;
            }

            if (std::strncmp(key.data(), "argument",
                             codec::key_max_size_null_terminated()) == 0) {
                argument = value;
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

    FString command_string(UTF8_TO_TCHAR(command.data()));
    FString argument_string(UTF8_TO_TCHAR(argument.data()));

    // Send an allocatedExtracted event with the extracted payload.
    customCommandExtracted(command_string, argument_string);
}