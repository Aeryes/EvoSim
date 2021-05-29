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
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <direct.h>

//Prints the working directory.
void PrintFullPath()
{
    char full[_MAX_PATH];
    if (_fullpath(full, ".//", _MAX_PATH) != NULL)
        printf("Full path is: %s\n", full);
    else
        printf("Invalid path\n");
}

Gui::Gui()
{
    setOrganismSize(10.0);
    setFoodSize(5.0);

	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    setHeight(desktop.height);
    setWidth(desktop.width);
	m_window.create(sf::VideoMode(getWidth(), getHeight(), desktop.bitsPerPixel), "EvoSim", sf::Style::None);

	// Try to achieve 60 FPS.
	m_window.setFramerateLimit(60);

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

    //Load the text for the GUI.
    loadText();

    // Change some parameters
    music.setPosition(0, 1, 10); // change its 3D position
    music.setPitch(2);           // increase the pitch
    music.setVolume(10);         // reduce the volume
    music.setLoop(true);         // make it loop

    // Play it
    music.play();

    //Create initial organisms.
	for (int i = 0; i < getStartingPop(); i++)
    {
		Organism organism(getWidth() / 3, getHeight() / 3); // Starts all boids in the center of the screen
		sf::CircleShape shape(8, 3);

		// Changing the Visual Properties of the shape
		// shape.setPosition(b.location.x, b.location.y); // Sets position of shape to random location that boid was set to.
		shape.setPosition(getWidth(), getHeight()); // Testing purposes, starts all shapes in the center of screen.
		shape.setOutlineColor(sf::Color(0, 255, 0));
		shape.setFillColor(sf::Color::Green);
		shape.setOutlineColor(sf::Color::White);
		shape.setOutlineThickness(1);
		shape.setRadius(getOrganismSize());

        //Check to see if the Organism has the flock trait, if it does add to active flock.
        if(organism.m_hasFlockTrait)
        {
            m_flock.addOrganism(organism);
            m_shapes.push_back(shape);
        }
        else
        {
            m_notFlocking.addOrganism(organism);
            m_shapesNotFlocking.push_back(shape);
        }
	}

    //Create food.
    for (int i = 0; i < getFoodAmount(); i++)
    {
        //Create shape and food object.
        sf::CircleShape food(rand() % 100, 4);

        //Set the position of the food.
        food.setPosition(rand() % getWidth(), rand() % getHeight());
        food.setOutlineColor(sf::Color(0, 255, 0));
        food.setFillColor(sf::Color::White);
        food.setOutlineColor(sf::Color::White);
        food.setOutlineThickness(2);
        food.setRadius(getFoodSize());

        Food foodObj(100, food);

        m_foodObjStorage.push_back(foodObj);
    }


	while (m_window.isOpen()) 
    {
        //Handles events from the simulation.
        handleEvents();

        //Handles keyboard and mouse inpput from the user.
		handleInput();

        //Renders all of the organisms to the screen along with anything else that needs to be drawn.
		render();
	}
}

