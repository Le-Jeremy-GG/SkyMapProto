// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.
#pragma once

#include "Ephemeris/JPLDE.h"
#include <string.h>
#include "Misc/Paths.h"
#include "AstroLibBPLibrary.h"




UJPLDE::UJPLDE()
{



	BodyToIntArray.Add(FBodyIntMap(EBodys::Mercury, 1));
	BodyToIntArray.Add(FBodyIntMap(EBodys::Venus, 2));
	BodyToIntArray.Add(FBodyIntMap(EBodys::Earth, 399));
	BodyToIntArray.Add(FBodyIntMap(EBodys::Mars, 4));
	BodyToIntArray.Add(FBodyIntMap(EBodys::Jupiter, 5));
	BodyToIntArray.Add(FBodyIntMap(EBodys::Saturn, 6));
	BodyToIntArray.Add(FBodyIntMap(EBodys::Uranus, 7));
	BodyToIntArray.Add(FBodyIntMap(EBodys::Neptune, 8));
	BodyToIntArray.Add(FBodyIntMap(EBodys::Pluto, 9));
	BodyToIntArray.Add(FBodyIntMap(EBodys::Moon, 301));
	BodyToIntArray.Add(FBodyIntMap(EBodys::Sun, 10));
	BodyToIntArray.Add(FBodyIntMap(EBodys::SolarSystemBarycenter, 0));
	BodyToIntArray.Add(FBodyIntMap(EBodys::EarthMoonBaryCenter, 3));
	BodyToIntArray.Add(FBodyIntMap(EBodys::Nutations, 14));
	BodyToIntArray.Add(FBodyIntMap(EBodys::Librations, 15));
	BodyToIntArray.Add(FBodyIntMap(EBodys::LunarMantle, 16));
	BodyToIntArray.Add(FBodyIntMap(EBodys::TTTDBDiff, 17));

	Name = "JPLDE";

	J2000ET = j2000_c();
	SecondsPerDay = spd_c();


}

UJPLDE::~UJPLDE()
{
	CloseEphemeris();
}

FString UJPLDE::MakeDataPaths(FString FilePath)
{
	FString ThePath;

	if (FilePath == "")
	{

		#if WITH_EDITOR
		ThePath = FPaths::ConvertRelativePathToFull(FPaths::EnginePluginsDir() + "Marketplace\\AstronomyPlugin\\Source\\ThirdParty\\AstronomyData\\SPKData\\");
		if (!FPaths::DirectoryExists(ThePath))
			{
			ThePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir() + "AstronomyPlugin\\Source\\ThirdParty\\AstronomyData\\SPKData\\");
			}
		if (!FPaths::DirectoryExists(ThePath))
		{
			ThePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() + "SPKData\\");
		}
		#else
		ThePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() + "SPKData\\");
		#endif	
		//LogTemp, Warning, TEXT("Eph Path: %s"), *ThePath);
	}
	else
	{
		FString GameContentDirectory = FPaths::ProjectContentDir();
		FString GCD = "$GameContentDir/";
		ThePath = FilePath.Replace(*GCD, *GameContentDirectory);
		ThePath = FPaths::ConvertRelativePathToFull(ThePath);
		//UE_LOG(LogTemp, Warning, TEXT("Eph Path: %s"), *ThePath);
	}

	return ThePath;
}

int UJPLDE::InitEphemeris(TArray<FString> DataFiles)
{
	char* p = (char*)"NONE, LONG";
	char* w = (char*)"RETURN";
	errprt_c("SET", 199, p);
	erract_c("SET", 199, w);

	if (DataFiles.Num() == 0)
	{
		FString BasePath = MakeDataPaths("");
		LoadedKernalArray.Add(BasePath + "TimeData.tls");
		LoadedKernalArray.Add(BasePath + "PlanetData.bsp");
		LoadedKernalArray.Add(BasePath + "pck00010.tpc");

	}
	else
	{
		for (auto& Str : DataFiles)
		{
		LoadedKernalArray.Add(MakeDataPaths(Str));
		}
	}


	for (auto& Str : LoadedKernalArray)
	{
		furnsh_c(TCHAR_TO_ANSI(*Str));
	}

	if (CheckError())
	{
		isInitialized = false;
		return 1;
	}
	isInitialized = true;
	return 0;
}

