// Copyright i3D.net, 2021. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FONEGameClientPluginModule : public IModuleInterface {
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