//Handles simulation events such as collision detection and genetic mutations.
void Gui::handleEvents()
{
    //Check to see if any non-flocking organisms collided with any food objects.
    for (int i = 0; i < m_flock.getSize(); i++)
    {
        for (int k = 0; k < m_foodObjStorage.size(); k++)
        {
            //Get the amount of energy to add to the organism if food is consumed.
            int energy = m_foodObjStorage[k].getEnergyAmount();

            //If the organism collides with the food then add energy to its m_energyStore and remove the food.
            if (isCollided(m_flock.getOrganism(i), m_foodObjStorage[k]))
            {
                //Increase the organisms energy level.
                m_flock.getOrganism(i).setEnergyStore(energy);

                //Remove the consumed food from the food vector.
                m_foodObjStorage.erase(m_foodObjStorage.begin() + k);
            }
        }
    }

    //Check to see if any flocking organisms collided with any food objects.
    for (int i = 0; i < m_notFlocking.getSize(); i++)
    {
        for (int k = 0; k < m_foodObjStorage.size(); k++)
        {
            //Get the amount of energy to add to the organism if food is consumed.
            int energy = m_foodObjStorage[k].getEnergyAmount();

            //If the organism collides with the food then add energy to its m_energyStore and remove the food.
            if (isCollided(m_notFlocking.getOrganism(i), m_foodObjStorage[k]))
            {
                //Increase the organisms energy level.
                m_notFlocking.getOrganism(i).setEnergyStore(energy);

                //Remove the consumed food from the food vector.
                m_foodObjStorage.erase(m_foodObjStorage.begin() + k);
            }
        }
    }

    //Energy spending for all alive flocking organisms.
    for (int i = 0; i < m_flock.getSize(); i++)
    {
        sf::Time elapsed = m_flock.getOrganism(i).m_energyTimer.getElapsedTime();

        //Kill the organism if its energy store reaches 0.
        if (elapsed.asSeconds() >= m_flock.getOrganism(i).getEnergyUseTime())
        {
            m_flock.getOrganism(i).spendEnergy();

            //Restart the organisms alive timer.
            m_flock.getOrganism(i).m_energyTimer.restart();
        }
    }

    //Energy spending for all alive non-flocking organisms.
    for (int i = 0; i < m_notFlocking.getSize(); i++)
    {
        sf::Time elapsed = m_notFlocking.getOrganism(i).m_energyTimer.getElapsedTime();

        //Kill the organism if its energy store reaches 0.
        if (elapsed.asSeconds() >= m_notFlocking.getOrganism(i).getEnergyUseTime())
        {
            m_notFlocking.getOrganism(i).spendEnergy();

            //Restart the organisms alive timer.
            m_notFlocking.getOrganism(i).m_energyTimer.restart();
        }
    }

    //Start depeleting non-flocking organisms health when their energy store hits 0.
    for (int i = 0; i < m_notFlocking.getSize(); i++)
    {
        sf::Time elapsed = m_notFlocking.getOrganism(i).m_healthTimer.getElapsedTime();

        int timeToDrainHealth = 2;

        if (elapsed.asSeconds() >= timeToDrainHealth)
        {
            if (m_notFlocking.getOrganism(i).getEnergyStore() <= 0)
            {
                //Starvation process.
                m_notFlocking.getOrganism(i).starvation();

                //Check to see if the organisms health reaches 0. Kill it if it is.
                if (m_notFlocking.getOrganism(i).getHealth() <= 0)
                {
                    m_notFlocking.removeOrganism(i);
                    m_shapesNotFlocking.erase(m_shapesNotFlocking.begin() + i);
                }
            }

            m_notFlocking.getOrganism(i).m_healthTimer.restart();
        }
    }

    //Start depeleting flocking organisms health when their energy store hits 0.
    for (int i = 0; i < m_flock.getSize(); i++)
    {
        sf::Time elapsed = m_flock.getOrganism(i).m_healthTimer.getElapsedTime();

        int timeToDrainHealth = 2;

        if (elapsed.asSeconds() >= timeToDrainHealth)
        {
            if (m_flock.getOrganism(i).getEnergyStore() <= 0)
            {
                //Starvation process.
                m_flock.getOrganism(i).starvation();

                //Check to see if the organisms health reaches 0. Kill it if it is.
                if (m_flock.getOrganism(i).getHealth() <= 0)
                {
                    m_flock.removeOrganism(i);
                    m_shapes.erase(m_shapes.begin() + i);
                }
            }

            m_flock.getOrganism(i).m_healthTimer.restart();
        }
    }

    //Display all the text vaalues to the screen.
    displayText();

    //Create new food when the timer allows it.
    sf::Time elapsed = m_foodTimer.getElapsedTime();
    int amount = rand() % ((getFoodAmountRespawn() + 1) - 1);

    if (elapsed.asSeconds() >= getFoodRespawn())
    {
        for (int i = 0; i < amount; i++)
        {
            //Create shape and food object.
            sf::CircleShape food(rand() % 100, 4);

            //Set the position of the food.
            food.setPosition(rand() % 1920, rand() % 1080);
            food.setOutlineColor(sf::Color(0, 255, 0));
            food.setFillColor(sf::Color::White);
            food.setOutlineColor(sf::Color::White);
            food.setOutlineThickness(2);
            food.setRadius(getFoodSize());

            Food foodObj(100, food);

            m_foodObjStorage.push_back(foodObj);
        }

        m_foodTimer.restart();
    }

    //Asexual reproduction event for both flocking and non-floccking orgaisms.
    //Non-flocking.
    for (int i = 0; i < m_notFlocking.getSize(); i++)
    {
        sf::Time elapsed = m_notFlocking.getOrganism(i).m_reproduceTimer.getElapsedTime();

        //Check to see if it is time to reproduce.
        if (elapsed.asSeconds() >= m_notFlocking.getOrganism(i).getTimeUntilReproduce())
        {
            if (m_notFlocking.getOrganism(i).getEnergyStore() >=500)
            {
                //Check to make sure the organism is in good health.
                if (m_notFlocking.getOrganism(i).getHealth() >= 50)
                {
                    //Creating a new child organism based on parents offspring amount value.
                    for (int i = 0; i < m_notFlocking.getOrganism(i).getOffSpringAmount(); i++)
                    {
                        createOrganism(m_notFlocking.getOrganism(i));
                        cout << "CreateOrganism activated..." << endl;
                    }

                    //Removing energy needed to pro-create.
                    m_notFlocking.getOrganism(i).reduceEnergy(200);
                }
            }

            m_notFlocking.getOrganism(i).m_reproduceTimer.restart();
        }
    }

    //Flocking.
    for (int i = 0; i < m_flock.getSize(); i++)
    {
        sf::Time elapsed = m_flock.getOrganism(i).m_reproduceTimer.getElapsedTime();

        //Check to see if it is time to reproduce.
        if (elapsed.asSeconds() >= m_flock.getOrganism(i).getTimeUntilReproduce())
        {
            if (m_flock.getOrganism(i).getEnergyStore() >= 500)
            {
                //Check to make sure the organism is in good health.
                if (m_flock.getOrganism(i).getHealth() >= 50)
                {
                    //Creating a new child organism based on parents offspring amount value.
                    for (int i = 0; i < m_flock.getOrganism(i).getOffSpringAmount(); i++)
                    {
                        createOrganism(m_flock.getOrganism(i));
                        cout << "CreateOrganism activated..." << endl;
                    }

                    //Removing energy needed to pro-create.
                    m_flock.getOrganism(i).reduceEnergy(200);
                }
            }

            m_flock.getOrganism(i).m_reproduceTimer.restart();
        }
    }

}

