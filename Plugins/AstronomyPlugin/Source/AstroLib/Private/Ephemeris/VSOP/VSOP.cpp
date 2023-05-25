// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.


#pragma once

#include "Ephemeris/VSOP.h"
#include "Ephemeris/JPLLowPrecision.h"
#include "Ephemeris/EphemerisManager.h"
#include "AstroLibBPLibrary.h"



UVSOP::UVSOP()
{

	Name = "VSOP";



}





void UVSOP::GetCoords(double InDate, EBodys InCentralBody, FAstroDoubleVector & Location, FAstroDoubleVector & Velocity, EPlane CoordinatePlane, const EEphemerisUnits OutUnit)
{
	double T = (InDate - 2451545) / 365250; //get julian centuries
	
	if (InCentralBody == EBodys::Sun)
	{
		double X = HelioX0(T) + HelioX1(T) + HelioX2(T) + HelioX3(T) + HelioX4(T) + HelioX5(T);
		double Y = HelioY0(T) + HelioY1(T) + HelioY2(T) + HelioY3(T) + HelioY4(T) + HelioY5(T);
		double Z = HelioZ0(T) + HelioZ1(T) + HelioZ2(T) + HelioZ3(T) + HelioZ4(T) + HelioZ5(T);

		T = (InDate + 1 - 2451545) / 365250; //One day in JD

		double X2 = HelioX0(T) + HelioX1(T) + HelioX2(T) + HelioX3(T) + HelioX4(T) + HelioX5(T);
		double Y2 = HelioY0(T) + HelioY1(T) + HelioY2(T) + HelioY3(T) + HelioY4(T) + HelioY5(T);
		double Z2 = HelioZ0(T) + HelioZ1(T) + HelioZ2(T) + HelioZ3(T) + HelioZ4(T) + HelioZ5(T);

		Velocity.SetDoubleVector(X2 - X, Y2 - Y, Z2 - Z);
		Location.SetDoubleVector(X, Y, Z);

		if (OutUnit == EEphemerisUnits::Kilometers)
		{
			Location.SetDoubleVector(Location.x * AUToKm, Location.y * AUToKm, Location.z * AUToKm);
			Velocity.SetDoubleVector(Velocity.x * AUToKm, Velocity.y * AUToKm, Velocity.z * AUToKm);
		}

		if (CoordinatePlane == EPlane::Equatorial)
		{
			ConvertToEquatorial(Location);
			ConvertToEquatorial(Velocity);
		}


	}

	else if (InCentralBody == EBodys::SolarSystemBarycenter)
	{
		double X = BaryX0(T) + BaryX1(T) + BaryX2(T) + BaryX3(T) + BaryX4(T) + BaryX5(T);
		double Y = BaryY0(T) + BaryY1(T) + BaryY2(T) + BaryY3(T) + BaryY4(T) + BaryY5(T);
		double Z = BaryZ0(T) + BaryZ1(T) + BaryZ2(T) + BaryZ3(T) + BaryZ4(T) + BaryZ5(T);

		T = (InDate+1 - 2451545) / 365250;//One day in JD
		
		double X2 = BaryX0(T) + BaryX1(T) + BaryX2(T) + BaryX3(T) + BaryX4(T) + BaryX5(T);
		double Y2 = BaryY0(T) + BaryY1(T) + BaryY2(T) + BaryY3(T) + BaryY4(T) + BaryY5(T);
		double Z2 = BaryZ0(T) + BaryZ1(T) + BaryZ2(T) + BaryZ3(T) + BaryZ4(T) + BaryZ5(T);

		Velocity.SetDoubleVector(X2 - X, Y2 - Y, Z2 - Z);
		Location.SetDoubleVector(X, Y, Z);

		if (OutUnit == EEphemerisUnits::Kilometers)
		{
			Location.SetDoubleVector(Location.x * AUToKm, Location.y * AUToKm, Location.z * AUToKm);
			Velocity.SetDoubleVector(Velocity.x * AUToKm, Velocity.y * AUToKm, Velocity.z * AUToKm);
		}


		if (CoordinatePlane == EPlane::Equatorial)
		{
			ConvertToEquatorial(Location);
			ConvertToEquatorial(Velocity);
		}


	}
	else
	{
		double X = BaryX0(T) + BaryX1(T) + BaryX2(T) + BaryX3(T) + BaryX4(T) + BaryX5(T);
		double Y = BaryY0(T) + BaryY1(T) + BaryY2(T) + BaryY3(T) + BaryY4(T) + BaryY5(T);
		double Z = BaryZ0(T) + BaryZ1(T) + BaryZ2(T) + BaryZ3(T) + BaryZ4(T) + BaryZ5(T);


		Location.SetDoubleVector(X, Y, Z);
		
		T = (InDate + 1 - 2451545) / 365250; //One day in JD

		double X2 = BaryX0(T) + BaryX1(T) + BaryX2(T) + BaryX3(T) + BaryX4(T) + BaryX5(T);
		double Y2 = BaryY0(T) + BaryY1(T) + BaryY2(T) + BaryY3(T) + BaryY4(T) + BaryY5(T);
		double Z2 = BaryZ0(T) + BaryZ1(T) + BaryZ2(T) + BaryZ3(T) + BaryZ4(T) + BaryZ5(T);

		//unit is au per day
		Velocity.SetDoubleVector(X2 - X, Y2 - Y, Z2 - Z);

		if (OutUnit == EEphemerisUnits::Kilometers)
		{
			Location.SetDoubleVector(Location.x * AUToKm, Location.y * AUToKm, Location.z * AUToKm);
			Velocity.SetDoubleVector(Velocity.x * AUToKm, Velocity.y * AUToKm, Velocity.z * AUToKm);
		}

		if (CoordinatePlane == EPlane::Equatorial)
		{
			ConvertToEquatorial(Location);
			ConvertToEquatorial(Velocity);
		}

		FAstroDoubleVector CentralBodyLoc, CentralBodyVel;
		GetObjectState(InCentralBody, EBodys::SolarSystemBarycenter, InDate,0, CentralBodyLoc, CentralBodyVel, CoordinatePlane, OutUnit);
		
		Location.SetDoubleVector(Location.x - CentralBodyLoc.x, Location.y - CentralBodyLoc.y, Location.z - CentralBodyLoc.z);
		Velocity.SetDoubleVector(Velocity.x - CentralBodyVel.x, Velocity.y - CentralBodyVel.y, Velocity.z - CentralBodyVel.z);
	
	}


}

