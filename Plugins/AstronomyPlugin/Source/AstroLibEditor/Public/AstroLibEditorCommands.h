// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "AstroLibEditorStyle.h"

class FAstroLibEditorCommands : public TCommands<FAstroLibEditorCommands>
{
public:

	FAstroLibEditorCommands()
		: TCommands<FAstroLibEditorCommands>(TEXT("AstroLibEditor"), NSLOCTEXT("Contexts", "AstroLibEditor", "AstroLibEditor Plugin"), NAME_None, FAstroLibEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
