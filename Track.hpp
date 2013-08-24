#ifndef TRACK_HPP
#define TRACK_HPP


#include <Thor/Graphics.hpp>
#include <SFML/Graphics.hpp>

#include "Defines.hpp"
#include "Checkpoint.hpp"
#include "Car.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Track
{
public:

    Track(const std::string& name, sf::RenderWindow& window);

    void update(float elapsedTime);
    void draw() const;


private:

    sf::RenderWindow& window;

    sf::Image         trackArea;
    thor::BigTexture  trackTexture;
    thor::BigSprite   trackSprite;

    std::vector<Checkpoint> checkpoints;
    std::vector<Checkpoint>::iterator nextCheckpoint = checkpoints.end();

    Car car;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TRACK_HPP