int UVSOP::InitEphemeris(TArray<FString> DataFiles)
{

	bHasBeenActivated = true;
	return 0;
}

int UVSOP::CloseEphemeris()
{
	SunLink = nullptr;
	MercuryLink = nullptr;
	VenusLink = nullptr;
	EarthLink = nullptr;
	MarsLink = nullptr;
	JupiterLink = nullptr;
	SaturnLink = nullptr;
	UranusLink = nullptr;
	NeptuneLink = nullptr;
	return 0;
}

int UVSOP::GetObjectState(EBodys ObjBody, EBodys ObjOrigin, FAstroDouble jd1, FAstroDouble Optionaljd2, FAstroDoubleVector & Location, FAstroDoubleVector & Velocity, EPlane CoordinatePlane, const EEphemerisUnits OutUnit)
{

	switch (ObjBody)
	{
		case EBodys::Sun:
		{
			if (!SunLink)
			{
				SunLink = NewObject<UVSOPSun>();
			}
			SunLink->GetCoords(jd1, ObjOrigin, Location, Velocity, CoordinatePlane);
			break;
		}
		case EBodys::Mercury:
		{
			if (!MercuryLink)
			{
				MercuryLink = NewObject<UVSOPMercury>();
			}
			MercuryLink->GetCoords(jd1, ObjOrigin, Location, Velocity, CoordinatePlane);
			break;
		}
		case EBodys::Venus:
		{
			if (!VenusLink)
			{
				VenusLink = NewObject<UVSOPVenus>();
			}
			VenusLink->GetCoords(jd1, ObjOrigin, Location, Velocity, CoordinatePlane);
			break;
		}
		case EBodys::Earth:
		{
			if (!EarthLink)
			{
				EarthLink = NewObject<UVSOPEarth>();
			}
			EarthLink->GetCoords(jd1, ObjOrigin, Location, Velocity, CoordinatePlane);
			break;
		}
		case EBodys::Mars:
		{
			if (!MarsLink)
			{
				MarsLink = NewObject<UVSOPMars>();
			}
			MarsLink->GetCoords(jd1, ObjOrigin, Location, Velocity, CoordinatePlane);
			break;
		}
		case EBodys::Jupiter:
		{
			if (!JupiterLink)
			{
				JupiterLink = NewObject<UVSOPJupiter>();
			}
			JupiterLink->GetCoords(jd1, ObjOrigin, Location, Velocity, CoordinatePlane);
			break;
		}
		case EBodys::Saturn:
		{
			if (!SaturnLink)
			{
				SaturnLink = NewObject<UVSOPSaturn>();
			}
			SaturnLink->GetCoords(jd1, ObjOrigin, Location, Velocity, CoordinatePlane);
			break;
		}
		case EBodys::Uranus:
		{
			if (!UranusLink)
			{
				UranusLink = NewObject<UVSOPUranus>();
			}
			UranusLink->GetCoords(jd1, ObjOrigin, Location, Velocity, CoordinatePlane);
			break;
		}
		case EBodys::Neptune:
		{
			if (!NeptuneLink)
			{
				NeptuneLink = NewObject<UVSOPNeptune>();
			}
			NeptuneLink->GetCoords(jd1, ObjOrigin, Location, Velocity, CoordinatePlane);
			break;
		}
		case EBodys::Moon:
		{
			//Since VSOP dosen't have Moon, use JPLLowPrecision with VSOP data for solar system barycenter
			if (ObjOrigin == EBodys::SolarSystemBarycenter)
			{
				UEphemerisObject * NewEphem = UEphemerisManager::CreateNewEphemeris(EEphemeris::JPLLowPrecision);
				TArray<FString> EmptyArray;
				NewEphem->InitEphemeris(EmptyArray);

				FAstroDoubleVector OriginLocation, OriginVelocity;
				GetObjectState(EBodys::Earth, EBodys::SolarSystemBarycenter, jd1, Optionaljd2, OriginLocation, OriginVelocity, CoordinatePlane);

				FAstroDoubleVector MoonGeoLoc, MoonGeoVel;
				NewEphem->GetObjectState(EBodys::Moon, EBodys::Earth, jd1, Optionaljd2, MoonGeoLoc, MoonGeoVel, CoordinatePlane);

				Location = OriginLocation + MoonGeoLoc;
				Velocity = OriginVelocity + MoonGeoVel;

				UEphemerisManager::CloseEphemeris(NewEphem);


				return 0;
			}
			else if (ObjOrigin == EBodys::Earth)
			{
				UEphemerisObject * NewEphem = UEphemerisManager::CreateNewEphemeris(EEphemeris::JPLLowPrecision);
				TArray<FString> EmptyArray;
				NewEphem->InitEphemeris(EmptyArray);
				FAstroDoubleVector MoonGeoLoc, MoonGeoVel;
				NewEphem->GetObjectState(EBodys::Moon, EBodys::Earth, jd1, Optionaljd2, MoonGeoLoc, MoonGeoVel, CoordinatePlane);

				Location = MoonGeoLoc;
				Velocity = MoonGeoVel;

				UEphemerisManager::CloseEphemeris(NewEphem);

			}

			return 0;
			 break;
		}
		default:
		{
			FString EnumStrBody = GetEnumStr("EBodys", ObjBody);
			UE_LOG(LogTemp, Warning, TEXT("VSOP does not contain data for %s"), *EnumStrBody);
			Location.SetDoubleVector(0.l, 0.l, 0.l);
			Velocity.SetDoubleVector(0.l, 0.l, 0.l);
			return 0;
		}
	}

	return 0;
}

