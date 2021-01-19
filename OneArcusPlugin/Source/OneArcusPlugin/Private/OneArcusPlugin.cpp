// Copyright Epic Games, Inc. All Rights Reserved.

#include "OneArcusPlugin.h"

#include <one/game/game.h>

#define LOCTEXT_NAMESPACE "FOneArcusPluginModule"

void FOneArcusPluginModule::StartupModule()
{
    one_integration::Game game;

    game.init(90000, 16, "test_name", "test_map", "test_mode", "test_version", std::chrono::seconds(10));
}

void FOneArcusPluginModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FOneArcusPluginModule, OneArcusPlugin)