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

	Pvector m_position;
	sf::CircleShape m_shape;

	//Getters and setters.
	void setPosition(Pvector position);
	void setEnergyAmount(int value);

	int getEnergyAmount();
	Pvector getPosition();

private:
	int m_energyAmount;
};

