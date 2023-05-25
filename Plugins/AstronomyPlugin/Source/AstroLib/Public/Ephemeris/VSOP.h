// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#include "EphemerisObject.h"
#include "VSOP.generated.h"


class UVSOPMercury;
class UVSOPVenus;
class UVSOPEarth;
class UVSOPMars;
class UVSOPJupiter;
class UVSOPSaturn;
class UVSOPUranus;
class UVSOPNeptune;
class UVSOPSun;


UCLASS()
class UVSOP : public UEphemerisObject
{
	GENERATED_BODY()

public:

virtual	double HelioX0(double t);
virtual	double HelioX1(double t);
virtual	double HelioX2(double t);
virtual double HelioX3(double t);
virtual	double HelioX4(double t);
virtual	double HelioX5(double t);

virtual	double HelioY0(double t);
virtual	double HelioY1(double t);
virtual	double HelioY2(double t);
virtual	double HelioY3(double t);
virtual	double HelioY4(double t);
virtual double HelioY5(double t);

virtual	double HelioZ0(double t);
virtual	double HelioZ1(double t);
virtual	double HelioZ2(double t);
virtual	double HelioZ3(double t);
virtual	double HelioZ4(double t);
virtual	double HelioZ5(double t);

virtual	double BaryX0(double t);
virtual	double BaryX1(double t);
virtual	double BaryX2(double t);
virtual double BaryX3(double t);
virtual	double BaryX4(double t);
virtual	double BaryX5(double t);

virtual	double BaryY0(double t);
virtual	double BaryY1(double t);
virtual	double BaryY2(double t);
virtual	double BaryY3(double t);
virtual	double BaryY4(double t);
virtual double BaryY5(double t);

virtual	double BaryZ0(double t);
virtual	double BaryZ1(double t);
virtual	double BaryZ2(double t);
virtual	double BaryZ3(double t);
virtual	double BaryZ4(double t);
virtual	double BaryZ5(double t);



public:
	UVSOP();

	void ConvertToEquatorial(FAstroDoubleVector & InVec);

	void GetCoords(double InDate, EBodys InCentralBody, FAstroDoubleVector & Location, FAstroDoubleVector & Velocity, const EPlane CoordinatePlane = EPlane::Equatorial, const EEphemerisUnits OutUnit = EEphemerisUnits::AU);


	virtual int InitEphemeris(TArray<FString> DataFiles) override;

		virtual int CloseEphemeris() override; //no closing needed.

		virtual int GetObjectState(EBodys ObjBody, EBodys ObjOrigin, FAstroDouble jd1, FAstroDouble Optionaljd2, FAstroDoubleVector & Location, FAstroDoubleVector & Velocity, const EPlane CoordinatePlane = EPlane::Equatorial, const EEphemerisUnits OutUnit = EEphemerisUnits::AU) override;

		virtual bool IsInitialized() override { return bHasBeenActivated; };

	UPROPERTY()
	UVSOPMercury* MercuryLink;

	UPROPERTY()
	UVSOPVenus* VenusLink;
	
	UPROPERTY()
	UVSOPEarth* EarthLink;

	UPROPERTY()
	UVSOPMars* MarsLink;

	UPROPERTY()
	UVSOPJupiter* JupiterLink;

	UPROPERTY()
	UVSOPSaturn* SaturnLink;

	UPROPERTY()
	UVSOPUranus*UranusLink;

	UPROPERTY()
	UVSOPNeptune* NeptuneLink;

	UPROPERTY()
	UVSOPSun* SunLink;

	UPROPERTY()
	bool bHasBeenActivated = false;
};

UCLASS()
class UVSOPMercury : public UVSOP
{

	GENERATED_BODY()

private:
	virtual double HelioX0(double t);
	virtual	double HelioX1(double t);
	virtual	double HelioX2(double t);
	virtual	double HelioX3(double t);
	virtual	double HelioX4(double t);
	virtual	double HelioX5(double t);

	virtual	double HelioY0(double t);
	virtual	double HelioY1(double t);
	virtual	double HelioY2(double t);
	virtual	double HelioY3(double t);
	virtual	double HelioY4(double t);
	virtual	double HelioY5(double t);

