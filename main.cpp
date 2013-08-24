#include "Game.hpp"

int main()
{
    Game game;
    game.run();
/*
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Racer", sf::Style::Titlebar | sf::Style::Close);

    sf::Image        trackArea;
    thor::BigTexture trackTexture;
    thor::BigSprite  trackSprite;

    trackArea.loadFromFile("Track1_Area.png");
    trackTexture.loadFromFile("Track1.png");
    trackSprite.setTexture(trackTexture);

    window.setView(sf::View(sf::FloatRect(1700, 1000, SCREEN_WIDTH, SCREEN_HEIGHT)));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(trackSprite);
        window.display();
    }
*/
}
