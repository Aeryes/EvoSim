/*
	Gui.cpp

	Description: Gui.cpp contains the defintions for the prototypes listed
	inside Gui.h

    Credits: Jorge Yanar - https://github.com/jyanar/Boids for the boids.cpp file.

*/

#include "Gui.h"
#include "Food.h"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

Gui::Gui()
{
	this->organismSize = 10.0;
    this->foodSize = 5.0;

	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

	this->height = desktop.height;
	this->width = desktop.width;
	this->window.create(sf::VideoMode(width, height, desktop.bitsPerPixel), "EvoSim", sf::Style::None);

	// Try to achieve 60 FPS.
	window.setFramerateLimit(60);
};

void Gui::run()
{
    // Play Music
    // Declare a new music
    sf::Music music;

    // Open it from an audio file
    if (!music.openFromFile("trackOne.ogg"))
    {
        cout << "Music cant play..." << endl;
    }

    // Change some parameters
    music.setPosition(0, 1, 10); // change its 3D position
    music.setPitch(2);           // increase the pitch
    music.setVolume(10);         // reduce the volume
    music.setLoop(true);         // make it loop

    // Play it
    music.play();

    //Create initial organisms.
    for (int i = 0; i < 10; i++) 
    {
		Organism organism(width / 3, height / 3); // Starts all boids in the center of the screen
		sf::CircleShape shape(8, 3);

		// Changing the Visual Properties of the shape
		// shape.setPosition(b.location.x, b.location.y); // Sets position of shape to random location that boid was set to.
		shape.setPosition(width, height); // Testing purposes, starts all shapes in the center of screen.
		shape.setOutlineColor(sf::Color(0, 255, 0));
		shape.setFillColor(sf::Color::Green);
		shape.setOutlineColor(sf::Color::White);
		shape.setOutlineThickness(1);
		shape.setRadius(organismSize);

        //Check to see if the Organism has the flock trait, if it does add to active flock.
        if(organism.hasFlockTrait)
        {
            flock.addOrganism(organism);
            shapes.push_back(shape);
        }
        else
        {
            notFlocking.addOrganism(organism);
            shapesNotFlocking.push_back(shape);
        }

	}

    //Create food.
    for (int i = 0; i < foodAmount; i++)
    {
        //Create shape aand food object.
        sf::CircleShape food(rand() % 100, 4);

        //Set the position of the food.
        food.setPosition(rand() % 1920, rand() % 1080);
        food.setOutlineColor(sf::Color(0, 255, 0));
        food.setFillColor(sf::Color::White);
        food.setOutlineColor(sf::Color::White);
        food.setOutlineThickness(2);
        food.setRadius(foodSize);

        Food foodObj(100, food);

        foodObjStorage.push_back(foodObj);
    }

	while (window.isOpen()) 
    {
        //Handles keyboard and mouse inpput from the user.
		handleInput();

        handleEvents();

        //Renders all of the organisms to the screen along with anything else that needs to be drawn.
		render();
	}
};

//Handles simulation events such as collision detection and genetic mutations.
void Gui::handleEvents()
{
    //Check to see if any organisms collided with any food objects.

}

void Gui::handleInput()
{
    sf::Event event;
    while (window.pollEvent(event)) 
    {
        // "close requested" event: we close the window
        // Implemented alternate ways to close the window. (Pressing the escape, X, and BackSpace key also close the program.)
        if ((event.type == sf::Event::Closed) ||
            (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape) ||
            (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::BackSpace) ||
            (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::X))
        {
            window.close();
        }
    }

    // Check for mouse click, draws and adds Organism to flock if so.
    // These Organisms will automaticaly flock.
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
    {
        // Gets mouse coordinates, sets that as the location of the Organism and the shape
        sf::Vector2i mouseCoords = sf::Mouse::getPosition(window);
        Organism organism(mouseCoords.x, mouseCoords.y, false, true);
        sf::CircleShape shape(10, 3);

        // Changing visual properties of newly created Organism
        shape.setPosition(mouseCoords.x, mouseCoords.y);
        shape.setOutlineColor(sf::Color(255, 0, 0));
        shape.setFillColor(sf::Color(255, 0, 0));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1);
        shape.setRadius(organismSize);

        // Adds newly created Organism and shape to their respective data structure
        //Check to see if the Organism has the flock trait, if it does add to active flock.
        if(organism.hasFlockTrait)
        {
            flock.addOrganism(organism);
            shapes.push_back(shape);
        }
        else
        {
            notFlocking.addOrganism(organism);
            shapesNotFlocking.push_back(shape);
        }

        // New Shape is drawn
        window.draw(shapes[shapes.size() - 1]);
    }
}

