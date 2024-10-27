// Copyright HungryHusky Games 2024

#pragma once

#include "CoreMinimal.h"
#include "Slate/SlateBrushAsset.h"
#include "UObject/Interface.h"

#include "IntuitiveFPSRadarObjectInterface.generated.h"

UINTERFACE(MinimalAPI)
class UIntuitiveFPSRadarObjectInterface : public UInterface
{
	GENERATED_BODY()
};

class INTUITIVEFPSRADAR_API IIntuitiveFPSRadarObjectInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Radar)
	USlateBrushAsset* GetRadarObjectIcon() const;
	virtual USlateBrushAsset* GetRadarObjectIcon_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Radar)
	FLinearColor GetRadarIconColor() const;
	virtual FLinearColor GetRadarIconColor_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Radar)
	FVector2D GetRadarIconSize() const;
	virtual FVector2D GetRadarIconSize_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Radar)
	bool ShouldShowOnRadar() const;
	virtual bool ShouldShowOnRadar_Implementation() const;
};
