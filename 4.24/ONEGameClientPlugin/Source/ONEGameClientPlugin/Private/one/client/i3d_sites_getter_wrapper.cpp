// Copyright i3D.net, 2021. All Rights Reserved.

#include <one/client/i3d_sites_getter_wrapper.h>

#include <assert.h>
#include <string>
#include <cstring>

#include <one/ping/c_api.h>
#include <one/ping/c_error.h>

#include <CoreMinimal.h>

namespace i3d_ping_integration {
namespace {

// Local cached memory function overrides, to assist in override pattern.
std::function<void *(size_t)> _sites_getter_alloc = nullptr;
std::function<void(void *)> _sites_getter_free = nullptr;
std::function<void *(void *, size_t)> _sites_getter_realloc = nullptr;

// Logger to pass into One.
void sites_getter_log(void *userdata, I3dPingLogLevel level, const char *message) {
    if (userdata == nullptr) {
        return;
    }

    auto wrapper = reinterpret_cast<I3dSitesGetterWrapper *>(userdata);

    if (wrapper->quiet()) {
        return;
    }

    switch (level) {
        case I3D_PING_LOG_LEVEL_INFO:
            UE_LOG(LogTemp, Log, TEXT("ONE CLIENT: %s"), *FString(message));
            break;
        case I3D_PING_LOG_LEVEL_ERROR:
            UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"), *FString(message));
            break;
        default:
            break;
    }
}

}  // namespace

I3dSitesGetterWrapper::I3dSitesGetterWrapper() : _sites_getter(nullptr), _quiet(true) {}

I3dSitesGetterWrapper::~I3dSitesGetterWrapper() {
    shutdown();
}

bool I3dSitesGetterWrapper::init(void (*callback)(const char *url,
                                                  void (*)(bool success, const char *json, void *parsing_userdata),
                                                  void *parsing_userdata, void *http_get_metadata),
                                 void *userdata) {
    const std::lock_guard<std::mutex> lock(_wrapper);

    if (_sites_getter != nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: already initialized"));
        return false;
    }

    //-----------------------
    // Create the i3D Ping Sites.

    I3dPingError err = i3d_ping_sites_getter_create(&_sites_getter, callback, userdata);
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    // Set custom logger - optional.
    err = i3d_ping_sites_getter_set_logger(
        _sites_getter, sites_getter_log,
        nullptr);  // null userdata as global log is used.
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("I3dPingSitesWrapper init complete"));
    return true;
}

void I3dSitesGetterWrapper::shutdown() {
    const std::lock_guard<std::mutex> lock(_wrapper);

    if (_sites_getter == nullptr) {
        return;
    }

    // Free all objects created via the ONE API. This also shuts down the server
    // first, ending any active connection to it.
    i3d_ping_sites_getter_destroy(_sites_getter);
    _sites_getter = nullptr;
}

void I3dSitesGetterWrapper::update(bool quiet) {
    const std::lock_guard<std::mutex> lock(_wrapper);
    assert(_sites_getter != nullptr);

    I3dPingError err = i3d_ping_sites_getter_update(_sites_getter);
    if (i3d_ping_is_error(err)) {
        if (!quiet)
            UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
                   *FString(i3d_ping_error_text(err)));
        return;
    }
}

std::string I3dSitesGetterWrapper::status_to_string(Status status) {
    switch (status) {
        case Status::uninitialized:
            return "uninitialized";
        case Status::initialized:
            return "initialized";
        case Status::ready:
            return "ready";
        case Status::error:
            return "error";
        default:
            return "unknown";
    }
}

bool I3dSitesGetterWrapper::quiet() const {
    const std::lock_guard<std::mutex> lock(_wrapper);
    return _quiet;
}

I3dSitesGetterWrapper::Status I3dSitesGetterWrapper::status() const {
    const std::lock_guard<std::mutex> lock(_wrapper);

    I3dSitesGetterStatus status;
    I3dPingError err = i3d_ping_sites_getter_status(_sites_getter, &status);
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return Status::unknown;
    }
    switch (status) {
        case I3D_SITES_GETTER_STATUS_UNINITIALIZED:
            return Status::uninitialized;
        case I3D_SITES_GETTER_STATUS_INITIALIZED:
            return Status::initialized;
        case I3D_SITES_GETTER_STATUS_READY:
            return Status::ready;
        case I3D_SITES_GETTER_STATUS_ERROR:
            return Status::error;
        default:
            return Status::unknown;
    }
}

