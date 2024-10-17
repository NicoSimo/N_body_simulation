#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include "Body.h"

class Simulation {
public:
    Simulation();
    ~Simulation();
    void initializeBodies();
    void initializeShaders();
    void update();
    void render() const;

private:
    std::vector<Body> bodies_;
    double time_step_;

    unsigned int shaderProgram_;
    int modelLoc_, viewLoc_, projectionLoc_;

    unsigned int sphereVAO_, sphereVBO_;
    void setupSphere();
    void drawSphere() const;
};

#endif /* SIMULATION_H */
