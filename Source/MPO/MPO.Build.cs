// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MPO : ModuleRules
{
    public MPO(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        OptimizeCode = CodeOptimization.Never;

        PublicIncludePaths.AddRange(new string[]
        {
            "MPO/DataAssets",
            "MPO/InventorySystem",
            "MPO/CombatSystem",
            "MPO/Gameplay",
        });
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "UMG",
            "Engine",
            "InputCore",
            "HeadMountedDisplay",
            "NavigationSystem",
            "AIModule"
        });
    }
}
