/*
    Gui.h

	Description: Gui.h is the header file for the programs visual interface.
	Gui.h provides prototypes for the functions that will display graphics to
	the user.

	Credits: Jorge Yanar - https://github.com/jyanar/Boids for the boids.cpp file.

*/

#pragma once

#include <Flock.h>
#include "Food.h"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

class Gui
{
public:
	//Constructors.
	Gui();

	//Creates the programs initial state and loop.
	void run();

	//Start a new clock for simulation elapsed time.
	sf::Clock clock;

private:
	int width;
	int height;
	int foodAmount = 20;

	sf::RenderWindow window;

	Flock flock;
	Flock notFlocking;

	float organismSize;
	float foodSize;

	vector<sf::CircleShape> shapes;
	vector<sf::CircleShape> shapesNotFlocking;
	vector<Food> foodObjStorage;

	void render();
	void handleInput();
	void handleEvents();
	
};

