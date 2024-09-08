#ifndef EFFECT_H
#define EFFECT_H

#include "Particle.hpp"
#include "defines.hpp"
#include <SFML/Graphics.hpp>


class Effect {
  private:
    std::vector<Particle> particles;
    sf::VertexArray particleVertices;
    sf::Vector2u imgSz;
    sf::Vector2f offset;

  public:
    void applyRepulsion(sf::Vector2f mpos);
    void update(f32 dt);
    void draw(sf::RenderWindow &window);
    bool loadImage(const std::string &filename);
    Effect();
    ~Effect();
};

#endif // EFFECT_H