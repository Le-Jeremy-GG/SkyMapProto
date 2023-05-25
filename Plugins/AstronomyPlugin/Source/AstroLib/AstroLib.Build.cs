// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.


using UnrealBuildTool;
using System.IO;

public class AstroLib : ModuleRules
{ 
    private string ModulePath
{
    get { return ModuleDirectory; }
}

private string ThirdPartyPath
{
    get { return Path.GetFullPath(Path.Combine(ModulePath, "../ThirdParty/")); }
}




    public AstroLib(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));

        //Test for Module Path
        //System.Console.WriteLine("... ModuleDirectory -> " + ModuleDirectory);


        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"DoubleModule"
				
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
               

				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        LoadNovascLib(Target);
        LoadSpiceLib(Target);
    }

    public bool LoadNovascLib(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;
        string LibrariesPath = "";

        string ConfigVersion = "Debug";
        if (Target.Configuration == UnrealTargetConfiguration.Shipping)
        {
            ConfigVersion = "Release";
        }

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;
            string PlatformString = "x64";
            if (Target.Platform == UnrealTargetPlatform.Win32)
            {
                PlatformString = "Win32";
            }
 
            LibrariesPath = Path.Combine(ThirdPartyPath, "Novasc", PlatformString, ConfigVersion);
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "Novasc.lib"));
        }
        
        if (Target.Platform == UnrealTargetPlatform.Mac )
         {
            isLibrarySupported = true;
            LibrariesPath = Path.Combine(ThirdPartyPath, "Novasc", "Mac", ConfigVersion);
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "libNovasMacStaticLib.a"));
         }

        if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            isLibrarySupported = true;
            LibrariesPath = Path.Combine(ThirdPartyPath, "Novasc", "iOS", ConfigVersion);
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "libNovasMacStaticLib.a"));
        }

        PublicDefinitions.Add(string.Format("WITH_NOVASC_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }

    public bool LoadSpiceLib(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;
        string PlatformString;
        string LibrariesPath;

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "Win32";
            LibrariesPath = Path.Combine(ThirdPartyPath, "cspice", "lib", PlatformString);

            //Test for Library Path
            //System.Console.WriteLine("... LibrariesPath -> " + LibrariesPath);

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "cspice.lib"));
        }
         
        if (Target.Platform == UnrealTargetPlatform.Mac)
        {
                isLibrarySupported = true;
                LibrariesPath = Path.Combine(ThirdPartyPath, "cspice", "lib", "Mac");
                PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "cspice.a"));
        }

        if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            isLibrarySupported = true;
            LibrariesPath = Path.Combine(ThirdPartyPath, "cspice", "lib", "iOS");
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "cspice.a"));
        }


        if (isLibrarySupported)
        {
            // Include path
            //string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "Win32";
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "cspice", "include"));
         }

        PublicDefinitions.Add(string.Format("WITH_CSPICE_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }


}
