// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#if defined(_WIN32)
#define ASTROCOPYFUNCTION strncpy_s
#endif
#if defined(__MACH__)
#define ASTROCOPYFUNCTION strlcpy
#endif


#include <string.h>
#include "CoreMinimal.h"
#include "EphemerisManager.h"
#include "JPLDE.h"
#include "SpiceUsr.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AstroTLE.generated.h"

//Struct that contains data for a single Two Line Element set
USTRUCT(BlueprintType, meta = (HasNativeMake = "AstroLib.AstroTLE.MakeTLE"))
struct FSingleTLEStruct
{
	GENERATED_BODY()

	SpiceDouble epoch;
	SpiceDouble elems[10];
	bool operator<(const FSingleTLEStruct& rhs) const
	{ 

	//FTLEStruct& b = const_cast<FTLEStruct&>(rhs);
	//double testVar = b.EpochV1;
		
	//return (double(EpochV1) < (testVar));

	return double(EpochV2) < double(rhs.EpochV2);
	
	}

	//The first line of a TLE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
	FString Line1 = "";

	//The second line of a TLE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FString Line2 = "";

	//The satellite catalog number from the TLE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FString SatelliteCatalogNumber = "";

	//The NAID ID of the satellite. This is converted from the TLE satellide id by NAIFid = -100000 - Sat Catalog ID. The NAIF ID is used as the id for SPK ephemeris.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		int32 NAIFId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FString LastTwoDigitsOfLaunchYear = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FString LaunchNumberOfYear = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FString PieceOfLaunch = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FString EpochYear = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FString EpochDay = "";


	//NDT20
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FAstroDouble FirstDerivativeofMeanMotion = 0.l;

	//NDD60
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FAstroDouble SecondDerivativeofMeanMotion = 0.l;

	//BSTAR
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FAstroDouble DragTerm = 0.l;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FString ElementSetNumber = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FString ChecksumLine1 = "";

	
	//Inclination in degrees     
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FAstroDouble InclinationDegrees = 0.l;

	//Node0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FAstroDouble RightAscension = 0.l;

	//Eccentricity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FAstroDouble Eccentricity = 0.l;

	//Omega
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FAstroDouble ArgumentOfPerigee = 0.l;

	//M0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FAstroDouble MeanAnomaly = 0.l;

	//N0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FAstroDouble MeanMotion = 0.l;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FString RevolutionNumber = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FString ChecksumLine2 = "";

	//In Julian date format
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FAstroDouble EpochV1 = 0;

	//In Julian date format
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		FAstroDouble EpochV2 = 0;

	FSingleTLEStruct()
	{
		//Refresh();
	}

	FSingleTLEStruct(FString FirstLine, FString SecondLine)
	{
		SpiceChar lines[2][70];

		Line1 = FirstLine;
		Line2 = SecondLine;

		ASTROCOPYFUNCTION(lines[0], TCHAR_TO_ANSI(*Line1.Left(69)), 70);
		ASTROCOPYFUNCTION(lines[1], TCHAR_TO_ANSI(*Line2.Left(69)), 70);

		//assuming no data earlier than 2000. Since TLEs only use two diget year, need to set a base date.
		getelm_c(2000, 70, lines, &epoch, elems);

		FirstDerivativeofMeanMotion = elems[0];
		SecondDerivativeofMeanMotion = elems[1];
		DragTerm = elems[2];
		InclinationDegrees = elems[3];
		RightAscension = elems[4];
		Eccentricity = elems[5];
		ArgumentOfPerigee = elems[6];
		MeanAnomaly = elems[7];
		MeanMotion = elems[8];
		EpochV1 = elems[9];
		EpochV2 = epoch;

		
		SatelliteCatalogNumber = TCHAR_TO_UTF8(*Line1.Mid(2, 5));
		NAIFId = -100000 - FCString::Atoi(*SatelliteCatalogNumber);
		LastTwoDigitsOfLaunchYear = TCHAR_TO_UTF8(*Line1.Mid(9, 2));
		LaunchNumberOfYear = TCHAR_TO_UTF8(*Line1.Mid(11, 3));
		PieceOfLaunch = TCHAR_TO_UTF8(*Line1.Mid(14, 3));
		EpochYear = TCHAR_TO_UTF8(*Line1.Mid(18, 2));
		EpochDay = TCHAR_TO_UTF8(*Line1.Mid(20, 12));

		ElementSetNumber = TCHAR_TO_UTF8(*Line1.Mid(64, 4));
		ChecksumLine1 = TCHAR_TO_UTF8(*Line1.Mid(68, 1));
		ChecksumLine2 = TCHAR_TO_UTF8(*Line2.Mid(68, 1));
		RevolutionNumber = TCHAR_TO_UTF8(*Line2.Mid(63, 5));


	}



