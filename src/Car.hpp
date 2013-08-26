#ifndef CAR_HPP
#define CAR_HPP


#include <SFML/Graphics.hpp>

#include "Defines.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Car : public sf::Drawable, public sf::Transformable
{
public:

    Car(float topSpeed, float timeToReachTopSpeed, const std::string& imageFilename);

    float getTopSpeed() const;
    float getTimeToReachTopSpeed() const;

    void reset();

    void update(const sf::Image& trackArea, float elapsedTime);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:

    sf::Texture  texture;
    sf::Sprite   sprite;

    sf::Vector2f speed;
    sf::Vector2f velocity;

    float topSpeed;
    float timeToReachTopSpeed;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // CAR_HPP


