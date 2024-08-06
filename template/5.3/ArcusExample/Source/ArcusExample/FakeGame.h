// Copyright i3D.net, 2021. All Rights Reserved.

#pragma once

#include <one_arcus_array.h>
#include <one_arcus_object.h>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FakeGame.generated.h"

UCLASS(Blueprintable)
class ARCUSEXAMPLE_API AFakeGame : public AActor {
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AFakeGame();

    // Set quiet to true, in order to the logging off and vice versa.
    UFUNCTION(BlueprintCallable, Category = "ArcusExample")
    void set_game_quiet(bool quiet) {
        _quiet = quiet;
    }

    // To parse the command line for the option '-ManagementPort <port>.
    // If not found, it defaults on the provided default value.
    UFUNCTION(BlueprintCallable, Category = "ArcusExample")
    int64 parse_command_line_management_port(int64 default_value);

    // -------------------------------
    // Message parsing functions

    // Optional user defined payload parsing. This implementation
    // match the example found in the documentation.
    UFUNCTION(BlueprintCallable, Category = "ArcusExample")
    void allocatedPayloadParse(UOneArcusArray *array);

    // Optional user defined payload parsing. This implementation
    // match the example found in the documentation.
    UFUNCTION(BlueprintCallable, Category = "ArcusExample")
    void metaDataPayloadParse(UOneArcusArray *array);

    // Parsing the host information payload. This implementation
    // only parse for some fields of the payload for simplicity.
    UFUNCTION(BlueprintCallable, Category = "ArcusExample")
    void hostInformationPayloadParse(UOneArcusObject *object);

    // Parsing the application instance information payload.
    // This implementation only parse for some fields of the
    // payload for simplicity.
    UFUNCTION(BlueprintCallable, Category = "ArcusExample")
    void applicationInstanceInformationPayloadParse(UOneArcusObject *object);

    // Parsing the custom command payload.
    // This implementation match the example found in the documentation.
    UFUNCTION(BlueprintCallable, Category = "ArcusExample")
    void customCommandPayloadParse(UOneArcusArray *array);

    // Message parsing functions
    // -------------------------------

    // -------------------------------
    // Events for received messages with their Unreal basic types.

    UFUNCTION(BlueprintImplementableEvent, Category = "ArcusExample")
    void allocatedExtracted(int32 players, int32 duration);

    UFUNCTION(BlueprintImplementableEvent, Category = "ArcusExample")
    void metaDataExtracted(const FString &map, const FString &mode, const FString &type);

    UFUNCTION(BlueprintImplementableEvent, Category = "ArcusExample")
    void hostInformationExtracted(int32 id, int32 serverId, const FString &serverName);

    UFUNCTION(BlueprintImplementableEvent, Category = "ArcusExample")
    void applicationInstanceInformationExtracted(const FString &fleet_id, int32 host_id,
                                                 bool is_virtual);

    UFUNCTION(BlueprintImplementableEvent, Category = "ArcusExample")
    void customCommandExtracted(const FString &command, const FString &argument);

    // Events for received messages with their Unreal basic types.
    // -------------------------------

protected:
    bool _quiet;
};