	void Refresh()
	{
		FSingleTLEStruct(Line1, Line2);
	}

	void DebugPrint()
	{
		UE_LOG(LogTemp, Warning, TEXT("Line1: %s"), *Line1);
		UE_LOG(LogTemp, Warning, TEXT("Line2: %s"), *Line2);

		UE_LOG(LogTemp, Warning, TEXT("SatelliteCatalogNumber: %s"), *SatelliteCatalogNumber);
		UE_LOG(LogTemp, Warning, TEXT("NAIFId: %d"), NAIFId);
		UE_LOG(LogTemp, Warning, TEXT("LastTwoDigitsOfLaunchYear: %s"), *LastTwoDigitsOfLaunchYear);
		UE_LOG(LogTemp, Warning, TEXT("LaunchNumberOfYear: %s"), *LaunchNumberOfYear);
		UE_LOG(LogTemp, Warning, TEXT("PieceOfLaunch: %s"), *PieceOfLaunch);
		UE_LOG(LogTemp, Warning, TEXT("EpochYear: %s"), *EpochYear);
		UE_LOG(LogTemp, Warning, TEXT("EpochDay: %s"), *EpochDay);
		UE_LOG(LogTemp, Warning, TEXT("FirstDerivativeofMeanMotion: %s"), *FString::SanitizeFloat(FirstDerivativeofMeanMotion));
		UE_LOG(LogTemp, Warning, TEXT("SecondDerivativeofMeanMotion: %s"), *FString::SanitizeFloat(SecondDerivativeofMeanMotion));
		UE_LOG(LogTemp, Warning, TEXT("DragTerm: %s"), *FString::SanitizeFloat(DragTerm));
		UE_LOG(LogTemp, Warning, TEXT("ElementSetNumber: %s"), *ElementSetNumber);
		UE_LOG(LogTemp, Warning, TEXT("ChecksumLine1: %s"), *ChecksumLine1);

		UE_LOG(LogTemp, Warning, TEXT("InclinationDegrees: %s"), *FString::SanitizeFloat(InclinationDegrees));
		UE_LOG(LogTemp, Warning, TEXT("RightAscension: %s"), *FString::SanitizeFloat(RightAscension));
		UE_LOG(LogTemp, Warning, TEXT("Eccentricity: %s"), *FString::SanitizeFloat(Eccentricity));
		UE_LOG(LogTemp, Warning, TEXT("ArgumentOfPerigee: %s"), *FString::SanitizeFloat(ArgumentOfPerigee));
		UE_LOG(LogTemp, Warning, TEXT("MeanAnomaly: %s"), *FString::SanitizeFloat(MeanAnomaly));
		UE_LOG(LogTemp, Warning, TEXT("MeanMotion: %s"), *FString::SanitizeFloat(MeanMotion));
		UE_LOG(LogTemp, Warning, TEXT("RevolutionNumber: %s"), *RevolutionNumber);
		UE_LOG(LogTemp, Warning, TEXT("EpochV1: %s"), *FString::SanitizeFloat(EpochV1));
		UE_LOG(LogTemp, Warning, TEXT("EpochV2: %s"), *FString::SanitizeFloat(EpochV2));

		UE_LOG(LogTemp, Warning, TEXT("elems[0]: %s"), *FString::SanitizeFloat(elems[0]));
		UE_LOG(LogTemp, Warning, TEXT("elems[1]: %s"), *FString::SanitizeFloat(elems[1]));
		UE_LOG(LogTemp, Warning, TEXT("elems[2]: %s"), *FString::SanitizeFloat(elems[2]));
		UE_LOG(LogTemp, Warning, TEXT("elems[3]: %s"), *FString::SanitizeFloat(elems[3]));
		UE_LOG(LogTemp, Warning, TEXT("elems[4]: %s"), *FString::SanitizeFloat(elems[4]));
		UE_LOG(LogTemp, Warning, TEXT("elems[5]: %s"), *FString::SanitizeFloat(elems[5]));
		UE_LOG(LogTemp, Warning, TEXT("elems[6]: %s"), *FString::SanitizeFloat(elems[6]));
		UE_LOG(LogTemp, Warning, TEXT("elems[7]: %s"), *FString::SanitizeFloat(elems[7]));
		UE_LOG(LogTemp, Warning, TEXT("elems[8]: %s"), *FString::SanitizeFloat(elems[8]));
		UE_LOG(LogTemp, Warning, TEXT("elems[9]: %s"), *FString::SanitizeFloat(elems[9]));

		UE_LOG(LogTemp, Warning, TEXT("epoch: %s"), *FString::SanitizeFloat(epoch));



	}

};

