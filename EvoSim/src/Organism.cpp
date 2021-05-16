/*
    Organism.cpp

    Description: Organism.cpp contains function definitions for the Organism.h file.

    Credits: Jorge Yanar - https://github.com/jyanar/Boids for the boids.cpp file.

*/

#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include "Organism.h"
#include "SFML/Graphics.hpp"

// Global Variables for borders()
// desktopTemp gets screen resolution of PC running the program
sf::VideoMode desktopTemp = sf::VideoMode::getDesktopMode();
const int window_height = desktopTemp.height;
const int window_width = desktopTemp.width;

#define w_height window_height
#define w_width window_width
#define PI 3.141592635

Organism::Organism(float x, float y)
{
    m_acceleration = Pvector(0, 0);
    m_velocity = Pvector(rand() % 3 - 2, rand() % 3 - 2);
    m_location = Pvector(x, y);
    m_maxSpeed = 3.5;
    m_maxForce = 0.5;
}

Organism::Organism(float x, float y, bool predCheck, bool flockTrait)
{
    m_predator = predCheck;
    m_hasFlockTrait = flockTrait;

    if (predCheck == true) {
        m_maxSpeed = 5.5;
        m_maxForce = 0.5;
        m_velocity = Pvector(rand() % 3 - 1, rand() % 3 - 1);
    }
    else {
        m_maxSpeed = 3.5;
        m_maxForce = 0.5;
        m_velocity = Pvector(rand() % 3 - 2, rand() % 3 - 2);
    }

    m_acceleration = Pvector(0, 0);
    m_location = Pvector(x, y);
}

// Adds force Pvector to current force Pvector
void Organism::applyForce(const Pvector& force)
{
    m_acceleration.addVector(force);
}

// Separation
// Keeps organisms from getting too close to one another
Pvector Organism::separation(const vector<Organism>& organisms, float sep)
{
    // Distance of field of vision for separation between organisms
    m_desiredseparation = sep;

    //Direction of movement and actual movement vector.
    Pvector steer(0, 0);
    int count = 0;

    // For every organism in the system, check if it's too close
    for (int i = 0; i < organisms.size(); i++) 
    {

        // Calculate distance from current organism to organism we're looking at
        float d = m_location.distance(organisms[i].m_location);

        // If this is a fellow organism and it's too close, move away from it
        if ((d > 0) && (d < m_desiredseparation)) 
        {
            Pvector diff(0, 0);
            diff = diff.subTwoVector(m_location, organisms[i].m_location);
            diff.normalize();
            diff.divScalar(d);      // Weight by distance
            steer.addVector(diff);
            count++;
        }

        // If current organism is a predator and the organism we're looking at is also
        // a predator, then separate only slightly
        if ((d > 0) && (d < m_desiredseparation) && m_predator == true
            && organisms[i].m_predator == true) 
        {
            Pvector pred2pred(0, 0);
            pred2pred = pred2pred.subTwoVector(m_location, organisms[i].m_location);
            pred2pred.normalize();
            pred2pred.divScalar(d);
            steer.addVector(pred2pred);
            count++;
        }

        // If current organism is not a predator, but the organism we're looking at is
        // a predator, then create a large separation Pvector
        else if ((d > 0) && (d < m_desiredseparation + 70) && organisms[i].m_predator == true) 
        {
            Pvector pred(0, 0);
            pred = pred.subTwoVector(m_location, organisms[i].m_location);
            pred.mulScalar(900);
            steer.addVector(pred);
            count++;
        }
    }

    // Adds average difference of location to acceleration
    if (count > 0)
        steer.divScalar((float)count);

    if (steer.magnitude() > 0) 
    {
        // Steering = Desired - Velocity
        steer.normalize();
        steer.mulScalar(m_maxSpeed);
        steer.subVector(m_velocity);
        steer.limit(m_maxForce);
    }

    return steer;
}

// Alignment
// Calculates the average velocity of organisms in the field of vision and
// manipulates the velocity of the current organism in order to match it
Pvector Organism::alignment(const vector<Organism>& organisms)
{
    float neighbordist = 50; // Field of vision

    Pvector sum(0, 0);
    int count = 0;

    for (int i = 0; i < organisms.size(); i++) 
    {
        float d = m_location.distance(organisms[i].m_location);
        if ((d > 0) && (d < neighbordist)) 
        { // 0 < d < 50
            sum.addVector(organisms[i].m_velocity);
            count++;
        }
    }

    // If there are organisms close enough for alignment...
    if (count > 0) 
    {
        sum.divScalar((float)count);// Divide sum by the number of close organisms (average of velocity)
        sum.normalize();            // Turn sum into a unit vector, and
        sum.mulScalar(m_maxSpeed);    // Multiply by maxSpeed
        // Steer = Desired - Velocity
        Pvector steer;
        steer = steer.subTwoVector(sum, m_velocity); //sum = desired(average)
        steer.limit(m_maxForce);
        return steer;
    }
    else 
    {
        Pvector temp(0, 0);
        return temp;
    }
}

