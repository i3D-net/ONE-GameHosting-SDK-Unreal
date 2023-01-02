// Copyright i3D.net, 2021. All Rights Reserved.

using UnrealBuildTool;

// Arcus Example Module build depedencies.
public class ArcusExample : ModuleRules
{
    public ArcusExample(ReadOnlyTargetRules Target) : base(Target)
    {
        // To ensure that Verifying IWYU is Enabled
        // See https://docs.unrealengine.com/en-US/ProductionPipelines/BuildTools/UnrealBuildTool/IWYU/index.html.
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "ONEGameHostingPlugin"
            }
            );

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
