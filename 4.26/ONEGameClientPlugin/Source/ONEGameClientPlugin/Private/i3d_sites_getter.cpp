// Copyright i3D.net, 2021. All Rights Reserved.

#include "i3d_sites_getter.h"

#include "i3d_ip_list.h"

#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Runtime/Online/HTTP/Public/Http.h"

#include <one/ping/c_error.h>

#include <string>

using namespace i3d_ping_integration;

AI3dSitesGetter::AI3dSitesGetter()
    : _i3d_sites_getter()
    , _quiet(true)
    , _is_initialized(false)
    , _is_ready(false)
    , _parsing_callback(nullptr)
    , _parsing_userdata(nullptr) {}

void AI3dSitesGetter::BeginDestroy() {
    //------------------------------------------------------------
    // Shutdown & proper memory deallocation.

    _i3d_sites_getter.shutdown();
    Super::BeginDestroy();
}

void AI3dSitesGetter::init() {
    //------------------------------------------------------------
    // Initialization of the sites getter.

    if (_is_initialized) {
        if (!_quiet)
            UE_LOG(LogTemp, Log,
                   TEXT("I3D SITES GETTER: is already initialized, skipping..."));
        return;
    }

    if (!_quiet) UE_LOG(LogTemp, Log, TEXT("I3D SITES GETTER: init"));

    //------------------------------------------------------------
    // Initialization.

    if (!_i3d_sites_getter.init()) {
        UE_LOG(LogTemp, Error, TEXT("I3D SITES GETTER: failed to init i3d sites getter"));
        return;
    }

    //------------------------------------------------------------
    // Setting callbacks.

    if (!_i3d_sites_getter.set_http_get_callback(&AI3dSitesGetter::http_request, this)) {
        UE_LOG(LogTemp, Error, TEXT("I3D SITES GETTER: failed to set http get callback"));
        return;
    }

    //------------------------------------------------------------
    // Initialization last checks for valid callbacks.

    if (!_i3d_sites_getter.init_http_callback()) {
        UE_LOG(LogTemp, Error, TEXT("I3D SITES GETTER: failed to init http callback for i3d sites getter"));
        return;
    }

    _is_initialized =
        (_i3d_sites_getter.status() == I3dSitesGetterWrapper::Status::initialized);

    if (!_is_initialized) {
        UE_LOG(LogTemp, Error, TEXT("I3D SITES GETTER: failed to init i3d sites getter"));
        return;
    }
}

void AI3dSitesGetter::update() {
    //------------------------------------------------------------
    // Updates: sening out pings.

    if (!_is_initialized) {
        return;
    }

    _i3d_sites_getter.update(_quiet);
}

void AI3dSitesGetter::shutdown() {
    //------------------------------------------------------------
    // Shutdown the sites getter.

    if (!_quiet) UE_LOG(LogTemp, Log, TEXT("I3D SITES GETTER: shutdown"));
    _i3d_sites_getter.shutdown();
}

bool AI3dSitesGetter::is_ready() const {
    return (_i3d_sites_getter.status() == I3dSitesGetterWrapper::Status::ready);
}

int64 AI3dSitesGetter::sites_size() const {
    unsigned int size = 0;
    if (!_i3d_sites_getter.sites_size(size)) {
        UE_LOG(LogTemp, Error, TEXT("I3D SITES GETTER: failed to get sites getter size"));
        return 0;
    }
    return size;
}

int64 AI3dSitesGetter::site_continent_id(int64 pos) {
    int site_continent_id = 0;
    if (!_i3d_sites_getter.site_continent_id(pos, site_continent_id)) {
        UE_LOG(LogTemp, Error, TEXT("I3D SITES GETTER: failed to get site continent id"));
        return 0;
    }
    return site_continent_id;
}

FString AI3dSitesGetter::site_country(int64 pos) {
    std::string site_country;
    if (!_i3d_sites_getter.site_country(pos, site_country)) {
        UE_LOG(LogTemp, Error, TEXT("I3D SITES GETTER: failed to get site country"));
        return FString("");
    }
    FString s(site_country.c_str());
    return s;
}

int64 AI3dSitesGetter::site_dc_location_id(int64 pos) {
    int site_dc_location_id = 0;
    if (!_i3d_sites_getter.site_dc_location_id(pos, site_dc_location_id)) {
        UE_LOG(LogTemp, Error,
               TEXT("I3D SITES GETTER: failed to get site dc location id"));
        return 0;
    }
    return site_dc_location_id;
}

