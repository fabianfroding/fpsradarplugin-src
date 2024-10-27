// Copyright HungryHusky Games 2024

#include "IntuitiveFPSRadarWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "IntuitiveFPSRadarObjectInterface.h"
#include "Kismet/KismetMathLibrary.h"

int32 UIntuitiveFPSRadarWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	/*if (GameUserSettings.Get() && !FMath::IsNearlyEqual(FOV, GameUserSettings.Get()->GetFieldOfView() * 2.f))
	{
		FOV = GameUserSettings.Get()->GetFieldOfView() * 2.f;
	}*/

	//----- Draw Radar Circle -----//
	const int32 LastIndex = UKismetMathLibrary::FTrunc(360.f / DegreeStep);
	for (int32 i = 0; i <= LastIndex; i++)
	{
		UWidgetBlueprintLibrary::DrawLine(Context, FVector2D(),
			FVector2D(UKismetMathLibrary::DegCos(DegreeStep * i) * RadarUIRadius, UKismetMathLibrary::DegSin(DegreeStep * i) * RadarUIRadius),
			(i > FOVDegreeThreshold - FOV && i < FOVDegreeThreshold + FOV) ? RadarCircleFOVColor : RadarCircleColor);
	}

	if (RadarIcon)
	{
		UWidgetBlueprintLibrary::DrawBox(Context, RadarIconSize * FVector2D(-0.5f, -0.5f), RadarIconSize, RadarIcon, RadarIconColor);
	}

	//----- Draw Player -----//
	if (PlayerIcon)
	{
		UWidgetBlueprintLibrary::DrawBox(Context, PlayerIconSize * FVector2D(-0.7f, -0.7f), PlayerIconSize, PlayerIcon, PlayerIconColor);
	}

	//----- Draw Other Objects -----//
	for (AActor* RadarObject : RadarObjects)
	{
		if (!RadarObject) { continue; }
		if (!RadarObject->GetClass()->ImplementsInterface(UIntuitiveFPSRadarObjectInterface::StaticClass())) { continue; }
		if (RadarObject->IsHidden()) { continue; }
		if (!IIntuitiveFPSRadarObjectInterface::Execute_ShouldShowOnRadar(RadarObject)) { continue; }

		USlateBrushAsset* Icon = IIntuitiveFPSRadarObjectInterface::Execute_GetRadarObjectIcon(RadarObject);
		FLinearColor IconColor = IIntuitiveFPSRadarObjectInterface::Execute_GetRadarIconColor(RadarObject);
		FVector2D IconSize = IIntuitiveFPSRadarObjectInterface::Execute_GetRadarIconSize(RadarObject);

		if (!Icon) { continue; }

		const APlayerController* PlayerController = GetOwningPlayer();
		if (RadarObject == PlayerController->GetPawn()) { continue; }
		const FVector2D PositionInRadar = GetPositionInRadar(RadarObject, PlayerController, RadarDistanceScale, RadarUIRadius, IconSize);
		UWidgetBlueprintLibrary::DrawBox(Context, PositionInRadar, IconSize, Icon, IconColor);

		//----- Draw Height Indicator -----//
		const FVector TargetLocation = RadarObject->GetActorLocation();
		const FVector PlayerLocation = PlayerController->GetPawn() != nullptr ? PlayerController->GetPawn()->GetActorLocation() : FVector::Zero();

		if (FVector::DistSquared(TargetLocation, PlayerLocation) > FMath::Square(RadarDistanceScale * 100.f)) { continue; }

		const float HeightDifference = TargetLocation.Z - PlayerLocation.Z;

		if (HeightDifference > -HeightIndicatorThreshold && HeightDifference < HeightIndicatorThreshold) { continue; }

		const bool bIsAbove = HeightDifference > HeightIndicatorThreshold ? true : false;
		USlateBrushAsset* HeightIndicatorIcon = bIsAbove ? HeightIndicatorUpIcon : HeightIndicatorDownIcon;

		if (!HeightIndicatorIcon) { continue; }

		UWidgetBlueprintLibrary::DrawBox(Context, GetRadarHeightIndicatorPosition(PositionInRadar, HeightIndicatorIconSize, bIsAbove), HeightIndicatorIconSize, HeightIndicatorIcon, IconColor);
	}

	return FMath::Max(LayerId, Context.MaxLayer);
}

FVector2D UIntuitiveFPSRadarWidget::GetPositionInRadar(const AActor* Actor, const APlayerController* PlayerController, const float DistanceScale, const float UIRadius, const FVector2D IconSize)
{
	if (!Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("UIntuitiveFPSRadarWidget::GetRadarPosition: Invalid actor."));
		return FVector2D();
	}

	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UIntuitiveFPSRadarWidget::GetRadarPosition: Invalid player controller."));
		return FVector2D();
	}

	APawn* ControlledPawn = PlayerController->GetPawn();
	if (!ControlledPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UIntuitiveFPSRadarWidget::GetRadarPosition: Invalid pawn."));
		return FVector2D();
	}

	if (DistanceScale <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("UIntuitiveFPSRadarWidget::GetRadarPosition: Invalid value for RadarDistanceScale (<= 0)."));
		return FVector2D();
	}

	FTransform PlayerTransform = ControlledPawn->GetActorTransform();
	PlayerTransform.SetRotation(ControlledPawn->GetActorRotation().Quaternion());

	const FVector InverseTransformLocation = UKismetMathLibrary::InverseTransformLocation(PlayerTransform, Actor->GetActorLocation());
	const FVector TransformLocationRotated = UKismetMathLibrary::GreaterGreater_VectorRotator(InverseTransformLocation, UKismetMathLibrary::MakeRotator(0.f, 0.f, -90.f));
	const FVector2D TransformLocationScaled = (FVector2D)TransformLocationRotated / DistanceScale;
	const FVector2D TransformLocationAdjusted = TransformLocationScaled;

	const float Atan2 = UKismetMathLibrary::DegAtan2(TransformLocationAdjusted.Y, TransformLocationAdjusted.X);
	const float Sqrt = UKismetMathLibrary::Sqrt(UKismetMathLibrary::MultiplyMultiply_FloatFloat(TransformLocationAdjusted.X, 2.f) + UKismetMathLibrary::MultiplyMultiply_FloatFloat(TransformLocationAdjusted.Y, 2.f));
	const float Clamped = FMath::Clamp(Sqrt, -UIRadius, UIRadius);

	return FVector2D(UKismetMathLibrary::DegCos(Atan2) * Clamped, UKismetMathLibrary::DegSin(Atan2) * Clamped) - IconSize * 0.5f;
}

FVector2D UIntuitiveFPSRadarWidget::GetRadarHeightIndicatorPosition(const FVector2D PositionInRadar, const FVector2D IconSize, const bool IsAbove)
{
	return FVector2D(PositionInRadar.X + IconSize.X * 0.15f, PositionInRadar.Y + IconSize.Y * (IsAbove ? -0.75f : 1.25f));
}
