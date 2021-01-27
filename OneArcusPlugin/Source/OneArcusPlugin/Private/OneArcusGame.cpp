#include "OneArcusGame.h"
#include "OneArcusPlugin.h"

#include <one/game/parsing.h>

#include <chrono>
#include <array>

using namespace one_integration;

UOneArcusGame::UOneArcusGame() {}

void UOneArcusGame::init(int64 port, int64 max_players, FString name,
              FString map, FString mode, FString version,
              int64 delay) {
    std::array<char, codec::value_max_size_null_terminated()> name_value;
    std::array<char, codec::value_max_size_null_terminated()> map_value;
    std::array<char, codec::value_max_size_null_terminated()> mode_value;
    std::array<char, codec::value_max_size_null_terminated()> version_value;

    if (codec::value_max_size_null_terminated() < static_cast<size_t>(name.Len()) + 1) {
        UE_LOG(LogTemp, Error, TEXT("name length is too big"));
        return;
    }

    for(int32 i = 0; i < name.Len(); ++i) {
        name_value.data()[i] = name[i];
    }

    name_value[name.Len()] = '\0';

    if (codec::value_max_size_null_terminated() < static_cast<size_t>(map.Len()) + 1) {
        UE_LOG(LogTemp, Error, TEXT("map length is too big"));
        return;
    }

    for(int32 i = 0; i < map.Len(); ++i) {
        map_value.data()[i] = map[i];
    }

    map_value[name.Len()] = '\0';

    if (codec::value_max_size_null_terminated() < static_cast<size_t>(mode.Len()) + 1) {
        UE_LOG(LogTemp, Error, TEXT("mode length is too big"));
        return;
    }

    for(int32 i = 0; i < mode.Len(); ++i) {
        mode_value.data()[i] = mode[i];
    }

    mode_value[mode.Len()] = '\0';

    if (codec::value_max_size_null_terminated() < static_cast<size_t>(version.Len()) + 1) {
        UE_LOG(LogTemp, Error, TEXT("version length is too big"));
        return;
    }

    for(int32 i = 0; i < version.Len(); ++i) {
        version_value.data()[i] = version[i];
    }

    version_value[version.Len()] = '\0';

    _game.init(port, max_players, name_value, map_value, mode_value, version_value,
               std::chrono::seconds(delay));
}

void UOneArcusGame::update() {
    _game.update();
}

void UOneArcusGame::shutdown() {
    _game.shutdown();
}
