﻿// Copyright i3D.net, 2021. All Rights Reserved.

#pragma once

#include <functional>
#include <mutex>
#include <string>

//----------------------------------------------
// ONE SDK object types forward declarations

struct I3dIpList;
typedef I3dIpList *I3dIpListPtr;

struct I3dSitesGetter;
typedef I3dSitesGetter *I3dSitesGetterPtr;

namespace i3d_ping_integration {

/// I3dSitesGettertWrapper encapsulates the integration for the i3D SitesGetter and
/// provides a ping sites interface that hides the i3D Ping Client API implementation
/// from the game. Errors are handled directly in the implementation of the
/// wrapper.
///
/// This is provided as a potential copy-paste initial integration solution for
/// users that would like to hide the c-api under a game-specific C++ class, and
/// also as a complete example of an integration.
class I3dSitesGetterWrapper final {
public:
    I3dSitesGetterWrapper();
    I3dSitesGetterWrapper(const I3dSitesGetterWrapper &) = delete;
    I3dSitesGetterWrapper &operator=(const I3dSitesGetterWrapper &) = delete;
    ~I3dSitesGetterWrapper();

    //------------
    // Life cycle.

    struct AllocationHooks {
        // The hooks are optional, they can be left null.
        AllocationHooks() : alloc(nullptr), free(nullptr), realloc(nullptr) {}
        AllocationHooks(std::function<void *(size_t)> alloc_in,
                        std::function<void(void *)> free_in,
                        std::function<void *(void *, size_t)> realloc_in)
            : alloc(alloc_in), free(free_in), realloc(realloc_in) {}

        std::function<void *(size_t)> alloc;
        std::function<void(void *)> free;
        std::function<void *(void *, size_t)> realloc;
    };

    // alloc and free are optional allocation override handlers. Both may be nullptr,
    // otherwise both are required.
    bool init(const AllocationHooks &hooks);
    void shutdown();

    // Must called often (e.g. each frame) until the status change to Ready. That means
    // that the HTTP Get callback has succesfully finished.
    void update(bool quiet);

    enum class Status { uninitialized = 0, initialized, ready, error, unknown };
    static std::string status_to_string(Status status);
    Status status() const;

    bool quiet() const;

    bool set_http_get_callback(
        void (*callback)(const char *url,
                         void (*)(bool success, const char *json, void *parsing_userdata),
                         void *parsing_userdata, void *http_get_metadata),
        void *userdata);

    bool sites_size(unsigned int &size) const;

    bool site_continent_id(unsigned int pos, int &continent_id);
    bool site_country(unsigned int pos, std::string &country);
    bool site_dc_location_id(unsigned int pos, int &dc_location_id);
    bool site_dc_location_name(unsigned int pos, std::string &dc_location_name);
    bool site_hostname(unsigned int pos, std::string &hostname);
    bool site_ipv4_size(unsigned int pos, unsigned int &size);
    bool site_ipv4(unsigned int pos, unsigned int ip_pos, std::string &ip);
    bool site_ipv6_size(unsigned int pos, unsigned int &size);
    bool site_ipv6(unsigned int pos, unsigned int ip_pos, std::string &ip);

    bool ipv4_list(I3dIpListPtr ip_list) const;
    bool ipv6_list(I3dIpListPtr ip_list) const;

private:
    // The SitesGetter itself.
    mutable std::mutex _wrapper;
    I3dSitesGetterPtr _sites_getter;
    bool _quiet;
};

}  // namespace i3d_ping_integration
