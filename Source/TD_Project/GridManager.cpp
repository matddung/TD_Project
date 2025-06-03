#include "GridManager.h"
#include "Engine/EngineTypes.h" 

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
    DrawSelectedCell();
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

void AGridManager::UpdateSelectedCell(const FVector& WorldLocation)
{
    FIntPoint HitIdx = WorldToGrid(WorldLocation);
    if (IsValidGridIndex(HitIdx.X, HitIdx.Y))
    {
        SelectedCell = HitIdx;
    }
    else
    {
        SelectedCell = FIntPoint(-1, -1);
    }
}

void AGridManager::DrawGrid()
{
    if (!GetWorld()) return;

    const float Z = GridOrigin.Z + 1.0f;
    const FVector Color = FVector(1.0f, 1.0f, 1.0f);

    for (int32 Col = 0; Col <= NumColumns; ++Col)
    {
        float X = GridOrigin.X + (Col * CellSize);
        FVector Start = FVector(X, GridOrigin.Y, Z);
        FVector End = FVector(X, GridOrigin.Y + NumRows * CellSize, Z);
        DrawDebugLine(
            GetWorld(),
            Start,
            End,
            FColor::White,
            false,
            -1.0f,
            0,
            2.0f
        );
    }

    for (int32 Row = 0; Row <= NumRows; ++Row)
    {
        float Y = GridOrigin.Y + (Row * CellSize);
        FVector Start = FVector(GridOrigin.X, Y, Z);
        FVector End = FVector(GridOrigin.X + NumColumns * CellSize, Y, Z);
        DrawDebugLine(
            GetWorld(),
            Start,
            End,
            FColor::White,
            false,
            -1.0f,
            0,
            2.0f
        );
    }
}

void AGridManager::DrawSelectedCell()
{
    if (!GetWorld()) return;

    if (!IsValidGridIndex(SelectedCell.X, SelectedCell.Y))
    {
        return;
    }

    FVector CellCenter = GridToWorld(SelectedCell.X, SelectedCell.Y);

    FVector BoxExtent = FVector(CellSize * 0.5f, CellSize * 0.5f, 5.0f);

    DrawDebugSolidBox(
        GetWorld(),
        CellCenter,
        BoxExtent,
        FColor(0, 255, 0, 100)
    );
}