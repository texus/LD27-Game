#include "Game.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Game::Game() :
    window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Racer", sf::Style::Titlebar | sf::Style::Close),
    gui(window)
{
    if (gui.setGlobalFont("gui/DejaVuSans.ttf") == false)
        throw std::runtime_error("Exception while loading font. Files are missing.");

    beep.openFromFile("beep.wav");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::run()
{
    loadTrackList();
    loadTrack();

    try
    {
        mainLoop();
    }
    catch (std::exception& e)
    {
        gui.removeAllWidgets();

        tgui::MessageBox::Ptr message(gui);

        if (message->load("gui/Black.conf") == false)
            throw std::runtime_error("Exception while handling error. Files are missing.");

        message->setTitle("Error");
        message->setText(e.what());
        message->addButton("Ok");
        message->setPosition((SCREEN_WIDTH - message->getSize().x) / 2, (SCREEN_HEIGHT - message->getSize().y) / 2);
        message->bindCallback([this](){ window.close(); }, tgui::MessageBox::Closed | tgui::MessageBox::ButtonClicked);

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();

                gui.handleEvent(event);
            }

            window.clear();
            gui.draw();
            window.display();

            sf::sleep(sf::milliseconds(1));
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::mainLoop()
{
    sf::Clock clock;

    while (window.isOpen())
    {
        handleEvents();

        int ret = update(clock.restart().asSeconds());
        if (ret == 1)
        {
            window.close();
            std::cout << "Level completed" << std::endl;
        }
        else if (ret == 2)
        {
            window.close();
            std::cout << "Level FAILED" << std::endl;
        }

        render();

        sf::sleep(sf::milliseconds(1));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Game::update(float elapsedTime)
{
    int ret = track->update(elapsedTime);

    if (ret == 1)
    {
        // Checkpoint reached
        countdown = 10;
        return 0;
    }
    else if (ret == 2)
    {
        // Level finished
        return 1;
    }

    countdown -= elapsedTime;

    if (countdown <= 0)
        return 2;

    if (static_cast<int>(countdown + 0.5) != roundedCountdown)
    {
        roundedCountdown = static_cast<int>(countdown + 0.5);
        beep.setVolume(100 - roundedCountdown * 10);
        beep.play();
    }

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::render()
{
    window.clear();

    track->draw();

    sf::Text text(std::to_string(roundedCountdown), gui.getGlobalFont(), 30);
    text.setColor(sf::Color(255 - ((countdown / 10.0) * 255), (countdown / 10.0) * 255, 0));
    text.setPosition(window.getView().getCenter() - (window.getView().getSize() / 2.0f));
    window.draw(text);

    gui.draw();

    window.display();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::loadTrackList()
{
    std::ifstream file("TrackList.dat");
    if (!file.is_open())
        throw std::runtime_error("Failed to open 'TrackList.dat'.");

    while (!file.eof())
    {
        std::string word;
        file >> word;

        if (!word.empty())
            trackNames.push_back(word);
    }

    if (trackNames.empty())
        throw std::runtime_error("No tracks were loaded from 'TrackList.dat'. Is this file empty?");

    currentTrack = trackNames.begin();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::loadTrack()
{
    if (currentTrack == trackNames.end())
        throw std::runtime_error("Trying to load the next track when there are no more tracks.");

    track = std::unique_ptr<Track>(new Track(*currentTrack, window));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
