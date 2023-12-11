// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_Momo : ModuleRules
{
	public Project_Momo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule" });
	}
}
