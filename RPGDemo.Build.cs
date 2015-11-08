// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RPGDemo : ModuleRules
{
	public RPGDemo(TargetInfo Target)
	{
        MinFilesUsingPrecompiledHeaderOverride = 1;
        bFasterWithoutUnity = true;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "HTTP", "Json", "JsonUtilities", "OnlineSubsystem", "OnlineSubsystemUtils", "Steamworks" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "HTTP", "OnlineSubsystem" });
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
	}
}