// Copyright i3D.net, 2021. All Rights Reserved.

#include "one_game_hosting_plugin.h"

#include "one_allocator.h"

#define LOCTEXT_NAMESPACE "FONEGameHostingPluginModule"

using namespace one_integration;

void FONEGameHostingPluginModule::StartupModule() {
    //----------------------
    // Set custom allocator.

    one_allocator_set_alloc(allocation::alloc);
    one_allocator_set_free(allocation::free);
    one_allocator_set_realloc(allocation::realloc);
}

void FONEGameHostingPluginModule::ShutdownModule() {}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FONEGameHostingPluginModule, ONEGameHostingPlugin)