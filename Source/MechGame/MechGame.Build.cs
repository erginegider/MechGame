// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MechGame : ModuleRules
{
	public MechGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","GameplayAbilities","GameplayTasks","GameplayTags","UMG", "Slate", "SlateCore" });
	}
}
