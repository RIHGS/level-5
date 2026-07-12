// main.cpp - Entry point for the platformer demo
// Sets up the game window, player, orb, fireballs, dripstones, and monk.
// Handles input, physics, collision, and rendering each frame.

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <optional>
#include<iostream>
#include "dripstone.h"
#include"Monk.hpp"

class Fireball
{
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
};

int main()
{

    srand(static_cast<unsigned>(time(nullptr)));
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Platform - Orb Fireball Demo");
    window.setFramerateLimit(60);

    sf::RectangleShape ground({ 800.f, 50.f });
    ground.setPosition({ 0.f, 550.f });
    ground.setFillColor(sf::Color(100, 100, 100, 50));

    sf::RectangleShape player({ 40.f, 60.f });
    player.setFillColor(sf::Color::Green);
    player.setPosition({ 380.f, 490.f });

    float playerSpeed = 300.f;
    float velocityY = 0.f;
    float gravity = 900.f;
    float jumpStrength = -450.f;
    bool onGround = true;

    sf::CircleShape orb(20.f);
    orb.setFillColor(sf::Color::Yellow);
    orb.setOrigin({ 20.f, 20.f });
    orb.setPosition({ 400.f, 100.f });

    std::vector<Fireball> fireballs;
    float fireballSpeed = 250.f;
    float spawnTimer = 0.f;
    float spawnInterval = 0.1f;

    DripstoneManager dm;
    Monk monk({ 600.f, 200.f });
    bool gameOver = false;
    sf::Font font;
    bool fontLoaded = font.openFromFile("Data/Roboto-Medium.ttf");

    sf::Text gameOverText(font);

    if (fontLoaded)
    {
        gameOverText.setString("GAME OVER - Press R to Restart");
        gameOverText.setCharacterSize(36);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition({ 150.f, 50.f });
    }
    sf::Clock clock;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        if (!gameOver)
        {
            sf::Vector2f pos = player.getPosition();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
                pos.x -= playerSpeed * dt;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
                pos.x += playerSpeed * dt;
            }
            if (pos.x < 0)
                pos.x = 0;

            if (pos.x > 760)
                pos.x = 760;
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) &&
                onGround)
            {
                velocityY = jumpStrength;
                onGround = false;
            }
            velocityY += gravity * dt;
            pos.y += velocityY * dt;

            if (pos.y >= 490)
            {
                pos.y = 490;
                velocityY = 0;
                onGround = true;
            }
            player.setPosition(pos);

            spawnTimer += dt;

            if (spawnTimer >= spawnInterval)
            {
                spawnTimer = 0.f;

                Fireball fb;

                fb.shape.setRadius(8.f);
                fb.shape.setOrigin({ 8.f, 8.f });
                fb.shape.setFillColor(sf::Color(255, 80, 0));
                fb.shape.setPosition(orb.getPosition());

                float angle =
                    static_cast<float>(rand() % 360) *
                    3.14159265f / 180.f;

                fb.velocity =
                {
                    std::cos(angle) * fireballSpeed,
                    std::sin(angle) * fireballSpeed
                };

                fireballs.push_back(fb);
            }
            for (auto& fb : fireballs)
            {
                fb.shape.move(fb.velocity * dt);
            }

            fireballs.erase(
                std::remove_if(
                    fireballs.begin(),
                    fireballs.end(),
                    [](const Fireball& fb)
                    {
                        sf::Vector2f p = fb.shape.getPosition();

                        return p.x < -50 ||
                            p.x > 850 ||
                            p.y < -50 ||
                            p.y > 550.f;
                    }),
                fireballs.end());

            dm.update(dt);
            monk.update(dt, player.getPosition());
            auto playerBounds = player.getGlobalBounds();

            for (auto& fb : fireballs)
            {
                if (playerBounds.findIntersection(fb.shape.getGlobalBounds()))
                {
                    gameOver = true;
                    break;
                }
                if (dm.checkCollision(playerBounds))
                {
                    gameOver = true;
                }
                if (monk.checkCollision(playerBounds))
                {
                    gameOver = true;
                }
            }

        }
        else
        {

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
            {
                gameOver = false;

                player.setPosition({ 380.f, 490.f });

                velocityY = 0.f;
                onGround = true;

                fireballs.clear();

                spawnTimer = 0.f;
                dm.reset();
                monk.reset();
            }
        }


        window.clear(sf::Color(30, 30, 40));

        window.draw(ground);
        window.draw(orb);
        window.draw(player);
        dm.draw(window);
        monk.draw(window);

        for (auto& fb : fireballs)
        {
            window.draw(fb.shape);
        }

        if (gameOver && fontLoaded)
        {
            window.draw(gameOverText);
        }

        window.display();

    }

    return 0;
}