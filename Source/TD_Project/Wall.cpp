#include "Wall.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

AWall::AWall()
{
	PrimaryActorTick.bCanEverTick = false;

    WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
    SetRootComponent(WallMesh);

    TurretSocket = CreateDefaultSubobject<USceneComponent>(TEXT("TurretSocket"));
    TurretSocket->SetupAttachment(WallMesh);

    TurretSocket->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
}

void AWall::BeginPlay()
{
	Super::BeginPlay();
	
    FVector NewScale = FVector::OneVector;
    FVector SocketOffset = FVector::ZeroVector;

    if (bIsHorizontal)
    {
        NewScale = FVector(WallLength * 2.5, 2.5f, 4.f);
        SocketOffset = FVector((WallLength * 50.f), 0.f, 50.f);
    }
    else
    {
        NewScale = FVector(2.5f, WallLength * 2.5f, 4.f);
        SocketOffset = FVector(0.f, (WallLength * 50.f), 50.f);
    }

    WallMesh->SetWorldScale3D(NewScale);
    TurretSocket->SetRelativeLocation(SocketOffset);
}