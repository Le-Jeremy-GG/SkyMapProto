// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#include "EphemerisObject.h"
#include "JPLLowPrecision.generated.h"


UCLASS()
class UJPLLowPrecision : public UEphemerisObject
{

	GENERATED_BODY() 

public:

	UJPLLowPrecision();

	UFUNCTION(BlueprintPure, Category = "AstroLib|Ephemeris|JPLLowPrecision", meta=(Keywords="ios planet position"))
	static void CalcHelioLocation(EBodys ObjBody, FAstroDouble jd1, FAstroDouble Optionaljd2, FAstroDoubleVector & Location);

	static int MoonGeocentricEquatorialPosition(FAstroDouble jd1, FAstroDouble Optionaljd2, FAstroDoubleVector & Location);

	virtual int InitEphemeris(TArray<FString> DataFiles) override { return 0; }; //no init needed

	virtual int GetObjectState(EBodys ObjBody, EBodys ObjOrigin, FAstroDouble jd1, FAstroDouble Optionaljd2, FAstroDoubleVector & Location, FAstroDoubleVector & Velocity, const EPlane CoordinatePlane = EPlane::Equatorial, const EEphemerisUnits OutUnit = EEphemerisUnits::AU) override;

	virtual int CloseEphemeris() override { return 0; }; //no closing needed.

	static double D2R(double InDeg);

	static double R2D(double InRad);

	UFUNCTION(BlueprintPure, Category = "AstroLib|Ephemeris|JPLLowPrecision", meta=(Keywords="julian date"))
	static FAstroDouble GetLowPrecisionJulianDate(FDateTime inDate);

	virtual bool IsInitialized() override { return true; };

};

