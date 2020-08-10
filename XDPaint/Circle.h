#pragma once
#include "Shape.h"
class Circle :
	public Shape
{
	std::vector<sf::Vector2f> normalizedPoints;
public:
	Circle() {
		const int POINT_COUNT = 30;
		const float PI = 3.1428;
		const float RADIUS = 100;

		float angleCurr = 0;
		float angleMax = 2 * PI;

		std::vector<sf::Vector2f> defaultPoints;

		while (angleCurr < angleMax) {
			sf::Vector2f point(cosf(angleCurr) * RADIUS, sinf(angleCurr) * RADIUS);
			defaultPoints.push_back(point);

			angleCurr += angleMax / (double)POINT_COUNT;
		}


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
	~Circle() {	};

	// Inherited via Shape
	virtual void setSize(sf::Vector2f size) override;
};
