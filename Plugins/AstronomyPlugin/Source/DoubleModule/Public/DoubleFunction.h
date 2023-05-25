// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#include "UObject/TextProperty.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DoubleFunction.generated.h"

USTRUCT(BlueprintType, meta = (HasNativeMake = "DoubleModule.DoubleFunction.MakeFAstroDouble"))
struct FAstroDouble
{
	GENERATED_BODY()

	double value = 0;
	
	/*
	The value of the variable in floating point precision.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Astronomy Coordinate")
	float floatValue = 0;
	
	/*
	The value of the variable in double point precision, returned as a string.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Astronomy Coordinate")
	FString StringValue = "";

	FAstroDouble ()
	{
		SetValue(0.0);
	}

	void SetValue(double InVal)
	{
		value = InVal;
		floatValue = value;	
		StringValue = FStringFromDouble(InVal);
	}

	void SetValue(FString InVal)
	{
		value = DoubleFromFString(InVal);
		floatValue = value;
		StringValue = InVal;
	}

	void SetValue(float InVal)
	{
		value = InVal;
		floatValue = InVal;
		StringValue = FStringFromDouble(value);
	}
	

	void Refresh()
	{

		if (value == 0.0)
		{
			if (DoubleFromFString(StringValue) != 0.0)
			{
				SetValue(StringValue);
			}

		}

	}


	FString FStringFromDouble(const double myDouble)
	{
		double TempMyDouble = myDouble;
		// Avoids negative zero
		if (TempMyDouble == 0)
		{
			TempMyDouble = 0;
		}

		FString TempString = FString::Printf(TEXT("%.16f"), TempMyDouble);
		return TempString;
	}


	double DoubleFromFString(FString InStr) const
	{
		double tempDouble;

		if (!InStr.IsNumeric())
		{
			UE_LOG(LogTemp, Warning, TEXT("Error! %s is not numeric! Could not create Double variable"), *InStr);
			return 0;
		}

		tempDouble = FCString::Atod(*InStr);


		return tempDouble;
	}

	
	FAstroDouble & operator=(double n)
	{
		
		SetValue(n);
		return *this;
	}

	FAstroDouble & operator=(FString n)
	{
		SetValue(n);
		return *this;
	}

	FAstroDouble & operator=(FAstroDouble  n)
	{
		n.Refresh();
		SetValue(n.value);
		return *this;
	}


	


	operator double()
	{ 
		Refresh();
		return value;
	}
	operator double() const
	{
		if (value == 0.0)
		{
			return DoubleFromFString(StringValue);
		}
		else
		{
			return value;
		}
	}
	operator FString() const
	{
		return StringValue;
	}
	
	/*
	operator FAstroDouble () 
	{
		Refresh();
		return *this;
	}
	*/

	


	FAstroDouble (double inDouble)
	{
		SetValue(inDouble);
	}
	FAstroDouble (FString inStr)
	{
		SetValue(inStr);
	}
	FAstroDouble (FString inStr, float inFloat)
	{
		SetValue(inStr);
	}
	FAstroDouble (float inFloat, FString inStr)
	{
		SetValue(inStr);
	}




};





