#pragma once

#include <CoreMinimal.h>

#include <one/game/game.h>

#include "OneArcusGame.generated.h"

UCLASS(Blueprintable)
class UOneArcusGame : public UObject 
{
	GENERATED_BODY()

public:
	UOneArcusGame();

	UFUNCTION(BlueprintCallable, Category = "Arcus")
	void init(int64 port, int64 max_players, FString name,
              FString map, FString mode, FString version,
              int64 delay);
	
	UFUNCTION(BlueprintCallable, Category = "Arcus")
	void update();

	UFUNCTION(BlueprintCallable, Category = "Arcus")
	void shutdown();

protected:
	one_integration::Game _game;
};
