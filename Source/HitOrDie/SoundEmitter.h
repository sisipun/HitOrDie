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
	int Index;
};

UCLASS()
class HITORDIE_API ASoundEmitter : public AActor
{
	GENERATED_BODY()
	
private:
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

public:	
	ASoundEmitter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool Auth_PerformAction(TObjectPtr<AHitterController> Hitter, EActionType Action);
	
	TArray<FTiming> GetPossibleActions(TObjectPtr<AHitterController> Hitter, float PeriodBefore, float PeriodAfter) const;
	
	float GetPlaybackValue() const;

protected:
	virtual void BeginPlay() override; 

private:
	void SyncActionIndex(FString Name, int Index);

private:
	TMap<FString, int> HitterToActionIndex;
};
