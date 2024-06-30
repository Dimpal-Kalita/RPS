#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

class Game {
private:
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event ev;

    sf::Texture scissorTexture;
    sf::Texture rockTexture;
    sf::Texture paperTexture;

    // Use a pair to store sprite and its velocity together
    std::vector<std::pair<sf::Sprite, sf::Vector2f>> scissors;
    std::vector<std::pair<sf::Sprite, sf::Vector2f>> rocks;
    std::vector<std::pair<sf::Sprite, sf::Vector2f>> papers;

    sf::Font font;
    sf::Text endText;
    bool gameEnded;

    void initVariables();
    void initWindow();
    void initTextures();
    void initSprites();
    void initText();

    void moveObjects();
    void handleObjectCollisions();
    void checkEndCondition();

public:
    // Constructor and destructor
    Game();
    virtual ~Game();

    // Accessors
    const bool running() const;

    // Public methods
    void pollEvents();
    void update();
    void render();

    // Initialization functions
    void initScissor(const sf::Vector2f& position);
    void initRock(const sf::Vector2f& position);
    void initPaper(const sf::Vector2f& position);
};
