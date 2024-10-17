#ifndef BODY_H
#define BODY_H

#include "Vector.h"

class Body {
public:
    Body(Vector position, Vector velocity, double mass);
    void updatePosition(double time_step);
    Vector getPosition() const { return position_; }
    double getRadius() const { return radius_; }

private:
    Vector position_;
    Vector velocity_;
    double mass_;
    double radius_;
};

#endif /* BODY_H */
