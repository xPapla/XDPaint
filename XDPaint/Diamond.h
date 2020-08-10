#pragma once
#include "Shape.h"
class Diamond :
	public Shape
{
	std::vector<sf::Vector2f> normalizedPoints = { {0.5, 0}, {1, 0.5}, {0.5, 1}, {0, 0.5} };
public:
	Diamond() {
		UpdatePoints();
	}
	void UpdatePoints();
	~Diamond() {	};

	// Inherited via Shape
	virtual void setSize(sf::Vector2f size) override;
};
