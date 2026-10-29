// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Project_Lady_SlashTarget : TargetRules
{
	public Project_Lady_SlashTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		
		DefaultBuildSettings = BuildSettingsVersion.V7;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		
       ExtraModuleNames.AddRange(new string[] { "Project_Lady_Slash" });
	   
	}
}
