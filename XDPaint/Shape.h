#pragma once
#include <SFML/Graphics.hpp>
class Shape :
	public sf::Shape
{
	
public:
	virtual void Draw() = 0;
	Shape();
	~Shape();

	// Inherited via Shape
	virtual std::size_t getPointCount() = 0;
	virtual sf::Vector2f getPoint(std::size_t index) = 0;
};