double UVSOP::HelioX0(double t)
{
	return 0;
}
double UVSOP::HelioX1(double t)
{
	return 0;
}
double UVSOP::HelioX2(double t)
{
	return 0;
}
double UVSOP::HelioX3(double t)
{
	return 0;
}
double UVSOP::HelioX4(double t)
{
	return 0;
}
double UVSOP::HelioX5(double t)
{
	return 0;
}
double UVSOP::HelioY0(double t)
{
	return 0;
}
double UVSOP::HelioY1(double t)
{
	return 0;
}
double UVSOP::HelioY2(double t)
{
	return 0;
}
double UVSOP::HelioY3(double t)
{
	return 0;
}
double UVSOP::HelioY4(double t)
{
	return 0;
}
double UVSOP::HelioY5(double t)
{
	return 0;
}
double UVSOP::HelioZ0(double t)
{
	return 0;
}
double UVSOP::HelioZ1(double t)
{
	return 0;
}
double UVSOP::HelioZ2(double t)
{
	return 0;
}
double UVSOP::HelioZ3(double t)
{
	return 0;
}
double UVSOP::HelioZ4(double t)
{
	return 0;
}
double UVSOP::HelioZ5(double t)
{
	return 0;
}





double UVSOP::BaryX0(double t)
{
	return 0;
}
double UVSOP::BaryX1(double t)
{
	return 0;
}
double UVSOP::BaryX2(double t)
{
	return 0;
}
double UVSOP::BaryX3(double t)
{
	return 0;
}
double UVSOP::BaryX4(double t)
{
	return 0;
}
double UVSOP::BaryX5(double t)
{
	return 0;
}
double UVSOP::BaryY0(double t)
{
	return 0;
}
double UVSOP::BaryY1(double t)
{
	return 0;
}
double UVSOP::BaryY2(double t)
{
	return 0;
}
double UVSOP::BaryY3(double t)
{
	return 0;
}
double UVSOP::BaryY4(double t)
{
	return 0;
}
double UVSOP::BaryY5(double t)
{
	return 0;
}
double UVSOP::BaryZ0(double t)
{
	return 0;
}
double UVSOP::BaryZ1(double t)
{
	return 0;
}
double UVSOP::BaryZ2(double t)
{
	return 0;
}
double UVSOP::BaryZ3(double t)
{
	return 0;
}
double UVSOP::BaryZ4(double t)
{
	return 0;
}
double UVSOP::BaryZ5(double t)
{
	return 0;
}


void UVSOP::ConvertToEquatorial(FAstroDoubleVector & InVec)
{
	FAstroDoubleVector NewEquatorialVector;

	UAstroLibBPLibrary::ConvertToEquatorial(2451545.0, ECelestialCoordSystems::MeanEquatorAndEquinoxOfDate, InVec, NewEquatorialVector);

	InVec = NewEquatorialVector;

}