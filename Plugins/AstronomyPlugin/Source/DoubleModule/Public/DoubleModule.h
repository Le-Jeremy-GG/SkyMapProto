// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.


#pragma once

#include "Modules/ModuleManager.h"


class FDoubleModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
