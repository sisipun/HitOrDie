#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"

#include "SoundEmitter.generated.h"

class AHitterController;
class UAudioComponent;
class UPlayer;
class USoundBase;

UENUM(BlueprintType)
enum class EActionType : uint8
{
	NONE UMETA(DisplayName = "None"),
	FIRE UMETA(DisplayName = "Fire"),
	GRENADE UMETA(DisplayName = "Grenade"),
	RELOAD UMETA(DisplayName = "Reload")
};

USTRUCT(BlueprintType)
struct FTiming
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StartSecond;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EndSecond;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
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

USTRUCT(BlueprintType)
struct FHitterActionIndex 
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Index;
};

UCLASS()
class HITORDIE_API ASoundEmitter : public AActor
{
	GENERATED_BODY()
	
private:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartSound();

	UFUNCTION()
	void Auth_OnCountdown();

	UFUNCTION()
	void Auth_OnAudioPlaybackPercent(const USoundWave* PlayingSoundWave, const float PlaybackPercent);

	UFUNCTION()
	void OnRep_HitterActionIndices();

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

	UPROPERTY(ReplicatedUsing = OnRep_HitterActionIndices, BlueprintReadOnly, Category = Timing)
	TArray<FHitterActionIndex> HitterActionIndices;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Countdown)
	int32 CountdownLength;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Countdown)
	int32 CountdownCurrentValue;

public:	
	ASoundEmitter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void Auth_Play();

	bool Auth_PerformAction(TObjectPtr<AHitterController> Hitter, EActionType Action);
	
	TArray<FTiming> GetPossibleActions(TObjectPtr<AHitterController> Hitter, float PeriodBefore, float PeriodAfter) const;
	
	float GetPlaybackValue() const;

	int32 GetCountdownValue() const;

protected:
	virtual void BeginPlay() override; 

private:
	void SyncActionIndex(FString Name, int32 Index);

private:
	FTimerHandle CountdownTimer;
	TMap<FString, int32> HitterToActionIndex;
};