FString AI3dSitesGetter::site_dc_location_name(int64 pos) {
    std::string site_dc_location_name;
    if (!_i3d_sites_getter.site_dc_location_name(pos, site_dc_location_name)) {
        UE_LOG(LogTemp, Error,
               TEXT("I3D SITES GETTER: failed to get site dc location name"));
        return FString("");
    }
    FString s(site_dc_location_name.c_str());
    return s;
}

FString AI3dSitesGetter::site_hostname(int64 pos) {
    std::string site_hostname;
    if (!_i3d_sites_getter.site_hostname(pos, site_hostname)) {
        UE_LOG(LogTemp, Error,
               TEXT("I3D SITES GETTER: failed to get site dc site hostname"));
        return FString("");
    }
    FString s(site_hostname.c_str());
    return s;
}

int64 AI3dSitesGetter::site_ipv4_size(int64 pos) {
    unsigned int site_ipv4_size = 0;
    if (!_i3d_sites_getter.site_ipv4_size(pos, site_ipv4_size)) {
        UE_LOG(LogTemp, Error, TEXT("I3D SITES GETTER: failed to get site ipv4 size"));
        return 0;
    }
    return site_ipv4_size;
}

FString AI3dSitesGetter::site_ipv4(int64 pos, int64 ip_pos) {
    std::string site_ipv4;
    if (!_i3d_sites_getter.site_ipv4(pos, ip_pos, site_ipv4)) {
        UE_LOG(LogTemp, Error, TEXT("I3D SITES GETTER: failed to get site site ipv4"));
        return FString("");
    }
    FString s(site_ipv4.c_str());
    return s;
}

int64 AI3dSitesGetter::site_ipv6_size(int64 pos) {
    unsigned int site_ipv6_size = 0;
    if (!_i3d_sites_getter.site_ipv6_size(pos, site_ipv6_size)) {
        UE_LOG(LogTemp, Error, TEXT("I3D SITES GETTER: failed to get site ipv6 size"));
        return 0;
    }
    return site_ipv6_size;
}

FString AI3dSitesGetter::site_ipv6(int64 pos, int64 ip_pos) {
    std::string site_ipv6;
    if (!_i3d_sites_getter.site_ipv6(pos, ip_pos, site_ipv6)) {
        UE_LOG(LogTemp, Error, TEXT("I3D SITES GETTER: failed to get site site ipv4"));
        return FString("");
    }
    FString s(site_ipv6.c_str());
    return s;
}

UI3dIpList *AI3dSitesGetter::ipv4_list() const {
    UI3dIpList *list = NewObject<UI3dIpList>();
    if (!_i3d_sites_getter.ipv4_list(list->list())) {
        UE_LOG(LogTemp, Error, TEXT("I3D SITES GETTER: failed to get ipv4 list"));
        return 0;
    }
    return list;
}

UI3dIpList *AI3dSitesGetter::ipv6_list() const {
    UI3dIpList *list = NewObject<UI3dIpList>();
    if (!_i3d_sites_getter.ipv6_list(list->list())) {
        UE_LOG(LogTemp, Error, TEXT("I3D SITES GETTER: failed to get ipv6 list"));
        return 0;
    }
    return list;
}

void AI3dSitesGetter::http_request(const char *url,
                                   void (*parsing_callback)(bool success,
                                                            const char *json,
                                                            void *parsing_userdata),
                                   void *parsing_userdata, void *http_get_metadata) {
    auto p = (AI3dSitesGetter *)(http_get_metadata);
    if (p == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("I3D SITES GETTER: http get metadata is nullptr"));
        return;
    }

    auto HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb("GET");
    HttpRequest->SetURL(*FString::Printf(TEXT("%s"), *FString(url)));
    p->_parsing_callback = parsing_callback;
    p->_parsing_userdata = parsing_userdata;
    HttpRequest->OnProcessRequestComplete().BindUObject(
        p, &AI3dSitesGetter::on_http_completed);
    HttpRequest->ProcessRequest();
}

void AI3dSitesGetter::on_http_completed(FHttpRequestPtr Request,
                                        FHttpResponsePtr Response, bool success) {
    if (success) {
        _parsing_callback(success, TCHAR_TO_ANSI(*Response->GetContentAsString()),
                          _parsing_userdata);
    } else {
        _parsing_callback(false, "", _parsing_userdata);
    }
}
