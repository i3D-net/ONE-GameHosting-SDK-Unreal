// Copyright Epic Games, Inc. All Rights Reserved.

#include "OneArcusPlugin.h"

#include <one/game/game.h>

#define LOCTEXT_NAMESPACE "FOneArcusPluginModule"

void FOneArcusPluginModule::StartupModule() {}

void FOneArcusPluginModule::ShutdownModule() {}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FOneArcusPluginModule, OneArcusPlugin)