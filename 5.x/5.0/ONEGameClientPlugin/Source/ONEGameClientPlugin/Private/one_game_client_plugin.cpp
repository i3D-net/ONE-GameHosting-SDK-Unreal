// Copyright i3D.net, 2021. All Rights Reserved.

#include "one_game_client_plugin.h"

#include "i3d_allocator.h"

#include <one/ping/c_api.h>

#define LOCTEXT_NAMESPACE "FONEGameClientPluginModule"

using namespace i3d_ping_integration;

void FONEGameClientPluginModule::StartupModule() {
    //----------------------
    // Set custom allocator.

    i3d_ping_allocator_set_alloc(allocation::alloc);
    i3d_ping_allocator_set_free(allocation::free);
    i3d_ping_allocator_set_realloc(allocation::realloc);
}

void FONEGameClientPluginModule::ShutdownModule() {}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FONEGameClientPluginModule, ONEGameClientPlugin)