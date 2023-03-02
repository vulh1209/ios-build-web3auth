// Copyright 2022 3S Game Studio OU. All Rights Reserved.

using UnrealBuildTool;

public class TSBC_Plugin_Editor : ModuleRules {

    public TSBC_Plugin_Editor(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                // ... add public include paths required here ...
            }
        );


        PrivateIncludePaths.AddRange(
            new string[] {
                // ... add other private include paths required here ...
            }
        );


        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "AssetTools",
                "UnrealEd",
            }
        );


        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "CoreUObject",
                "Engine",
                "TSBC_Plugin_Runtime",
                "Slate",
                "SlateCore",
                "EditorStyle",
                "PropertyEditor",
                "GraphEditor",
                "KismetCompiler",
                "BlueprintGraph",
                "Projects", // <-- The Module for using FSlateStyleSet
            }
        );


        DynamicallyLoadedModuleNames.AddRange(
            new string[] {
                // ... add any modules that your module loads dynamically here ...
            }
        );
    }

}