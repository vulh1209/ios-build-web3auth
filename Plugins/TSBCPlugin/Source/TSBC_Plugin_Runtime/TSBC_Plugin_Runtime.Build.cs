// Copyright 2022 3S Game Studio OU. All Rights Reserved.

using UnrealBuildTool;

public class TSBC_Plugin_Runtime : ModuleRules {

    public TSBC_Plugin_Runtime(ReadOnlyTargetRules Target) : base(Target) {
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
                // ... add other public dependencies that you statically link with here ...
                "HTTP",
                "Json",
            }
        );


        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                // ... add private dependencies that you statically link with here ...
            }
        );


        DynamicallyLoadedModuleNames.AddRange(
            new string[] {
                // ... add any modules that your module loads dynamically here ...
            }
        );
    }

}
