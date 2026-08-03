#include "game.hpp"
#include <array>

game::game() : renderer(window)
{
    pinfo[0] = new info;
    pinfo[1] = new info;
    pieceSelected = false;
    selectedRow = -1;
    selectedCol = -1;
    gameOver = false;
    promotionPending = false;
    promotionPlayerId = 0;
    statusFontLoaded = false;
    statusMessage = "Player 1 turn";
}

void game::run()
{
    window.create(
        sf::VideoMode({ static_cast<unsigned int>(Renderer::WINDOW_WIDTH),
                         static_cast<unsigned int>(Renderer::WINDOW_HEIGHT) }),
        "Chess Game"
    );
    chessboard.allotpices();
    renderer.loadTextures();

    const std::array<std::string, 3> fontPaths = {
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/tahoma.ttf"
    };

    for (const std::string& path : fontPaths)
    {
        if (statusFont.openFromFile(path))
        {
            statusFontLoaded = true;
            break;
        }
    }

    if (statusFontLoaded)
    {
        statusText.emplace(statusFont);
        statusText->setCharacterSize(32);
        statusText->setFillColor(sf::Color(245, 245, 245));
    }

    int currentPlayer = 0;
    setStatusMessage("Player 1 turn");

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>();

            if (mousePressed && !gameOver && promotionPending)
            {
                char promotionChoice = renderer.getPromotionChoiceAt(
                    mousePressed->position.x,
                    mousePressed->position.y,
                    promotionPlayerId == 1
                );

                if (promotionChoice != '\0')
                {
                    board::MoveResult promotionResult = chessboard.completePromotion(
                        promotionChoice,
                        *pinfo[currentPlayer],
                        *pinfo[1 - currentPlayer]
                    );

                    if (promotionResult == board::MoveResult::Checkmate)
                    {
                        gameOver = true;
                        promotionPending = false;
                        setStatusMessage("Checkmate! Player " + to_string(currentPlayer + 1) + " wins");
                    }
                    else if (promotionResult == board::MoveResult::Moved)
                    {
                        promotionPending = false;
                        currentPlayer = 1 - currentPlayer;
                        setStatusMessage("Player " + to_string(currentPlayer + 1) + " turn");
                    }
                }
            }
            else if (mousePressed && !gameOver)
            {
                if (mousePressed->position.y >= Renderer::BOARD_SIZE)
                    continue;

                int col = mousePressed->position.x / Renderer::TILE;
                int row = mousePressed->position.y / Renderer::TILE;

                if (row < 0 || row > 7 || col < 0 || col > 7)
                    continue;

                if (!pieceSelected)
                {
                    char piece = chessboard.arr[row][col];
                    if (piece != '-')
                    {
                        selectedRow = row;
                        selectedCol = col;
                        pieceSelected = true;
                    }
                }
                else
                {
                    board::MoveResult moveResult = chessboard.moveSFML(
                        selectedRow, selectedCol,
                        row, col,
                        *pinfo[currentPlayer],
                        *pinfo[1 - currentPlayer]
                    );

                    if (moveResult == board::MoveResult::Moved)
                    {
                        currentPlayer = 1 - currentPlayer;
                        setStatusMessage("Player " + to_string(currentPlayer + 1) + " turn");
                    }
                    else if (moveResult == board::MoveResult::PromotionPending)
                    {
                        promotionPending = true;
                        promotionPlayerId = pinfo[currentPlayer]->getid();
                        setStatusMessage("Choose a promotion for Player " + to_string(currentPlayer + 1));
                    }
                    else if (moveResult == board::MoveResult::Checkmate)
                    {
                        gameOver = true;
                        setStatusMessage("Checkmate! Player " + to_string(currentPlayer + 1) + " wins");
                    }

                    pieceSelected = false;
                }
            }
        }

        window.clear(sf::Color(24, 24, 24));
        renderer.drawBoard();

        if (pieceSelected)
            renderer.highlightSquare(selectedRow, selectedCol);

        renderer.drawPieces(chessboard);

        sf::RectangleShape statusBar(sf::Vector2f(
            static_cast<float>(Renderer::WINDOW_WIDTH),
            static_cast<float>(Renderer::STATUS_BAR_HEIGHT)
        ));
        statusBar.setPosition({ 0.f, static_cast<float>(Renderer::BOARD_SIZE) });
        statusBar.setFillColor(sf::Color(34, 34, 34));
        window.draw(statusBar);

        if (statusFontLoaded && statusText)
        {
            statusText->setPosition({ 18.f, static_cast<float>(Renderer::BOARD_SIZE + 18) });
            window.draw(*statusText);
        }

        if (promotionPending)
        {
            renderer.drawPromotionOverlay(promotionPlayerId == 1);
        }

        window.display();
    }
}

void game::setStatusMessage(const std::string& message)
{
    statusMessage = message;
    window.setTitle("Chess Game - " + statusMessage);

    if (statusFontLoaded && statusText)
    {
        statusText->setString(statusMessage);
    }
}

game::~game()
{
    delete pinfo[0];
    delete pinfo[1];
}
