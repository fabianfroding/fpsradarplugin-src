// Copyright HungryHusky Games 2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Slate/SlateBrushAsset.h"

#include "IntuitiveFPSRadarWidget.generated.h"

UCLASS()
class INTUITIVEFPSRADAR_API UIntuitiveFPSRadarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	const float DegreeStep = 0.25f;
	const float FOVDegreeThreshold = 1080.f;
	mutable float FOV = 90.f;

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> RadarObjects;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PlayerClass;

	// The size of the radar circle.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RadarUIRadius = 100.f;

	// The distance scale at which objects appear close or far away from the player.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RadarDistanceScale = 50.f;

	// How much height difference is needed between the radar object and the player to show height indicators on the object.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HeightIndicatorThreshold = 450.f;

	// The color of the radar.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor RadarCircleColor;

	// The color of the FOV part of the radar circle.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor RadarCircleFOVColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USlateBrushAsset* RadarIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D RadarIconSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor RadarIconColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USlateBrushAsset* PlayerIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D PlayerIconSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor PlayerIconColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USlateBrushAsset* EnemyIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D EnemyIconSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor EnemyIconColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USlateBrushAsset* RoomObjectiveIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D RoomObjectiveIconSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor RoomObjectiveIconColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USlateBrushAsset* HeightIndicatorUpIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USlateBrushAsset* HeightIndicatorDownIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D HeightIndicatorIconSize;

protected:
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const;

	static FVector2D GetPositionInRadar(const AActor* Actor, const APlayerController* PlayerController, const float DistanceScale, const float UIRadius, const FVector2D IconSize);

	static FVector2D GetRadarHeightIndicatorPosition(const FVector2D PositionInRadar, const FVector2D IconSize, const bool IsAbove);
};