bool I3dSitesGetterWrapper::sites_size(unsigned int &size) const {
    const std::lock_guard<std::mutex> lock(_wrapper);

    unsigned int s = 0;
    I3dPingError err = i3d_ping_sites_getter_site_list_size(_sites_getter, &s);
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    size = static_cast<size_t>(s);
    return true;
}

bool I3dSitesGetterWrapper::site_continent_id(unsigned int pos, int &continent_id) {
    const std::lock_guard<std::mutex> lock(_wrapper);

    I3dPingError err =
        i3d_ping_sites_getter_list_site_continent_id(_sites_getter, pos, &continent_id);
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    return true;
}

bool I3dSitesGetterWrapper::site_country(unsigned int pos, std::string &country) {
    const std::lock_guard<std::mutex> lock(_wrapper);

    country.clear();
    country.reserve(64);
    I3dPingError err =
        i3d_ping_sites_getter_list_site_country(_sites_getter, pos, &(country[0]));
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    return true;
}

bool I3dSitesGetterWrapper::site_dc_location_id(unsigned int pos, int &dc_location_id) {
    const std::lock_guard<std::mutex> lock(_wrapper);

    I3dPingError err = i3d_ping_sites_getter_list_site_dc_location_id(_sites_getter, pos,
                                                                      &dc_location_id);
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    return true;
}

bool I3dSitesGetterWrapper::site_dc_location_name(unsigned int pos,
                                                  std::string &dc_location_name) {
    const std::lock_guard<std::mutex> lock(_wrapper);

    dc_location_name.clear();
    dc_location_name.resize(64);
    I3dPingError err = i3d_ping_sites_getter_list_site_dc_location_name(_sites_getter, pos,
                                                                        &(dc_location_name[0]));
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    return true;
}

bool I3dSitesGetterWrapper::site_hostname(unsigned int pos, std::string &hostname) {
    const std::lock_guard<std::mutex> lock(_wrapper);

    hostname.clear();
    hostname.resize(64);
    I3dPingError err = i3d_ping_sites_getter_list_site_hostname(_sites_getter, pos, &(hostname[0]));
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    return true;
}

bool I3dSitesGetterWrapper::site_ipv4_size(unsigned int pos, unsigned int &size) {
    const std::lock_guard<std::mutex> lock(_wrapper);

    unsigned int s = 0;
    I3dPingError err = i3d_ping_sites_getter_list_site_ipv4_size(_sites_getter, pos, &s);
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    size = static_cast<size_t>(s);
    return true;
}

bool I3dSitesGetterWrapper::site_ipv4(unsigned int pos, unsigned int ip_pos,
                                      std::string &ip) {
    const std::lock_guard<std::mutex> lock(_wrapper);

    ip.clear();
    ip.reserve(16);
    I3dPingError err = i3d_ping_sites_getter_list_site_ipv4_ip(_sites_getter, pos, ip_pos, &(ip[0]));
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    return true;
}

bool I3dSitesGetterWrapper::site_ipv6_size(unsigned int pos, unsigned int &size) {
    const std::lock_guard<std::mutex> lock(_wrapper);

    unsigned int s = 0;
    I3dPingError err = i3d_ping_sites_getter_list_site_ipv6_size(_sites_getter, pos, &s);
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    size = static_cast<size_t>(s);
    return true;
}

bool I3dSitesGetterWrapper::site_ipv6(unsigned int pos, unsigned int ip_pos,
                                      std::string &ip) {
    const std::lock_guard<std::mutex> lock(_wrapper);

    ip.clear();
    ip.reserve(46);
    I3dPingError err = i3d_ping_sites_getter_list_site_ipv6_ip(_sites_getter, pos, ip_pos, &(ip[0]));
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    return true;
}

bool I3dSitesGetterWrapper::ipv4_list(I3dIpListPtr ip_list) const {
    const std::lock_guard<std::mutex> lock(_wrapper);

    I3dPingError err = i3d_ping_sites_getter_ipv4_list(_sites_getter, ip_list);
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    return true;
}
bool I3dSitesGetterWrapper::ipv6_list(I3dIpListPtr ip_list) const {
    const std::lock_guard<std::mutex> lock(_wrapper);

    I3dPingError err = i3d_ping_sites_getter_ipv6_list(_sites_getter, ip_list);
    if (i3d_ping_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE CLIENT: %s"),
               *FString(i3d_ping_error_text(err)));
        return false;
    }

    return true;
}

}  // namespace i3d_ping_integration
