#pragma once
#include "Shape.h"
class Circle :
	public Shape
{
public:
	Circle();
	~Circle();

	// Inherited via Shape
	virtual void Draw() override;
};

