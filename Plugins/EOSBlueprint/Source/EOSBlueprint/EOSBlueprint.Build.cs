// Some copyright should be here...

using UnrealBuildTool;

public class EOSBlueprint : ModuleRules
{
	public EOSBlueprint(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
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
			new string[]
			{
				  "Core"
				,"OnlineSubsystem"
				,"OnlineSubsystemEOS"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				  "CoreUObject"
				, "Engine"
				, "Slate"
				, "SlateCore"
				, "EOSSDK",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				//"OnlineSubsystem"
				//, "OnlineSubsystemEOS"
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
