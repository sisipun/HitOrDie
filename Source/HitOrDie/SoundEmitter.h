#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"

#include "SoundEmitter.generated.h"

class UAudioComponent;
class USoundBase;

UENUM(BlueprintType)
enum class EActionType : uint8
{
	NONE UMETA(DisplayName = "None"),
	FIRE UMETA(DisplayName = "Fire"),
	RELOAD UMETA(DisplayName = "Reload")
};

USTRUCT(BlueprintType)
struct FTiming
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, Category = Timing)
	float StartSecond;

	UPROPERTY(EditAnywhere, Category = Timing)
	EActionType Action;
};

USTRUCT(BlueprintType)
struct FSoundProperties : public FTableRowBase
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = Timing)
	TArray<FTiming> ActionTimings;

	UPROPERTY(EditAnywhere, Category = Timing)
	float ActionLenght;
};

UCLASS()
class HITORDIE_API ASoundEmitter : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category = Audio)
	TObjectPtr<UAudioComponent> Audio;

	UPROPERTY(EditAnywhere, Category = Audio)
	TObjectPtr<UDataTable> SoundPropertiesDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	FName CurrentSound;

public:	
	ASoundEmitter();

	EActionType GetPossibleAction() const;

protected:
	virtual void BeginPlay() override; 

private:
	UFUNCTION() 
	void OnAudioPlaybackPercent(const USoundWave* PlayingSoundWave, const float PlaybackPercent);

private:
	float ActionLenght;
	TArray<FTiming> ActionTimings;

	float PlaybackValue;
};
