// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TD_PROJECT_API ATDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    void OnLeftMouseClick();

    UPROPERTY()
    class AGridManager* GridManagerRef;

    UPROPERTY()
    AActor* FixedCameraActor;
};