	virtual	double HelioZ0(double t);
	virtual	double HelioZ1(double t);
	virtual	double HelioZ2(double t);
	virtual	double HelioZ3(double t);
	virtual	double HelioZ4(double t);
	virtual	double HelioZ5(double t);

	virtual	double BaryX0(double t);
	virtual	double BaryX1(double t);
	virtual	double BaryX2(double t);
	virtual double BaryX3(double t);
	virtual	double BaryX4(double t);
	virtual	double BaryX5(double t);

	virtual	double BaryY0(double t);
	virtual	double BaryY1(double t);
	virtual	double BaryY2(double t);
	virtual	double BaryY3(double t);
	virtual	double BaryY4(double t);
	virtual double BaryY5(double t);

	virtual	double BaryZ0(double t);
	virtual	double BaryZ1(double t);
	virtual	double BaryZ2(double t);
	virtual	double BaryZ3(double t);
	virtual	double BaryZ4(double t);
	virtual	double BaryZ5(double t);

public:

};

UCLASS()
class UVSOPVenus : public UVSOP
{

	GENERATED_BODY()

private:
	virtual	double HelioX0(double t);
	virtual	double HelioX1(double t);
	virtual	double HelioX2(double t);
	virtual	double HelioX3(double t);
	virtual	double HelioX4(double t);
	virtual	double HelioX5(double t);

	virtual	double HelioY0(double t);
	virtual	double HelioY1(double t);
	virtual	double HelioY2(double t);
	virtual	double HelioY3(double t);
	virtual	double HelioY4(double t);
	virtual	double HelioY5(double t);

	virtual	double HelioZ0(double t);
	virtual	double HelioZ1(double t);
	virtual	double HelioZ2(double t);
	virtual	double HelioZ3(double t);
	virtual	double HelioZ4(double t);
	virtual	double HelioZ5(double t);

	virtual	double BaryX0(double t);
	virtual	double BaryX1(double t);
	virtual	double BaryX2(double t);
	virtual double BaryX3(double t);
	virtual	double BaryX4(double t);
	virtual	double BaryX5(double t);

	virtual	double BaryY0(double t);
	virtual	double BaryY1(double t);
	virtual	double BaryY2(double t);
	virtual	double BaryY3(double t);
	virtual	double BaryY4(double t);
	virtual double BaryY5(double t);

	virtual	double BaryZ0(double t);
	virtual	double BaryZ1(double t);
	virtual	double BaryZ2(double t);
	virtual	double BaryZ3(double t);
	virtual	double BaryZ4(double t);
	virtual	double BaryZ5(double t);

public:

};

UCLASS()
class UVSOPEarth : public UVSOP
{

	GENERATED_BODY()
private:
	virtual	double HelioX0(double t);
	virtual	double HelioX1(double t);
	virtual	double HelioX2(double t);
	virtual	double HelioX3(double t);
	virtual	double HelioX4(double t);
	virtual	double HelioX5(double t);

	virtual	double HelioY0(double t);
	virtual	double HelioY1(double t);
	virtual	double HelioY2(double t);
	virtual	double HelioY3(double t);
	virtual	double HelioY4(double t);
	virtual	double HelioY5(double t);

	virtual	double HelioZ0(double t);
	virtual	double HelioZ1(double t);
	virtual	double HelioZ2(double t);
	virtual	double HelioZ3(double t);
	virtual	double HelioZ4(double t);
	virtual	double HelioZ5(double t);

	virtual	double BaryX0(double t);
	virtual	double BaryX1(double t);
	virtual	double BaryX2(double t);
	virtual double BaryX3(double t);
	virtual	double BaryX4(double t);
	virtual	double BaryX5(double t);

	virtual	double BaryY0(double t);
	virtual	double BaryY1(double t);
	virtual	double BaryY2(double t);
	virtual	double BaryY3(double t);
	virtual	double BaryY4(double t);
	virtual double BaryY5(double t);

	virtual	double BaryZ0(double t);
	virtual	double BaryZ1(double t);
	virtual	double BaryZ2(double t);
	virtual	double BaryZ3(double t);
	virtual	double BaryZ4(double t);
	virtual	double BaryZ5(double t);

public:

};

UCLASS()
class UVSOPMars : public UVSOP
{

	GENERATED_BODY()
private:
	virtual	double HelioX0(double t);
	virtual	double HelioX1(double t);
	virtual	double HelioX2(double t);
	virtual	double HelioX3(double t);
	virtual	double HelioX4(double t);
	virtual	double HelioX5(double t);

