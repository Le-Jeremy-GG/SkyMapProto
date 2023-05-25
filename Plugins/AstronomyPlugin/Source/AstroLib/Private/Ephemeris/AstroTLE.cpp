// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.
#pragma once

#include "Ephemeris/AstroTLE.h"
#include "Ephemeris/JPLDE.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "AstroLibBPLibrary.h"


UAstroTLE::UAstroTLE()
{

}

FSingleTLEStruct UAstroTLE::MakeTLE(FString Line1, FString Line2)
{
	FSingleTLEStruct TLE(Line1, Line2);
	return TLE;
}


int UAstroTLE::MakeSPKFileFromTLEs(UJPLDE* Ephem, FTLEListForObject inTLEObject, FString& OutFilePath, FString SPKFilePathandName, int HoursValidBeforeFirstTLE, int HoursValidAfterLastTLE, bool AutoLoadKernalAfterCreation)
{
	//https://naif.jpl.nasa.gov/pub/naif/toolkit_docs/C/cspice/spkw10_c.html
	if (inTLEObject.TLEList.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No TLEs passed to MakeSPKFileFromTLEs"));

		return 1;
	}

	SpiceDouble geophs[8];
	geophs[0] = 1.082616e-3;
	geophs[1] = -2.53881e-6;
	geophs[2] = -1.65597e-6;
	geophs[3] = 7.43669161e-2;
	geophs[4] = 120.0;
	geophs[5] = 78.0;
	geophs[6] = 6378.135;
	geophs[7] = 1.0;


	TArray<FSingleTLEStruct> VerifiedTLEItemArray;
	TArray <FSingleTLEStruct> InputedTLEArray = inTLEObject.TLEList;

	FString SatCatNum = InputedTLEArray[0].SatelliteCatalogNumber;
	InputedTLEArray.Sort();
	
	VerifiedTLEItemArray.Emplace(InputedTLEArray[0]);

	for (FSingleTLEStruct TLEItem : InputedTLEArray)
	{
		if (TLEItem.SatelliteCatalogNumber != SatCatNum)
		{
			UE_LOG(LogTemp, Warning, TEXT("SPK file can only contain one Satellite. Only processing SPK for Satellite number %s. Not using TLE for satellite number %s."), *SatCatNum, *TLEItem.SatelliteCatalogNumber);
		}
		else
		{
			if (TLEItem.EpochV1 > VerifiedTLEItemArray.Last().EpochV1)
			{
				VerifiedTLEItemArray.Emplace(TLEItem);
			}
		}
	}

	int NumOfTLEs = VerifiedTLEItemArray.Num();
	double *  EphemDblArray = static_cast<double*>(FMemory::Malloc(NumOfTLEs * sizeof(double)));
	double*   ElemDblArray = static_cast<double*>(FMemory::Malloc(10 * NumOfTLEs * sizeof(double)));

	VerifiedTLEItemArray.Sort();
	for (int32 i = 0; i < NumOfTLEs; i++)
	{

		FSingleTLEStruct TLEItem = VerifiedTLEItemArray[i];

		//UE_LOG(LogTemp, Warning, TEXT("Adding data for tle with epoch %s"), *FString(TLEItem.EpochV1));

		
		EphemDblArray[i] = VerifiedTLEItemArray[i].EpochV1;

		ElemDblArray[i * 10] = TLEItem.elems[0];
		ElemDblArray[i * 10 + 1] = TLEItem.elems[1];
		ElemDblArray[i * 10 + 2] = TLEItem.elems[2];
		ElemDblArray[i * 10 + 3] = TLEItem.elems[3];
		ElemDblArray[i * 10 + 4] = TLEItem.elems[4];
		ElemDblArray[i * 10 + 5] = TLEItem.elems[5];
		ElemDblArray[i * 10 + 6] = TLEItem.elems[6];
		ElemDblArray[i * 10 + 7] = TLEItem.elems[7];
		ElemDblArray[i * 10 + 8] = TLEItem.elems[8];
		ElemDblArray[i * 10 + 9] = TLEItem.elems[9];
		
	}

	//UE_LOG(LogTemp, Warning, TEXT("EphemDblArray[0]: %s"), *FString::SanitizeFloat(EphemDblArray[0]));
	//UE_LOG(LogTemp, Warning, TEXT("EphemDblArray[1]: %s"), *FString::SanitizeFloat(EphemDblArray[1]));
	//UE_LOG(LogTemp, Warning, TEXT("EphemDblArray[2]: %s"), *FString::SanitizeFloat(EphemDblArray[2]));

	FMemory::Free(ElemDblArray);
	FMemory::Free(EphemDblArray);
	
	FString SPKFilePath;
	SpiceInt SPKHandle;

	if (SPKFilePathandName == "")
	{
		FString Filename = VerifiedTLEItemArray[0].SatelliteCatalogNumber + ".spk";
		SPKFilePath = FPaths::Combine(Ephem->MakeDataPaths(""), Filename);
	}
	else
	{
		if (FPaths::GetCleanFilename(SPKFilePathandName) == "")
		{
			FString Filename = VerifiedTLEItemArray[0].SatelliteCatalogNumber + ".spk";
			FString LastChar = SPKFilePathandName.Right(1);
			if (LastChar == "/" || LastChar == "\\")
			{
				SPKFilePathandName = SPKFilePathandName + Filename;

			}
			else
			{
				SPKFilePathandName = SPKFilePathandName + "\\" + Filename;
			}
		}
		SPKFilePath = Ephem->MakeDataPaths(SPKFilePathandName);
	}
	


	Ephem->CheckError();
	//UE_LOG(LogTemp, Warning, TEXT("Path: %s"), *Path);
	
	if (exists_c(TCHAR_TO_ANSI(*SPKFilePath)))
	{
		SpiceInt OldHandle;
		SpiceBoolean FileLoaded;
		SpiceChar Source[256];
		SpiceChar FileTyp[33];

		kinfo_c(TCHAR_TO_ANSI(*SPKFilePath), 33, 256, FileTyp, Source, &OldHandle, &FileLoaded);
		if (FileLoaded)
		{
			UE_LOG(LogTemp, Warning, TEXT("SPK File already loaded. Unloading to write SPK data."), *SPKFilePath);
			unload_c(TCHAR_TO_ANSI(*SPKFilePath));
		}
		
		UE_LOG(LogTemp, Warning, TEXT("SPK File already created. Appending data."), *SPKFilePath);
		spkopa_c(TCHAR_TO_ANSI(*SPKFilePath), &SPKHandle);
	}
	else
	{
		spkopn_c(TCHAR_TO_ANSI(*SPKFilePath), "spktle", 1, &SPKHandle);
	}
		
	if (!Ephem->CheckError())
	{
		double first;
		double last;

		if (HoursValidBeforeFirstTLE != -1) {
			first = VerifiedTLEItemArray[0].EpochV1 - HoursValidBeforeFirstTLE * spd_c();
		}
		else
		{
			first = 1;
		}

		if (HoursValidBeforeFirstTLE != -1) {
			last = VerifiedTLEItemArray[NumOfTLEs - 1].EpochV1 + HoursValidBeforeFirstTLE * spd_c();
		}
		else
		{
			last = DBL_MAX;
		}


		
		spkw10_c(SPKHandle, VerifiedTLEItemArray[0].NAIFId, 399, "J2000", first, last,
		TCHAR_TO_ANSI(*VerifiedTLEItemArray[0].SatelliteCatalogNumber), geophs, NumOfTLEs, ElemDblArray, EphemDblArray);
		
		spkcls_c(SPKHandle);

		if (AutoLoadKernalAfterCreation)
		{
			FString KerInfo;
			Ephem->LoadKernal(SPKFilePath, KerInfo);
		}
	}

	else
	{
		return 1;
	}
	
	if (!Ephem->CheckError())
	{		
		OutFilePath = SPKFilePath;
		return 0;
	}

	return 1;
	
}