void Gui::handleInput()
{
    sf::Event event;
    while (m_window.pollEvent(event)) 
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
            m_window.close();
        }
    }

    // Check for mouse click, draws and adds Organism to flock if so.
    // These Organisms will automaticaly flock.
    /*if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
    {
        // Gets mouse coordinates, sets that as the location of the Organism and the shape
        sf::Vector2i mouseCoords = sf::Mouse::getPosition(m_window);
        Organism organism(mouseCoords.x, mouseCoords.y, false, true);
        sf::CircleShape shape(10, 3);

        // Changing visual properties of newly created Organism
        shape.setPosition(mouseCoords.x, mouseCoords.y);
        shape.setOutlineColor(sf::Color(255, 0, 0));
        shape.setFillColor(sf::Color(255, 0, 0));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1);
        shape.setRadius(getOrganismSize());

        // Adds newly created Organism and shape to their respective data structure
        //Check to see if the Organism has the flock trait, if it does add to active flock.
        if(organism.m_hasFlockTrait)
        {
            m_flock.addOrganism(organism);
            m_shapes.push_back(shape);
        }
        else
        {
            m_notFlocking.addOrganism(organism);
            m_shapesNotFlocking.push_back(shape);
        }

        // New Shape is drawn
        m_window.draw(m_shapes[m_shapes.size() - 1]);
    }*/
}