// Cohesion
// Finds the average location of nearby organisms and manipulates the
// steering force to move in that direction.
Pvector Organism::cohesion(const vector<Organism>& organisms)
{
    float neighbordist = 50;
    Pvector sum(0, 0);
    int count = 0;

    for (int i = 0; i < organisms.size(); i++) 
    {
        float d = m_location.distance(organisms[i].m_location);

        if ((d > 0) && (d < neighbordist)) 
        {
            sum.addVector(organisms[i].m_location);
            count++;
        }
    }

    if (count > 0) 
    {
        sum.divScalar(count);
        return seek(sum);
    }
    else 
    {
        Pvector temp(0, 0);
        return temp;
    }
}

// Limits the maxSpeed, finds necessary steering force and
// normalizes vectors
Pvector Organism::seek(const Pvector& v)
{
    Pvector desired;
    desired.subVector(v);  // A vector pointing from the location to the target

    // Normalize desired and scale to maximum speed
    desired.normalize();
    desired.mulScalar(m_maxSpeed);

    // Steering = Desired minus Velocity
    m_acceleration.subTwoVector(desired, m_velocity);
    m_acceleration.limit(m_maxForce);  // Limit to maximum steering force
    return m_acceleration;
}

// Modifies velocity, location, and resets acceleration with values that
// are given by the three laws.
void Organism::update()
{
    //To make the slow down not as abrupt
    m_acceleration.mulScalar(.4);

    // Update velocity
    m_velocity.addVector(m_acceleration);

    // Limit speed
    m_velocity.limit(m_maxSpeed);

    //Moves the organisms.
    m_location.addVector(m_velocity);

    // Reset accelertion to 0 each cycle to allow smoooth movement dring flocking.
    m_acceleration.mulScalar(0);
}

//Modifies velocity, location and acceleration for non-flocking organisms.
void Organism::updateNonFlock()
{
    //To make the slow down not as abrupt
    m_acceleration.mulScalar(.4);

    // Update velocity
    m_velocity.addVector(m_acceleration);

    // Limit speed
    m_velocity.limit(m_maxSpeed);

    //Moves the organisms.
    sf::Time elapsed = m_movementClock.getElapsedTime();

    if (elapsed.asSeconds() > 3)
    {
        m_movementClock.restart();

        m_velocity.subVector(Pvector(rand() % 2 - 0.5, rand() % 2 - 0.5));
    }

    m_location.addVector(m_velocity);
    m_location.addVector(m_velocity);
 
    // Reset accelertion to 0 each cycle to allow smoooth movement dring flocking.
    m_acceleration.mulScalar(0);
}

// Run flock() on the flock of organisms.
// This applies the three rules, modifies velocities accordingly, updates data,
// and corrects boids which are sitting outside of the window
void Organism::run(const vector <Organism>& v, float sep)
{
    flock(v, sep);
    update();
    borders();
}

// Run for non-flocking organisms
void Organism::runNonFlock(const vector <Organism>& v, float separation)
{
    updateNonFlock();
    borders();
}

// Applies the three laws to the flock of organisms
void Organism::flock(const vector<Organism>& v, float sepr)
{
    Pvector sep = separation(v, sepr);
    Pvector ali = alignment(v);
    Pvector coh = cohesion(v);

    // Arbitrarily weight these forces
    sep.mulScalar(1.5);
    ali.mulScalar(1.0); // Might need to alter weights for different characteristics
    coh.mulScalar(1.0);

    // Add the force vectors to acceleration
    applyForce(sep);
    applyForce(ali);
    applyForce(coh);
}

// Checks if boids go out of the window and if so, wraps them around to
// the other side.
void Organism::borders()
{
    if (m_location.x < 0)    m_location.x += w_width;
    if (m_location.y < 0)    m_location.y += w_height;
    if (m_location.x > 1920) m_location.x -= w_width;
    if (m_location.y > 1080) m_location.y -= w_height;
}

// Calculates the angle for the velocity of a boid which allows the visual
// image to rotate in the direction that it is going in.
float Organism::angle(const Pvector& v)
{
    // From the definition of the dot product
    float angle = (float)(atan2(v.x, -v.y) * 180.0 / PI);
    return angle;
}

//This function spends energy while the organism is alive.
void Organism::spendEnergy()
{
    this->m_energyStore = this->m_energyStore - this->m_energyUseLevel;
    cout << "ENERGY STORE: " << this->m_energyStore << endl;
}

//Getters and setters
void Organism::setEnergyStore(int amount)
{
    this->m_energyStore = this->m_energyStore + amount;
}

int Organism::getEnergyStore()
{
    return this->m_energyStore;
}

void Organism::setEnergyUsetime(int value)
{
    this->m_energyUseTime = value;
}

int Organism::getEnergyUseTime()
{
    return this->m_energyUseTime;
}
