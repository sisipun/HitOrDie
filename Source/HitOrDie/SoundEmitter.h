#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"

#include "SoundEmitter.generated.h"

class UAudioComponent;
class USoundBase;
class UPlayer;
class AHitterController;

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timing)
	float StartSecond;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timing)
	float EndSecond;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timing)
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
};

UCLASS()
class HITORDIE_API ASoundEmitter : public AActor
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void Auth_OnAudioPlaybackPercent(const USoundWave* PlayingSoundWave, const float PlaybackPercent);

protected:
	UPROPERTY(VisibleAnywhere, Category = Audio)
	TObjectPtr<UAudioComponent> Audio;

	UPROPERTY(EditAnywhere, Category = Audio)
	TObjectPtr<UDataTable> SoundPropertiesDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	FName CurrentSound;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Audio)
	float PlaybackValue;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Timing)
	TArray<FTiming> ActionTimings;

public:	
	ASoundEmitter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool Auth_PerformAction(UPlayer* Hitter, EActionType Action);
	
	TArray<FTiming> GetPossibleActions(AHitterController* Hitter, float PeriodBefore, float PeriodAfter) const;
	
	float GetPlaybackValue() const;

protected:
	virtual void BeginPlay() override; 

private:
	TMap<FString, int> HitterActionIndices;
};
