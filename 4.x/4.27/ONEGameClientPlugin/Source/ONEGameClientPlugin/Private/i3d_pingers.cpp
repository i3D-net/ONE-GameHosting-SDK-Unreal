// Copyright i3D.net, 2021. All Rights Reserved.

#include "i3d_pingers.h"

#include "i3d_ip_list.h"

#include <one/ping/c_error.h>

using namespace i3d_ping_integration;

AI3dPingers::AI3dPingers() : _i3d_pingers(), _quiet(true), _is_initialized(false) {}

void AI3dPingers::BeginDestroy() {
    //------------------------------------------------------------
    // Shutdown & proper memory deallocation.

    _i3d_pingers.shutdown();
    Super::BeginDestroy();
}

void AI3dPingers::init(UI3dIpList *ip_list) {
    //------------------------------------------------------------
    // Initialization of the pingers.

    if (_is_initialized) {
        if (!_quiet)
            UE_LOG(LogTemp, Log, TEXT("I3D PINGER: is already initialized, skipping..."));
        return;
    }

    if (!_quiet) UE_LOG(LogTemp, Log, TEXT("I3D PINGER: init"));

    //------------------------------------------------------------
    // Initialization.

    if (!_i3d_pingers.init(ip_list->list())) {
        UE_LOG(LogTemp, Error, TEXT("I3D PINGER: failed to init i3d pingers"));
        return;
    }

    _is_initialized = (_i3d_pingers.status() == I3dPingersWrapper::Status::initialized);

    if (!_is_initialized) {
        UE_LOG(LogTemp, Error, TEXT("I3D PINGER: failed to init i3d pingers"));
        return;
    }
}

void AI3dPingers::update() {
    //------------------------------------------------------------
    // Updates: sening out pings.

    if (!_is_initialized) {
        return;
    }

    _i3d_pingers.update(_quiet);
}

void AI3dPingers::shutdown() {
    //------------------------------------------------------------
    // Shutdown the pingers.

    if (!_quiet) UE_LOG(LogTemp, Log, TEXT("I3D PINGER: shutdown"));
    _i3d_pingers.shutdown();
}

int32 AI3dPingers::pingers_size() const {
    unsigned int size = 0;
    if (!_i3d_pingers.pingers_size(size)) {
        UE_LOG(LogTemp, Error, TEXT("I3D PINGER: failed to get pingers size"));
        return 0;
    }

    return size;
}

int32 AI3dPingers::last_time(int32 pos) const {
    I3dPingersWrapper::PingStatistics s;
    if (!_i3d_pingers.statistics(pos, s)) {
        UE_LOG(LogTemp, Error, TEXT("I3D PINGER: failed to get last time"));
        return 0;
    }
    return s.last_time;
}

float AI3dPingers::average_time(int32 pos) const {
    I3dPingersWrapper::PingStatistics s;
    if (!_i3d_pingers.statistics(pos, s)) {
        UE_LOG(LogTemp, Error, TEXT("I3D PINGER: failed to get average time"));
        return 0;
    }
    return static_cast<float>(s.average_time);
}

int32 AI3dPingers::min_time(int32 pos) const {
    I3dPingersWrapper::PingStatistics s;
    if (!_i3d_pingers.statistics(pos, s)) {
        UE_LOG(LogTemp, Error, TEXT("I3D PINGER: failed to get average time"));
        return 0;
    }
    return s.min_time;
}

int32 AI3dPingers::max_time(int32 pos) const {
    I3dPingersWrapper::PingStatistics s;
    if (!_i3d_pingers.statistics(pos, s)) {
        UE_LOG(LogTemp, Error, TEXT("I3D PINGER: failed to get average time"));
        return 0;
    }
    return s.max_time;
}

float AI3dPingers::median_time(int32 pos) const {
    I3dPingersWrapper::PingStatistics s;
    if (!_i3d_pingers.statistics(pos, s)) {
        UE_LOG(LogTemp, Error, TEXT("I3D PINGER: failed to get average time"));
        return 0;
    }
    return static_cast<float>(s.median_time);
}

int32 AI3dPingers::ping_response_count(int32 pos) const {
    I3dPingersWrapper::PingStatistics s;
    if (!_i3d_pingers.statistics(pos, s)) {
        UE_LOG(LogTemp, Error, TEXT("I3D PINGER: failed to get ping response count"));
        return 0;
    }
    return s.ping_response_count;
}

bool AI3dPingers::at_least_one_site_has_been_pinged() const {
    bool at_least_one_sites = false;
    if (!_i3d_pingers.at_least_one_site_has_been_pinged(at_least_one_sites)) {
        UE_LOG(LogTemp, Error, TEXT("I3D PINGER: failed to get at least one site has been pigned"));
        return false;
    }
    return at_least_one_sites;
}

bool AI3dPingers::all_sites_have_been_pinged() const {
    bool all_sites = false;
    if (!_i3d_pingers.all_sites_have_been_pinged(all_sites)) {
        UE_LOG(LogTemp, Error,
               TEXT("I3D PINGER: failed to get all sites have been pinged"));
        return false;
    }
    return all_sites;
}
