////#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
////#include<iostream>
////#include<vector>
////#include<string>
////#include<SFML/Graphics.hpp>
////#include<SFML/Audio.hpp>
////
////using namespace std;
////
////
////struct Brick
////{
////	sf::RectangleShape shape;
////	int level; // 0 = block01 ... 4 = block05 (last level, next hit destroys it)
////};
////
////int main()
////{
////	const unsigned WIN_W = 900;
////	const unsigned WIN_H = 640;
////
////	sf::RenderWindow window(sf::VideoMode({ WIN_W, WIN_H }), "BREAK OUT");
////	window.setFramerateLimit(120);
////
////	// ---------------------------------------------------------------- assets
////	sf::Texture backt, ballt, paddlet;
////	backt.loadFromFile("assets/background.jpg");
////	ballt.loadFromFile("assets/ball.png");
////	paddlet.loadFromFile("assets/paddle.png");
////
////	sf::Texture bricktex[5];
////	bricktex[0].loadFromFile("assets/block01.png");
////	bricktex[1].loadFromFile("assets/block02.png");
////	bricktex[2].loadFromFile("assets/block03.png");
////	bricktex[3].loadFromFile("assets/block04.png");
////	bricktex[4].loadFromFile("assets/block05.png");
////
////	sf::RectangleShape back;
////	back.setTexture(&backt);
////	back.setPosition({ 0.f, 0.f });
////	back.setSize({ (float)WIN_W, (float)WIN_H });
////
////	// ------------------------------------------------------------ play area
////	const float leftBound = 30.f;
////	const float rightBound = WIN_W - 30.f;
////	const float topBound = 25.f;
////	const float floorY = 600.f;
////
////	// ----------------------------------------------------------------- mouse
////	sf::Vector2i mouse;
////
////	// ----------------------------------------------------------------paddle
////	bool ballrest = true;
////	sf::RectangleShape paddle;
////	paddle.setTexture(&paddlet);
////	paddle.setSize({ 110.f, 22.f });
////	paddle.setPosition({ (WIN_W - 110.f) / 2.f, 580.f });
////	sf::Vector2f velocity1(300.f, 0.f);
////
////	// ------------------------------------------------------------delta time
////	sf::Clock clock;
////	float dt;
////
////	// -------------------------------------------------------------------ball
////	sf::CircleShape ball;
////	ball.setTexture(&ballt);
////	ball.setRadius(14.f);
////	ball.setPosition({ paddle.getPosition().x + paddle.getSize().x / 2.f - 14.f, paddle.getPosition().y - 30.f });
////	sf::Vector2f velocity2(260.f, 210.f), currentv(0.f, 0.f);
////
////	// ------------------------------------------------------------------bricks
////	vector<Brick> bricks;
////
////	const int ROWS = 5;
////	const int COLS = 6;
////	const float brickW = 110.f;
////	const float brickH = 32.f;
////	const float gapX = 18.f;
////	const float gapY = 18.f;
////	const float fieldTop = 60.f;
////
////	float totalGridW = COLS * brickW + (COLS - 1) * gapX;
////	float startX = (WIN_W - totalGridW) / 2.f;
////
////	auto buildBricks = [&]()
////		{
////			bricks.clear();
////			for (int row = 0; row < ROWS; ++row)
////			{
////				for (int col = 0; col < COLS; ++col)
////				{
////					Brick b;
////					b.level = row; // top row starts at level 0 (block01), bottom row starts at level 4 (block05)
////					b.shape.setSize({ brickW, brickH });
////					b.shape.setPosition({ startX + col * (brickW + gapX), fieldTop + row * (brickH + gapY) });
////					b.shape.setTexture(&bricktex[b.level]);
////					bricks.push_back(b);
////				}
////			}
////		};
////	buildBricks();
////
////	// -------------------------------------------------------------------audio
////	sf::Music mplay;
////	bool hasMusic = mplay.openFromFile("assets/audio2.ogg");
////	if (hasMusic)
////	{
////		mplay.play();
////		mplay.setVolume(25);
////		mplay.setLooping(true);
////	}
////
////	sf::Image icon;
////	icon.loadFromFile("assets/breakouticon.png");
////	window.setIcon(icon.getSize(), icon.getPixelsPtr());
////	// --------------------------------------------------------------------text
////	int score = 0;
////	string strscore;
////	sf::Font font("assets/font1.ttf");
////
////	sf::Text te(font);
////	te.setCharacterSize(22);
////	te.setFillColor(sf::Color::White);
////	te.setPosition({ 20.f, 605.f });
////
////	sf::Text teLives(font);
////	teLives.setCharacterSize(22);
////	teLives.setFillColor(sf::Color::White);
////	teLives.setPosition({ WIN_W - 160.f, 605.f });
////
////	bool endgame2 = false;
////
////	sf::Text te2(font);
////	te2.setCharacterSize(34);
////	te2.setFillColor(sf::Color::White);
////	te2.setPosition({ WIN_W / 2.f - 110.f, 250.f });
////	te2.setString("GAME OVER!");
////
////	sf::Text te3(font);
////	te3.setCharacterSize(24);
////	te3.setFillColor(sf::Color::White);
////	te3.setPosition({ WIN_W / 2.f - 150.f, 300.f });
////	te3.setString("PRESS ESC TO EXIT");
////
////	sf::Text te4(font);
////	te4.setCharacterSize(24);
////	te4.setFillColor(sf::Color::White);
////	te4.setPosition({ WIN_W / 2.f - 140.f, 335.f });
////	te4.setString("PRESS P TO PLAY");
////
////	int lives = 3;
////
////	// ------------------------------------------------------------------ loop
////	while (window.isOpen())
////	{
////		dt = clock.restart().asSeconds();
////		while (const std::optional event = window.pollEvent())
////		{
////			if (event->is<sf::Event::Closed>())
////			{
////				window.close();
////			}
////		}
////		mouse = sf::Mouse::getPosition(window);
////
////		ball.move(currentv * dt);
////
////		if (ballrest && !endgame2)
////		{
////			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
////			{
////				currentv.x = velocity2.x;
////				currentv.y = -velocity2.y;
////				ballrest = false;
////			}
////			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
////			{
////				if (paddle.getPosition().x + paddle.getSize().x <= rightBound)
////				{
////					paddle.move({ velocity1.x * dt, 0.f });
////					ball.move({ velocity1.x * dt, 0.f });
////				}
////			}
////			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
////			{
////				if (paddle.getPosition().x >= leftBound)
////				{
////					paddle.move({ -velocity1.x * dt, 0.f });
////					ball.move({ -velocity1.x * dt, 0.f });
////				}
////			}
////		}
////
////		if (!ballrest && !endgame2)
////		{
////			// paddle movement while ball is in play
////			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
////			{
////				if (paddle.getPosition().x + paddle.getSize().x <= rightBound)
////				{
////					paddle.move({ velocity1.x * dt, 0.f });
////				}
////			}
////			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
////			{
////				if (paddle.getPosition().x >= leftBound)
////				{
////					paddle.move({ -velocity1.x * dt, 0.f });
////				}
////			}
////
////			// walls
////			if (ball.getPosition().x <= leftBound)
////			{
////				currentv.x = abs(currentv.x);
////			}
////			if (ball.getPosition().x + (ball.getRadius() * 2.f) >= rightBound)
////			{
////				currentv.x = -abs(currentv.x);
////			}
////			if (ball.getPosition().y <= topBound)
////			{
////				currentv.y = abs(currentv.y);
////			}
////
////			// paddle
////			if (ball.getGlobalBounds().findIntersection(paddle.getGlobalBounds()))
////			{
////				currentv.y = -abs(currentv.y);
////				float pushout = (ball.getPosition().y + ball.getRadius() * 2.f) - paddle.getPosition().y;
////				ball.move({ 0.f, -abs(pushout) });
////			}
////
////			// lower wall -> lose a life
////			if (ball.getPosition().y >= floorY)
////			{
////				ballrest = true;
////				ball.setPosition({ paddle.getPosition().x + paddle.getSize().x / 2.f - ball.getRadius(), paddle.getPosition().y - 30.f });
////				currentv = sf::Vector2f(0.f, 0.f);
////				if (lives > 0) lives--;
////			}
////
////			// collision with bricks
////			for (size_t i = 0; i < bricks.size(); ++i)
////			{
////				if (ball.getGlobalBounds().findIntersection(bricks[i].shape.getGlobalBounds()))
////				{
////					sf::FloatRect b = ball.getGlobalBounds();
////					sf::FloatRect r = bricks[i].shape.getGlobalBounds();
////
////					float overlapLeft = (b.position.x + b.size.x) - r.position.x;
////					float overlapRight = (r.position.x + r.size.x) - b.position.x;
////					float overlapTop = (b.position.y + b.size.y) - r.position.y;
////					float overlapBottom = (r.position.y + r.size.y) - b.position.y;
////
////					float minOverlapX = std::min(overlapLeft, overlapRight);
////					float minOverlapY = std::min(overlapTop, overlapBottom);
////
////					if (minOverlapX < minOverlapY)
////					{
////						currentv.x = (overlapLeft < overlapRight) ? -std::abs(currentv.x) : std::abs(currentv.x);
////						float push = (overlapLeft < overlapRight) ? -overlapLeft : overlapRight;
////						ball.move({ push, 0.f });
////					}
////					else
////					{
////						currentv.y = (overlapTop < overlapBottom) ? -std::abs(currentv.y) : std::abs(currentv.y);
////						float push = (overlapTop < overlapBottom) ? -overlapTop : overlapBottom;
////						ball.move({ 0.f, push });
////					}
////
////					// level-cascade logic:
////					// not yet at the last level color -> drop one level and re-skin
////					// already at the last level color -> this hit destroys it
////					if (bricks[i].level < 4)
////					{
////						bricks[i].level++;
////						bricks[i].shape.setTexture(&bricktex[bricks[i].level]);
////						score += 10;
////					}
////					else
////					{
////						bricks.erase(bricks.begin() + i);
////						score += 20;
////					}
////					break;
////				}
////			}
////		}
////
////		strscore = "SCORE : " + (string)(score < 10 ? "0" : "") + to_string(score);
////		te.setString(strscore);
////		teLives.setString("LIVES : " + to_string(lives));
////
////		if (bricks.empty() || lives <= 0)
////		{
////			endgame2 = true;
////			buildBricks();
////			lives = 3;
////		}
////
////		if (endgame2)
////		{
////			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
////			{
////				window.close();
////			}
////			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
////			{
////				score = 0;
////				lives = 3;
////				ball.setPosition({ paddle.getPosition().x + paddle.getSize().x / 2.f - ball.getRadius(), paddle.getPosition().y - 30.f });
////				currentv = sf::Vector2f(0.f, 0.f);
////				endgame2 = false;
////				ballrest = true;
////			}
////		}
////
////		window.clear();
////		window.draw(back);
////		for (size_t i = 0; i < bricks.size(); ++i)
////		{
////			window.draw(bricks[i].shape);
////		}
////		window.draw(ball);
////		window.draw(paddle);
////		window.draw(te);
////		window.draw(teLives);
////
////		if (endgame2)
////		{
////			window.draw(te2);
////			window.draw(te3);
////			window.draw(te4);
////		}
////		window.display();
////	}
////	return 0;
////}