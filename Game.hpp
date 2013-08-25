#ifndef GAME_HPP
#define GAME_HPP


#include <memory>

#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Defines.hpp"
#include "Track.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Game
{
public:

         Game();
    void runMenuScreen();


private:

    void runLevelSelection();
    void runGame();
    void runGameSuccess();
    void runGameFailed();

    void mainLoop();
    void handleEvents();
    void update(float elapsedTime);
    void render();

    void loadTrackList();
    void loadTrack();

    void playMusic(sf::Music& music);


private:

    sf::RenderWindow window;
    tgui::Gui gui;

    sf::Music beep;
    sf::Music menu;
    sf::Music track1;
    sf::Music desert;
    sf::Music track3;

    std::vector<std::string> trackNames;
    std::vector<std::string>::iterator currentTrack = trackNames.end();

    std::unique_ptr<Track> track = nullptr;

    float countdown = 10;
    int roundedCountdown = 10;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // GAME_HPP
