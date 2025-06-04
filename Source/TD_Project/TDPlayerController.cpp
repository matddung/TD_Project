// Fill out your copyright notice in the Description page of Project Settings.


#include "TDPlayerController.h"
#include "GridManager.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

void ATDPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;

    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);

    TArray<AActor*> FoundManagers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridManager::StaticClass(), FoundManagers);
    if (FoundManagers.Num() > 0)
    {
        GridManagerRef = Cast<AGridManager>(FoundManagers[0]);
    }

    TArray<AActor*> FoundCameras;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundCameras);
    for (AActor* Actor : FoundCameras)
    {
        if (Actor->GetName().Contains(TEXT("CameraActor")))
        {
            FixedCameraActor = Actor;
            break;
        }
    }

    if (FixedCameraActor)
    {
        SetViewTarget(FixedCameraActor);
    }
}

void ATDPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("LeftClick", IE_Pressed, this, &ATDPlayerController::OnLeftMouseClick);
}

void ATDPlayerController::OnLeftMouseClick()
{
    if (!GridManagerRef) return;

    FHitResult Hit;
    ETraceTypeQuery TraceQuery = UEngineTypes::ConvertToTraceType(ECC_Visibility);
    bool bHit = GetHitResultUnderCursorByChannel(TraceQuery, false, Hit);

    if (bHit && Hit.bBlockingHit)
    {
        FVector HitLocation = Hit.ImpactPoint;

        bool bSuccess = GridManagerRef->TryPlaceWallAtLocation(HitLocation);

        if (bSuccess)
        {
            GEngine->AddOnScreenDebugMessage(
                -1, 1.5f, FColor::Green,
                TEXT("Wall placed successfully."));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(
                -1, 1.5f, FColor::Red,
                TEXT("Cannot place wall here."));
        }
    }
}