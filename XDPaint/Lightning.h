#pragma once
#include "Shape.h"
class Lightning :
	public Shape
{
	std::vector<sf::Vector2f> normalizedPoints;
public:
	Lightning() {
		sf::Vector2f defaultPoints[] = {
		{74,6},
		{43,42},
		{69,42},
		{8,94},
		{44,53},
		{22,53} };
		sf::Vector2f min(defaultPoints[0]);
		sf::Vector2f max(defaultPoints[0]);
		for (auto &point : defaultPoints) {
			min.x = fminf(min.x, point.x);
			min.y = fminf(min.y, point.y);
			max.x = fmaxf(max.x, point.x);
			max.y = fmaxf(max.y, point.y);
		}

		max -= min;

		for (auto &point : defaultPoints) {
			point -= min;
			point.x /= max.x;
			point.y /= max.y;
			normalizedPoints.push_back(point);
		}

		UpdatePoints();
	}
	void UpdatePoints();
	~Lightning() {	};

	// Inherited via Shape
	virtual void setSize(sf::Vector2f size) override;
};

