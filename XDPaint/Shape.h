#pragma once
#include <SFML/Graphics.hpp>
class Shape :
	public sf::Shape
{
protected:
	sf::Vector2f size = { 1.0, 1.0 }; //scale for non-stretching scaling
	std::vector<sf::Vector2f> scaledPoints;
public:
	Shape();
	~Shape();

	virtual void setSize(sf::Vector2f size);
	virtual sf::Vector2f getSize();

	// Inherited via Shape
	virtual std::size_t getPointCount() const override;
	virtual sf::Vector2f getPoint(std::size_t index) const override;
};

