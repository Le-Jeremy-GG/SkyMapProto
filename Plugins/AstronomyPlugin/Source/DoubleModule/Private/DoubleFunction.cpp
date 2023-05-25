// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#include "DoubleFunction.h"


float UDoubleFunction::Conv_FAstroDoubleToFloat(FAstroDouble InFAstroDouble)
{
	InFAstroDouble.Refresh();
	return InFAstroDouble.floatValue;
}

FString UDoubleFunction::Conv_FAstroDoubleToFString(FAstroDouble InFAstroDouble)
{
	InFAstroDouble.Refresh();
	return InFAstroDouble.StringValue;
}

FText UDoubleFunction::Conv_FAstroDoubleToFText(FAstroDouble InFAstroDouble)
{
	InFAstroDouble.Refresh();
	return FText::FromString(InFAstroDouble.StringValue);

}

FAstroDouble UDoubleFunction::Conv_FloatToFAstroDouble(float inFloat)
{
	FAstroDouble tmp = FAstroDouble(inFloat);
	return tmp;
}

FAstroDouble UDoubleFunction::Conv_FStringToFAstroDouble(FString InFStr)
{
	return FAstroDouble(InFStr);
}

FAstroDouble UDoubleFunction::Conv_FTextToFAstroDouble(FText InFText)
{
	return FAstroDouble(InFText.ToString());
}


FAstroDouble UDoubleFunction::MakeFAstroDouble(const FString InValueString)
{
	FAstroDouble NewDouble;

	NewDouble.SetValue(InValueString);

	return NewDouble;
};


FAstroDouble UDoubleFunction::SetFAstroDoubleFromFloat(FAstroDouble InFAstroDouble, const float InValueFloat)
{
	InFAstroDouble.SetValue(InValueFloat);
	return InFAstroDouble;
}

FAstroDouble UDoubleFunction::SetFAstroDoubleFromFString(FAstroDouble InFAstroDouble, const FString InValueString)
{
	InFAstroDouble.SetValue(InValueString);
	return InFAstroDouble;
}

FAstroDouble UDoubleFunction::Multiply_FAstroDoubleFAstroDouble(FAstroDouble A, FAstroDouble B)
{
	FAstroDouble dbl(double(A) * double(B));
	return dbl;
}

FAstroDouble UDoubleFunction::Multiply_FAstroDoubleFloat(FAstroDouble A, float B)
{
	return FAstroDouble(double(A) * double(B));
}

FAstroDouble UDoubleFunction::Add_FAstroDoubleFAstroDouble(FAstroDouble A, FAstroDouble B)
{
	return FAstroDouble(double(A) + double(B));
}

FAstroDouble UDoubleFunction::Subtract_FAstroDoubleFAstroDouble(FAstroDouble A, FAstroDouble B)
{
	return FAstroDouble(double(A) - double(B));
}

FAstroDoubleVector UDoubleFunction::MakeDoubleVector(FAstroDouble inx, FAstroDouble iny, FAstroDouble inz)
{
	FAstroDoubleVector tempVec;
	tempVec.SetDoubleVector(inx.value, iny.value, inz.value);
	return tempVec;
}

FVector UDoubleFunction::Conv_FAstroDoubleVectorToFVector(FAstroDoubleVector InFAstroDoubleVec)
{
	return InFAstroDoubleVec.GetFVector();
}

FAstroDoubleVector UDoubleFunction::Conv_FVectorToFAstroDoubleVector(FVector InFVec)
{
	FAstroDoubleVector TempVec;
	TempVec.SetDoubleVector(InFVec.X, InFVec.Y, InFVec.Z );
	return TempVec;
}

FLinearColor UDoubleFunction::Conv_FAstroDoubleVectorToLinearColor(FAstroDoubleVector InFAstroDoubleVec)
{

	return FLinearColor(Conv_FAstroDoubleVectorToFVector(InFAstroDoubleVec));
	
}

FAstroDoubleVector UDoubleFunction::Add_FAstroDoubleVectorFAstroDoubleVector(FAstroDoubleVector A, FAstroDoubleVector B)
{
	A.Refresh();
	B.Refresh();
	double NewX = A.x + B.x;
	double NewY = A.y + B.y;
	double NewZ = A.z + B.z;
	FAstroDoubleVector NewVector;
	NewVector.SetDoubleVector(NewX, NewY, NewZ);
	return NewVector;
}

FAstroDoubleVector UDoubleFunction::Subtract_FAstroDoubleVectorFAstroDoubleVector(FAstroDoubleVector A, FAstroDoubleVector B)
{
	A.Refresh();
	B.Refresh();
	double NewX = A.x - B.x;
	double NewY = A.y - B.y;
	double NewZ = A.z - B.z;
	FAstroDoubleVector NewVector;
	NewVector.SetDoubleVector(NewX, NewY, NewZ);
	return NewVector;
}

FAstroDoubleVector UDoubleFunction::Divide_FAstroDoubleVectorFAstroDouble(FAstroDoubleVector A, FAstroDouble B)
{
	A.Refresh();
	B.Refresh();
	double NewX = A.x / double(B);
	double NewY = A.y / double(B);
	double NewZ = A.z / double(B);
	FAstroDoubleVector NewVector;
	NewVector.SetDoubleVector(NewX, NewY, NewZ);
	return NewVector;
}

FAstroDoubleVector UDoubleFunction::Multiply_FAstroDoubleVectorFAstroDouble(FAstroDoubleVector A, FAstroDouble B)
{
	A.Refresh();
	B.Refresh();
	double NewX = A.x * double(B);
	double NewY = A.y * double(B);
	double NewZ = A.z * double(B);
	FAstroDoubleVector NewVector;
	NewVector.SetDoubleVector(NewX, NewY, NewZ);
	return NewVector;
}

FAstroDoubleVector UDoubleFunction::RotateAngleAxisDblVec(FAstroDoubleVector InVect, float AngleDeg, FVector Axis)
{

	return InVect.RotateAngleAxisDbl(AngleDeg, Axis.GetSafeNormal());

}

