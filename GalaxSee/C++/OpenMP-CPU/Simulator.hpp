/**
 * @file Simulator.hpp
 * @brief Defines a class for creating an NbodyModel object 
 * and running the Nbody simulation.
 * 
 * Project originally by:
 * [Original Author Name]
 *
 * Modified by:
 * Steven McKelvey - Calvin University, January 2025
 */

#ifndef SIMULATOR
#define SIMULATOR

#include "nbody.hpp"
#include "graphics.hpp"

class Simulator
{
public:
    ~Simulator();
    Simulator(unsigned num_bodies, double rotation_factor, double initial_v,
        double tFinal, double tStep, int count_updates, int skip_updates, int show_display);
    void run(unsigned seed);

private:
    NbodyModel *myModel;
    double tFinal, tStep;
    int count_updates, skip_updates, show_display;
};

inline Simulator::Simulator(unsigned num_bodies, double rotation_factor, double initial_v,
    double the_tFinal, double the_tStep, int the_count_updates, int the_skip_updates, int the_show_display)
{
    // Initialize the N-body model
    myModel = new NbodyModel(num_bodies);
    myModel->setScaleNbodyModel(800.0); // Set spatial scale (parsecs)
    myModel->setMassNbodyModel(200000.0 / (double)num_bodies); // Set mass (solar masses)
    myModel->setGNbodyModel(0.0046254); // Set gravitational constant (pc^3/solar_mass/My^2)
    myModel->initializeNbodyModel(); // Initialize model (positions, velocities)
    myModel->spinNbodyModel(rotation_factor); // Apply rotational spin
    myModel->speedNbodyModel(initial_v); // Apply initial velocities

    // initialize values used by Simulator.run()
    tFinal = the_tFinal;
    tStep = the_tStep;
    count_updates = the_count_updates;
    skip_updates = the_skip_updates;
    show_display = the_show_display;
}

inline Simulator::~Simulator()
{
    delete myModel;
}

inline void Simulator::run(unsigned seed = 0)
{
    srand(seed); // Seed the random number generator for reproducibility

    // Main simulation loop
    while (myModel->t < tFinal) {
        // Advance the simulation by one time step
        myModel->stepNbodyModel(tStep);

        // Update the display periodically based on skip_updates
        if ((count_updates++) % skip_updates == 0 && show_display) {
            updateNbodyModel(myModel); // Visual update using X11
        }
    }
}

#endif