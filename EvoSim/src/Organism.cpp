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
    acceleration = Pvector(0, 0);
    velocity = Pvector(rand() % 3 - 2, rand() % 3 - 2);
    location = Pvector(x, y);
    maxSpeed = 3.5;
    maxForce = 0.5;
}

Organism::Organism(float x, float y, bool predCheck, bool flockTrait)
{
    predator = predCheck;
    hasFlockTrait = flockTrait;
    if (predCheck == true) {
        maxSpeed = 5.5;
        maxForce = 0.5;
        velocity = Pvector(rand() % 3 - 1, rand() % 3 - 1);
    }
    else {
        maxSpeed = 3.5;
        maxForce = 0.5;
        velocity = Pvector(rand() % 3 - 2, rand() % 3 - 2);
    }
    acceleration = Pvector(0, 0);
    location = Pvector(x, y);
}

// Adds force Pvector to current force Pvector
void Organism::applyForce(const Pvector& force)
{
    acceleration.addVector(force);
}

// Separation
// Keeps organisms from getting too close to one another
Pvector Organism::separation(const vector<Organism>& organisms, float sep)
{
    // Distance of field of vision for separation between organisms
    desiredseparation = sep;
    //Direction of movement and actual movement vector.
    Pvector steer(0, 0);
    int count = 0;

    // For every organism in the system, check if it's too close
    for (int i = 0; i < organisms.size(); i++) 
    {

        // Calculate distance from current organism to organism we're looking at
        float d = location.distance(organisms[i].location);

        // If this is a fellow organism and it's too close, move away from it
        if ((d > 0) && (d < desiredseparation)) 
        {
            Pvector diff(0, 0);
            diff = diff.subTwoVector(location, organisms[i].location);
            diff.normalize();
            diff.divScalar(d);      // Weight by distance
            steer.addVector(diff);
            count++;
        }

        // If current organism is a predator and the organism we're looking at is also
        // a predator, then separate only slightly
        if ((d > 0) && (d < desiredseparation) && predator == true
            && organisms[i].predator == true) 
        {
            Pvector pred2pred(0, 0);
            pred2pred = pred2pred.subTwoVector(location, organisms[i].location);
            pred2pred.normalize();
            pred2pred.divScalar(d);
            steer.addVector(pred2pred);
            count++;
        }

        // If current organism is not a predator, but the organism we're looking at is
        // a predator, then create a large separation Pvector
        else if ((d > 0) && (d < desiredseparation + 70) && organisms[i].predator == true) 
        {
            Pvector pred(0, 0);
            pred = pred.subTwoVector(location, organisms[i].location);
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
        steer.mulScalar(maxSpeed);
        steer.subVector(velocity);
        steer.limit(maxForce);
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
        float d = location.distance(organisms[i].location);
        if ((d > 0) && (d < neighbordist)) 
        { // 0 < d < 50
            sum.addVector(organisms[i].velocity);
            count++;
        }
    }

    // If there are organisms close enough for alignment...
    if (count > 0) 
    {
        sum.divScalar((float)count);// Divide sum by the number of close organisms (average of velocity)
        sum.normalize();            // Turn sum into a unit vector, and
        sum.mulScalar(maxSpeed);    // Multiply by maxSpeed
        // Steer = Desired - Velocity
        Pvector steer;
        steer = steer.subTwoVector(sum, velocity); //sum = desired(average)
        steer.limit(maxForce);
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
        float d = location.distance(organisms[i].location);

        if ((d > 0) && (d < neighbordist)) 
        {
            sum.addVector(organisms[i].location);
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
    desired.mulScalar(maxSpeed);

    // Steering = Desired minus Velocity
    acceleration.subTwoVector(desired, velocity);
    acceleration.limit(maxForce);  // Limit to maximum steering force
    return acceleration;
}

// Modifies velocity, location, and resets acceleration with values that
// are given by the three laws.
void Organism::update()
{
    //To make the slow down not as abrupt
    acceleration.mulScalar(.4);
    // Update velocity
    velocity.addVector(acceleration);
    // Limit speed
    velocity.limit(maxSpeed);
    //Moves the organisms.
    location.addVector(velocity);
    // Reset accelertion to 0 each cycle to allow smoooth movement dring flocking.
    acceleration.mulScalar(0);
}

//Modifies velocity, location and acceleration for non-flocking organisms.
void Organism::updateNonFlock()
{
    //To make the slow down not as abrupt
    acceleration.mulScalar(.4);
    // Update velocity
    velocity.addVector(acceleration);
    // Limit speed
    velocity.limit(maxSpeed);

    //Moves the organisms.
    sf::Time elapsed = this->movementClock.getElapsedTime();

    if (elapsed.asSeconds() > 3)
    {
        movementClock.restart();

        velocity.subVector(Pvector(rand() % 2 - 0.5, rand() % 2 - 0.5));
    }

    location.addVector(velocity);
    location.addVector(velocity);
 
    // Reset accelertion to 0 each cycle to allow smoooth movement dring flocking.
    acceleration.mulScalar(0);
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
    if (location.x < 0)    location.x += w_width;
    if (location.y < 0)    location.y += w_height;
    if (location.x > 1920) location.x -= w_width;
    if (location.y > 1080) location.y -= w_height;
}

// Calculates the angle for the velocity of a boid which allows the visual
// image to rotate in the direction that it is going in.
float Organism::angle(const Pvector& v)
{
    // From the definition of the dot product
    float angle = (float)(atan2(v.x, -v.y) * 180.0 / PI);
    return angle;
}