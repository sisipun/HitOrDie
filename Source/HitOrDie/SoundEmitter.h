#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "SoundEmitter.generated.h"

class UAudioComponent;
class USoundBase;

USTRUCT()
struct FSoundProperties
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = Timing)
	TMap<float, bool> ActionTimings;

	UPROPERTY(EditAnywhere, Category = Timing)
	float ActionLenght;
};

UCLASS()
class HITORDIE_API ASoundEmitter : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = Audio)
	TObjectPtr<UAudioComponent> Audio;

	UPROPERTY(EditAnywhere, Category = Audio)
	FSoundProperties SoundProperties;

public:	
	ASoundEmitter();

	bool GetPossibleAction() const;

protected:
	virtual void BeginPlay() override; 

private:
	UFUNCTION() 
	void OnAudioPlaybackPercent(const USoundWave* PlayingSoundWave, const float PlaybackPercent);

private:
	bool CanFire;
};
