/*
	Food.h

	Description: Food.h provides prototypes for the Food.cpp file. This header
	deals with foood for organisms to consume along with the energy values of that food.

*/

#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include "Pvector.h"

class Food
{
public:
	//Constrcutor;
	Food(int energyLevel, sf::CircleShape shape);

	int energyLevel;
	Pvector position;
	sf::CircleShape shape;

	void getPosition();
};

