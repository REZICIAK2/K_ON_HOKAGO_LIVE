// NONE OF THE CONTENT IN THIS IS MINE all things are the authors creation of kyoto animataion  

using UnrealBuildTool;
using System.Collections.Generic;

public class K_ON_HOKAGO_LIVETarget : TargetRules
{
	public K_ON_HOKAGO_LIVETarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "K_ON_HOKAGO_LIVE" } );
	}
}