//c style double vector
USTRUCT(BlueprintType, meta = (HasNativeMake = "DoubleModule.DoubleFunction.MakeDoubleVector"))
struct FAstroDoubleVector
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FVector FloatVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDouble  x;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDouble  y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Coordinate")
	FAstroDouble  z;

	FAstroDoubleVector()
	{
		SetDoubleVector(0.f,0.f,0.f);
	}

	FAstroDoubleVector(double x, double y, double z)
	{
		SetDoubleVector(x, y, z);
	}

	FAstroDoubleVector(FVector InVector)
	{
		SetDoubleVector(InVector.X, InVector.Y, InVector.Z);
	}

	void SetDoubleVector(double inx, double iny, double inz)
	{
		x = inx;
		y = iny;
		z = inz;
		FloatVector.X = inx;
		FloatVector.Y = iny;
		FloatVector.Z = inz;

	}

	void SetDoubleVector(float inx, float iny, float inz)
	{
		x = inx;
		y = iny;
		z = inz;
		FloatVector.X = inx;
		FloatVector.Y = iny;
		FloatVector.Z = inz;

	}

	void SetDoubleVector(long double inx, long double iny, long double inz)
	{
		x = inx;
		y = iny;
		z = inz;
		FloatVector.X = inx;
		FloatVector.Y = iny;
		FloatVector.Z = inz;

	}

	void SetDoubleVector(double InVec[3])
	{
		x = InVec[0];
		y = InVec[1];
		z = InVec[2];
		FloatVector.X = InVec[0];
		FloatVector.Y = InVec[1];
		FloatVector.Z = InVec[2];
	}




	FVector GetFVector()
	{
		FVector tempVector;
		tempVector = FVector(x.value, y.value, z.value);
		return tempVector;
	}
	
	void GetDoubleVector(double * InVec) const
	{
		InVec[0] = x.value;
		InVec[1] = y.value;
		InVec[2] = z.value;
	}

	void Refresh()
	{

		x.Refresh();
		y.Refresh();
		z.Refresh();
		SetDoubleVector(x, y, z);
	}

	FAstroDoubleVector& operator=(FVector FVec)
	{
		SetDoubleVector(FVec.X, FVec.Y, FVec.Z);
		return *this;
	}

	FAstroDoubleVector& operator=(FAstroDoubleVector FVec)
	{
		FVec.Refresh();
		SetDoubleVector(double(FVec.x), double(FVec.y), double(FVec.z));
		return *this;
	}

	FAstroDoubleVector operator+(FAstroDoubleVector FVec)
	{
		FVec.Refresh();
		Refresh();
		FAstroDoubleVector temp;
		temp.SetDoubleVector(double(x) + double(FVec.x), double(y) + double(FVec.y), double(z) + double(FVec.z));
		return temp;
	}
	FAstroDoubleVector operator*(FAstroDouble FDbl)
	{
		FDbl.Refresh();
		Refresh();
		FAstroDoubleVector temp;
		temp.SetDoubleVector(double(x) * double(FDbl), double(y) * double(FDbl), double(z) * double(FDbl));
		return temp;
	}
	FAstroDoubleVector operator*(double dbl)
	{
		Refresh();
		FAstroDoubleVector temp;
		temp.SetDoubleVector(double(x) * dbl, double(y) * dbl, double(z) * dbl);
		return temp;
	}

	FAstroDoubleVector RotateAngleAxisDbl(float AngleDeg, FVector Axis)
	{
		Axis = Axis.GetSafeNormal();
		Refresh();

		double S, C;

		double InAngleD2R = FMath::DegreesToRadians(AngleDeg);
		S = FMath::Sin(InAngleD2R);
		C = FMath::Cos(InAngleD2R);

		const double XX = Axis.X * Axis.X;
		const double YY = Axis.Y * Axis.Y;
		const double ZZ = Axis.Z * Axis.Z;

		const double XY = Axis.X * Axis.Y;
		const double YZ = Axis.Y * Axis.Z;
		const double ZX = Axis.Z * Axis.X;

		const double XS = Axis.X * S;
		const double YS = Axis.Y * S;
		const double ZS = Axis.Z * S;

		const double OMC = 1.f - C;

		return FAstroDoubleVector(
			(OMC * XX + C) * x + (OMC * XY - ZS) * y + (OMC * ZX + YS) * z,
			(OMC * XY + ZS) * x + (OMC * YY + C) * y + (OMC * YZ - XS) * z,
			(OMC * ZX - YS) * x + (OMC * YZ + XS) * y + (OMC * ZZ + C) * z
		);
	}

	/*
	operator FAstroDoubleVector()
	{
		Refresh();
		return *this;
	}
	*/

	operator FVector()
	{
		return GetFVector();
	}
	
	//Useful to switch Right/Left handed coordinate systems
	FAstroDoubleVector GetFlippedY()
	{
		FAstroDoubleVector NewVec;
		NewVec.SetDoubleVector(double(x), double(-y), double(z));
		return NewVec;
	}
	
};

