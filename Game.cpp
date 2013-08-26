#include "Game.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Game::Game() :
    window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Racer", sf::Style::Titlebar | sf::Style::Close),
    gui(window)
{
    window.setFramerateLimit(60);

    if (gui.setGlobalFont("gui/DejaVuSans.ttf") == false)
        throw std::runtime_error("Can't load font. Files are missing.");

    if ((!beep.openFromFile("beep.wav"))
     || (!menu.openFromFile("DST-MapLands2.wav"))
     || (!track1.openFromFile("DST-Blam.wav"))
     || (!desert.openFromFile("DST-Desert.wav"))
     || (!track3.openFromFile("DST-TheMusic.wav")))
        throw std::runtime_error("Can't load sound. Files are missing.");

    cars.emplace(cars.end(), 260, 1.0, "Car1.png");
    cars.emplace(cars.end(), 375, 2.25, "Car2.png");
    cars.emplace(cars.end(), 500, 3, "Car3.png");
    currentCar = cars.begin();

    loadTrackList();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::runMenuScreen()
{
    playMusic(menu);

    tgui::Picture::Ptr background(gui);
    background->load("MenuBackground.png");

    tgui::Grid::Ptr grid(gui);
    grid->setSize(800, 600);

    tgui::Label::Ptr label(*grid);
    label->load("gui/Black.conf");
    label->setText("RACER");
    label->setTextSize(100);

    tgui::Button::Ptr playButton(*grid);
    playButton->load("gui/Black.conf");
    playButton->setText("Play");
    playButton->setSize(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 8);
    playButton->bindCallback([this](){ gui.removeAllWidgets(); runLevelSelection(); }, tgui::Button::LeftMouseClicked);

    tgui::Button::Ptr selectCarButton(*grid);
    selectCarButton->load("gui/Black.conf");
    selectCarButton->setText("Select car");
    selectCarButton->setSize(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 8);
    selectCarButton->bindCallback([this](){ gui.removeAllWidgets(); runCarSelection(); }, tgui::Button::LeftMouseClicked);

    tgui::Button::Ptr exitButton(*grid);
    exitButton->load("gui/Black.conf");
    exitButton->setText("Exit");
    exitButton->setSize(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 8);
    exitButton->bindCallback([this](){ window.close(); }, tgui::Button::LeftMouseClicked);

    grid->addWidget(label, 0, 0);
    grid->addWidget(playButton, 1, 0);
    grid->addWidget(selectCarButton, 2, 0);
    grid->addWidget(exitButton, 3, 0);

    if ((!playButton->isLoaded()) || (!exitButton->isLoaded()))
        throw std::runtime_error("Failed to load the menu screen. Files are missing.");

    while (window.isOpen())
    {
        handleEvents();

        window.clear();
        gui.draw();
        window.display();

        sf::sleep(sf::milliseconds(1));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::runCarSelection()
{
    tgui::Picture::Ptr background(gui);
    background->load("MenuBackground.png");

    tgui::Grid::Ptr grid(gui);

    auto it = cars.begin();
    for (unsigned int i = 0; i < cars.size(); ++i, ++it)
    {
        tgui::Picture::Ptr preview(*grid);
        preview->load("PreviewCar" + tgui::to_string(i+1) + ".png");
        preview->setSize(SCREEN_WIDTH / (cars.size()+1), SCREEN_WIDTH / (cars.size()+1));

        tgui::LoadingBar::Ptr speed(*grid);
        speed->load("gui/Black.conf");
        speed->setText("Speed");
        speed->setTextColor(sf::Color(255, 255, 0, 150));
        speed->setMaximum(MAX_SPEED);
        speed->setValue((int)it->getTopSpeed());
        speed->setSize(SCREEN_WIDTH / (cars.size()+1), SCREEN_WIDTH / (cars.size()+1) / 8);

        tgui::LoadingBar::Ptr control(*grid);
        control->load("gui/Black.conf");
        control->setText("Control");
        control->setTextColor(sf::Color(255, 255, 0, 150));
        control->setMaximum(MAX_CONTROL);
        control->setValue(MAX_CONTROL - (int)it->getTimeToReachTopSpeed());
        control->setSize(SCREEN_WIDTH / (cars.size()+1), SCREEN_WIDTH / (cars.size()+1) / 8);

        tgui::Button::Ptr select(*grid);
        select->load("gui/Black.conf");
        select->setText("Car " + tgui::to_string(i+1));
        select->setSize(SCREEN_WIDTH / (cars.size()+1), SCREEN_WIDTH / (cars.size()+1) / 3);
        select->bindCallback([this, it](){ currentCar = it; gui.removeAllWidgets(); runMenuScreen(); }, tgui::Button::LeftMouseClicked);

        grid->addWidget(preview, 0, i);
        grid->addWidget(speed, 1, i, tgui::Borders(0, 10, 0, 10));
        grid->addWidget(control, 2, i, tgui::Borders(0, 10, 0, 10));
        grid->addWidget(select, 3, i, tgui::Borders(0, 50, 0, 0));
    }

    grid->setSize(SCREEN_WIDTH, grid->getSize().y);
    grid->setPosition((SCREEN_WIDTH - grid->getSize().x) / 2.0f, (SCREEN_HEIGHT - grid->getSize().y) / 2.0f);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::runLevelSelection()
{
    tgui::Picture::Ptr background(gui);
    background->load("MenuBackground.png");

    tgui::Grid::Ptr grid(gui);
    grid->setSize(800, 600);

    tgui::Label::Ptr label(*grid);
    label->load("gui/Black.conf");
    label->setText("RACER");
    label->setTextSize(100);
    grid->addWidget(label, 0, 0);

    for (unsigned int i = 0; i < trackNames.size(); ++i)
    {
        tgui::Button::Ptr button(*grid);
        button->load("gui/Black.conf");
        button->setSize(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 10);
        button->setText("Level " + tgui::to_string(i+1));
        button->bindCallback([this, i](){ gui.removeAllWidgets(); currentTrack = trackNames.begin() + i; runGame(); }, tgui::Button::LeftMouseClicked);

        grid->addWidget(button, i+1, 0);
    }

    while (window.isOpen())
    {
        handleEvents();

        window.clear();
        gui.draw();
        window.display();

        sf::sleep(sf::milliseconds(1));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::runGame()
{
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

void Game::runGameSuccess()
{
    tgui::Picture::Ptr background(gui);
    background->load("MenuBackground.png");

    tgui::Grid::Ptr grid(gui);
    grid->setSize(800, 600);

    tgui::Label::Ptr label(*grid);
    label->load("gui/Black.conf");
    label->setText("RACER");
    label->setTextSize(100);

    tgui::Label::Ptr congrats(*grid);
    congrats->load("gui/Black.conf");
    congrats->setText("Level complete!");
    congrats->setTextSize(50);

    tgui::Button::Ptr backButton(*grid);
    backButton->load("gui/Black.conf");
    backButton->setSize(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 8);
    backButton->setText("Back to menu");
    backButton->bindCallback([this](){ gui.removeAllWidgets(); runMenuScreen(); }, tgui::Button::LeftMouseClicked);

    grid->addWidget(label, 0, 0);
    grid->addWidget(congrats, 1, 0);
    grid->addWidget(backButton, 2, 0);

    while (window.isOpen())
    {
        handleEvents();

        window.clear();
        gui.draw();
        window.display();

        sf::sleep(sf::milliseconds(1));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::runGameFailed()
{
    tgui::Picture::Ptr background(gui);
    background->load("MenuBackground.png");

    tgui::Grid::Ptr grid(gui);
    grid->setSize(800, 600);

    tgui::Label::Ptr label(*grid);
    label->load("gui/Black.conf");
    label->setText("RACER");
    label->setTextSize(100);

    tgui::Label::Ptr congrats(*grid);
    congrats->load("gui/Black.conf");
    congrats->setText("Level failed!");
    congrats->setTextSize(50);

    tgui::Button::Ptr replayButton(*grid);
    replayButton->load("gui/Black.conf");
    replayButton->setSize(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 8);
    replayButton->setText("Retry level");
    replayButton->bindCallback([this](){ gui.removeAllWidgets(); runGame(); }, tgui::Button::LeftMouseClicked);

    tgui::Button::Ptr backButton(*grid);
    backButton->load("gui/Black.conf");
    backButton->setSize(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 8);
    backButton->setText("Back to menu");
    backButton->bindCallback([this](){ gui.removeAllWidgets(); runMenuScreen(); }, tgui::Button::LeftMouseClicked);

    grid->addWidget(label, 0, 0);
    grid->addWidget(congrats, 1, 0);
    grid->addWidget(replayButton, 2, 0);
    grid->addWidget(backButton, 3, 0);

    while (window.isOpen())
    {
        handleEvents();

        window.clear();
        gui.draw();
        window.display();

        sf::sleep(sf::milliseconds(1));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::mainLoop()
{
    sf::Clock clock;

    while (window.isOpen())
    {
        handleEvents();
        update(clock.restart().asSeconds());
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

        gui.handleEvent(event);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::update(float elapsedTime)
{
    int ret = track->update(elapsedTime);

    if (ret == 1)
    {
        // Checkpoint reached
        countdown = 10;
        return;
    }
    else if (ret == 2)
    {
        window.setView(window.getDefaultView());
        runGameSuccess();
        return;
    }

    countdown -= elapsedTime;

    if (countdown <= 0)
    {
        window.setView(window.getDefaultView());
        runGameFailed();
        return;
    }

    if (static_cast<int>(countdown + 0.5) != roundedCountdown)
    {
        roundedCountdown = static_cast<int>(countdown + 0.5);
        beep.setVolume(60 - std::min(roundedCountdown * 10, 55));
        beep.play();
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::render()
{
    window.clear();

    track->draw();

    sf::Text text(tgui::to_string(roundedCountdown), gui.getGlobalFont(), 30);
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
        throw std::runtime_error("Trying to load an unexisting track.");

    if (*currentTrack == "Track1")
        playMusic(track1);
    else if (*currentTrack == "Track2")
        playMusic(desert);
    else if (*currentTrack == "Track3")
        playMusic(track3);

    track = std::unique_ptr<Track>(new Track(*currentTrack, window, *currentCar));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::playMusic(sf::Music& music)
{
    menu.stop();
    track1.stop();
    desert.stop();
    track3.stop();

    music.play();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