/*

    Gui::Render => This function draws all Organisms to the screen. First we render the Organisms that
    do not have the flocking trait, then we render all the ones that do.

*/
void Gui::render()
{
    m_window.clear();

    //Draw all text to the screen.
    m_window.draw(m_text);
    m_window.draw(m_numberAlive);
    m_window.draw(m_foodInPlay);
    m_window.draw(m_timeElapsed);

    //Draws all the food to the screen and sets positions.
    for (int i = 0; i < m_foodObjStorage.size(); i++)
    {
        m_window.draw(m_foodObjStorage[i].m_shape);

        //Set the position off the food.
        sf::Vector2f sfmlPosition = m_foodObjStorage[i].m_shape.getPosition();
        m_foodObjStorage[i].m_position.x = sfmlPosition.x;
        m_foodObjStorage[i].m_position.y = sfmlPosition.y;
    }

    // Draws all of the Organisms out, and applies functions that are needed to update.
    //Draw all of the non-flocking organisms.
    for (int i = 0; i < m_shapesNotFlocking.size(); i++)
    {
        m_window.draw(m_shapesNotFlocking[i]);

        // Matches up the location of the shape to the organism.
        m_shapesNotFlocking[i].setPosition(m_notFlocking.getOrganism(i).m_location.x, m_notFlocking.getOrganism(i).m_location.y);

        // Calculates the angle where the velocity is pointing so that the triangle turns towards it.
        float theta = m_notFlocking.getOrganism(i).angle(m_notFlocking.getOrganism(i).m_velocity);
        m_shapesNotFlocking[i].setRotation(theta);

        // Prevent boids from moving off the screen through wrapping
        // If boid exits right boundary
        if (m_shapesNotFlocking[i].getPosition().x > getWidth())
            m_shapesNotFlocking[i].setPosition(m_shapesNotFlocking[i].getPosition().x - getWidth(), m_shapesNotFlocking[i].getPosition().y);
        // If boid exits bottom boundary
        if (m_shapesNotFlocking[i].getPosition().y > getHeight())
            m_shapesNotFlocking[i].setPosition(m_shapesNotFlocking[i].getPosition().x, m_shapesNotFlocking[i].getPosition().y - getHeight());
        // If boid exits left boundary
        if (m_shapesNotFlocking[i].getPosition().x < 0)
            m_shapesNotFlocking[i].setPosition(m_shapesNotFlocking[i].getPosition().x + getWidth(), m_shapesNotFlocking[i].getPosition().y);
        // If boid exits top boundary
        if (m_shapesNotFlocking[i].getPosition().y < 0)
            m_shapesNotFlocking[i].setPosition(m_shapesNotFlocking[i].getPosition().x, m_shapesNotFlocking[i].getPosition().y + getHeight());
    }

    //Draw all of the Flocking Organisms.
    for (int i = 0; i < m_shapes.size(); i++)
    {
        m_window.draw(m_shapes[i]);

        // Matches up the location of the shape to the boid
        m_shapes[i].setPosition(m_flock.getOrganism(i).m_location.x, m_flock.getOrganism(i).m_location.y);

        // Calculates the angle where the velocity is pointing so that the triangle turns towards it.
        float theta = m_flock.getOrganism(i).angle(m_flock.getOrganism(i).m_velocity);
        m_shapes[i].setRotation(theta);

        // Prevent boids from moving off the screen through wrapping
        // If boid exits right boundary
        if (m_shapes[i].getPosition().x > getWidth())
            m_shapes[i].setPosition(m_shapes[i].getPosition().x - getWidth(), m_shapes[i].getPosition().y);
        // If boid exits bottom boundary
        if (m_shapes[i].getPosition().y > getHeight())
            m_shapes[i].setPosition(m_shapes[i].getPosition().x, m_shapes[i].getPosition().y - getHeight());
        // If boid exits left boundary
        if (m_shapes[i].getPosition().x < 0)
            m_shapes[i].setPosition(m_shapes[i].getPosition().x + getWidth(), m_shapes[i].getPosition().y);
        // If boid exits top boundary
        if (m_shapes[i].getPosition().y < 0)
            m_shapes[i].setPosition(m_shapes[i].getPosition().x, m_shapes[i].getPosition().y + getHeight());
    }
    // Applies the three rules to each boid in the flock and changes them accordingly.
    m_flock.flocking(20);
    m_notFlocking.nonflocking(20);

    m_window.display();
}

/*

    Events: The fuctions below deal with simulation events that are handled from within the handleEvents() function.

*/

//Checks for collision between organism and food.
bool Gui::isCollided(Organism organism, Food food)
{
    //Check the organism for collision with food objects.
    float distance = organism.m_location.distance(food.m_position);

    if (distance < 20)
    {
        return true;
    }
    
    return false;
}

//This functions loads the text for the GUI window.
void Gui::loadText()
{
    //Prepare the statistics text.
    // select the font
    if (!m_font.loadFromFile("Roboto-Thin.ttf"))
    {
        // error...
        cout << "Font could not load..." << endl;
    }

    //DEBUG
    //PrintFullPath();

    m_text.setFont(m_font); // font is a sf::Font
    // set the string to display
    m_text.setString("EvoSim");
    // set the character size
    m_text.setCharacterSize(24); // in pixels, not points!
    // set the color
    m_text.setFillColor(sf::Color::Red);
    // set the text style
    m_text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    m_numberAlive.setFont(m_font); // font is a sf::Font
    // set the character size
    m_numberAlive.setCharacterSize(24); // in pixels, not points!
    // set the color
    m_numberAlive.setFillColor(sf::Color::Red);
    // set the text style
    m_numberAlive.setStyle(sf::Text::Bold | sf::Text::Underlined);
    //Set the position of the text.
    m_numberAlive.setPosition(0, 50);

    m_foodInPlay.setFont(m_font); // font is a sf::Font
    // set the character size
    m_foodInPlay.setCharacterSize(24); // in pixels, not points!
    // set the color
    m_foodInPlay.setFillColor(sf::Color::Red);
    // set the text style
    m_foodInPlay.setStyle(sf::Text::Bold | sf::Text::Underlined);
    //Set the position of the text.
    m_foodInPlay.setPosition(0, 100);

    m_timeElapsed.setFont(m_font); // font is a sf::Font
    // set the character size
    m_timeElapsed.setCharacterSize(24); // in pixels, not points!
    // set the color
    m_timeElapsed.setFillColor(sf::Color::Red);
    // set the text style
    m_timeElapsed.setStyle(sf::Text::Bold | sf::Text::Underlined);
    //Set the position of the text.
    m_timeElapsed.setPosition(0, 150);
}

