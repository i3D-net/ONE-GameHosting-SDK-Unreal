// Copyright i3D.net, 2021. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

// Arcus Example Target for game without editor.
public class ArcusExampleTarget : TargetRules
{
	public ArcusExampleTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
	    IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.AddRange( new string[] { "ArcusExample" } );
	}
}
