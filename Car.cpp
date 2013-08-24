#include "Car.hpp"

#include <iostream>
#include <cmath>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Car::Car()
{
    if (!texture.loadFromFile("Car.png"))
        throw std::runtime_error("Failed to load 'Car.png'.");

    sprite.setTexture(texture);
//    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);

    setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Car::update(const sf::Image& trackArea, float elapsedTime)
{
    // Update velocity
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            velocity.x -= (elapsedTime / TIME_TO_REACH_MAX_VELOCITY) * (-std::cos((360 - getRotation()) / 180 * pi));
            velocity.y -= (elapsedTime / TIME_TO_REACH_MAX_VELOCITY) * (std::sin((360 - getRotation()) / 180 * pi));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            velocity.x += (elapsedTime / TIME_TO_REACH_MAX_VELOCITY) * (-std::cos((360 - getRotation()) / 180 * pi));
            velocity.y += (elapsedTime / TIME_TO_REACH_MAX_VELOCITY) * (std::sin((360 - getRotation()) / 180 * pi));
        }

        if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
        {
            sf::Vector2f oldVelocity = velocity;
            sf::Vector2f oldSpeed = speed;

            if (velocity.x != 0)
            {
                if (velocity.x < 0)
                    velocity.x += std::abs((elapsedTime / TIME_TO_REACH_MAX_VELOCITY) * (std::cos((360 - getRotation()) / 180 * pi)));
                else
                    velocity.x -= std::abs((elapsedTime / TIME_TO_REACH_MAX_VELOCITY) * (std::cos((360 - getRotation()) / 180 * pi)));

                if (oldVelocity.x * velocity.x < 0)
                    velocity.x = 0;
            }
            else
            {
                speed.x -= (10.0 * speed.x / TOP_SPEED) * TOP_SPEED / TIME_TO_REACH_TOP_SPEED * elapsedTime;
                if (oldSpeed.x * speed.x < 0)
                    speed.x = 0;
            }

            if (velocity.y != 0)
            {
                if (velocity.y < 0)
                    velocity.y += std::abs((elapsedTime / TIME_TO_REACH_MAX_VELOCITY) * (std::sin((360 - getRotation()) / 180 * pi)));
                else
                    velocity.y -= std::abs((elapsedTime / TIME_TO_REACH_MAX_VELOCITY) * (std::sin((360 - getRotation()) / 180 * pi)));

                if (oldVelocity.y * velocity.y < 0)
                    velocity.y = 0;
            }
            else
            {
                speed.y -= (10.0 * speed.y / TOP_SPEED) * TOP_SPEED / TIME_TO_REACH_TOP_SPEED * elapsedTime;
                if (oldSpeed.y * speed.y < 0)
                    speed.y = 0;
            }
        }
    }

    float velocityLength = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));
    if (velocityLength > 1)
        velocity /= velocityLength;

    // Rotate the car
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            float cos = std::cos((360 - getRotation()) / 180 * pi);
            float sin = std::sin((360 - getRotation()) / 180 * pi);

            setRotation(getRotation() - (90 * elapsedTime));
            velocity.x *= std::abs(cos);
            velocity.y *= std::abs(sin);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            float cos = std::cos((360 - getRotation()) / 180 * pi);
            float sin = std::sin((360 - getRotation()) / 180 * pi);

            setRotation(getRotation() + (90 * elapsedTime));
            velocity.x *= std::abs(cos);
            velocity.y *= std::abs(sin);
        }
    }

    {
        sf::Vector2f oldSpeed = speed;

        if (velocity.x != 0)
        {
            if ((speed.x / (velocity.x * TOP_SPEED / TIME_TO_REACH_TOP_SPEED * elapsedTime)) > 200)
            {
                speed.x -= (3.0 * speed.x / TOP_SPEED) * TOP_SPEED / TIME_TO_REACH_TOP_SPEED * elapsedTime;
                if (oldSpeed.x * speed.x < 0)
                    speed.x = 0;
            }
        }
        if (velocity.y != 0)
        {
            if ((speed.y / (velocity.y * TOP_SPEED / TIME_TO_REACH_TOP_SPEED * elapsedTime)) > 200)
            {
                speed.y -= (3.0 * speed.y / TOP_SPEED) * TOP_SPEED / TIME_TO_REACH_TOP_SPEED * elapsedTime;
                if (oldSpeed.y * speed.y < 0)
                    speed.y = 0;
            }
        }
    }

    speed += velocity * TOP_SPEED / TIME_TO_REACH_TOP_SPEED * elapsedTime;

    float speedLength = std::sqrt(std::pow(speed.x, 2) + std::pow(speed.y, 2));
    if (speedLength > TOP_SPEED)
        speed /= (speedLength / TOP_SPEED);

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
            if (speed.x < -0.2 * TOP_SPEED)
                speed.x = -0.2 * TOP_SPEED;
            else if (speed.x > 0.2 * TOP_SPEED)
                speed.x = 0.2 * TOP_SPEED;

            if (speed.y < -0.2 * TOP_SPEED)
                speed.y = -0.2 * TOP_SPEED;
            else if (speed.y > 0.2 * TOP_SPEED)
                speed.y = 0.2 * TOP_SPEED;
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
