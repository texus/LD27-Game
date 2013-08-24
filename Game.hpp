#ifndef GAME_HPP
#define GAME_HPP


#include <memory>

#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>

#include "Defines.hpp"
#include "Track.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Game
{
public:

         Game();
    void run();


private:

    void mainLoop();
    void handleEvents();
    void update(float elapsedTime);
    void render();

    void loadTrackList();
    void loadTrack();

private:

    sf::RenderWindow window;
    tgui::Gui gui;

    std::vector<std::string> trackNames;
    std::vector<std::string>::iterator currentTrack = trackNames.end();

    std::unique_ptr<Track> track = nullptr;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // GAME_HPP