/*
    Gui::Render => This function draws all Organisms to the screen. First we render the Organisms that
    do not have the flocking trait, then we render all the ones that do.

    TODO => This is where movement will need to take place for Organisms that are not in a flock.
*/
void Gui::render()
{
    window.clear();

    //Draws all the food to the screen.
    for (int i = 0; i < foodObjStorage.size(); i++)
    {
        window.draw(foodObjStorage[i].shape);
    }

    // Draws all of the Organisms out, and applies functions that are needed to update.
    //Draw all of the non-flocking organisms.
    for (int i = 0; i < shapesNotFlocking.size(); i++) 
    {
        window.draw(shapesNotFlocking[i]);

        // Matches up the location of the shape to the organism.
        shapesNotFlocking[i].setPosition(notFlocking.getOrganism(i).location.x, notFlocking.getOrganism(i).location.y);

        // Calculates the angle where the velocity is pointing so that the triangle turns towards it.
        float theta = notFlocking.getOrganism(i).angle(notFlocking.getOrganism(i).velocity);
        shapesNotFlocking[i].setRotation(theta);

        // Prevent boids from moving off the screen through wrapping
        // If boid exits right boundary
        if (shapesNotFlocking[i].getPosition().x > width)
            shapesNotFlocking[i].setPosition(shapesNotFlocking[i].getPosition().x - width, shapesNotFlocking[i].getPosition().y);
        // If boid exits bottom boundary
        if (shapesNotFlocking[i].getPosition().y > height)
            shapesNotFlocking[i].setPosition(shapesNotFlocking[i].getPosition().x, shapesNotFlocking[i].getPosition().y - height);
        // If boid exits left boundary
        if (shapesNotFlocking[i].getPosition().x < 0)
            shapesNotFlocking[i].setPosition(shapesNotFlocking[i].getPosition().x + width, shapesNotFlocking[i].getPosition().y);
        // If boid exits top boundary
        if (shapesNotFlocking[i].getPosition().y < 0)
            shapesNotFlocking[i].setPosition(shapesNotFlocking[i].getPosition().x, shapesNotFlocking[i].getPosition().y + height);
    }

    //Draw all of the Flocking Organisms.
    for (int i = 0; i < shapes.size(); i++) 
    {
        window.draw(shapes[i]);

        // Matches up the location of the shape to the boid
        shapes[i].setPosition(flock.getOrganism(i).location.x, flock.getOrganism(i).location.y);

        // Calculates the angle where the velocity is pointing so that the triangle turns towards it.
        float theta = flock.getOrganism(i).angle(flock.getOrganism(i).velocity);
        shapes[i].setRotation(theta);

        // Prevent boids from moving off the screen through wrapping
        // If boid exits right boundary
        if (shapes[i].getPosition().x > width)
            shapes[i].setPosition(shapes[i].getPosition().x - width, shapes[i].getPosition().y);
        // If boid exits bottom boundary
        if (shapes[i].getPosition().y > height)
            shapes[i].setPosition(shapes[i].getPosition().x, shapes[i].getPosition().y - height);
        // If boid exits left boundary
        if (shapes[i].getPosition().x < 0)
            shapes[i].setPosition(shapes[i].getPosition().x + width, shapes[i].getPosition().y);
        // If boid exits top boundary
        if (shapes[i].getPosition().y < 0)
            shapes[i].setPosition(shapes[i].getPosition().x, shapes[i].getPosition().y + height);
    }
    // Applies the three rules to each boid in the flock and changes them accordingly.
    flock.flocking(20);
    notFlocking.nonflocking(20);

    window.display();
}
