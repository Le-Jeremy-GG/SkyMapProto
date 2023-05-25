// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#include "Time.h"

Time::Time()
{


}


// Implementation of algorithm by Espenak & Meeus (2006) for DeltaT computation
 double Time::getDeltaTByEspenakMeeus(FDateTime InDate)
{
	int year, month, day;
	year = InDate.GetYear();
	month = InDate.GetMonth();
	day = InDate.GetDay();
	// Note: the method here is adapted from
	// "Five Millennium Canon of Solar Eclipses" [Espenak and Meeus, 2006]
	// A summary is described here:
	// http://eclipse.gsfc.nasa.gov/SEhelp/deltatpoly2004.html

	double y = year + ((month - 1)*30.5 + day / 31.*30.5) / 366;

	// set the default value for Delta T
	double u = (y - 1820) / 100.;
	double r = (-20 + 32 * u * u);

	if (y < -500)
	{
		// values are equal to defaults!
		// u = (y-1820)/100.;
		// r = (-20 + 32 * u * u);
	}
	else if (y < 500)
	{
		u = y / 100;
		//r = (10583.6 - 1014.41 * u + 33.78311 * std::pow(u,2) - 5.952053 * std::pow(u,3)
		//       - 0.1798452 * std::pow(u,4) + 0.022174192 * std::pow(u,5) + 0.0090316521 * std::pow(u,6));
		r = (((((0.0090316521*u + 0.022174192)*u - 0.1798452)*u - 5.952053)*u + 33.78311)*u - 1014.41)*u + 10583.6;
	}
	else if (y < 1600)
	{
		u = (y - 1000) / 100;
		//r = (1574.2 - 556.01 * u + 71.23472 * std::pow(u,2) + 0.319781 * std::pow(u,3)
		//       - 0.8503463 * std::pow(u,4) - 0.005050998 * std::pow(u,5) + 0.0083572073 * std::pow(u,6));
		r = (((((0.0083572073*u - 0.005050998)*u - 0.8503463)*u + 0.319781)*u + 71.23472)*u - 556.01)*u + 1574.2;
	}
	else if (y < 1700)
	{
		double t = y - 1600;
		//r = (120 - 0.9808 * t - 0.01532 * std::pow(t,2) + std::pow(t,3) / 7129);
		r = ((t / 7129.0 - 0.01532)*t - 0.9808)*t + 120.0;
	}
	else if (y < 1800)
	{
		double t = y - 1700;
		//r = (8.83 + 0.1603 * t - 0.0059285 * std::pow(t,2) + 0.00013336 * std::pow(t,3) - std::pow(t,4) / 1174000);
		r = (((-t / 1174000.0 + 0.00013336)*t - 0.0059285)*t + 0.1603)*t + 8.83;
	}
	else if (y < 1860)
	{
		double t = y - 1800;
		//r = (13.72 - 0.332447 * t + 0.0068612 * std::pow(t,2) + 0.0041116 * std::pow(t,3) - 0.00037436 * std::pow(t,4)
		//       + 0.0000121272 * std::pow(t,5) - 0.0000001699 * std::pow(t,6) + 0.000000000875 * std::pow(t,7));
		r = ((((((.000000000875*t - .0000001699)*t + 0.0000121272)*t - 0.00037436)*t + 0.0041116)*t + 0.0068612)*t - 0.332447)*t + 13.72;
	}
	else if (y < 1900)
	{
		double t = y - 1860;
		//r = (7.62 + 0.5737 * t - 0.251754 * std::pow(t,2) + 0.01680668 * std::pow(t,3)
		//	-0.0004473624 * std::pow(t,4) + std::pow(t,5) / 233174);
		r = ((((t / 233174.0 - 0.0004473624)*t + 0.01680668)*t - 0.251754)*t + 0.5737)*t + 7.62;
	}
	else if (y < 1920)
	{
		double t = y - 1900;
		//r = (-2.79 + 1.494119 * t - 0.0598939 * std::pow(t,2) + 0.0061966 * std::pow(t,3) - 0.000197 * std::pow(t,4));
		r = (((-0.000197*t + 0.0061966)*t - 0.0598939)*t + 1.494119)*t - 2.79;
	}
	else if (y < 1941)
	{
		double t = y - 1920;
		//r = (21.20 + 0.84493*t - 0.076100 * std::pow(t,2) + 0.0020936 * std::pow(t,3));
		r = ((0.0020936*t - 0.076100)*t + 0.84493)*t + 21.20;
	}
	else if (y < 1961)
	{
		double t = y - 1950;
		//r = (29.07 + 0.407*t - std::pow(t,2)/233 + std::pow(t,3) / 2547);
		r = ((t / 2547.0 - 1.0 / 233.0)*t + 0.407)*t + 29.07;
	}
	else if (y < 1986)
	{
		double t = y - 1975;
		//r = (45.45 + 1.067*t - std::pow(t,2)/260 - std::pow(t,3) / 718);
		r = ((-t / 718.0 - 1 / 260.0)*t + 1.067)*t + 45.45;
	}
	else if (y < 2005)
	{
		double t = y - 2000;
		//r = (63.86 + 0.3345 * t - 0.060374 * std::pow(t,2) + 0.0017275 * std::pow(t,3) + 0.000651814 * std::pow(t,4) + 0.00002373599 * std::pow(t,5));
		r = ((((0.00002373599*t + 0.000651814)*t + 0.0017275)*t - 0.060374)*t + 0.3345)*t + 63.86;
	}
	else if (y < 2050)
	{
		double t = y - 2000;
		//r = (62.92 + 0.32217 * t + 0.005589 * std::pow(t,2));
		r = (0.005589*t + 0.32217)*t + 62.92;
	}
	else if (y < 2150)
	{
		//r = (-20 + 32 * std::pow((y-1820)/100,2) - 0.5628 * (2150 - y));
		// r has been precomputed before, just add the term patching the discontinuity
		r -= 0.5628*(2150.0 - y);
	}

//	UE_LOG(LogTemp, Warning, TEXT("(delta T: %f"), float(r));

	return r;
}