#include "renderer.h"
#include <iostream>

Renderer::Renderer(sf::RenderWindow& win) : window(win)
{

}

void Renderer::loadTextures() {
    std::string files[12] = {
        "pieces/wP.png", "pieces/wR.png", "pieces/wN.png",
        "pieces/wB.png", "pieces/wQ.png", "pieces/wK.png",
        "pieces/bP.png", "pieces/bR.png", "pieces/bN.png",
        "pieces/bB.png", "pieces/bQ.png", "pieces/bK.png"
    };

    for (int i = 0; i < 12; i++) {
        bool loaded = textures[i].loadFromFile(files[i]);

        if (!loaded) {
            loaded = textures[i].loadFromFile("x64/Debug/" + files[i]);
        }

        if (!loaded) {
            loaded = textures[i].loadFromFile("Debug/" + files[i]);
        }

        if (!loaded) {
            std::cerr << "Failed to load image: " << files[i] << std::endl;
        }
    }

    // sf::Sprite has no default constructor in SFML 3, so sprites are built
    // only after every texture has finished loading (each sprite keeps a
    // reference to its texture, so the texture array must not be touched
    // again afterwards).
    sprites.clear();
    sprites.reserve(12);
    for (int i = 0; i < 12; i++) {
        sprites.emplace_back(textures[i]);
    }

    // board background
    std::string boardFile = "pieces/board.png";
    bool boardLoaded = boardTexture.loadFromFile(boardFile);

    if (!boardLoaded) {
        boardLoaded = boardTexture.loadFromFile("x64/Debug/" + boardFile);
    }
    if (!boardLoaded) {
        boardLoaded = boardTexture.loadFromFile("Debug/" + boardFile);
    }
    if (!boardLoaded) {
        std::cerr << "Failed to load image: " << boardFile << std::endl;
    }

    boardSprite.emplace(boardTexture);
}

int Renderer::getTextureIndex(char piece) {
    switch (piece) {
    case 'P': return 0;   case 'R': return 1;
    case 'N': return 2;   case 'B': return 3;
    case 'Q': return 4;   case 'K': return 5;
    case 'p': return 6;   case 'r': return 7;
    case 'n': return 8;   case 'b': return 9;
    case 'q': return 10;  case 'k': return 11;
    default:  return -1;
    }
}

void Renderer::drawBoard() {
    sf::Vector2u boardSize = boardTexture.getSize();

    if (boardSprite && boardSize.x > 0 && boardSize.y > 0)
    {
        float scaleX = static_cast<float>(BOARD_SIZE) / static_cast<float>(boardSize.x);
        float scaleY = static_cast<float>(BOARD_SIZE) / static_cast<float>(boardSize.y);

        boardSprite->setPosition({ 0.f, 0.f });
        boardSprite->setScale({ scaleX, scaleY });
        window.draw(*boardSprite);
        return;
    }

    // fallback: procedurally drawn squares if board.png failed to load
    sf::RectangleShape square(sf::Vector2f(TILE, TILE));

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if ((r + c) % 2 == 0)
                square.setFillColor(sf::Color(176, 196, 222));
            else
                square.setFillColor(sf::Color(70, 130, 180));

            square.setPosition({ static_cast<float>(c * TILE), static_cast<float>(r * TILE) });
            window.draw(square);
        }
    }
}

void Renderer::drawPieces(board& b) {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            char piece = b.arr[r][c];
            int idx = getTextureIndex(piece);

            if (idx != -1) {
                sf::Vector2u size = textures[idx].getSize();
                if (size.x > 0 && size.y > 0) {
                    sprites[idx].setScale({
                        static_cast<float>(TILE) / static_cast<float>(size.x),
                        static_cast<float>(TILE) / static_cast<float>(size.y)
                    });
                }
                sprites[idx].setPosition({ static_cast<float>(c * TILE), static_cast<float>(r * TILE) });
                window.draw(sprites[idx]);
            }
        }
    }
}

void Renderer::highlightSquare(int row, int col) {
    sf::RectangleShape highlight(sf::Vector2f(TILE, TILE));
    highlight.setFillColor(sf::Color(255, 255, 0, 100));
    highlight.setPosition({ static_cast<float>(col * TILE), static_cast<float>(row * TILE) });
    window.draw(highlight);
}

std::array<sf::FloatRect, 4> Renderer::getPromotionOptionRects() const
{
    const float boxSize = static_cast<float>(TILE);
    const float spacing = 18.f;
    const float totalWidth = (boxSize * 4.f) + (spacing * 3.f);
    const float startX = (static_cast<float>(BOARD_SIZE) - totalWidth) / 2.f;
    const float startY = (static_cast<float>(BOARD_SIZE) - boxSize) / 2.f;

    return {
        sf::FloatRect({ startX + 0.f * (boxSize + spacing), startY }, { boxSize, boxSize }),
        sf::FloatRect({ startX + 1.f * (boxSize + spacing), startY }, { boxSize, boxSize }),
        sf::FloatRect({ startX + 2.f * (boxSize + spacing), startY }, { boxSize, boxSize }),
        sf::FloatRect({ startX + 3.f * (boxSize + spacing), startY }, { boxSize, boxSize })
    };
}

void Renderer::drawPromotionOverlay(bool whitePlayer)
{
    sf::RectangleShape shade(sf::Vector2f(
        static_cast<float>(BOARD_SIZE),
        static_cast<float>(BOARD_SIZE)
    ));
    shade.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(shade);

    const std::array<int, 4> textureIndices = whitePlayer
        ? std::array<int, 4>{4, 1, 3, 2}
        : std::array<int, 4>{10, 7, 9, 8};
    const std::array<sf::FloatRect, 4> optionRects = getPromotionOptionRects();

    for (size_t i = 0; i < optionRects.size(); ++i)
    {
        sf::RectangleShape box(sf::Vector2f(optionRects[i].size.x, optionRects[i].size.y));
        box.setPosition({ optionRects[i].position.x, optionRects[i].position.y });
        box.setFillColor(sf::Color(246, 240, 218));
        box.setOutlineThickness(3.f);
        box.setOutlineColor(sf::Color(60, 60, 60));
        window.draw(box);

        sf::Sprite choiceSprite = sprites[textureIndices[i]];
        sf::Vector2u size = textures[textureIndices[i]].getSize();
        if (size.x > 0 && size.y > 0)
        {
            const float scale = (optionRects[i].size.x * 0.78f) / static_cast<float>(size.x);
            choiceSprite.setScale({ scale, scale });

            const float spriteWidth = static_cast<float>(size.x) * scale;
            const float spriteHeight = static_cast<float>(size.y) * scale;
            choiceSprite.setPosition({
                optionRects[i].position.x + (optionRects[i].size.x - spriteWidth) / 2.f,
                optionRects[i].position.y + (optionRects[i].size.y - spriteHeight) / 2.f
            });
        }
        window.draw(choiceSprite);
    }
}

char Renderer::getPromotionChoiceAt(int mouseX, int mouseY, bool whitePlayer) const
{
    const std::array<char, 4> whiteChoices = { 'Q', 'R', 'B', 'N' };
    const std::array<char, 4> blackChoices = { 'q', 'r', 'b', 'n' };
    const std::array<sf::FloatRect, 4> optionRects = getPromotionOptionRects();

    for (size_t i = 0; i < optionRects.size(); ++i)
    {
        if (optionRects[i].contains(sf::Vector2f(static_cast<float>(mouseX), static_cast<float>(mouseY))))
        {
            return whitePlayer ? whiteChoices[i] : blackChoices[i];
        }
    }

    return '\0';
}
