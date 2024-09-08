#ifndef PARTICLE_H
#define PARTICLE_H

#include "defines.hpp"
#include <SFML/Graphics.hpp>

class Particle {
  private:
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2f origin;
    sf::Color col;

  public:
    Particle(sf::Vector2f p, sf::Vector2f v, sf::Color c);
    ~Particle();

    void update(f32 dt);
    void applyRepulsion(sf::Vector2f mpos);
    void applyReturnForce();

    sf::Vector2f getPosition() const;
};

#endif // PARTICLE_H