void UJPLDE::LoadKernal(FString FilePath, FString & KernelInfo, bool LogKernelInfo)
{
	if (FilePath != "")
	{
		FString Path = MakeDataPaths(FilePath);
		LoadedKernalArray.Add(Path);
		furnsh_c(TCHAR_TO_ANSI(*Path));
		CheckError();
	}

	SpiceInt        count;
	SpiceInt        handle;

	SpiceChar       file[256];
	SpiceChar       filtyp[33];
	SpiceChar       source[256];

	SpiceBoolean    found;

	ktotal_c("all", &count);

	if (count == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No kernal files loaded at this time."));
	}
	else
	{

		kdata_c(count-1, "all", 256, 33, 256, file, filtyp, source, &handle, &found);
		KernelInfo =  file;

		if (LogKernelInfo)
		{
			UE_LOG(LogTemp, Warning, TEXT("Kernel loaded: %s"), *KernelInfo);
		}
	}
}



TArray<FString> UJPLDE::GetAllLoadedObjects(FString Template)
{
	TArray<FString> ObjectList;

	#define  ROOM           50
	#define  LNSIZE         81

	SpiceBoolean            found;

	SpiceChar               kvars[ROOM][LNSIZE];

	SpiceInt                i;
	SpiceInt                n;
	SpiceInt                start;

	start = 0;

	gnpool_c(TCHAR_TO_ANSI(*Template), start, ROOM, LNSIZE, &n, kvars, &found);

	if (!found)
	{
		UE_LOG(LogTemp, Warning, TEXT("No objects loaded with template %s."), *Template);
	}
	else
	{

		for (i = 0; i < n; i++)
		{
			ObjectList.Add(kvars[i]);
		}

		while (n == ROOM)
		{
			start += n;

			gnpool_c(TCHAR_TO_ANSI(*Template), start, ROOM, LNSIZE, &n, kvars, &found);

			for (i = 0; i < n; i++)
			{
				ObjectList.Add(kvars[i]);
			}
		}
	}

	return ObjectList;
}


int UJPLDE::CloseEphemeris()
{

	for (auto& Str : LoadedKernalArray)
	{
		//Unloading will unload kernal files globally, which will break if switching back and forth between ephemeris. You may uncomment this if that is not an issue for your program.
		//unload_c(TCHAR_TO_ANSI(*Str));
	}

	return 0;

}

int UJPLDE::GetObjectState(EBodys ObjBody, EBodys ObjOrigin, FAstroDouble TDBjd1, FAstroDouble Optionaljd2, FAstroDoubleVector & Location, FAstroDoubleVector & Velocity, EPlane CoordinatePlane, const EEphemerisUnits OutUnit)
{
	
	//UE_LOG(LogTemp, Warning, TEXT("-------------------------------------------------------"));

	SpiceDouble    state[6];
	SpiceDouble    lt;

	//Seconds from j2000
	double spref = (double(TDBjd1) - J2000ET) * SecondsPerDay;
	//UE_LOG(LogTemp, Warning, TEXT("spref: %s"), *FString::SanitizeFloat(spref));


	int OriginInt = GetIntForBody(ObjOrigin);
	//UE_LOG(LogTemp, Warning, TEXT("Origin: %d"), OriginInt);

	int TargetInt = GetIntForBody(ObjBody);
	//UE_LOG(LogTemp, Warning, TEXT("Target: %d"), TargetInt);

	if (CoordinatePlane == EPlane::Ecliptical)
	{
		spkez_c(TargetInt, spref, "ECLIPJ2000", "NONE", OriginInt, state, &lt);
	}
	else
	{
		spkez_c(TargetInt, spref, "J2000", "NONE", OriginInt, state, &lt);
	}


	if (!CheckError())
	{
		if (OutUnit == EEphemerisUnits::AU)
		{
			Location.SetDoubleVector(state[0] / AUToKm, state[1] / AUToKm, state[2] / AUToKm);
			Velocity.SetDoubleVector(state[3] / AUToKm, state[4] / AUToKm, state[5] / AUToKm);
		}
		else 
		{
			Location.SetDoubleVector(state[0], state[1], state[2]);
			Velocity.SetDoubleVector(state[3], state[4], state[5]);
		}
	}
	else
	{

		return 1;
	}
return 0;
}

