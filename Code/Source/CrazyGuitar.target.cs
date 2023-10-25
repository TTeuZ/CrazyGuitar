using UnrealBuildTool;
using System.Collections.Generic;

public class CrazyGuitarTarget : TargetRules {
	public CrazyGuitarTarget( TargetInfo Target) : base(Target) {
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "CrazyGuitar" } );
	}
}
