#pragma once
#include "board.h"
#include "playerinfo.hpp"
#include <SFML/Graphics.hpp>
#include "renderer.h"
#include <iostream>
#include <string>
#include <optional>

using namespace std;

class game
{
private:
    info* pinfo[2];
    board chessboard;
    sf::RenderWindow window;
    Renderer renderer;
    bool pieceSelected;
    int selectedRow, selectedCol;
    bool gameOver;
    bool promotionPending;
    int promotionPlayerId;
    bool statusFontLoaded;
    sf::Font statusFont;
    std::optional<sf::Text> statusText; // SFML 3: sf::Text has no default ctor, needs a font up front
    std::string statusMessage;

    void setStatusMessage(const std::string& message);

public:
    game();
    void run();
    ~game();
};
