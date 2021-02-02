#pragma once

#include <one/server/one_server_wrapper.h>
#include <one_arcus_object.h>

#include "GameFramework/Actor.h"
#include <CoreMinimal.h>

#include "one_arcus_server.generated.h"

UCLASS(Blueprintable)
class AOneArcusServer : public AActor {
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

    // Life cycle
    // -------------------------------

    // -------------------------------
    // Game state

    // Update the current game state.
    UFUNCTION(BlueprintCallable, Category = "Arcus")
    void set_game_state(int64 players, int64 max_players, FString name, FString map,
                        FString mode, FString version);

    // Game state
    // -------------------------------

    // -------------------------------
    // Message parsing functions

    // Optional user defined payload parsing. This implementation
    // match the example found in the documentation.
    UFUNCTION(BlueprintCallable, Category = "Arcus")
    void allocatedPayloadParse(UOneArcusArray *array);

    // Optional user defined payload parsing. This implementation
    // match the example found in the documentation.
    UFUNCTION(BlueprintCallable, Category = "Arcus")
    void metaDataPayloadParse(UOneArcusArray *array);

    // Parsing the host information payload. This implementation
    // only parse for some fields of the payload for simplicity.
    UFUNCTION(BlueprintCallable, Category = "Arcus")
    void hostInformationPayloadParse(UOneArcusObject *object);

    // Parsing the application instance information payload.
    // This implementation only parse for some fields of the
    // payload for simplicity.
    UFUNCTION(BlueprintCallable, Category = "Arcus")
    void applicationInstanceInformationPayloadParse(UOneArcusObject *object);

    // Message parsing functions
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

    // Events for received messages with their Arcus SDK types
    // -------------------------------

    // -------------------------------
    // Events for received messages with their Unreal basic types.

    UFUNCTION(BlueprintImplementableEvent, Category = "Arcus")
    void allocatedExtracted(int32 players, int32 duration);

    UFUNCTION(BlueprintImplementableEvent, Category = "Arcus")
    void metaDataExtracted(const FString &map, const FString &mode, const FString &type);

    UFUNCTION(BlueprintImplementableEvent, Category = "Arcus")
    void hostInformationExtracted(int32 id, int32 serverId, const FString &serverName);

    UFUNCTION(BlueprintImplementableEvent, Category = "Arcus")
    void applicationInstanceInformationExtracted(const FString &fleet_id, int32 host_id,
                                                 bool is_virtual);

    // Events for received messages with their Unreal basic types.
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

    // Server callbacks
    // -------------------------------

protected:
    one_integration::OneServerWrapper _one_server;

    bool _quiet;
    bool _is_init;
};