int UAstroTLE::ReadTLEFile(UJPLDE* Ephem, FString FilePath, TArray<FTLEListForObject>& OutTLEObjectsArray)
{
	if (FilePath != "")
	{
		TArray<FSingleTLEStruct> FullTLEList;
		FString UpdatedFilePath = Ephem->MakeDataPaths(FilePath);
		TArray<FString> TLEFileLineArray;
		FFileHelper::LoadFileToStringArray(TLEFileLineArray, *UpdatedFilePath);

		if (TLEFileLineArray.IsValidIndex(0))
		{

			int32 LineIndex = 0;
			while (LineIndex < TLEFileLineArray.Num())
			{
				if (TLEFileLineArray[LineIndex].StartsWith("1"))
				{
					FSingleTLEStruct NewTLE(TLEFileLineArray[LineIndex], TLEFileLineArray[LineIndex + 1]);
					LineIndex = LineIndex + 2;
					FullTLEList.Emplace(NewTLE);
					continue;
				}
				LineIndex = LineIndex + 1;
				UE_LOG(LogTemp, Warning, TEXT("TLEFileLineArray[]: %s"), *TLEFileLineArray[LineIndex]);
			}

			//
			for (auto& SingleTLE : FullTLEList)
			{
				bool ExistingTLEObjectStructFound = false;

				for (auto& TLEObjectStruct : OutTLEObjectsArray)
				{
					if (TLEObjectStruct.TLEList[0].SatelliteCatalogNumber == SingleTLE.SatelliteCatalogNumber)
					{
						TLEObjectStruct.TLEList.Emplace(SingleTLE);
						ExistingTLEObjectStructFound = true;
						break;
					}
				}

				if (!ExistingTLEObjectStructFound)
				{
					FTLEListForObject NewTLEObjStruct;
					NewTLEObjStruct.TLEList.Emplace(SingleTLE);
					OutTLEObjectsArray.Emplace(NewTLEObjStruct);
				}

			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No file or data found in FilePath:"), *UpdatedFilePath);
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ReadTLEFile() called with no filepath"));
		return 1;
	}
	

	return 0;
}

int UAstroTLE::ConvertTLEIdtoJPLId(int TLEId)
{
	return -100000 - TLEId;
}


