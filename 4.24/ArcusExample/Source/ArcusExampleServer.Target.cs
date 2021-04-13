// Copyright i3D.net, 2021. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;


// Arcus Example Target for dedicated game server.
public class ArcusExampleServerTarget : TargetRules
{
	public ArcusExampleServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "ArcusExample" } );
	}
}
