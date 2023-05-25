// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#include <string.h>
#include "Misc/CString.h"
#include "NovasCFunctions.h"
#include "DoubleFunction.h"
#include "Enumerations.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AstroStructs.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAstroLog, Warning, Warning);


USTRUCT(BlueprintType, meta = (HasNativeMake = "AstroLib.AstroStructs.MakeStarCatalogEntry"))
struct FStarCatalogEntry
{
	GENERATED_BODY()

public:

	//optional NameKey, used as the first column when importing from a DataRow to help with looking up catalog data from a datatable.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FName NameKey;

	//Name of celestial object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FString StarNameFString;
	//char StarName[SIZE_OF_OBJ_NAME];

	//Catalog designator / Name(e.g., HIP)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FString CatalogFString;
	//char Catalog[SIZE_OF_CAT_NAME];

	//Integer identifier assigned to object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	int StarNumberInt = 0;
	long int StarNumber;

	//ICRS right ascension(hours)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDouble RightAscenstion;

	//ICRS declination(degrees)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDouble Declination;

	//ICRS proper motion in right ascension(milliarcseconds / year)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDouble ProperMotionInRightAscension;

	//ICRS proper motion in declination (milliarcseconds / year)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDouble ProperMotionInDeclination;

	//promodec = parallax(milliarcseconds) 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDouble Parallax;

	//radial velocity(km / s)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDouble RadialVelocity;

	//Visual magnitude (optional)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	float VisualMagnitude=0;


	cat_entry GetNovasStruct()
	{

		cat_entry star;

		//FCString::Strcpy(UTF8_TO_TCHAR(StarName), SIZE_OF_OBJ_NAME, *StarNameFString.Left(SIZE_OF_OBJ_NAME));
		//FCString::Strcpy(UTF8_TO_TCHAR(Catalog), SIZE_OF_CAT_NAME, *CatalogFString.Left(SIZE_OF_CAT_NAME));

		//make_cat_entry((char*)StarName, (char*)Catalog, StarNumber, RightAscenstion * 0.06666666666666668, Declination, ProperMotionInRightAscension, ProperMotionInDeclination, Parallax, RadialVelocity, &star);

		make_cat_entry(TCHAR_TO_ANSI(*StarNameFString.Left(SIZE_OF_OBJ_NAME)), TCHAR_TO_ANSI(*CatalogFString.Left(SIZE_OF_CAT_NAME)), StarNumber, RightAscenstion * 0.06666666666666668, Declination, ProperMotionInRightAscension, ProperMotionInDeclination, Parallax, RadialVelocity, &star);

		return star;
	}

	void SetupCatalogEntry(FString Name, FString Cat, long int Num, double raf, double decf, double pmraf, double pmdecf, double parallaxf, double radvecf, float VisMag=0)
	{
		StarNameFString = Name;
		//FCString::Strcpy(UTF8_TO_TCHAR(StarName), SIZE_OF_OBJ_NAME, *Name.Left(SIZE_OF_OBJ_NAME));
		CatalogFString = Cat;
		//FCString::Strcpy(UTF8_TO_TCHAR(Catalog), SIZE_OF_CAT_NAME, *Cat.Left(SIZE_OF_CAT_NAME));
		StarNumber = Num;
		RightAscenstion = raf;
		Declination = decf;
		ProperMotionInRightAscension = pmraf;
		ProperMotionInDeclination = pmdecf;
		Parallax = parallaxf;
		RadialVelocity = radvecf;
		VisualMagnitude = VisMag;
	}

	FStarCatalogEntry()
	{


	}

	FStarCatalogEntry(cat_entry inCatEntry)
	{
		SetupCatalogEntry(inCatEntry.starname, inCatEntry.catalog, inCatEntry.starnumber, inCatEntry.ra, inCatEntry.dec, inCatEntry.promora, inCatEntry.promodec, inCatEntry.parallax, inCatEntry.radialvelocity);
	}

	FStarCatalogEntry(FString Name, FString Cat, long int Num, double raf, double decf, double pmraf, double pmdecf, double parallaxf, double radvecf, float VisMag=0)
	{
		SetupCatalogEntry(Name, Cat, Num, raf, decf, pmraf, pmdecf, parallaxf, radvecf, VisMag);
	}

	void Refresh()
	{
			RightAscenstion.Refresh();
			 Declination.Refresh();
			 ProperMotionInRightAscension.Refresh();
			 ProperMotionInDeclination.Refresh();
			 Parallax.Refresh();
			 RadialVelocity.Refresh();

	
	}
	
	
};


USTRUCT(BlueprintType)
struct FEarthSurfaceLocation
{
	GENERATED_BODY()

public:

	//Geodetic (ITRS) latitude; north positive (degrees)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	float Latitude =90;

	//Geodetic (ITRS) longitude; east positive (degrees)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	float Longitude;

	//Height of the observer (meters)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	float Height;

	//Height of the observer (meters). Only used for refraction calculations in Topocentric to AzEl Horizon function.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	float Temperature;
	
	//Atmospheric pressure (millibars). Only used for refraction calculations in Topocentric to AzEl Horizon function.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	float Pressure;
	
	on_surface GetNovasStruct()
	{
		on_surface OnSurface;
		make_on_surface(Latitude, Longitude, Height, Temperature, Pressure, &OnSurface);
		return OnSurface;
	}
};

//novas struct: in_space
USTRUCT(BlueprintType)
struct FSpaceLocation
{
	GENERATED_BODY()

public:

