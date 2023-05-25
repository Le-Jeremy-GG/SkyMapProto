// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#include "AstroLibEditor.h"
#include "AstroLibEditorStyle.h"
#include "AstroLibDetailLayout.h"
#include "AstroLibEditorCommands.h"

#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Editor/PropertyEditor/Private/SDetailsViewBase.h"
#include "SlateCore/Public/Styling/SlateTypes.h"
#include "IDetailsView.h"
#include "Editor/PropertyEditor/Private/SDetailsView.h"
#include "IStructureDetailsView.h"
#include "PropertyEditorModule.h"
#include "LevelEditor.h"
#include "DetailCategoryBuilder.h"
#include "PropertyEditorModule.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Customizations/SlateFontInfoCustomization.h"
#include "AssetData.h"
#include "Editor/PropertyEditor/Private/DetailItemNode.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "PropertyEditorDelegates.h"
#include "DetailCustomizations.h"







static const FName AstroLibEditorTabName("AstroLibEditor");

#define LOCTEXT_NAMESPACE "FAstroLibEditorModule"

void FAstroLibEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FAstroLibEditorStyle::Initialize();
	FAstroLibEditorStyle::ReloadTextures();

	FAstroLibEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	

	FAstroLibDetailLayout * LayoutClass = new FAstroLibDetailLayout;
	TSharedRef< FAstroLibDetailLayout > DetailClass = MakeShareable(LayoutClass);

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout("StarManager", FOnGetDetailCustomizationInstance::CreateStatic(&FAstroLibDetailLayout::MakeInstance));


	
}

void FAstroLibEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FAstroLibEditorStyle::Shutdown();

	FAstroLibEditorCommands::Unregister();

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomPropertyTypeLayout("StarManager");


}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAstroLibEditorModule, AstroLibEditor)