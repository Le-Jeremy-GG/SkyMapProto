// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.
#pragma once

#include "AstroLibEditorCommands.h"

#define LOCTEXT_NAMESPACE "FAstroLibEditorModule"

void FAstroLibEditorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "AstroLibEditor", "Execute AstroLibEditor action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
