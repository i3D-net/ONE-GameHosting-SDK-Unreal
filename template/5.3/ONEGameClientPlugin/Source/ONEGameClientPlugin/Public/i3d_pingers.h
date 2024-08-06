// Copyright i3D.net, 2021. All Rights Reserved.

#pragma once

#include <one/client/i3d_pingers_wrapper.h>
#include <i3d_ip_list.h>

#include "GameFramework/Actor.h"
#include <CoreMinimal.h>

#include "i3d_pingers.generated.h"

UCLASS(Blueprintable)
class ONEGAMECLIENTPLUGIN_API AI3dPingers : public AActor {
    GENERATED_BODY()

public:
    AI3dPingers();

    virtual void BeginDestroy() override;

    // Set quiet to true, in order to the logging off and vice versa.
    UFUNCTION(BlueprintCallable, Category = "Pinger")
    void set_quiet(bool quiet) {
        _quiet = quiet;
    }

    // -------------------------------
    // Life cycle

    // Init must be called first to initialized the pinger.
    UFUNCTION(BlueprintCallable, Category = "Pinger")
    void init(UI3dIpList *ip_list);

    // Update can be called at each frame in order ping the sites with the pinger.
    UFUNCTION(BlueprintCallable, Category = "Pinger")
    void update();

    // Shutdown must be called last to de-initialized the pinger.
    UFUNCTION(BlueprintCallable, Category = "Pinger")
    void shutdown();

    // Life cycle
    // -------------------------------

    // Returns true if the pinger has been initialized.
    UFUNCTION(BlueprintCallable, Category = "Pinger")
    bool is_initialized() const {
        return _is_initialized;
    }

    // Returns the number of pingers.
    UFUNCTION(BlueprintCallable, Category = "Pinger")
    int32 pingers_size() const;

    UFUNCTION(BlueprintCallable, Category = "Pinger")
    int32 last_time(int32 pos) const;

    UFUNCTION(BlueprintCallable, Category = "Pinger")
    float average_time(int32 pos) const;

    UFUNCTION(BlueprintCallable, Category = "Pinger")
    int32 min_time(int32 pos) const;

    UFUNCTION(BlueprintCallable, Category = "Pinger")
    int32 max_time(int32 pos) const;

    UFUNCTION(BlueprintCallable, Category = "Pinger")
    float median_time(int32 pos) const;

    UFUNCTION(BlueprintCallable, Category = "Pinger")
    int32 ping_response_count(int32 pos) const;

    UFUNCTION(BlueprintCallable, Category = "Pinger")
    bool at_least_one_site_has_been_pinged() const;

    UFUNCTION(BlueprintCallable, Category = "Pinger")
    bool all_sites_have_been_pinged() const;

protected:
    i3d_ping_integration::I3dPingersWrapper _i3d_pingers;

    bool _quiet;
    bool _is_initialized;
};