	virtual	double HelioY0(double t);
	virtual	double HelioY1(double t);
	virtual	double HelioY2(double t);
	virtual	double HelioY3(double t);
	virtual	double HelioY4(double t);
	virtual	double HelioY5(double t);

	virtual	double HelioZ0(double t);
	virtual	double HelioZ1(double t);
	virtual	double HelioZ2(double t);
	virtual	double HelioZ3(double t);
	virtual	double HelioZ4(double t);
	virtual	double HelioZ5(double t);

	virtual	double BaryX0(double t);
	virtual	double BaryX1(double t);
	virtual	double BaryX2(double t);
	virtual double BaryX3(double t);
	virtual	double BaryX4(double t);
	virtual	double BaryX5(double t);

	virtual	double BaryY0(double t);
	virtual	double BaryY1(double t);
	virtual	double BaryY2(double t);
	virtual	double BaryY3(double t);
	virtual	double BaryY4(double t);
	virtual double BaryY5(double t);

	virtual	double BaryZ0(double t);
	virtual	double BaryZ1(double t);
	virtual	double BaryZ2(double t);
	virtual	double BaryZ3(double t);
	virtual	double BaryZ4(double t);
	virtual	double BaryZ5(double t);

public:

};

UCLASS()
class UVSOPJupiter : public UVSOP
{

	GENERATED_BODY()
private:
	virtual	double HelioX0(double t);
	virtual	double HelioX1(double t);
	virtual	double HelioX2(double t);
	virtual	double HelioX3(double t);
	virtual	double HelioX4(double t);
	virtual	double HelioX5(double t);

	virtual	double HelioY0(double t);
	virtual	double HelioY1(double t);
	virtual	double HelioY2(double t);
	virtual	double HelioY3(double t);
	virtual	double HelioY4(double t);
	virtual	double HelioY5(double t);

	virtual	double HelioZ0(double t);
	virtual	double HelioZ1(double t);
	virtual	double HelioZ2(double t);
	virtual	double HelioZ3(double t);
	virtual	double HelioZ4(double t);
	virtual	double HelioZ5(double t);
	virtual	double BaryX0(double t);
	virtual	double BaryX1(double t);
	virtual	double BaryX2(double t);
	virtual double BaryX3(double t);
	virtual	double BaryX4(double t);
	virtual	double BaryX5(double t);

	virtual	double BaryY0(double t);
	virtual	double BaryY1(double t);
	virtual	double BaryY2(double t);
	virtual	double BaryY3(double t);
	virtual	double BaryY4(double t);
	virtual double BaryY5(double t);

	virtual	double BaryZ0(double t);
	virtual	double BaryZ1(double t);
	virtual	double BaryZ2(double t);
	virtual	double BaryZ3(double t);
	virtual	double BaryZ4(double t);
	virtual	double BaryZ5(double t);

public:

};

UCLASS()
class UVSOPSaturn : public UVSOP
{

	GENERATED_BODY()
private:
	virtual	double HelioX0(double t);
	virtual	double HelioX1(double t);
	virtual	double HelioX2(double t);
	virtual	double HelioX3(double t);
	virtual	double HelioX4(double t);
	virtual	double HelioX5(double t);

	virtual	double HelioY0(double t);
	virtual	double HelioY1(double t);
	virtual	double HelioY2(double t);
	virtual	double HelioY3(double t);
	virtual	double HelioY4(double t);
	virtual	double HelioY5(double t);

	virtual	double HelioZ0(double t);
	virtual	double HelioZ1(double t);
	virtual	double HelioZ2(double t);
	virtual	double HelioZ3(double t);
	virtual	double HelioZ4(double t);
	virtual	double HelioZ5(double t);

	virtual	double BaryX0(double t);
	virtual	double BaryX1(double t);
	virtual	double BaryX2(double t);
	virtual double BaryX3(double t);
	virtual	double BaryX4(double t);
	virtual	double BaryX5(double t);

	virtual	double BaryY0(double t);
	virtual	double BaryY1(double t);
	virtual	double BaryY2(double t);
	virtual	double BaryY3(double t);
	virtual	double BaryY4(double t);
	virtual double BaryY5(double t);

