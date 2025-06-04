// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

UCLASS()
class TD_PROJECT_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Settings")
    int32 NumColumns = 20;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Settings")
    int32 NumRows = 20;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Settings")  
    float CellSize = 250;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Settings")
    FVector GridOrigin = FVector(-2500, -2500, 0);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid State")
    TArray<bool> GridOccupied;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid State")
    TArray<AActor*> GridWallActors;

    UFUNCTION(BlueprintCallable, Category = "Grid Functions")
    FIntPoint WorldToGrid(const FVector& WorldLocation) const;

    UFUNCTION(BlueprintCallable, Category = "Grid Functions")
    FVector GridToWorld(int32 GridX, int32 GridY) const;

    UFUNCTION(BlueprintCallable, Category = "Grid Functions")
    bool IsValidGridIndex(int32 GridX, int32 GridY) const;

    UFUNCTION(BlueprintCallable, Category = "Grid Functions")
    void InitializeGrid();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid Visualization")
    FIntPoint SelectedCell = FIntPoint(-1, -1);

    void DrawGrid();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Settings")
    TSubclassOf<class AWall> WallClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Settings")
    int32 WallLength = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid Settings")
    bool bPlacementHorizontal = true;

    UFUNCTION(BlueprintCallable, Category = "Grid Functions")
    bool TryPlaceWallAtLocation(const FVector& WorldLocation);

    bool PlaceWallAtGrid(int32 GridX, int32 GridY);
};
