#include "Body.h"

Body::Body(Vector position, Vector velocity, double mass)
    : position_(position), velocity_(velocity), mass_(mass) {}

void Body::updatePosition(double time_step) {
    position_ = position_ + velocity_ * time_step;
}
