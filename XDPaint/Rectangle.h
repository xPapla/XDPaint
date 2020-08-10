#pragma once
#include "Shape.h"
class Rectangle :
	public Shape
{
	std::vector<sf::Vector2f> normalizedPoints = { {0,0}, {0,1}, {1,1}, {1,0} };
public:
	Rectangle() {
		UpdatePoints();
	}
	void UpdatePoints();
	~Rectangle() {	};

	// Inherited via Shape
	virtual void setSize(sf::Vector2f size) override;
};
