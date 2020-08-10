#include "Lightning.h"

void Lightning::UpdatePoints()
{
	sf::Vector2f scale = size;
	scaledPoints.clear();
	for (auto point : normalizedPoints) {
		point.x *= scale.x;
		point.y *= scale.y;
		scaledPoints.push_back(point);
	}
	update();
}

void Lightning::setSize(sf::Vector2f size)
{
	this->size = size;
	UpdatePoints();
}

