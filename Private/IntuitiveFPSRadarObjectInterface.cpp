// Copyright HungryHusky Games 2024

#include "IntuitiveFPSRadarObjectInterface.h"

USlateBrushAsset* IIntuitiveFPSRadarObjectInterface::GetRadarObjectIcon_Implementation() const
{
	return nullptr;
}

FLinearColor IIntuitiveFPSRadarObjectInterface::GetRadarIconColor_Implementation() const
{
	return FLinearColor(1.f, 1.f, 1.f, 1.f);
}

FVector2D IIntuitiveFPSRadarObjectInterface::GetRadarIconSize_Implementation() const
{
	return FVector2D(10.f, 10.f);
}

bool IIntuitiveFPSRadarObjectInterface::ShouldShowOnRadar_Implementation() const
{
	return true;
}
