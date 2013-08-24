#include "Track.hpp"
#include <fstream>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Track::Track(const std::string& name, sf::RenderWindow& window) :
    window     (window)
{
    // Load images
    {
        if (!trackArea.loadFromFile(name + "_Area.png"))
        throw std::runtime_error("Failed to load '" + name + "_Area.png'.");

        if (!trackTexture.loadFromFile(name + ".png"))
            throw std::runtime_error("Failed to load '" + name + ".png'.");

        trackSprite.setTexture(trackTexture);
    }

    // Load track file
    {
        std::ifstream file(name + ".dat");
        if (!file.is_open())
            throw std::runtime_error("Failed to open '" + name + ".dat'.");

        std::string carRotation;
        file >> carRotation;
        if (!carRotation.empty())
        {
            car.setRotation(std::stoi(carRotation));

            while (!file.eof())
            {
                std::string radius, posX, posY;

                file >> posX;
                file >> posY;
                file >> radius;

                if (radius.empty() || posX.empty() || posY.empty())
                    continue;

                Checkpoint checkpoint;
                checkpoint.radius = std::stoi(radius);
                checkpoint.position.x = std::stoi(posX);
                checkpoint.position.y = std::stoi(posY);
                checkpoints.push_back(std::move(checkpoint));
            }
        }

        if (checkpoints.empty())
            throw std::runtime_error("No checkpoints were loaded from '" + name + ".dat'.");
    }

    car.setPosition(sf::Vector2f(checkpoints[0].position));
    window.setView(sf::View(sf::FloatRect(car.getPosition().x - (SCREEN_WIDTH / 2.0), car.getPosition().y - (SCREEN_HEIGHT / 2.0), SCREEN_WIDTH, SCREEN_HEIGHT)));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Track::update(float elapsedTime)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        car.addVelocity(-1.0 / TIME_TO_REACH_MAX_VELOCITY, 0, elapsedTime);
    else
        car.removeVelocity(-3.0 / TIME_TO_REACH_MAX_VELOCITY, 0, elapsedTime);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        car.addVelocity(0, -1.0 / TIME_TO_REACH_MAX_VELOCITY, elapsedTime);
    else
        car.removeVelocity(0, -3.0 / TIME_TO_REACH_MAX_VELOCITY, elapsedTime);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        car.addVelocity(1.0 / TIME_TO_REACH_MAX_VELOCITY, 0, elapsedTime);
    else
        car.removeVelocity(3.0 / TIME_TO_REACH_MAX_VELOCITY, 0, elapsedTime);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        car.addVelocity(0, 1.0 / TIME_TO_REACH_MAX_VELOCITY, elapsedTime);
    else
        car.removeVelocity(0, 3.0 / TIME_TO_REACH_MAX_VELOCITY, elapsedTime);

    car.update(trackArea, elapsedTime);
    window.setView(sf::View(sf::FloatRect(car.getPosition().x - (SCREEN_WIDTH / 2.0), car.getPosition().y - (SCREEN_HEIGHT / 2.0), SCREEN_WIDTH, SCREEN_HEIGHT)));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Track::draw() const
{
    window.draw(trackSprite);

    car.draw(window, sf::RenderStates());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
