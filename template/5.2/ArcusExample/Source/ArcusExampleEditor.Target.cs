// Copyright i3D.net, 2021. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

// Arcus Example Target for game with the editor.
public class ArcusExampleEditorTarget : TargetRules
{
	public ArcusExampleEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V3;
	    IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
		ExtraModuleNames.AddRange( new string[] { "ArcusExample" } );
	}
}
