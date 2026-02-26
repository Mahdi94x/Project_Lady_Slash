// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Project_Lady_SlashEditorTarget : TargetRules
{
	public Project_Lady_SlashEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;

		BuildEnvironment = TargetBuildEnvironment.Shared;

		bOverrideBuildEnvironment = true;

		ExtraModuleNames.AddRange( new string[] { "Project_Lady_Slash" } );
	}
}