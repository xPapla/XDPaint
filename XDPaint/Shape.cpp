#include "Shape.h"



Shape::Shape()
{
}


Shape::~Shape()
{
}

void Shape::setSize(sf::Vector2f size)
{
	this->size = size;
}

sf::Vector2f Shape::getSize()
{
	return size;
}

std::size_t Shape::getPointCount() const
{
	return scaledPoints.size();
}

sf::Vector2f Shape::getPoint(std::size_t index) const
{
	return scaledPoints.at(index);
}
