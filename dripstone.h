#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <algorithm>//useful to access some inbuilt stuff

class Dripstone
{
public:
    sf::ConvexShape shape;
    sf::Vector2f velocity;
};

class DripstoneManager
{
private:
    std::vector<Dripstone> dripstones;

    float spawnTimer = 0.f;
    float spawnInterval = 1.2f;
    float fallSpeed = 400.f;

public:

    void update(float dt)
    {
        spawnTimer += dt;

        if (spawnTimer >= spawnInterval)
        {
            spawnTimer = 0.f;

            Dripstone d;

            d.shape.setPointCount(3);

            d.shape.setPoint(0, { 0.f,0.f });
            d.shape.setPoint(1, { 40.f,0.f });
            d.shape.setPoint(2, { 20.f,70.f });

            d.shape.setFillColor(sf::Color(90, 90, 90,100));

            float x = static_cast<float>(rand() % 760);

            d.shape.setPosition({ x,-70.f });

            d.velocity = { 0.f, fallSpeed };

            dripstones.push_back(d);
        }

        for (auto& d : dripstones)
        {
            d.shape.move(d.velocity * dt);
        }

        dripstones.erase(
            std::remove_if(
                dripstones.begin(),
                dripstones.end(),
                [](const Dripstone& d)
                {
                    return d.shape.getPosition().y > 500.f;
                }),
            dripstones.end());
    }

    void draw(sf::RenderWindow& window)
    {
        for (auto& d : dripstones)
            window.draw(d.shape);
    }

    bool checkCollision(const sf::FloatRect& playerBounds)
    {
        for (auto& d : dripstones)
        {
            if (playerBounds.findIntersection(d.shape.getGlobalBounds()))
                return true;
        }

        return false;
    }

    void reset()
    {
        dripstones.clear();
        spawnTimer = 0.f;
    }
};