UCLASS()
class UDoubleFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	//FAstroDouble
	/*
	Creates a struct containing a double type number. A number passed in as an FString will override a number passed into as float, because it can contain more precision.
	@param InValueFloat - Float value of the number. Will have less precision than the string version of the number.
	@param InValueString -  String value of the number
	@return A reference to the created double structure.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MakeFAstroDouble", Keywords = "double astronomy", NativeMakeFunc, Category = "AstroLib"))
	static FAstroDouble MakeFAstroDouble(FString InValueString);

	/*
	Sets a struct containing a double type number created from a float. Setting a double from a float will cause a loss in precision.
	@param InFAstroDouble - The FAstroDouble structure that will contain the new value.
	@param InValueFloat -  A floating point precision number.
	@return A reference to the double structure (same reference as InFAstroDouble, returned for convience in blueprinting)
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Make FAstroDouble From Float", Keywords = "double astronomy", Category = "AstroLib"))
	static FAstroDouble SetFAstroDoubleFromFloat(FAstroDouble InFAstroDouble, const float InValueFloat);

	/*
	Sets a struct containing a double type number created from a string. The string can contain double point precision (~15 digits).
	@param InFAstroDouble - The FAstroDouble structure that will contain the new value.
	@param InValueString -  An FString containaing a number. The string can contain double point precision (~15 digits).
	@return A reference to the double structure (same reference as InFAstroDouble, returned for convience in blueprinting)
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Make FAstroDouble From FString", Keywords = "double astronomy", Category = "AstroLib"))
	static FAstroDouble SetFAstroDoubleFromFString(FAstroDouble InFAstroDouble, const FString InValueString);

	/** FAstroDouble multiply by a FAstroDouble */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "FAstroDouble * FAstroDouble", CompactNodeTitle = "*", Keywords = "* multiply division"), Category = "Math|Vector|Astronomy")
	static FAstroDouble Multiply_FAstroDoubleFAstroDouble(FAstroDouble A, FAstroDouble B);

	/** FAstroDouble multiply by a FAstroDouble */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "FAstroDouble * float", CompactNodeTitle = "*", Keywords = "* multiply division"), Category = "Math|Vector|Astronomy")
	static FAstroDouble Multiply_FAstroDoubleFloat(FAstroDouble A, float B);

	/** FAstroDouble Add by a FAstroDouble */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "FAstroDouble + FAstroDouble", CompactNodeTitle = "+", Keywords = "+ addition"), Category = "Math|Vector|Astronomy")
	static FAstroDouble Add_FAstroDoubleFAstroDouble (FAstroDouble A, FAstroDouble B);

	/** FAstroDouble subtract by a FAstroDouble */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "FAstroDouble - FAstroDouble", CompactNodeTitle = "-", Keywords = "- subtract minus"), Category = "Math|Vector|Astronomy")
	static FAstroDouble Subtract_FAstroDoubleFAstroDouble(FAstroDouble A, FAstroDouble B);


	
	// Converts a FAstroDouble to a float 
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFloat (FAstroDouble)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions|Astronomy")
	static float Conv_FAstroDoubleToFloat(FAstroDouble InFAstroDouble);

	// Converts a FAstroDouble to a string 
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFString (FAstroDouble)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions|Astronomy")
	static FString Conv_FAstroDoubleToFString(FAstroDouble InFAstroDouble);

	// Converts a FAstroDouble to a Text 
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFText (FAstroDouble)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions|Astronomy")
	static FText Conv_FAstroDoubleToFText(FAstroDouble InFAstroDouble);

	// Converts a float to FAstroDouble
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFAstroDouble (float)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions|Astronomy")
	static FAstroDouble Conv_FloatToFAstroDouble(float inFloat);

	// Converts a string to FAstroDouble  
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFAstroDouble (FString)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions|Astronomy")
	static FAstroDouble Conv_FStringToFAstroDouble(FString InFStr);

	// Converts a Text to FAstroDouble
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFAstroDouble (FText)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions|Astronomy")
	static FAstroDouble Conv_FTextToFAstroDouble(FText InFText);
	

	//FAstroDoubleVector
	/*
	Creates a struct containing a vector of double type numbers.
	@param inx -  X Coordinate
	@param iny -  Y Coordinate
	@param inz -  Z Coordinate
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MakeDoubleVector", Keywords = "double vector astronomy", NativeMakeFunc, Category = "AstroLib"))
	static FAstroDoubleVector MakeDoubleVector(FAstroDouble inx, FAstroDouble iny, FAstroDouble inz);

	// Converts a FAstroDoubleVector to an FVector 
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFVector (FAstroDoubleVector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions|Astronomy")
	static FVector Conv_FAstroDoubleVectorToFVector(FAstroDoubleVector InFAstroDoubleVec);

	// Converts an FVector to an FAstroDoubleVector (will lose precision)
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFAstroDoubleVector (FVector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions|Astronomy")
	static FAstroDoubleVector Conv_FVectorToFAstroDoubleVector(FVector InFVec);

	// Converts an FAstroDoubleVector to a LinearColor (will lose precision)
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToLinearColor (FAstroDoubleVector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions|Astronomy")
	static FLinearColor Conv_FAstroDoubleVectorToLinearColor(FAstroDoubleVector InFAstroDoubleVec);

	/** FAstroDoubleVector addition */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "dbl vector + dbl vector", CompactNodeTitle = "+", Keywords = "+ add plus"), Category = "Math|Vector|Astronomy")
	static FAstroDoubleVector Add_FAstroDoubleVectorFAstroDoubleVector(FAstroDoubleVector A, FAstroDoubleVector B);

	/** FAstroDoubleVector subtraction */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "dbl vector - dbl vector", CompactNodeTitle = "-", Keywords = "- minus plus"), Category = "Math|Vector|Astronomy")
	static FAstroDoubleVector Subtract_FAstroDoubleVectorFAstroDoubleVector(FAstroDoubleVector A, FAstroDoubleVector B);

	/** FAstroDoubleVector divide by a FAstroDouble */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "dbl vector / dbl", CompactNodeTitle = "/", Keywords = "/ divide division"), Category = "Math|Vector|Astronomy")
	static FAstroDoubleVector Divide_FAstroDoubleVectorFAstroDouble(FAstroDoubleVector A, FAstroDouble B);

	/** FAstroDoubleVector multiply by a FAstroDouble */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "dbl vector * dbl", CompactNodeTitle = "*", Keywords = "* multiply division"), Category = "Math|Vector|Astronomy")
	static FAstroDoubleVector Multiply_FAstroDoubleVectorFAstroDouble(FAstroDoubleVector A, FAstroDouble B);

	/** Returns result of vector A (DoubleVector) rotated by AngleDeg around Axis */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "RotateDblVectorAroundAxis"), Category = "Math|Vector|Astronomy")
	static FAstroDoubleVector RotateAngleAxisDblVec(FAstroDoubleVector InVect, float AngleDeg, FVector Axis);

	

};

