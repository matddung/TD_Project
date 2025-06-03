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
        UE_LOG(LogTemp, Warning, TEXT("SetViewTarget"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("TDPlayerController: GridManager not found"));
    }
}

void ATDPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("LeftClick", IE_Pressed, this, &ATDPlayerController::OnLeftMouseClick);

    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridManager::StaticClass(), Found);
    if (Found.Num() > 0)
    {
        GridManagerRef = Cast<AGridManager>(Found[0]);
    }
}

void ATDPlayerController::OnLeftMouseClick()
{
    if (!GridManagerRef) return;

    FHitResult Hit;
    ETraceTypeQuery TraceQuery = UEngineTypes::ConvertToTraceType(ECC_Visibility);

    bool bHit = GetHitResultUnderCursorByChannel(TraceQuery, false, Hit);

    if (bHit && Hit.bBlockingHit)
    {
        FVector HitLocation = Hit.Location;
        GridManagerRef->UpdateSelectedCell(HitLocation);
    }
}