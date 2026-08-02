////  #pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
////
////#include<iostream>
////#include<SFML/Graphics.hpp>
////#include<SFML/Audio.hpp>
////#include<string>
////
////
////using namespace std;
////
////
////int main()
////{
////
////
////
////	sf::RenderWindow win(sf::VideoMode({ 800,400 }), "ping pong");
////	
////	
////	
////	sf::RectangleShape r1, r2, paddle1, paddle2;
////
////	r1.setSize({ 400,400 });
////	r1.setPosition({ 0,0 });
////	r1.setFillColor(sf::Color::Blue);
////
////	r2.setSize({ 400,400 });
////	r2.setPosition({ 400,0 });
////	r2.setFillColor(sf::Color::Red);
////
////	
////	paddle1.setSize({ 20,70 });
////	paddle1.setPosition({10,165});
////	paddle1.setFillColor(sf::Color::White);
////
////	paddle2.setSize({20,70});
////	paddle2.setPosition({770,165});
////	paddle2.setFillColor(sf::Color::White);
////
////
////	sf::CircleShape ball;
////
////	ball.setRadius({ 20 });
////	ball.setFillColor(sf::Color::White);
////	ball.setPosition({34,185});
////
////
////	float ballback = ball.getPosition().x + (ball.getRadius() * 2);
////	sf::Vector2f baseposition1 = ball.getPosition() , baseposition2(726,paddle1.getPosition().y-15)  ;
////
////	bool ballrest = true;
////
////
////	unsigned int score1=0, score2 = 0;
////	sf::Vector2f realvelocity(375, 250),zerovelocity(0,0),currentvelocity=zerovelocity;
////	
////	sf::Font font;
////	font.openFromFile("assets/font1.ttf");
////	
////	sf::Text s1(font), s2(font);
////	
////	s1.setCharacterSize(16);
////	s1.setFillColor(sf::Color::White);
////	s1.setPosition({10,5});
////
////	s2.setCharacterSize(16);
////	s2.setFillColor(sf::Color::White);
////	s2.setPosition({ 700,5 });
////	
////	string str1, str2;
////	sf::Clock clock;
////
////	sf::Vector2f speed(0, 200);
////
////	sf::Music song1;
////	song1.openFromFile("assets/audio2.ogg");
////	song1.setVolume(25);
////	song1.play();
////	song1.setLooping(true);
////
////	float current,end;
////	sf::Time start = song1.getPlayingOffset();
////	
////	end = song1.getDuration().asSeconds();
////	string end1=" BLUE PLAYER WIN ", end2 = " RED PLAYER WIN ";
////	sf::Text winer(font);
////	winer.setFillColor(sf::Color::White);
////	winer.setCharacterSize(20);
////	winer.setPosition({300,180});
////
////	string message = ("PRESS ESC TO EXIT OR SPACE TO PALY");
////
////	sf::Text lastmessage(font);
////	lastmessage.setCharacterSize(20);
////	lastmessage.setFillColor(sf::Color::White);
////	lastmessage.setPosition({200,220});
////	lastmessage.setString(message);
////
////
////	string message2 = ("PRESS ESC TO EXIT");
////
////	sf::Text lastmessage2(font);
////	lastmessage2.setCharacterSize(20);
////	lastmessage2.setFillColor(sf::Color::White);
////	lastmessage2.setPosition({ 30,370 });
////	lastmessage2.setString(message2);
////	
////	
////	sf::Image img;
////	img.loadFromFile("assets/pingpongicon.png");
////	//win.setIcon(img);
////	win.setIcon(img.getSize(), img.getPixelsPtr());
////
////	bool endgame=false;
////	
////
////	sf::SoundBuffer hit;
////	hit.loadFromFile("assets/hitsound2.ogg");
////	sf::Sound hitsound(hit);
////	hitsound.setVolume(70);
////	while (win.isOpen())
////	{
////		float delta = clock.restart().asSeconds();
////		win.setMouseCursorVisible(false);
////		while (const std::optional event = win.pollEvent())
////		{
////			if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
////			{
////				win.close();
////			}
////			
////		}
////
////		ball.move(currentvelocity*delta);
////
////		if (ballrest && !endgame)
////		{
////			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
////			{
////				if (ball.getPosition().x == 34  )
////				{
////					currentvelocity = realvelocity;
////					ballrest = false;
////				}
////				if (ball.getPosition().x == 726 )
////				{
////					currentvelocity.x = -realvelocity.x;
////					currentvelocity.y = realvelocity.y;
////					ballrest = false;
////				}
////				ballrest = false;
////
////			}
////			if (ball.getPosition().x == 34)
////			{
////				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
////				{
////					if (paddle1.getPosition().y >= 5)
////					{
////						paddle1.move(-speed * delta);
////						ball.move(-speed * delta);
////						baseposition1 = ball.getPosition();
////					}
////				}
////				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
////				{
////					if (paddle1.getPosition().y <= 325)
////					{
////						paddle1.move(speed * delta);
////						ball.move(speed * delta);
////						baseposition1 = ball.getPosition();
////					}
////				}
////			}
////			if (ball.getPosition().x == 726)
////			{
////				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
////				{
////					if (paddle2.getPosition().y>=5)
////					{
////						paddle2.move(-speed * delta);
////						ball.move(-speed * delta);
////						baseposition2 = ball.getPosition();
////					}
////				}
////				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
////				{
////					if (paddle2.getPosition().y <= 325)
////					{
////						paddle2.move(speed * delta);
////						ball.move(speed * delta);
////						baseposition2 = ball.getPosition();
////					}
////				}
////			}
////			if (ball.getPosition().x!=34)
////			{
////				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
////				{
////					if(paddle1.getPosition().y>=5)
////					paddle1.move(-speed * delta);	
////				}
////				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
////				{
////					if(paddle1.getPosition().y<=325)
////					paddle1.move(speed * delta);
////				}
////			}
////			if (ball.getPosition().x !=726)
////			{
////				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
////				{
////					if(paddle2.getPosition().y>=5)
////					paddle2.move(-speed * delta);
////				}
////				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
////				{
////					if(paddle2.getPosition().y<=325)
////					paddle2.move(speed * delta);
////				}
////			}
////			
////		}
////		
////		if (!ballrest && !endgame)
////		{
////			if (ball.getPosition().y <= 4)
////			{
////				currentvelocity.y = abs(realvelocity.y);
////			}
////
////			if (ball.getPosition().y >= 356 )
////			{
////				currentvelocity.y = -abs(currentvelocity.y);
////			}
////
////			if (ball.getPosition().x <= 4)
////			{
////				++score2;
////				ballrest = true;
////				ball.setPosition({34,paddle1.getPosition().y+15});
////				currentvelocity = zerovelocity;
////			}
////			if (ball.getPosition().x >= 754)
////			{
////				++score1;
////				ballrest = true;
////				ball.setPosition({726,paddle2.getPosition().y+15});
////				currentvelocity = zerovelocity;
////			}
////
////			if (ball.getGlobalBounds().findIntersection(paddle1.getGlobalBounds()))
////			{
////				currentvelocity.x = abs(currentvelocity.x);
////				float push1 = (paddle1.getPosition().x+paddle1.getGlobalBounds().size.x) - ball.getPosition().x;
////				ball.move({ push1,0.f });
////				hitsound.play();
////			}
////
////			if (ball.getGlobalBounds().findIntersection(paddle2.getGlobalBounds()))
////			{
////				currentvelocity.x = -abs(currentvelocity.x);
////				float push2= (ball.getPosition().x + ball.getGlobalBounds().size.x) - paddle2.getPosition().x;
////				ball.move({ push2,0.f });
////				hitsound.play();
////			}
////
////			
////				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
////				{
////					if(paddle1.getPosition().y>=5)
////					paddle1.move(-speed * delta);
////
////
////				}
////				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
////				{
////					if(paddle1.getPosition().y<=325)
////					paddle1.move(speed * delta);
////
////				}
////			
////				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
////				{
////					if(paddle2.getPosition().y>=5)
////					paddle2.move(-speed * delta);
////
////				}
////				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
////				{
////					if(paddle2.getPosition().y<=325)
////					paddle2.move(speed * delta);
////
////				}
////
////		}
////
////
////	    str1 = "SCORE : " + (string)( score1<10?"0":"") + to_string(score1);
////		str2 = "SCORE : " + (string)(score1 < 10 ? "0" : "") + to_string(score2);
////
////
////		current = song1.getPlayingOffset().asSeconds();
////
////		if (current >= end - 10)
////		{
////			song1.setPlayingOffset(start);
////		}
////		
////
////		if (score1 >= 5)
////		{
////			endgame = true;
////			winer.setString(end1);
////			score1 = 0;
////			score2 = 0;
////		}
////		if (score2 >= 5)
////		{
////			endgame = true;
////			winer.setString(end2);
////			score1 = 0;
////			score2 = 0;
////		}
////
////		if (endgame)
////		{
////			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
////			{
////				win.close();
////			}
////
////			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
////			{
////				endgame = false;
////			}
////		}
////
////
////
////
////		s1.setString(str1);
////		s2.setString(str2);
////		win.clear();
////		win.draw(r1);
////		win.draw(r2);
////		win.draw(paddle1);
////		win.draw(paddle2);
////		win.draw(ball);
////		win.draw(s1);
////		win.draw(s2);
////		
////		if (endgame)
////		{
////			win.draw(winer);
////			win.draw(lastmessage);
////		}
////		else
////		{
////			win.draw(lastmessage2);
////		}
////		win.display();
////	
////
////
////    }
////
////
////	
////	
////}