	//geocentric position vector (x, y, z), components in km. With respect to true equator and equinox of date.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDoubleVector SpaceLocation;

	//geocentric velocity vector (x_dot, y_dot, z_dot), components in km / s. With respect to true equator and equinox of date.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDoubleVector VelocityVector;

	in_space GetNovasStruct()
	{
		in_space InSpace;
		double Pos[3];
		Pos[0] = SpaceLocation.x;
		Pos[1] = SpaceLocation.y;
		Pos[2] = SpaceLocation.z;

		double Vel[3];
		Vel[0] = VelocityVector.x;
		Vel[1] = VelocityVector.y;
		Vel[2] = VelocityVector.z;
		
		make_in_space(Pos, Vel, &InSpace);
		return InSpace;
	}
	
};

//novas struct Observer
USTRUCT(BlueprintType)
struct FObserver
{
	GENERATED_BODY()

public:

	//Location Type of observer.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	EWhereIsObserver LocationType;

	//data for an observer's location on the surface of the Earth (Used when LocationType = EarthSurface)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FEarthSurfaceLocation SurfaceLocation;

	//data for an observer's location in space (Used when LocationType = NearEarthSpaceCraft)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FSpaceLocation SpaceLocation;


	observer GetNovasStruct()
	{
		observer Obs;
		short LocType = (short)LocationType;

		on_surface Surf = SurfaceLocation.GetNovasStruct();
		in_space Space = SpaceLocation.GetNovasStruct();

		make_observer(LocType, &Surf, &Space,&Obs);
		return Obs;
	}

};

USTRUCT(BlueprintType)
struct FSphereCoords
{

	GENERATED_BODY()

	//Hours
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDouble RightAscension;

	//Degrees
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDouble Declination;
	
	FSphereCoords()
	{
	}

	FSphereCoords(double ra, double dec)
	{
		RightAscension = ra;
		Declination = dec;
	}

	void Refresh()
	{
		RightAscension.Refresh();
		Declination.Refresh();
	}
};

USTRUCT(BlueprintType)
struct FLatLong
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDouble Latitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDouble Longitude;

	FLatLong()
	{
	}

	FLatLong(double Lat, double Long)
	{
		Latitude = Lat;
		Longitude = Long;
	}

	void SetLatLong(double Lat, double Long)
	{
		Latitude = Lat;
		Longitude = Long;
	}
};

//	Celestial sky position is the star's position based on changes in time and depends on the ECoordSystems reference coordinate system. Novas struct skypos
USTRUCT(BlueprintType)
struct FSkyPosition
{
	GENERATED_BODY()

public:

	//CoordType returned from Planet or Star location calculation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	ECoordTypes CoordType = ECoordTypes::Unset;
	
	//unit vector toward the object (dimensionless) returned from Planet or Star location calculation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDoubleVector PositionVector;

	//apparent, topocentric, or astrometric right ascension (hours) and declination (degrees) returned from Planet or Star location calculation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FSphereCoords SphericalCoords;

	//true (geometric, Euclidian) distance to solar system body or 0.0 for star(AU) returned from Planet or Star location calculation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDouble Distance;

	//radial velocity (km/s) returned from Planet or Star location calculation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDouble RadialVelocity;
	
	void MakeUnrealStruct(sky_pos inSkyPos)
	{
		PositionVector.SetDoubleVector(inSkyPos.r_hat[0], inSkyPos.r_hat[1], inSkyPos.r_hat[2]);
		SphericalCoords.RightAscension = inSkyPos.ra;
		SphericalCoords.Declination = inSkyPos.dec;
		Distance = inSkyPos.dis;
		RadialVelocity = inSkyPos.rv;
	}

	sky_pos GetNovasStruct()
	{
		sky_pos SkyPos;

		SkyPos.r_hat[0] = PositionVector.x;
		SkyPos.r_hat[1] = PositionVector.y;
		SkyPos.r_hat[2] = PositionVector.z.value;
		SkyPos.ra = SphericalCoords.RightAscension;
		SkyPos.dec = SphericalCoords.Declination;
		SkyPos.dis = Distance;
		SkyPos.rv = RadialVelocity;

		return SkyPos;
	}
	
	void Refresh()
	{
		PositionVector.Refresh();
		SphericalCoords.Refresh();
		Distance.Refresh();
		RadialVelocity.Refresh();

	}


};


UCLASS()
class UAstroStructs : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

		
	

	/*
	Basic astrometric data for any celestial object
	located outside the solar system; the catalog data for a star.

	@param NameF - Name of celestial object
	@param CatF - Catalog designator/Name (e.g., HIP)
	@param Num - Integer identifier assigned to object
	@param raf - ICRS right ascension (hours)
	@param decf - ICRS declination (degrees)
	@param pmraf - ICRS proper motion in right ascension (milliarcseconds/year)
	@param pmdecf - ICRS proper motion in declination (milliarcseconds/year)
	@param parallaxf - parallax (milliarcseconds)
	@param radvecf - radial velocity (km/s)
	@param VisualMagnitude - Optional visual magnitude.
	@return Reference to created FStarCatalogEntry struct.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Make StarCatalogEntry", Keywords = "star catalog, astronomy", NativeMakeFunc, Category = "AstroLib"))
	static FStarCatalogEntry MakeStarCatalogEntry(FString NameF, FString CatF, int Num, FAstroDouble raf, FAstroDouble decf, FAstroDouble pmraf, FAstroDouble pmdecf, FAstroDouble parallaxf, FAstroDouble radvecf, float VisualMagnitude =0);
	

};

