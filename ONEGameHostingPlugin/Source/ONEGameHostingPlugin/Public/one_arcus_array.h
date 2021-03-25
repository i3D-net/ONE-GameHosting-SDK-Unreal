// Copyright i3D.net, 2021. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "one_arcus_array.generated.h"

//----------------------------------------------
// One SDK object types forward declarations

struct OneArray;
typedef OneArray *OneArrayPtr;

UCLASS(Blueprintable)
class ONEGAMEHOSTINGPLUGIN_API UOneArcusArray : public UObject {
    GENERATED_BODY()

public:
    UOneArcusArray();

    virtual void BeginDestroy() override;

    OneArrayPtr array() const;

protected:
    OneArrayPtr _array;
};
