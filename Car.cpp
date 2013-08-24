#include "Car.hpp"

#include <iostream>
#include <cmath>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Car::Car()
{
    if (!texture.loadFromFile("Car.png"))
        throw std::runtime_error("Failed to load 'Car.png'.");

    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Car::addVelocity(float x, float y, float elapsedTime)
{
    velocity.x += x * elapsedTime;
    velocity.y += y * elapsedTime;

    float velocityLength = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));
    if (velocityLength > 1)
        velocity /= velocityLength;

    speed += (velocity * TOP_SPEED / TIME_TO_REACH_TOP_SPEED * elapsedTime);

    float speedLength = std::sqrt(std::pow(speed.x, 2) + std::pow(speed.y, 2));
    if (speedLength > TOP_SPEED)
        speed /= (speedLength / TOP_SPEED);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Car::removeVelocity(float x, float y, float elapsedTime)
{
    if (speed.x * x > 0)
    {
        if (velocity.x * x <= 0)
            velocity.x -= x * elapsedTime;
        else
            velocity.x = 0;

        speed += velocity * TOP_SPEED / TIME_TO_REACH_TOP_SPEED * elapsedTime;

        float speedLength = std::sqrt(std::pow(speed.x, 2) + std::pow(speed.y, 2));
        if (speedLength > TOP_SPEED)
            speed /= (speedLength / TOP_SPEED);

        if (speed.x * x <= 0)
        {
            velocity.x = 0;
            speed.x = 0;
        }
    }

    if (speed.y * y > 0)
    {
        if (velocity.y * y <= 0)
            velocity.y -= y * elapsedTime;
        else
            velocity.y = 0;

        speed += (velocity * TOP_SPEED / TIME_TO_REACH_TOP_SPEED * elapsedTime);

        float speedLength = std::sqrt(std::pow(speed.x, 2) + std::pow(speed.y, 2));
        if (speedLength > TOP_SPEED)
            speed /= (speedLength / TOP_SPEED);

        if (speed.y * y <= 0)
        {
            velocity.y = 0;
            speed.y = 0;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Car::update(const sf::Image& trackArea, float elapsedTime)
{
    if (((std::abs(speed.x) > 0.000001) || (std::abs(speed.y) > 0.000001)) && ((std::abs(velocity.x) > 0.1) || (std::abs(velocity.y) > 0.1)))
        setRotation(360 - ((std::atan2(std::min(1.0f, -speed.y / TOP_SPEED), std::min(1.0f, speed.x / TOP_SPEED))) * 180 / pi));

    // If the car goes of the track then slow it down
    {
        sf::Vector2f normalizedSpeed = speed / (float)std::sqrt(std::pow(speed.x, 2) + std::pow(speed.y, 2));

        sf::Vector2f topLeft(getPosition().x + (texture.getSize().x/-2.0) * normalizedSpeed.x - (texture.getSize().y/-2.0) * normalizedSpeed.y,
                             getPosition().y + (texture.getSize().x/-2.0) * normalizedSpeed.y + (texture.getSize().y/-2.0) * normalizedSpeed.x);
        sf::Vector2f topRight(getPosition().x + (texture.getSize().x/2.0) * normalizedSpeed.x - (texture.getSize().y/2.0) * -normalizedSpeed.y,
                              getPosition().y + (texture.getSize().x/-2.0) * -normalizedSpeed.y + (texture.getSize().y/-2.0) * normalizedSpeed.x);
        sf::Vector2f bottomLeft(getPosition().x + (texture.getSize().x/-2.0) * normalizedSpeed.x - (texture.getSize().y/-2.0) * -normalizedSpeed.y,
                                getPosition().y + (texture.getSize().x/2.0) * -normalizedSpeed.y + (texture.getSize().y/2.0) * normalizedSpeed.x);
        sf::Vector2f bottomRight(getPosition().x + (texture.getSize().x/2.0) * normalizedSpeed.x - (texture.getSize().y/2.0) * normalizedSpeed.y,
                                 getPosition().y + (texture.getSize().x/2.0) * normalizedSpeed.y + (texture.getSize().y/2.0) * normalizedSpeed.x);

        if ((trackArea.getPixel(topLeft.x, topLeft.y) == sf::Color::White)
         || (trackArea.getPixel(topRight.x, topRight.y) == sf::Color::White)
         || (trackArea.getPixel(bottomLeft.x, bottomLeft.y) == sf::Color::White)
         || (trackArea.getPixel(bottomRight.x, bottomRight.y) == sf::Color::White))
        {
            if (speed.x < -0.3 * TOP_SPEED)
                speed.x = -0.3 * TOP_SPEED;
            else if (speed.x > 0.3 * TOP_SPEED)
                speed.x = 0.3 * TOP_SPEED;

            if (speed.y < -0.3 * TOP_SPEED)
                speed.y = -0.3 * TOP_SPEED;
            else if (speed.y > 0.3 * TOP_SPEED)
                speed.y = 0.3 * TOP_SPEED;
        }

    }

    setPosition(getPosition() + (speed * elapsedTime));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Car::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(sprite, states);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
