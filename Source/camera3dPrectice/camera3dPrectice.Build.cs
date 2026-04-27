// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class camera3dPrectice : ModuleRules
{
	public camera3dPrectice(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
