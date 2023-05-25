// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


class FToolBarBuilder;
class FMenuBuilder;

struct FAssetData;

class FAstroLibEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	



	
private:


private:
	TSharedPtr<class FUICommandList> PluginCommands;



	
};

