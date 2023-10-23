using UnrealBuildTool;

public class CrazyGuitarEditorTarget : TargetRules {
    public CrazyGuitarEditorTarget(TargetInfo Target) : base(Target) {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange( new string[] { "CrazyGuitarCore" } );
    }
}