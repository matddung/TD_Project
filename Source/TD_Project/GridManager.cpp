#include "GridManager.h"
#include "Wall.h"

#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"

AGridManager::AGridManager()
{
    PrimaryActorTick.bCanEverTick = true;

}

void AGridManager::BeginPlay()
{
    Super::BeginPlay();

    InitializeGrid();
}

void AGridManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawGrid();
}

void AGridManager::InitializeGrid()
{
    int32 TotalCells = NumColumns * NumRows;
    GridOccupied.Init(false, TotalCells);
    GridWallActors.Init(nullptr, TotalCells);
}

bool AGridManager::IsValidGridIndex(int32 GridX, int32 GridY) const
{
    return (GridX >= 0 && GridX < NumColumns && GridY >= 0 && GridY < NumRows);
}

FIntPoint AGridManager::WorldToGrid(const FVector& WorldLocation) const
{
    FVector Local = WorldLocation - GridOrigin;

    int32 X = FMath::FloorToInt(Local.X / CellSize);
    int32 Y = FMath::FloorToInt(Local.Y / CellSize);

    if (!IsValidGridIndex(X, Y))
    {
        return FIntPoint(-1, -1);
    }
    return FIntPoint(X, Y);
}

FVector AGridManager::GridToWorld(int32 GridX, int32 GridY) const
{
    if (!IsValidGridIndex(GridX, GridY))
    {
        return FVector::ZeroVector;
    }

    float WorldX = GridOrigin.X + GridX * CellSize + CellSize * 0.5f;
    float WorldY = GridOrigin.Y + GridY * CellSize + CellSize * 0.5f;
    float WorldZ = GridOrigin.Z;

    return FVector(WorldX, WorldY, WorldZ);
}

void AGridManager::DrawGrid()
{
    if (!GetWorld()) return;

    const float Z = GridOrigin.Z + 10.0f;

    for (int32 Col = 0; Col <= NumColumns; ++Col)
    {
        float X = GridOrigin.X + (Col * CellSize);
        FVector Start = FVector(X, GridOrigin.Y, Z);
        FVector End = FVector(X, GridOrigin.Y + NumRows * CellSize, Z);
        DrawDebugLine(GetWorld(), Start, End, FColor::Black, false, -1.f, 0, 2.f);
    }

    for (int32 Row = 0; Row <= NumRows; ++Row)
    {
        float Y = GridOrigin.Y + (Row * CellSize);
        FVector Start = FVector(GridOrigin.X, Y, Z);
        FVector End = FVector(GridOrigin.X + NumColumns * CellSize, Y, Z);
        DrawDebugLine(GetWorld(), Start, End, FColor::Black, false, -1.f, 0, 2.f);
    }
}

bool AGridManager::TryPlaceWallAtLocation(const FVector& WorldLocation)
{
    FIntPoint GridIdx = WorldToGrid(WorldLocation);

    if (GridIdx.X < 0 || GridIdx.Y < 0)
    {
        return false;
    }

    return PlaceWallAtGrid(GridIdx.X, GridIdx.Y);
}

bool AGridManager::PlaceWallAtGrid(int32 GridX, int32 GridY)
{
    if (!IsValidGridIndex(GridX, GridY)) return false;

    int32 FirstIndex = GridY * NumColumns + GridX;
    if (GridOccupied[FirstIndex]) return false;

    for (int32 Offset = 1; Offset < WallLength; Offset++)
    {
        int32 CheckX = GridX + Offset;
        int32 CheckY = GridY;

        if (!IsValidGridIndex(CheckX, CheckY)) return false;

        int32 CheckIndex = CheckY * NumColumns + CheckX;
        if (GridOccupied[CheckIndex]) return false;
    }

    if (!WallClass) return false;

    int32 CenterX = GridX;
    int32 CenterY = GridY;
    FVector CenterLocation = GridToWorld(CenterX, CenterY);

    CenterLocation.Z = 200;

    FActorSpawnParameters Params;
    Params.Owner = this;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AWall* NewWall = GetWorld()->SpawnActor<AWall>(WallClass, CenterLocation, FRotator::ZeroRotator, Params);
    if (!NewWall) return false;

    NewWall->SetGridIndex(GridX, GridY);
    NewWall->WallLength = WallLength;
    NewWall->bIsHorizontal = true;

    for (int32 Offset = 0; Offset < WallLength; ++Offset)
    {
        int32 OccupyX = GridX + Offset;
        int32 OccupyY = GridY;
        int32 Index = OccupyY * NumColumns + OccupyX;

        GridOccupied[Index] = true;
        GridWallActors[Index] = NewWall;
    }

    return true;
}