// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.
#pragma once

#include "DoubleModule.h"

#define LOCTEXT_NAMESPACE "FDoubleModule"

void FDoubleModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FDoubleModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDoubleModule, DoubleModule)
