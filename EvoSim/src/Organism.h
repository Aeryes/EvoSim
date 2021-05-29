/*
	Organism.h

	Description: Organism.h contains the prototype functions for organism operation.

    Credits: Jorge Yanar - https://github.com/jyanar/Boids for the boids.h file.
*/

/*
    Note:
    I decided not to have levels of inheritance for different types of organisms because they need to be able to
    mutate freely while the program executes. Having child classes of Organism would resullt in a more restricted
    mutation capability where Organisms would be less free to adapt and change to each other.
*/

#pragma once

#include <vector>
#include "Pvector.h"
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

class Organism
{
public:
    Organism(float x, float y);
    Organism(float x, float y, bool predCheck, bool hasFlockTrait);

    //Movement values.
    Pvector m_location;
    Pvector m_velocity;
    Pvector m_acceleration;

    float m_maxSpeed;
    float m_maxForce;

    //Trait values.
    bool m_predator;

    //Timers.
    sf::Clock m_movementClock;
    sf::Clock m_energyTimer;
    sf::Clock m_mutationTimer;
    sf::Clock m_healthTimer;
    sf::Clock m_reproduceTimer;

    //Flocking fields.
    bool m_hasFlockTrait = false;
    float m_desiredseparation;

    void applyForce(const Pvector& force);

    // Three Laws that boids follow
    Pvector separation(const vector<Organism>& Organsims, float sep);
    Pvector nonFlockSep(const vector<Organism>& organisms, float sep);
    Pvector alignment(const vector<Organism>& Organsims);
    Pvector cohesion(const vector<Organism>& Organisms);

    //Prototypes involving visualisation linking
    Pvector seek(const Pvector& v);

    void run(const vector<Organism>& v, float sep);
    void runNonFlock(const vector <Organism>& v, float separation);
    void update();
    void updateNonFlock();
    void flock(const vector<Organism>& v, float separaton);
    void borders();
    float angle(const Pvector& v);

    //Getters and setters.
    void setEnergyStore(int value);
    void setEnergyUsetime(int value);
    void setHealth(int value);
    void setDefense(int value);
    void setLocation(Pvector location);
    void setVelocity(Pvector velocity);
    void setAcceleration(Pvector acceleration);
    void setHasFlockTrait(bool trait);
    void setDesiredSep(float sep);
    void setMaxSpeed(float speed);
    void setMaxForce(float force);
    void setPredator(bool value);
    void setOffSpringAmount(int value);

    int getEnergyStore();
    int getEnergyUseTime();
    int getHealth();
    int getDefense();
    int getTimeUntilReproduce();
    int getOffSpringAmount();

    Pvector getLocation();
    Pvector getVelocity();
    Pvector getAcceleration();

    bool getHasFlockTrait();
    bool getPredator();

    float getDesiredSep();
    float getMaxSpeed();
    float getMaxForce();

    void spendEnergy();
    void reduceEnergy(int amount);

    //Prototypes for functions affected by adaption.
    //Lifecycle prototypes.
    void starvation();

    //Direct adaption prototypes.

private:
    //Organism life values.
    int m_health = 100;
    int m_healthDrain = 20;
    int m_defense = 100;
    int m_attack = 10;
    int m_energyStore = 200;
    int m_energyUseLevel = 100;
    int m_energyUseTime = 10;

    //Adaption weights.
    //Adaption weights are % based modifiers.
    //Health modifiers.
    int m_starvationWeight = 0;

    //Sexual reproduction modifiers.
    //Organisms main goal is to reproduce and create copies.

    //Time in seconds.
    int m_timeUntilReproduce = 20;

    //Number of copies to make.
    int m_offspringAmount = 1;
};


