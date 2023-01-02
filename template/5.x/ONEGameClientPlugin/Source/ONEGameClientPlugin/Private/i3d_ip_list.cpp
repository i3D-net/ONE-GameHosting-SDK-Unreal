// Copyright i3D.net, 2021. All Rights Reserved.

#include "i3d_ip_list.h"

#include <one/ping/c_api.h>

UI3dIpList::UI3dIpList() {
    i3d_ping_ip_list_create(&_list);
}

void UI3dIpList::BeginDestroy() {
    i3d_ping_ip_list_destroy(_list);
    Super::BeginDestroy();
}

I3dIpListPtr UI3dIpList::list() const {
    return _list;
}
