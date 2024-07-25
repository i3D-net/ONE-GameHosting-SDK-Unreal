// Copyright i3D.net, 2021. All Rights Reserved.

#pragma once

#include <one/server/one_server_wrapper.h>
#include <one_arcus_object.h>

#include "GameFramework/Actor.h"
#include <CoreMinimal.h>

#include "one_arcus_server.generated.h"

UCLASS(Blueprintable)
class ONEGAMEHOSTINGPLUGIN_API AOneArcusServer : public AActor {
    GENERATED_BODY()

public:
    AOneArcusServer();

    virtual void BeginDestroy() override;

    // Set quiet to true, in order to the logging off and vice versa.
    UFUNCTION(BlueprintCallable, Category = "Arcus")
    void set_quiet(bool quiet) {
        _quiet = quiet;
    }

    // -------------------------------
    // Life cycle

    // Init must be called first to initialized the server.
    UFUNCTION(BlueprintCallable, Category = "Arcus")
    void init(int64 port, int64 players, int64 max_players, FString name, FString map,
              FString mode, FString version);

    // Update can be called at each frame in order to poll for incoming messages and send
    // outgoing messages.
    UFUNCTION(BlueprintCallable, Category = "Arcus")
    void update();

    // Shutdown must be called last to de-initialized the server.
    UFUNCTION(BlueprintCallable, Category = "Arcus")
    void shutdown();

    // Retrieve the server status and return true if the server has reached 
    // the ready-state (is connected to the agent).
    UFUNCTION(BlueprintCallable, Category = "Arcus")
    bool is_ready();
    
    // Life cycle
    // -------------------------------

    // -------------------------------
    // Game state

    // Update the current game state.
    UFUNCTION(BlueprintCallable, Category = "Arcus")
    void set_game_state(int64 players, int64 max_players, FString name, FString map,
                        FString mode, FString version);

    // Set the application instance status to starting.
    UFUNCTION(BlueprintCallable, Category = "Arcus")
    void set_application_instance_starting();

    // Set the application instance status to online.
    // The status is online when the application is started but not yet allocated.
    UFUNCTION(BlueprintCallable, Category = "Arcus")
    void set_application_instance_online();

    // Set the application instance status to allocated.
    // The status is allocated when the application is started and allocated.
    UFUNCTION(BlueprintCallable, Category = "Arcus")
    void set_application_instance_allocated();

    // Send the reverse metadata.
    UFUNCTION(BlueprintCallable, Category = "Arcus")
    void send_reverse_metadata(FString map, FString mode, FString type);

    // Game state
    // -------------------------------

    // -------------------------------
    // Events for received messages with their Arcus SDK types

    UFUNCTION(BlueprintImplementableEvent, Category = "Arcus")
    void softStopReceived(int32 timeout);

    UFUNCTION(BlueprintImplementableEvent, Category = "Arcus")
    void allocatedReceived(UOneArcusArray *payload);

    UFUNCTION(BlueprintImplementableEvent, Category = "Arcus")
    void metaDataReceived(UOneArcusArray *payload);

    UFUNCTION(BlueprintImplementableEvent, Category = "Arcus")
    void hostInformationReceived(UOneArcusObject *payload);

    UFUNCTION(BlueprintImplementableEvent, Category = "Arcus")
    void applicationInstanceInformationReceived(UOneArcusObject *payload);

    UFUNCTION(BlueprintImplementableEvent, Category = "Arcus")
    void customCommandReceived(UOneArcusArray *payload);

    // Events for received messages with their Arcus SDK types
    // -------------------------------

    // -------------------------------
    // Server callbacks
    // So that the UOneArcusServer can trigger events
    // when a message is received.
    static void soft_stop_callback(int timeout, void *userdata);
    static void allocated_callback(OneArrayPtr data, void *userdata);
    static void metadata_callback(OneArrayPtr data, void *userdata);
    static void host_information_callback(OneObjectPtr data, void *userdata);
    static void application_instance_information_callback(OneObjectPtr data,
                                                          void *userdata);
    static void custom_command_callback(OneArrayPtr data, void *userdata);

    // Server callbacks
    // -------------------------------

protected:
    one_integration::OneServerWrapper _one_server;

    bool _quiet;
    bool _is_initialized;
};
