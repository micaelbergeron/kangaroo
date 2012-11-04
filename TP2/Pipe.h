#pragma once
#include "Quaternion.h"

class Pipe
{
	quaternion::Quaternion orientation;
	float length;

public:
	Pipe(void);
	Pipe(quaternion::Quaternion orientation, int length)
	{
		this->orientation = quaternion::Quaternion(orientation);
		this->length = length;
	}

	~Pipe(void)
	{
	}

	quaternion::Quaternion getOrientation()
	{
		return orientation;
	}
	void setOrientation(quaternion::Quaternion val)
	{
		orientation = quaternion::Quaternion(val);
	}

	float getLength() { return this->length; }
	void setLength(int value) { this->length = value; }
};

