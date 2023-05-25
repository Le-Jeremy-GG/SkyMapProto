// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.
#pragma once

#include "AstroStructs.h"
#include "NovasCFunctions.h"
//#include <string.h>


DEFINE_LOG_CATEGORY(LogAstroLog);



FStarCatalogEntry UAstroStructs::MakeStarCatalogEntry(FString NameF, FString CatF, int Num, FAstroDouble raf, FAstroDouble decf, FAstroDouble pmraf, FAstroDouble pmdecf, FAstroDouble parallaxf, FAstroDouble radvecf, float VisualMagnitude)
{
	FStarCatalogEntry StarCat;

	//FCString::Strcpy(UTF8_TO_TCHAR(StarCat.StarName), SIZE_OF_OBJ_NAME, *NameF.Left(SIZE_OF_OBJ_NAME));
	//FCString::Strcpy(UTF8_TO_TCHAR(StarCat.Catalog), SIZE_OF_CAT_NAME, *CatF.Left(SIZE_OF_CAT_NAME));
	StarCat.StarNameFString = NameF;
	StarCat.CatalogFString = CatF;
	StarCat.StarNumber = Num;
	StarCat.StarNumberInt = Num;
	StarCat.RightAscenstion = raf;
	StarCat.Declination = decf;
	StarCat.ProperMotionInDeclination = pmraf;
	StarCat.ProperMotionInDeclination = pmdecf;
	StarCat.Parallax = parallaxf;
	StarCat.RadialVelocity = radvecf;
	StarCat.VisualMagnitude = VisualMagnitude;

	return StarCat;
}

