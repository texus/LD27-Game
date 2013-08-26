#include "Track.hpp"
#include <fstream>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Track::Track(const std::string& name, sf::RenderWindow& window, Car& car) :
    window(window),
    car   (car)
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
            car.setRotation(std::atoi(carRotation.c_str()));

            while (!file.eof())
            {
                std::string radius, posX, posY;

                file >> posX;
                file >> posY;
                file >> radius;

                if (radius.empty() || posX.empty() || posY.empty())
                    continue;

                Checkpoint checkpoint;
                checkpoint.radius = std::atoi(radius.c_str());
                checkpoint.position.x = std::atoi(posX.c_str());
                checkpoint.position.y = std::atoi(posY.c_str());
                checkpoints.push_back(std::move(checkpoint));
            }
        }

        if (checkpoints.size() < 2)
            throw std::runtime_error("No checkpoints were loaded from '" + name + ".dat'.");

        nextCheckpoint = checkpoints.begin();
    }

    car.reset();
    car.setPosition(sf::Vector2f(checkpoints[0].position));
    window.setView(sf::View(sf::FloatRect(car.getPosition().x - (SCREEN_WIDTH / 2.0), car.getPosition().y - (SCREEN_HEIGHT / 2.0), SCREEN_WIDTH, SCREEN_HEIGHT)));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Track::update(float elapsedTime)
{
    car.update(trackArea, elapsedTime);
    window.setView(sf::View(sf::FloatRect(car.getPosition().x - (SCREEN_WIDTH / 2.0), car.getPosition().y - (SCREEN_HEIGHT / 2.0), SCREEN_WIDTH, SCREEN_HEIGHT)));

    sf::Vector2f checkpointPosition(nextCheckpoint->position.x, nextCheckpoint->position.y);
    sf::Vector2f carPosition = car.getPosition();

    if (std::sqrt(std::pow(checkpointPosition.x - carPosition.x, 2) + std::pow(checkpointPosition.y - carPosition.y, 2)) < nextCheckpoint->radius)
    {
        ++nextCheckpoint;
        if (nextCheckpoint == checkpoints.end())
            return 2;
        else
            return 1;
    }

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Track::draw() const
{
    window.draw(trackSprite);

    sf::CircleShape checkpoint(nextCheckpoint->radius);
    checkpoint.setFillColor(sf::Color(255, 255, 0, 150));
    checkpoint.setPosition(nextCheckpoint->position.x - nextCheckpoint->radius, nextCheckpoint->position.y - nextCheckpoint->radius);
    window.draw(checkpoint);

    car.draw(window, sf::RenderStates());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
