#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "SoundEmitter.generated.h"

class UAudioComponent;

UCLASS()
class HITORDIE_API ASoundEmitter : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = Audio)
	TObjectPtr<UAudioComponent> Audio;

public:	
	ASoundEmitter();

protected:
	virtual void BeginPlay() override;

};
