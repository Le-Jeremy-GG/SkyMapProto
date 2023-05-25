// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.


#pragma once


#include "AstroLibDetailLayout.h"

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




void FAstroLibDetailLayout::OnData1Changed(const FAssetData& InAssetData)
{
}

TSharedRef<IDetailCustomization> FAstroLibDetailLayout::MakeInstance()
{
	return MakeShareable(new FAstroLibDetailLayout());
}

void FAstroLibDetailLayout::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailBuilder.GetObjectsBeingCustomized(Objects);
	TWeakObjectPtr<AStarManager> AstroHelperObject = Cast<AStarManager>(Objects[0].Get());
	
	if (Objects.Num() != 1)
	{
		return;
	}


	// Create a category so this is displayed early in the properties
	IDetailCategoryBuilder& MyCategory = DetailBuilder.EditCategory("StarManager", FText::FromString("Star Actor Helper"), ECategoryPriority::Important);

	
	DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AStarManager, StarCatalogData));
	MyCategory.AddProperty(GET_MEMBER_NAME_CHECKED(AStarManager, StarCatalogData));
	
	DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AStarManager, StarClassTable));
	MyCategory.AddProperty(GET_MEMBER_NAME_CHECKED(AStarManager, StarClassTable));

	DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AStarManager, StarSizeTable));
	MyCategory.AddProperty(GET_MEMBER_NAME_CHECKED(AStarManager, StarSizeTable));

	DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AStarManager, DefaultStarClassToSpawn));
	MyCategory.AddProperty(GET_MEMBER_NAME_CHECKED(AStarManager, DefaultStarClassToSpawn));
	
	DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AStarManager, SpawnDistance));
	MyCategory.AddProperty(GET_MEMBER_NAME_CHECKED(AStarManager, SpawnDistance));

	DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AStarManager, UTCTime));
	MyCategory.AddProperty(GET_MEMBER_NAME_CHECKED(AStarManager, UTCTime));

	DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AStarManager, Observer));
	MyCategory.AddProperty(GET_MEMBER_NAME_CHECKED(AStarManager, Observer));

	DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AStarManager, RefractionOpts));
	MyCategory.AddProperty(GET_MEMBER_NAME_CHECKED(AStarManager, RefractionOpts));
	
	DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AStarManager, RequestedCoordSystem));
	MyCategory.AddProperty(GET_MEMBER_NAME_CHECKED(AStarManager, RequestedCoordSystem));
	
	DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AStarManager, StarCalculationMethod));
	MyCategory.AddProperty(GET_MEMBER_NAME_CHECKED(AStarManager, StarCalculationMethod));


	//FOnClicked::CreateSP(this, &FAstroLibDetailLayout::CreateStarButton, AstroHelperObject);

	MyCategory.AddCustomRow(FText::FromString("ButtonRow"))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString(""))
		]
		.ValueContent().MinDesiredWidth(400)
		[
			SAssignNew(RefreshButton, SButton)
			.OnClicked(this, &FAstroLibDetailLayout::CreateStarButton, AstroHelperObject)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Text(FText::FromString("Create Stars"))
		];

	MyCategory.AddCustomRow(FText::FromString("RemoveButtonRow"))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString(""))
		]
	.ValueContent().MinDesiredWidth(400)
		[
			SAssignNew(DeleteButton, SButton)
			.OnClicked(this, &FAstroLibDetailLayout::RemoveStarButton, AstroHelperObject)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Text(FText::FromString("Remove Stars"))
		];




	// Create a category so this is displayed early in the properties
	IDetailCategoryBuilder& MyCategory2 = DetailBuilder.EditCategory("AstroLibActorHelperFastUpdate", FText::FromString("Fast Update Settings"), ECategoryPriority::Important);

	DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AStarManager, bAlwaysUpdateDeltaT));
	MyCategory2.AddProperty(GET_MEMBER_NAME_CHECKED(AStarManager, bAlwaysUpdateDeltaT));

	DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AStarManager, bAlwaysUpdateEarthSurfaceLocation));
	MyCategory2.AddProperty(GET_MEMBER_NAME_CHECKED(AStarManager, bAlwaysUpdateEarthSurfaceLocation));

}


FReply FAstroLibDetailLayout::CreateStarButton(TWeakObjectPtr<AStarManager> InAstroObj)
{
	UWorld* World = GEditor->GetEditorWorldContext().World();


	InAstroObj->SpawnStars(World);

	return FReply::Handled();
}

FReply FAstroLibDetailLayout::RemoveStarButton(TWeakObjectPtr<AStarManager> InAstroObj)
{
	
	InAstroObj->RemoveAllStars();

	return FReply::Handled();
}