//Displays the updated text on the screen.
void Gui::displayText()
{
    //Count the number of organisms alive and display in text.
    int numberAliveTemp = m_flock.getSize() + m_notFlocking.getSize();
    auto result = std::to_string(numberAliveTemp);
    m_numberAlive.setString("Organisms Alive: " + result);

    //Count the number of food objects in play and set the text.
    int foodTemp = m_foodObjStorage.size();
    auto resultFood = std::to_string(foodTemp);
    m_foodInPlay.setString("Food Items: " + resultFood);

    //Get simulation time elapsed and set text.
    m_simTimerSeconds = m_simTimer.getElapsedTime();
    int timeTemp = m_simTimerSeconds.asSeconds();
    auto resultTime = std::to_string(timeTemp);
    m_timeElapsed.setString("Time Elapsed (Seconds): " + resultTime);
}

/*

    Adaption functions.

*/

//These functions create a new organism using a copy of the parent.
//This function should only be called in the events function of the main loop.
void Gui::createOrganism(Organism parent)
{
    //Create a new Organism object 
    Organism child(parent.m_location.x, parent.m_location.y);

    //Sets the new organism object's fields to be the same as its parent.
    child.setAcceleration(parent.getAcceleration());
    child.setVelocity(parent.getVelocity());
    child.setMaxSpeed(parent.getMaxSpeed());
    child.setMaxForce(parent.getMaxForce());
    child.setPredator(parent.getPredator());
    child.setHasFlockTrait(parent.getHasFlockTrait());
    child.setDesiredSep(parent.getDesiredSep());
    child.setHealth(parent.getHealth());
    child.setEnergyStore(parent.getEnergyStore());
    child.setEnergyUsetime(parent.getEnergyUseTime());
    child.setDefense(parent.getDefense());

    //Add the new object to the object vector and create and add a new shape to the shape vector.
    sf::CircleShape shape(8, 3);

    // Changing the Visual Properties of the shape
    shape.setPosition(getWidth(), getHeight()); // Testing purposes, starts all shapes in the center of screen.
    shape.setOutlineColor(sf::Color(0, 255, 0));
    shape.setFillColor(sf::Color::Green);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(1);
    shape.setRadius(getOrganismSize());

    //Check to see if the Organism has the flock trait, if it does add to active flock.
    if (child.m_hasFlockTrait)
    {
        m_flock.addOrganism(child);
        m_shapes.push_back(shape);
    }
    else
    {
        m_notFlocking.addOrganism(child);
        m_shapesNotFlocking.push_back(shape);
    }
}

/*

    Getters and Setters

*/

//Getters.
int Gui::getWidth()
{
    return m_width;
}

int Gui::getHeight()
{
    return m_height;
}

int Gui::getFoodAmount()
{
    return m_foodAmount;
}

int Gui::getFoodAmountRespawn()
{
    return m_foodAmountRespawn;
}

int Gui::getStartingPop()
{
    return m_startingPop;
}

int Gui::getFoodRespawn()
{
    return m_foodRespawn;
}

float Gui::getOrganismSize()
{
    return m_organismSize;
}

float Gui::getFoodSize()
{
    return m_foodSize;
}

//Setters.
void Gui::setWidth(int value)
{
    m_width = value;
}

void Gui::setHeight(int value)
{
    m_height = value;
}

void Gui::setFoodAmount(int value)
{
    m_foodAmount = value;
}

void Gui::setFoodAmountRespawn(int value)
{
    m_foodAmountRespawn = value;
}

void Gui::setStartingPop(int value)
{
    m_startingPop = value;
}

void Gui::setFoodRespawn(int value)
{
    m_foodRespawn = value;
}

void Gui::setOrganismSize(float value)
{
    m_organismSize = value;
}

void Gui::setFoodSize(float value)
{
    m_foodSize = value;
}