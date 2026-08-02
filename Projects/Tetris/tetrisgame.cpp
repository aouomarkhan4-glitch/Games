////
////#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
////#include<SFML/Graphics.hpp>
////#include <time.h>
////#include<SFML/Audio.hpp>
////using namespace sf;
////
////const int M = 20;
////const int N = 10;
////
////int field[M][N] = { 0 };
////
////struct Point
////{
////    int x, y;
////} a[4], b[4];
////
////int figures[7][4] =
////{
////    1,3,5,7, // I
////    2,4,5,7, // Z
////    3,5,4,6, // S
////    3,5,4,7, // T
////    2,3,5,7, // L
////    3,5,7,6, // J
////    2,3,4,5, // O
////};
////
////bool check()
////{
////    for (int i = 0; i < 4; i++)
////        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;
////        else if (field[a[i].y][a[i].x]) return 0;
////
////    return 1;
////};
////
////void drawPreview(Sprite& s, int figureIndex, RenderWindow& window, float startX, float startY)
////{
////    for (int i = 0; i < 4; i++)
////    {
////        int x = figures[figureIndex][i] % 2;
////        int y = figures[figureIndex][i] / 2;
////
////        s.setPosition({startX + x * 18.0f, startY + y * 18.0f});
////
////        window.draw(s);
////    }
////}
////
////
////void drawMiniPiece( RenderWindow& window,Sprite& s,int pieceIndex,int color,float offsetX,float offsetY)
////{
////    if (pieceIndex == -1)
////        return;
////
////    for (int i = 0; i < 4; i++)
////    {
////        int x = figures[pieceIndex][i] % 2;
////        int y = figures[pieceIndex][i] / 2;
////
////        s.setTextureRect(IntRect({ color * 18, 0 },{ 18, 18 }));
////
////        s.setPosition(Vector2f(offsetX + x * 18,offsetY + y * 18));
////
////        window.draw(s);
////    }
////}
////
////bool isGameOver()
////{
////    for (int i = 0; i < 4; i++)
////    {
////        if (field[a[i].y][a[i].x] != 0)
////        {
////            return true;
////        }
////    }
////
////    return false;
////}
////
////int main()
////{
////    srand(time(0));
////    
////    RenderWindow window(VideoMode({ 580, 480 }), "The Game!");
////    
////
////    sf::Image icon;
////    icon.loadFromFile("assets/tetrisicon.png");
////    window.setIcon({icon.getSize(),icon.getPixelsPtr()});
////    Texture t1, t2, t3;
////    t1.loadFromFile("assets/tiles.png");
////    t2.loadFromFile("assets/background.png");
////   // t3.loadFromFile("frame.png");
////
////
////    sf::Music backgroundMusic;
////    backgroundMusic.openFromFile("assets/tetris.ogg");
////    backgroundMusic.setLooping(true);
////    backgroundMusic.setVolume(50);
////    backgroundMusic.play();
////
////    SoundBuffer lineBuffer;
////    lineBuffer.loadFromFile("assets/linebreak.ogg");
////    Sound lineSound(lineBuffer);
////    lineSound.setVolume(80);
////
////
////    Sprite s(t1), background(t2), frame(t3);
////
////    int dx = 0; bool rotate = 0; int colorNum = 1;
////    float timer = 0, delay = 0.3;
////
////    Clock clock;
////
////    RectangleShape sidePanel;
////    sidePanel.setSize({ 200, 480 });
////    sidePanel.setPosition({ 340, 0 });
////    sidePanel.setFillColor(Color(20, 20, 20));
////
////
////    sf::Font font("assets/font1.ttf");
////    Text holdText(font);
////    holdText.setString("HOLD");
////    holdText.setCharacterSize(28);
////    holdText.setFillColor(Color::White);
////    holdText.setPosition({ 370, 40 });
////
////    Text nextText(font);
////    nextText.setString("NEXT");
////    nextText.setCharacterSize(28);
////    nextText.setFillColor(Color::White);
////    nextText.setPosition({ 370, 230 });
////
////    RectangleShape holdBox;
////    holdBox.setSize({ 120, 120 });
////    holdBox.setPosition({ 360, 80 });
////
////    holdBox.setFillColor(Color::Transparent);
////    holdBox.setOutlineThickness(3);
////    holdBox.setOutlineColor(Color::White);
////
////    RectangleShape nextBox;
////    nextBox.setSize({ 120, 120 });
////    nextBox.setPosition({ 360, 270 });
////
////    nextBox.setFillColor(Color::Transparent);
////    nextBox.setOutlineThickness(3);
////    nextBox.setOutlineColor(Color::White);
////
////    int nextFigure = rand() % 7;
////    int nextColor  = 1 + rand() % 7;
////    int currentFigure = rand() % 7;
////
////    int holdFigure = -1;
////    int holdColor = 1;
////    bool canHold = true;
////    for (int i = 0; i < 4; i++)
////    {
////        a[i].x = figures[currentFigure][i] % 2;
////        a[i].y = figures[currentFigure][i] / 2;
////    }
////
////
////    int score = 0;
////    Text scoreText(font);
////
////    scoreText.setString("SCORE");
////    scoreText.setCharacterSize(28);
////    scoreText.setFillColor(Color::White);
////    scoreText.setPosition({ 370, 410 });
////
////    Text scoreValue(font);
////    bool gameOver = false;
////
////    scoreValue.setCharacterSize(24);
////    scoreValue.setFillColor(Color::Yellow);
////    scoreValue.setPosition({ 390, 445 });
////
////    while (window.isOpen())
////    {
////        float time = clock.getElapsedTime().asSeconds();
////        clock.restart();
////        timer += time;
////
////      
////        while (const std::optional event=window.pollEvent())
////        {
////            if (event->is<sf::Event::Closed>() )
////                window.close();
////
////            if (const auto* key=event->getIf<sf::Event::KeyPressed>())
////                if (key->code == Keyboard::Key::Up) rotate = true;
////                else if (key->code == Keyboard::Key::Left) dx = -1;
////                else if (key->code == Keyboard::Key::Right) dx = 1;
////                else if (key->code == Keyboard::Key::C)
////                {
////                    if (canHold)
////                    {
////                        // Hold logic will go here
////                        if (holdFigure==-1)
////                        {
////                            holdFigure = currentFigure;
////                            holdColor = colorNum;
////                            colorNum = nextColor;
////                            nextColor = 1 + rand() % 7;
////                            currentFigure = nextFigure;
////                            nextFigure = rand() % 7;
////
////                        }
////                        else
////                        {
////                            std::swap(holdFigure, currentFigure);
////                            std::swap(holdColor, colorNum);
////                        }
////
////                      
////
////                        for (int i = 0; i < 4; i++)
////                        {
////                            a[i].x = figures[currentFigure][i] % 2;
////                            a[i].y = figures[currentFigure][i] / 2;
////                        }
////                       
////                        canHold = false;
////                    }
////                }
////                else if (gameOver && key->code == Keyboard::Key::R)
////                {
////                    // clear the board
////                    for (int i = 0; i < M; i++)
////                    {
////                        for (int j = 0; j < N; j++)
////                        {
////                            field[i][j] = 0;
////                        }
////                    }
////
////                    score = 0;
////                    gameOver = false;
////
////                    holdFigure = -1;
////                    holdColor = 1;
////                    canHold = true;
////
////                    currentFigure = rand() % 7;
////                    nextFigure = rand() % 7;
////
////                    colorNum = 1 + rand() % 7;
////                    nextColor = 1 + rand() % 7;
////
////                    for (int i = 0; i < 4; i++)
////                    {
////                        a[i].x = figures[currentFigure][i] % 2;
////                        a[i].y = figures[currentFigure][i] / 2;
////                    }
////                }
////            
////        }
////        if (gameOver)
////        {
////            window.clear();
////
////            Text gameOverText(font);
////            gameOverText.setString("GAME OVER");
////            gameOverText.setCharacterSize(40);
////            gameOverText.setFillColor(Color::Red);
////            gameOverText.setPosition({ 150, 150 });
////
////            Text finalScore(font);
////            finalScore.setString("Score: " + std::to_string(score));
////            finalScore.setCharacterSize(30);
////            finalScore.setFillColor(Color::White);
////            finalScore.setPosition({ 170, 230 });
////
////            Text restartText(font);
////            restartText.setString("Press R to restart");
////            restartText.setCharacterSize(25);
////            restartText.setFillColor(Color::Yellow);
////            restartText.setPosition({ 140, 300 });
////
////            window.draw(gameOverText);
////            window.draw(finalScore);
////            window.draw(restartText);
////
////            window.display();
////
////            continue;
////        }
////
////        if (Keyboard::isKeyPressed(Keyboard::Key::Down)) delay = 0.05;
////
////        //// <- Move -> ///
////        for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
////        if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
////
////        //////Rotate//////
////        if (rotate)
////        {   
////            Point p = a[1]; //center of rotation
////            for (int i = 0; i < 4; i++)
////            {
////                int x = a[i].y - p.y;
////                int y = a[i].x - p.x;
////                a[i].x = p.x - x;
////                a[i].y = p.y + y;
////            }
////            if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
////            
////        }
////
////           ///////Tick//////
////        if (timer > delay)
////        {
////            for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }
////
////            if (!check())
////            {
////                for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum;
////
////                colorNum = nextColor;
////                currentFigure = nextFigure;
////                nextFigure = rand() % 7;
////                nextColor = 1 + rand() % 7;
////                canHold = true;
////                for (int i = 0; i < 4; i++)
////                {
////                    a[i].x = figures[currentFigure][i] % 2;
////                    a[i].y = figures[currentFigure][i] / 2;
////                }
////                 if(isGameOver())
////                        {
////                            gameOver = true;
////                        }
////            }
////
////            timer = 0;
////        }
////
////        ///////check lines//////////
////        int k = M - 1;
////        for (int i = M - 1; i > 0; i--)
////        {
////            int count = 0;
////            for (int j = 0; j < N; j++)
////            {
////                if (field[i][j]) count++;
////                field[k][j] = field[i][j];
////            }
////            if (count < N)
////            {
////                k--;
////            }
////            else
////            {
////                score += 100;
////                lineSound.play();
////            }     // if line is clear this false so k value remain low and upper box are move to lower position
////        }
////
////        dx = 0; rotate = 0; delay = 0.3;
////
////        /////////draw//////////
////        window.clear();
////        window.draw(background);
////       
////        for (int i = 0; i < M; i++)
////            for (int j = 0; j < N; j++)
////            {
////                if (field[i][j] == 0) continue;
////                s.setTextureRect(IntRect({ field[i][j] * 18 , 0 }, { 18, 18 }));
////                s.setPosition(sf::Vector2f(j*18,i*18));
////                s.move({ 28, 31 }); //offset
////                window.draw(s);
////            }
////
////        for (int i = 0; i < 4; i++)
////        {
////            s.setTextureRect(IntRect({ colorNum * 18, 0 }, { 18, 18 }));
////            s.setPosition(sf::Vector2f(a[i].x*18,a[i].y*18));
////            s.move({ 28, 31 }); //offset
////            window.draw(s);
////        }
////        window.draw(sidePanel);
////        window.draw(frame);
////        window.draw(sidePanel);
////
////        window.draw(holdText);
////        window.draw(nextText);
////
////        window.draw(holdBox);
////        window.draw(nextBox);
////
////
////        s.setTextureRect(IntRect({nextColor * 18,0},{18,18}));
////        drawPreview(s, nextFigure, window, 390, 310);
////        drawMiniPiece(window, s, holdFigure, holdColor, 390, 120);
////
////
////        scoreValue.setString(std::to_string(score));
////
////        window.draw(scoreText);
////        window.draw(scoreValue);
////
////
////        window.display();
////    }
////
////    return 0;
////}
