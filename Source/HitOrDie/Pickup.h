#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class HITORDIE_API APickup : public AActor
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	virtual void Auth_OnOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void Auth_PickedUp(AHitter* Hitter);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<USphereComponent> Collider;

	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<UStaticMeshComponent> Mesh;

public:	
	APickup();

protected:
	virtual void BeginPlay() override;

};
