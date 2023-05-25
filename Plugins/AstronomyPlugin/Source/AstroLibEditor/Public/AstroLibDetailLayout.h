// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "StarManager.h"

#include "Editor/PropertyEditor/Public/IPropertyTypeCustomization.h"
#include "Editor/PropertyEditor/Public/IDetailCustomization.h"
#include "Editor/DetailCustomizations/Private/ActorDetails.h"
#include "Runtime/AssetRegistry/Public/AssetData.h"

#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor.h"
#include "StarInterface.h"
#include "SlateCore/Public/Styling/SlateTypes.h"


//IDetailCustomization
class FAstroLibDetailLayout : public IDetailCustomization
{

	
public:

	//virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	void OnData1Changed(const FAssetData& InAssetData);

	static TSharedRef< IDetailCustomization > MakeInstance();


	// IDetailCustomization interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
	// End of IDetailCustomization interface


	/**
	* Called when the header of the property (the row in the details panel where the property is shown)
	* If nothing is added to the row, the header is not displayed
	*
	* @param PropertyHandle			Handle to the property being customized
	* @param HeaderRow					A row that widgets can be added to
	* @param StructCustomizationUtils	Utilities for customization
	*/
	//virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils);

	/**
	* Called when the children of the property should be customized or extra rows added
	*
	* @param PropertyHandle			Handle to the property being customized
	* @param StructBuilder				A builder for adding children
	* @param StructCustomizationUtils	Utilities for customization
	*/
	//virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);



	TSharedPtr<SButton> RefreshButton;
	TSharedPtr<SButton> DeleteButton;

	FReply CreateStarButton(TWeakObjectPtr<AStarManager> InAstroObj);
	FReply RemoveStarButton(TWeakObjectPtr<AStarManager> InAstroObj);


};