bool UJPLDE::CheckError()
{
	if (failed_c())
	{
		SpiceChar ErrShrt[200];
		SpiceChar ErrLong[2000];
		getmsg_c("SHORT", 200, ErrShrt);
		getmsg_c("LONG", 2000, ErrLong);
		FString ShortErrorStr(ErrShrt);
		FString LongErrorStr(ErrLong);
		UE_LOG(LogTemp, Warning, TEXT("Ephemeris Error: %s"), *ShortErrorStr);
		UE_LOG(LogTemp, Warning, TEXT("Err:%s"), *LongErrorStr);
		reset_c();
		return true;
	}
	return false;
}


int UJPLDE::GetCustomObjectState(int TargetID, int OriginID, FAstroDouble TDBjd1, FAstroDouble Optionaljd2, FAstroDoubleVector & Location, FAstroDoubleVector & Velocity, EPlane CoordinatePlane, EEphemerisUnits OutUnit)
{
	//UE_LOG(LogTemp, Warning, TEXT("-------------------------------------------------------"));

	SpiceDouble    state[6];
	SpiceDouble    lt;

	//Seconds from j2000
	double spref = (double(TDBjd1) - J2000ET) * SecondsPerDay;
	//UE_LOG(LogTemp, Warning, TEXT("spref: %s"), *FString::SanitizeFloat(spref));

	
	if (CoordinatePlane == EPlane::Ecliptical)
	{
		spkez_c(TargetID, spref, "ECLIPJ2000", "NONE", OriginID, state, &lt);

	}
	else
	{
		spkez_c(TargetID, spref, "J2000", "NONE", OriginID, state, &lt);
	}
	if (!CheckError())
	{
		if (OutUnit == EEphemerisUnits::AU)
		{
			Location.SetDoubleVector(state[0] / AUToKm, state[1] / AUToKm, state[2] / AUToKm);
			Velocity.SetDoubleVector(state[3] / AUToKm, state[4] / AUToKm, state[5] / AUToKm);
		}
		else
		{
			Location.SetDoubleVector(state[0], state[1], state[2]);
			Velocity.SetDoubleVector(state[3], state[4], state[5] );
		}

	}
	else
	{

		return 1;
	}

	return 0;

	
}

int UJPLDE::GetCustomObjectStateByName(FString TargetName, FString OriginID, FAstroDouble TDBjd1, FAstroDouble Optionaljd2, FAstroDoubleVector& Location, FAstroDoubleVector& Velocity, EPlane CoordinatePlane, const EEphemerisUnits OutUnit)
{

	SpiceDouble    state[6];
	SpiceDouble    lt;

	//Seconds from j2000
	double spref = (double(TDBjd1) - J2000ET) * SecondsPerDay;
	//UE_LOG(LogTemp, Warning, TEXT("spref: %s"), *FString::SanitizeFloat(spref));



	if (CoordinatePlane == EPlane::Ecliptical)
	{
		spkezr_c(TCHAR_TO_ANSI(*TargetName), spref, "ECLIPJ2000", "NONE", TCHAR_TO_ANSI(*OriginID), state, &lt);

	}
	else
	{
		spkezr_c(TCHAR_TO_ANSI(*TargetName), spref, "J2000", "NONE", TCHAR_TO_ANSI(*OriginID), state, &lt);
	}
	if (!CheckError())
	{
		if (OutUnit == EEphemerisUnits::AU)
		{
			Location.SetDoubleVector(state[0] / AUToKm, state[1] / AUToKm, state[2] / AUToKm);
			Velocity.SetDoubleVector(state[3] / AUToKm, state[4] / AUToKm, state[5] / AUToKm);
		}
		else
		{
			Location.SetDoubleVector(state[0], state[1], state[2]);
			Velocity.SetDoubleVector(state[3], state[4], state[5]);
		}

	}
	else
	{

		return 1;
	}
	return 0;
}

