#include "Game.h"

// Constructor and destructor
Game::Game() : gameEnded(false) {
    this->initVariables();
    this->initWindow();
    this->initTextures();
    this->initSprites();
    this->initText();
}

Game::~Game() {
    delete this->window;
}

// Private methods
void Game::initVariables() {
    this->window = nullptr;
    this->videoMode.height = 800;
    this->videoMode.width = 1000; // Changed to provide more space
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void Game::initWindow() {
    this->window = new sf::RenderWindow(this->videoMode, "Rock Paper Scissors", sf::Style::Titlebar | sf::Style::Close);
}

void Game::initTextures() {
    // Load textures
    if (!this->scissorTexture.loadFromFile("scissor.png")) {
        printf("Cann't Read scissors png\n");
        return; 
    }
    if (!this->rockTexture.loadFromFile("rock.png")) {
        printf("Cann't Read rock png\n");
        return;       
    }
    if (!this->paperTexture.loadFromFile("paper.png")) {
        printf("Cann't Read paper png\n");
        return;     
    }
}

void Game::initSprites() {
    for (int i = 0; i < 6; ++i) {
        initScissor(sf::Vector2f(std::rand() % (this->videoMode.width - 50), std::rand() % (this->videoMode.height - 50)));
        initRock(sf::Vector2f(std::rand() % (this->videoMode.width - 50), std::rand() % (this->videoMode.height - 50)));
        initPaper(sf::Vector2f(std::rand() % (this->videoMode.width - 50), std::rand() % (this->videoMode.height - 50)));
    }
}

void Game::initText() {
    if (!this->font.loadFromFile("arial.ttf")) {
        printf("Can't load arial font\n");
        return;
    }
    this->endText.setFont(this->font);
    this->endText.setCharacterSize(50);
    this->endText.setFillColor(sf::Color::Red);
    this->endText.setStyle(sf::Text::Bold);
    this->endText.setPosition(this->videoMode.width / 2 - 150, this->videoMode.height / 2 - 25);
}

void Game::moveObjects() {
    if (this->gameEnded) return;

    // Move all objects and handle boundary collisions
    for (auto& pair : this->scissors) {
        sf::Sprite& scissor = pair.first;
        sf::Vector2f& velocity = pair.second;
        scissor.move(velocity);

        // Boundary collision for scissors
        if (scissor.getPosition().x <= 0 || scissor.getPosition().x + scissor.getGlobalBounds().width >= this->videoMode.width) {
            velocity.x = -velocity.x;
        }
        if (scissor.getPosition().y <= 0 || scissor.getPosition().y + scissor.getGlobalBounds().height >= this->videoMode.height) {
            velocity.y = -velocity.y;
        }
    }

    for (auto& pair : this->rocks) {
        sf::Sprite& rock = pair.first;
        sf::Vector2f& velocity = pair.second;
        rock.move(velocity);

        // Boundary collision for rocks
        if (rock.getPosition().x <= 0 || rock.getPosition().x + rock.getGlobalBounds().width >= this->videoMode.width) {
            velocity.x = -velocity.x;
        }
        if (rock.getPosition().y <= 0 || rock.getPosition().y + rock.getGlobalBounds().height >= this->videoMode.height) {
            velocity.y = -velocity.y;
        }
    }

    for (auto& pair : this->papers) {
        sf::Sprite& paper = pair.first;
        sf::Vector2f& velocity = pair.second;
        paper.move(velocity);

        // Boundary collision for papers
        if (paper.getPosition().x <= 0 || paper.getPosition().x + paper.getGlobalBounds().width >= this->videoMode.width) {
            velocity.x = -velocity.x;
        }
        if (paper.getPosition().y <= 0 || paper.getPosition().y + paper.getGlobalBounds().height >= this->videoMode.height) {
            velocity.y = -velocity.y;
        }
    }
}

void Game::handleObjectCollisions() {
    if (this->gameEnded) return;

    for (size_t i = 0; i < this->scissors.size(); ++i) {
        for (size_t j = 0; j < this->rocks.size(); ++j) {
            if (this->scissors[i].first.getGlobalBounds().intersects(this->rocks[j].first.getGlobalBounds())) {
                // Scissor to Rock collision: Remove scissor, add rock
                this->scissors.erase(this->scissors.begin() + i);
                initRock(this->rocks[j].first.getPosition());
                return;
            }
        }
        for (size_t j = 0; j < this->papers.size(); ++j) {
            if (this->scissors[i].first.getGlobalBounds().intersects(this->papers[j].first.getGlobalBounds())) {
                // Scissor to Paper collision: Remove paper, add scissor
                this->papers.erase(this->papers.begin() + j);
                initScissor(this->scissors[i].first.getPosition());
                return;
            }
        }
    }

    for (size_t i = 0; i < this->rocks.size(); ++i) {
        for (size_t j = 0; j < this->papers.size(); ++j) {
            if (this->rocks[i].first.getGlobalBounds().intersects(this->papers[j].first.getGlobalBounds())) {
                // Rock to Paper collision: Remove rock, add paper
                this->rocks.erase(this->rocks.begin() + i);
                initPaper(this->papers[j].first.getPosition());
                return;
            }
        }
    }
}

void Game::checkEndCondition() {
    if (this->scissors.empty() && this->papers.empty() && !this->gameEnded) {
        this->gameEnded = true;
        this->endText.setString("Rocks Win!");
    }
    else if (this->rocks.empty() && this->papers.empty() && !this->gameEnded) {
        this->gameEnded = true;
        this->endText.setString("Scissors Win!");
    }
    else if (this->scissors.empty() && this->rocks.empty() && !this->gameEnded) {
        this->gameEnded = true;
        this->endText.setString("Pappers Win!");
    }
}

// Accessors
const bool Game::running() const {
    return this->window->isOpen();
}

// Public methods
void Game::pollEvents() {
    while (this->window->pollEvent(this->ev)) {
        switch (this->ev.type) {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Escape)
                this->window->close();
            break;
        }
    }
}

void Game::update() {
    this->pollEvents();
    this->moveObjects();
    this->handleObjectCollisions();
    this->checkEndCondition();
}

void Game::render() {
    this->window->clear(sf::Color(255, 255, 255, 255));

    // Render all objects
    for (auto& pair : this->scissors) {
        this->window->draw(pair.first);
    }
    for (auto& pair : this->rocks) {
        this->window->draw(pair.first);
    }
    for (auto& pair : this->papers) {
        this->window->draw(pair.first);
    }

    if (this->gameEnded) {
        this->window->draw(this->endText);
    }

    this->window->display();
}

// Initialization functions
void Game::initScissor(const sf::Vector2f& position) {
    sf::Sprite scissor(this->scissorTexture);
    scissor.setScale(0.05f, 0.05f); // Adjust scale factor as needed
    scissor.setPosition(position);
    sf::Vector2f velocity(1.0f,1.0f);
    this->scissors.push_back(std::make_pair(scissor, velocity));
}

void Game::initRock(const sf::Vector2f& position) {
    sf::Sprite rock(this->rockTexture);
    rock.setScale(0.009f, 0.009f); // Adjust scale factor as needed
    rock.setPosition(position);
    sf::Vector2f velocity(1.0f,1.0f);
    this->rocks.push_back(std::make_pair(rock, velocity));
}

void Game::initPaper(const sf::Vector2f& position) {
    sf::Sprite paper(this->paperTexture);
    paper.setScale(0.04f, 0.04f); // Adjust scale factor as needed
    paper.setPosition(position);
    sf::Vector2f velocity(1.0f,1.0f);
    this->papers.push_back(std::make_pair(paper, velocity));
}
