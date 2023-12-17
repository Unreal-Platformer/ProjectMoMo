#pragma once

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	EMS_Patrolling UMETA(DisplayName = "Patrolling"),
	EMS_Chasing UMETA(DisplayName = "Chasing"),
	EMS_Attacking UMETA(DisplayName = "Attacking")
};