	virtual	double BaryZ0(double t);
	virtual	double BaryZ1(double t);
	virtual	double BaryZ2(double t);
	virtual	double BaryZ3(double t);
	virtual	double BaryZ4(double t);
	virtual	double BaryZ5(double t);

public:

};

UCLASS()
class UVSOPUranus : public UVSOP
{

	GENERATED_BODY()
private:
	virtual	double HelioX0(double t);
	virtual	double HelioX1(double t);
	virtual	double HelioX2(double t);
	virtual	double HelioX3(double t);
	virtual	double HelioX4(double t);
	virtual	double HelioX5(double t);

	virtual	double HelioY0(double t);
	virtual	double HelioY1(double t);
	virtual	double HelioY2(double t);
	virtual	double HelioY3(double t);
	virtual	double HelioY4(double t);
	virtual	double HelioY5(double t);

	virtual	double HelioZ0(double t);
	virtual	double HelioZ1(double t);
	virtual	double HelioZ2(double t);
	virtual	double HelioZ3(double t);
	virtual	double HelioZ4(double t);
	virtual	double HelioZ5(double t);

	virtual	double BaryX0(double t);
	virtual	double BaryX1(double t);
	virtual	double BaryX2(double t);
	virtual double BaryX3(double t);
	virtual	double BaryX4(double t);
	virtual	double BaryX5(double t);

	virtual	double BaryY0(double t);
	virtual	double BaryY1(double t);
	virtual	double BaryY2(double t);
	virtual	double BaryY3(double t);
	virtual	double BaryY4(double t);
	virtual double BaryY5(double t);

	virtual	double BaryZ0(double t);
	virtual	double BaryZ1(double t);
	virtual	double BaryZ2(double t);
	virtual	double BaryZ3(double t);
	virtual	double BaryZ4(double t);
	virtual	double BaryZ5(double t);

public:

};

UCLASS()
class UVSOPNeptune : public UVSOP
{

	GENERATED_BODY()
private:
	virtual	double HelioX0(double t);
	virtual	double HelioX1(double t);
	virtual	double HelioX2(double t);
	virtual	double HelioX3(double t);
	virtual	double HelioX4(double t);
	virtual	double HelioX5(double t);

	virtual	double HelioY0(double t);
	virtual	double HelioY1(double t);
	virtual	double HelioY2(double t);
	virtual	double HelioY3(double t);
	virtual	double HelioY4(double t);
	virtual	double HelioY5(double t);

	virtual	double HelioZ0(double t);
	virtual	double HelioZ1(double t);
	virtual	double HelioZ2(double t);
	virtual	double HelioZ3(double t);
	virtual	double HelioZ4(double t);
	virtual	double HelioZ5(double t);

	virtual	double BaryX0(double t);
	virtual	double BaryX1(double t);
	virtual	double BaryX2(double t);
	virtual double BaryX3(double t);
	virtual	double BaryX4(double t);
	virtual	double BaryX5(double t);

	virtual	double BaryY0(double t);
	virtual	double BaryY1(double t);
	virtual	double BaryY2(double t);
	virtual	double BaryY3(double t);
	virtual	double BaryY4(double t);
	virtual double BaryY5(double t);

	virtual	double BaryZ0(double t);
	virtual	double BaryZ1(double t);
	virtual	double BaryZ2(double t);
	virtual	double BaryZ3(double t);
	virtual	double BaryZ4(double t);
	virtual	double BaryZ5(double t);

public:

};

UCLASS()
class UVSOPSun : public UVSOP
{

	GENERATED_BODY()
private:
	virtual	double BaryX0(double t);
	virtual	double BaryX1(double t);
	virtual	double BaryX2(double t);
	virtual	double BaryX3(double t);
	virtual	double BaryX4(double t);
	virtual	double BaryX5(double t);

	virtual	double BaryY0(double t);
	virtual	double BaryY1(double t);
	virtual	double BaryY2(double t);
	virtual	double BaryY3(double t);
	virtual	double BaryY4(double t);
	virtual	double BaryY5(double t);

	virtual	double BaryZ0(double t);
	virtual	double BaryZ1(double t);
	virtual	double BaryZ2(double t);
	virtual	double BaryZ3(double t);
	virtual	double BaryZ4(double t);
	virtual	double BaryZ5(double t);

public:


};