#include "Effect.hpp"
#include "Particle.hpp"
#include "ThreadPool.hpp"
#include "config.hpp"
#include <SFML/Graphics.hpp>
#include <future>
#include <iostream>
#include <thread>

ThreadPool pool(12);
// ThreadPool pool(std::thread::hardware_concurrency());

Effect::Effect() : particleVertices(sf::Points) {
    if (!loadImage(imgPaths[idx])) {
        std::cerr << "[ERR] : Impossible de charger l'image par défaut" << std::endl;
        exit(EXIT_FAILURE);
    }
}

bool Effect::loadImage(const std::string &filename) {
    sf::Image img;
    if (!img.loadFromFile(filename)) {
        std::cerr << "[ERR] : Impossible de charger l'image " << filename << std::endl;
        return false;
    }

    particles.clear();
    particleVertices.clear();
    imgSz = img.getSize();

    const u32 totalParticles = (imgSz.x / gap) * (imgSz.y / gap);
    particles.reserve(totalParticles);
    particleVertices.resize(totalParticles);

    offset = sf::Vector2f((win_width - imgSz.x) / 2.f, (win_height - imgSz.y) / 2.f);

    u32 i = 0;
    for (u32 y = 0; y < imgSz.y; y += gap) {
        for (u32 x = 0; x < imgSz.x; x += gap) {
            const sf::Color &pxCol = img.getPixel(x, y);

            if (pxCol.a > 0) {
                const sf::Vector2f position = sf::Vector2f(x, y) + offset;
                particles.emplace_back(position, sf::Vector2f(0.f, 0.f), pxCol);
                particleVertices[i].position = position;
                particleVertices[i].color = pxCol;
                ++i;
            }
        }
    }

    return true;
}
void Effect::applyRepulsion(sf::Vector2f mpos) {
    size_t particleCount = particles.size();
    size_t threadCount =
        static_cast<size_t>(std::min(static_cast<unsigned int>(particleCount), std::thread::hardware_concurrency()));
    threadCount = threadCount == 0 ? 1 : threadCount;
    size_t chunkSize = particleCount / threadCount;

    std::vector<std::future<void>> futures;

    for (size_t t = 0; t < threadCount; ++t) {
        size_t start = t * chunkSize;
        size_t end = (t == threadCount - 1) ? particleCount : start + chunkSize;
        futures.emplace_back(pool.enqueue([this, start, end, mpos]() {
            for (size_t i = start; i < end; ++i) {
                sf::Vector2f dir = particles[i].getPosition() - mpos;
                float distSquared = dir.x * dir.x + dir.y * dir.y;
                if (distSquared < (repulsion_radius * repulsion_radius * 4)) {
                    particles[i].applyRepulsion(mpos);
                }
            }
        }));
    }

    for (auto &fut : futures) {
        fut.get(); // Attendre la fin de toutes les tâches
    }
}

void Effect::update(float dt) {
    size_t particleCount = particles.size();
    size_t threadCount =
        static_cast<size_t>(std::min(static_cast<unsigned int>(particleCount), std::thread::hardware_concurrency()));
    threadCount = threadCount == 0 ? 1 : threadCount;
    size_t chunkSize = particleCount / threadCount;

    std::vector<std::future<void>> futures;

    for (size_t t = 0; t < threadCount; ++t) {
        size_t start = t * chunkSize;
        size_t end = (t == threadCount - 1) ? particleCount : start + chunkSize;
        futures.emplace_back(pool.enqueue([this, start, end, dt]() {
            for (size_t i = start; i < end; ++i) {
                particles[i].applyReturnForce();
                particles[i].update(dt);
            }
        }));
    }

    for (auto &fut : futures) {
        fut.get(); // Attendre la fin de toutes les tâches
    }

    for (size_t i = 0; i < particleCount; ++i) {
        particleVertices[i].position = particles[i].getPosition();
    }
}

void updateParticles(std::vector<Particle> &particles, size_t start, size_t end, float dt) {
    for (size_t i = start; i < end; ++i) {
        particles[i].applyReturnForce();
        particles[i].update(dt);
    }
}

void Effect::draw(sf::RenderWindow &window) {
    window.draw(particleVertices);
}

Effect::~Effect() {
}