/*
	Struct that contains an array of TLEs for a single satellite of object. 
	The main reason for this struct is just to be able to have a nested array of TLEs grouped for a single satellite.
*/
USTRUCT(BlueprintType)
struct FTLEListForObject
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy TLE JPLDE Ephemeris")
		TArray<FSingleTLEStruct> TLEList;

	FTLEListForObject()
	{

	}

};

UCLASS()
class UAstroTLE : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UAstroTLE();

	/*
	Creates and populates an FSingleTLEStruct from two lines of a TLE

	@param Line1 Line 1 of a TLE
	@param Line2 Line 1 of a TLE

	@return Returns an FSingleTLEStruct which has been populated with the elements of the TLE
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Make TLEStruct", Keywords = "star catalog, astronomy, tle", NativeMakeFunc, Category = "AstroLib|Ephemeris|JPLDE|TLE"))
	static FSingleTLEStruct MakeTLE(FString Line1, FString Line2);

	
	/*
	Writes an SPK file (ephemeris) from a TLE (inputed as a FTLEListForObject). 
	Call the function ReadTLEFile to create the inTLEObject input. 
	If no filename is specified, a default of the satellite catalog number will be used.

	@param Ephem - A JPLDE Ephemeris (that has already been initialized)
	@param inTLEObject - An inTLEObject struct (usually created from the ReadTLEFile function). This is a list of TLEs (or a single TLE) for one satellite.
	@param OutFilePath - The filepath of the created SPK file.
	@param SPKFilePathandName - Specify the directory and optionally a file name for the spk file. The actual path and filename will be output in OutFilePath. Use "$GameContentDir" to access the project content directory.
	@param HoursValidBeforeFirstTLE - Number of hours before the first TLE this ephemeris file should be valid. Use -1 for valid for all time.
	@param HoursValidAfterLastTLE - Number of hours after the last TLE this ephemeris file should be valid. Use -1 for valid for all time.
	@param AutoLoadKernalAfterCreation - Set to true to automatically load the file into memory for use after it is created. (Automatically calls JPLDE LoadKernal function)

	@return 1 for error, 0 for success.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Ephemeris|JPLDE|TLE")
	static int MakeSPKFileFromTLEs(UJPLDE * Ephem, FTLEListForObject inTLEObject, FString& OutFilePath, FString SPKFilePathandName = "$GameContentDir/SPKData/", int HoursValidBeforeFirstTLE = -1, int HoursValidAfterLastTLE = -1, bool AutoLoadKernalAfterCreation = true);

	/*
	Reads a file containing a list of TLEs, and outputs an array of FTLEListForObject for each satellite found in the file.

	@param Ephem - A JPLDE Ephemeris (that has already been initialized)
	@param FilePath - The filepath to the tle file. Use "$GameContentDir/" to access the project content directory.
	@param OutTLEObjectsArray - An array of FTLEListForObject, one FTLEListForObject for each object found in the TLE file.

	@return 1 for error, 0 for success.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Ephemeris|JPLDE|TLE")
	static int ReadTLEFile(UJPLDE* Ephem, FString FilePath, TArray<FTLEListForObject> & OutTLEObjectsArray);

	/*
	Converts the TLE Satellite catalog ID to the NAID ID of the satellite used in JPLDE Ephemeris.  This is converted from the TLE satellide id by NAIFid = -100000 - Sat Catalog ID. The NAIF ID is used as the id for SPK ephemeris.
	@param TLEId - The Satellite Catalog number from a TLE

	@return The NAID of the satellite for use in JPLDE Ephemeris. 
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get JPLId From TLE Id", Keywords = "getjpl id naif satellite number star catalog, astronomy, tle", Category = "AstroLib|Ephemeris|JPLDE|TLE"))
	static int ConvertTLEIdtoJPLId(int TLEId);

};

