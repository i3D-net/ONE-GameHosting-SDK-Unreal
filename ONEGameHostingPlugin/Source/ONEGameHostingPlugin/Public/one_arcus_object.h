// Copyright i3D.net, 2021. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "one_arcus_object.generated.h"

//----------------------------------------------
// One SDK object types forward declarations

struct OneObject;
typedef OneObject *OneObjectPtr;

UCLASS(Blueprintable)
class ONEGAMEHOSTINGPLUGIN_API UOneArcusObject : public UObject {
    GENERATED_BODY()

public:
    UOneArcusObject();

    virtual void BeginDestroy() override;

    OneObjectPtr object() const;

protected:
    OneObjectPtr _object;
};
