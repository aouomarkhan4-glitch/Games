#pragma once
#include <SFML/Graphics.hpp>
#include "board.h"
#include <array>
#include <vector>
#include <optional>

class Renderer {
public:
    static constexpr int TILE = 60;
    static constexpr int BOARD_SIZE = TILE * 8;
    static constexpr int STATUS_BAR_HEIGHT = 84;
    static constexpr int WINDOW_WIDTH = BOARD_SIZE;
    static constexpr int WINDOW_HEIGHT = BOARD_SIZE + STATUS_BAR_HEIGHT;

    Renderer(sf::RenderWindow& window);
    void loadTextures();
    void drawBoard();
    void drawPieces(board& b);
    void highlightSquare(int row, int col);
    void drawPromotionOverlay(bool whitePlayer);
    char getPromotionChoiceAt(int mouseX, int mouseY, bool whitePlayer) const;

private:
    sf::RenderWindow& window;

    std::array<sf::Texture, 12> textures;
    std::vector<sf::Sprite>     sprites;   // SFML 3: sf::Sprite has no default ctor, built after textures load

    sf::Texture                 boardTexture;
    std::optional<sf::Sprite>   boardSprite;

    int getTextureIndex(char piece);
    std::array<sf::FloatRect, 4> getPromotionOptionRects() const;
};
