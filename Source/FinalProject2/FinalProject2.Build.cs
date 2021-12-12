// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FinalProject2 : ModuleRules
{
	public FinalProject2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
