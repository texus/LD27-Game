#ifndef CAR_HPP
#define CAR_HPP


#include <SFML/Graphics.hpp>

#include "Defines.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Car : public sf::Drawable, public sf::Transformable
{
public:

    Car();

    void update(const sf::Image& trackArea, float elapsedTime);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:

    sf::Texture  texture;
    sf::Sprite   sprite;

    sf::Vector2f speed;
    sf::Vector2f velocity;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // CAR_HPP