int UJPLDE::GetObjectOrientation(EBodys ObjBody, FAstroDouble TDBjd1, FVector& Axis, float& Rotation, EPlane CoordinatePlane)
{
	double spref = (double(TDBjd1) - J2000ET) * SecondsPerDay;
	int TargetInt = GetIntForBody(ObjBody);

	FString IAUPrefix = "IAU_";

	SpiceChar BodyName[50];
	bodc2s_c(TargetInt, 50, BodyName);
	
	FString FullBodyNameFstr = IAUPrefix + BodyName;

	if (CheckError())
	{
		return 1;
	}

	char * BodyNameCStr = TCHAR_TO_ANSI(*FullBodyNameFstr);
	char * ptr;
	ptr = strchr((char*)BodyNameCStr, ' ');
	if (ptr != NULL) {
		*ptr = '\0';
	}
	SpiceDouble mat[3][3];
	if (CoordinatePlane == EPlane::Ecliptical)
	{
		pxform_c("ECLIPJ2000", BodyNameCStr, spref, mat);
	}
	else
	{
		pxform_c("J2000", BodyNameCStr, spref, mat);
	}

	SpiceDouble AxisVec[3];
	SpiceDouble Angle;
	FAstroDoubleVector DblAxis;
	raxisa_c(mat, AxisVec, &Angle);



	if (!CheckError())
	{
		DblAxis = FAstroDoubleVector(AxisVec[0], AxisVec[1], AxisVec[2]);
		Rotation = Angle;
		Rotation = FMath::RadiansToDegrees(Rotation);

		Axis = DblAxis.GetFVector();
		Axis.Y = Axis.Y * -1;

	}
	else
	{
		return 1;
	}

	
	return 0;
}


bool UJPLDE::IsInitialized()
{
	return isInitialized;
}

/*
bool UJPLDE::ContainsDataForDateFromFDateTime(FDateTime InDate)
{

	//Not implemented
	return true;
}

bool UJPLDE::ContainsDataForDateFromJD(FAstroDouble JulianDate)
{

	//Not implemented
	return false;
}

FAstroDouble UJPLDE::StartDate()
{

	//Not implemented
		UE_LOG(LogTemp, Warning, TEXT("File Coverage 2-------------"));
		SPICEDOUBLE_CELL(cover2, 1000);
	spkcov_c(TCHAR_TO_ANSI(*SPKFilePath), -125544, &cover2);
		SpiceInt niv = wncard_c(&cover2);
			for (int i = 0; i < niv; i++)
			{
				SpiceDouble             b;
				SpiceDouble             e;
				SpiceChar               timstr[51];
				wnfetd_c(&cover2, i, &b, &e);

				timout_c(b, "YYYY MON DD HR:MN:SC.### (TDB) ::TDB", 51,timstr);
				FString tempDStr;
				tempDStr = timstr;
				UE_LOG(LogTemp, Warning, TEXT("Interval: %d  Start: %s"), i, *tempDStr);


				timout_c(e, "YYYY MON DD HR:MN:SC.### (TDB) ::TDB", 51,timstr);

				tempDStr = timstr;
				UE_LOG(LogTemp, Warning, TEXT("Stop: % s"), *tempDStr);


			}
	
	return FAstroDouble();

}

FAstroDouble UJPLDE::EndDate()
{
	//Not implemented
	return FAstroDouble();
}
*/

