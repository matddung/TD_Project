#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wall.generated.h"

UCLASS()
class TD_PROJECT_API AWall : public AActor
{
	GENERATED_BODY()
	
public:	
	AWall();

protected:
	virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall")
    int32 WallLength = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall")
    bool bIsHorizontal = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* WallMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* TurretSocket;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid Info")
    int32 GridX = -1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid Info")
    int32 GridY = -1;

    void SetGridIndex(int32 InX, int32 InY) { GridX = InX; GridY = InY; }
};