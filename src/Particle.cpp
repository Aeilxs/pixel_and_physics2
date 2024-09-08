#include "Particle.hpp"
#include "config.hpp"
#include "defines.hpp"
#include <SFML/Graphics.hpp>

Particle::Particle(sf::Vector2f p, sf::Vector2f v, sf::Color c) : pos(p), vel(v), origin(p), col(c) {
}

Particle::~Particle() {
}

void Particle::applyRepulsion(sf::Vector2f mpos) {
    sf::Vector2f pxDir = pos - mpos;
    f32 distSquared = pxDir.x * pxDir.x + pxDir.y * pxDir.y;
    if (distSquared < repulsion_radius * repulsion_radius && distSquared > 0) {
        f32 dist = std::sqrt(distSquared);
        sf::Vector2f normalized = pxDir / dist;
        vel += normalized * repulsion_strength / dist;
    }
}

void Particle::applyReturnForce() {
    vel += (origin - pos) * return_force;
}

void Particle::update(f32 dt) {
    // pos = vec2add(pos, vec2scale(vel, dt));
    pos += vel * dt;
    vel *= friction;
}

sf::Vector2f Particle::getPosition() const {
    return pos;
}
