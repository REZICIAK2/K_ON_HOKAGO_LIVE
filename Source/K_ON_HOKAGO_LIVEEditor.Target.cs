// NONE OF THE CONTENT IN THIS IS MINE all things are the authors creation of kyoto animataion  

using UnrealBuildTool;
using System.Collections.Generic;

public class K_ON_HOKAGO_LIVEEditorTarget : TargetRules
{
	public K_ON_HOKAGO_LIVEEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "K_ON_HOKAGO_LIVE" } );
	}
}
