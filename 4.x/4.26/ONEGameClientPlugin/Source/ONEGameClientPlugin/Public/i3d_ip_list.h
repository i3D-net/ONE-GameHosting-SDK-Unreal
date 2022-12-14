// Copyright i3D.net, 2021. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "i3d_ip_list.generated.h"

//----------------------------------------------
// One SDK object types forward declarations

struct I3dIpList;
typedef I3dIpList *I3dIpListPtr;

UCLASS(Blueprintable)
class ONEGAMECLIENTPLUGIN_API UI3dIpList : public UObject {
    GENERATED_BODY()

public:
    UI3dIpList();

    virtual void BeginDestroy() override;

    I3dIpListPtr list() const;

protected:
    I3dIpListPtr _list;
};
