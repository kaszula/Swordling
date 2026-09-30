// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DiplomaGame : ModuleRules
{
	public DiplomaGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"HTTP",
			"InputCore",
			"EnhancedInput",
			"Json",
			"JsonUtilities",
			"UMG"
		});

		PublicIncludePaths.AddRange(new string[] {
			"DiplomaGame"
		});
	}
}
