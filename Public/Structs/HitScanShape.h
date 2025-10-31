#include "HitScanShape.generated.h"
#pragma once

UENUM(BlueprintType)
enum class EHitScanShapeType: uint8
{
	Box UMETA(DisplayName = "Box"),
	Sphere UMETA(DisplayName = "Sphere"),
	Capsule UMETA(DisplayName = "Capsule"),
	Line UMETA(DisplayName = "Line")
};

USTRUCT(BlueprintType)
struct FHitScanShape
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EHitScanShapeType TypeOfShape;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category ="Box Parameters", meta =(EditCondition = "TypeOfShape == EHitScanShapeType::Box",EditConditionHides))
	FVector BoxScale = FVector(1,1,1);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category ="Sphere Parameters", meta =(EditCondition = "TypeOfShape == EHitScanShapeType::Sphere || TypeOfShape == EHitScanShapeType::Capsule",EditConditionHides))
	float Radius = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category ="Sphere Parameters",meta =(EditCondition = "TypeOfShape == EHitScanShapeType::Sphere || TypeOfShape == EHitScanShapeType::Capsule",EditConditionHides,ClampMin = 1,ClampMax=180,Units="Degrees",ToolTip="If left at 180, will use the full sphere circumference, Otherwise up to 1 Degree can you narrow the range akin to a cone"))
	float HitAngle = 180.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category ="Capsule Parameters", meta =(EditCondition = "TypeOfShape == EHitScanShapeType::Capsule",EditConditionHides))
	float CapsuleHalfHeight = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category ="Line Parameters", meta =(EditCondition = "TypeOfShape == EHitScanShapeType::Line",EditConditionHides))
	float LineLengthMultiplier = 100.0f;
};
