// Copyright i3D.net, 2021. All Rights Reserved.

#pragma once

#include <one/client/i3d_sites_getter_wrapper.h>
#include <i3d_ip_list.h>

#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include <CoreMinimal.h>

#include "i3d_sites_getter.generated.h"

UCLASS(Blueprintable)
class ONEGAMECLIENTPLUGIN_API AI3dSitesGetter : public AActor {
    GENERATED_BODY()

public:
    AI3dSitesGetter();

    virtual void BeginDestroy() override;

    // Set quiet to true, in order to the logging off and vice versa.
    UFUNCTION(BlueprintCallable, Category = "SitesGetter")
    void set_quiet(bool quiet) {
        _quiet = quiet;
    }

    // -------------------------------
    // Life cycle

    // Init must be called first to initialized the server.
    UFUNCTION(BlueprintCallable, Category = "SitesGetter")
    void init();

    // Update can be called at each frame in order to poll for incoming messages and send
    // outgoing messages.
    UFUNCTION(BlueprintCallable, Category = "SitesGetter")
    void update();

    // Shutdown must be called last to de-initialized the server.
    UFUNCTION(BlueprintCallable, Category = "SitesGetter")
    void shutdown();

    // Life cycle
    // -------------------------------

    UFUNCTION(BlueprintCallable, Category = "SitesGetter")
    bool is_ready() const;

    UFUNCTION(BlueprintCallable, Category = "SitesGetter")
    int64 sites_size() const;

    UFUNCTION(BlueprintCallable, Category = "SitesGetter")
    int64 site_continent_id(int64 pos);

    UFUNCTION(BlueprintCallable, Category = "SitesGetter")
    FString site_country(int64 pos);

    UFUNCTION(BlueprintCallable, Category = "SitesGetter")
    int64 site_dc_location_id(int64 pos);

    UFUNCTION(BlueprintCallable, Category = "SitesGetter")
    FString site_dc_location_name(int64 pos);

    UFUNCTION(BlueprintCallable, Category = "SitesGetter")
    FString site_hostname(int64 pos);

    UFUNCTION(BlueprintCallable, Category = "SitesGetter")
    int64 site_ipv4_size(int64 pos);

    UFUNCTION(BlueprintCallable, Category = "SitesGetter")
    FString site_ipv4(int64 pos, int64 ip_pos);

    UFUNCTION(BlueprintCallable, Category = "SitesGetter")
    int64 site_ipv6_size(int64 pos);

    UFUNCTION(BlueprintCallable, Category = "SitesGetter")
    FString site_ipv6(int64 pos, int64 ip_pos);

    UFUNCTION(BlueprintCallable, Category = "SitesGetter")
    UI3dIpList *ipv4_list() const;

    UFUNCTION(BlueprintCallable, Category = "SitesGetter")
    UI3dIpList *ipv6_list() const;

protected:
    static void http_request(const char *url,
                             void (*parsing_callback)(bool success, const char *json,
                                                      void *parsing_userdata),
                             void *parsing_userdata, void *http_get_metadata);
    void on_http_completed(FHttpRequestPtr Request, FHttpResponsePtr Response,
                           bool bWasSuccessful);

    i3d_ping_integration::I3dSitesGetterWrapper _i3d_sites_getter;

    bool _quiet;
    bool _is_initialized;
    bool _is_ready;

    void (*_parsing_callback)(bool success, const char *json, void *parsing_userdata);
    void *_parsing_userdata;
};
