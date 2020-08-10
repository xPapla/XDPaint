#pragma once
#include "Shape.h"
class Triangle :
	public Shape
{
	std::vector<sf::Vector2f> normalizedPoints = { {0.5,0}, {1,1}, {0,1} };
public:
	Triangle() {
		UpdatePoints();
	}
	void UpdatePoints();
	~Triangle() {	};

	// Inherited via Shape
	virtual void setSize(sf::Vector2f size) override;
};
