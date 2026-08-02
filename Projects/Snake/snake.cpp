//// #pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
////#include<iostream>
////#include<SFML/Graphics.hpp>
////#include<stdlib.h>
////#include<ctime>
////#include<SFML/Audio.hpp>
////
////
////using namespace std;
////
////enum Direction
////{
////	none=0,up=1,down,right1,left1
////};
////
////
////class Block
////{
////private:
////	int i, j;
////	sf::Color color;
////	sf::RectangleShape block;
////public:
////	Block(int ti, int tj,sf::Color tcolor ) :i(ti), j(tj)
////	{
////		block.setSize({ 30,30 });
////		block.setPosition(sf::Vector2f(30*j,30*i));
////		block.setFillColor(tcolor);
////		block.setOutlineColor(sf::Color::Black);
////		block.setOutlineThickness(1);
////	}
////	int geti()const
////	{
////		return i;
////	}
////	int getj()const 
////	{
////		return j;
////	}
////	const sf::RectangleShape& getblock() const
////	{
////		return block;
////	}
////
////	void setpositon(const int diraction)
////	{
////		if (diraction == up)
////		{
////			i = i - 1;
////			if (i == -1)
////				i = 19;
////			block.setPosition(sf::Vector2f(30 * j, 30 * i));
////		}
////		if (diraction == down)
////		{
////			i = i + 1;
////			if (i == 20)
////				i = 0;
////			block.setPosition(sf::Vector2f(30 * j, 30 * i));
////		}
////		if (diraction == right1)
////		{
////			j = j + 1;
////			if (j == 20)
////				j = 0;
////			block.setPosition(sf::Vector2f(30 * j, 30 * i));
////		}
////		if (diraction == left1)
////		{
////			j = j - 1;
////			if (j == -1)
////				j = 19;
////			block.setPosition(sf::Vector2f(30 * j, 30 * i));
////		}
////	}
////	void setbody(int ti, int tj)
////	{
////		i = ti; j = tj;
////		block.setPosition(sf::Vector2f(30 * j, 30 * i));
////	}
////};
////
////
////
////// checking for the index of the snake body block
////bool check(vector<Block>& snake,Block food)
////{
////	for (const auto& s : snake)
////	{
////		if (s.geti() == food.geti() && s.getj() == food.getj())
////		{
////			
////				return true;
////			
////		
////		}
////		
////	}
////	return false;
////}
////
////bool bodycollision(Block& snake, Block& tempbody)
////{
////	if (snake.geti()==tempbody.geti() && snake.getj()==tempbody.getj())
////	{
////		return true;
////	}
////	else
////	{
////		return false;
////	}
////}
////
////
////// total along x axis will be 600/30 is 20 and along y is 600/30 is 20
////int main()
////{
////	srand(time(0));
////
////	sf::RenderWindow window(sf::VideoMode({ 600,600 }), "snake");
////
////
////	vector<Block> background;  // this is for thr background 
////	for (int i = 0; i < 20; ++i)
////	{
////		for (int j = 0; j < 20; ++j)
////		{
////			Block temp(i, j,sf::Color(40,40,40));
////			background.push_back(temp);
////		}
////	}
////	vector<Block> snake; // this is the snake head 
////	Block head( 10, 10,sf::Color(0,220,0));
////	snake.push_back(head);
////
////
////	Direction cdirection = none;
////	Direction nextdirection = none;
////
////	sf::Clock clock;
////	float dt = 0;
////	float duration = 0;
////
////
////	vector<Block> food;
////	bool nofood = true;
////	
////	bool addbody = false;
////	int lasti = 0, lastj = 0;
////
////	bool endgame = false;
////    string scorestring = "";
////
////	
////	sf::Font font1("assets/font1.ttf");
////	sf::Text endgametext(font1);
////	endgametext.setCharacterSize(44);
////	endgametext.setString("         GAME END \n Press Space To Play ");
////	endgametext.setPosition({ 25,215 });
////	endgametext.setFillColor(sf::Color::White);
////
////	int score = 0;
////	
////	sf::Text point(font1);
////	point.setCharacterSize(20);
////	point.setFillColor(sf::Color::White);
////	point.setPosition({460,10});
////
////	sf::Text exit(font1);
////	exit.setCharacterSize(44);
////
////	exit.setPosition({ 50,320 });
////	exit.setFillColor(sf::Color::White);
////	exit.setString(" Press ESC To Exit ");
////	sf::Text endscore(font1);
////	endscore.setCharacterSize(44);
////	
////	endscore.setPosition({ 180,375 });
////	endscore.setFillColor(sf::Color::White);
//// 
////
////	  /// backgound music 
////	sf::Music sound1;
////	sound1.openFromFile("assets/music1.ogg");
////	sound1.setLooping(true);
////	
////	sound1.play();
////	sound1.setVolume(40);
////	/// eatting  sound 
////	sf::SoundBuffer soundeffect1;
////	soundeffect1.loadFromFile("assets/eatsound.wav");
////	sf::Sound eatsound(soundeffect1);
////	eatsound.setVolume(40);
////
////
////	sf::Image icon;
////	icon.loadFromFile("assets/snakeicon.png");
////	window.setIcon(icon.getSize(),icon.getPixelsPtr());
////	while (window.isOpen())
////	{
////		scorestring = "SCORE : " + to_string(score);
////		point.setString(scorestring);
////		endscore.setString(scorestring);
////
////		dt = clock.restart().asSeconds();
////		
////		if (!snake.empty())
////		{
////			lasti = snake.back().geti();
////			lastj = snake.back().getj();
////		}
////
////		while (const std::optional event = window.pollEvent())
////		{
////			if (event->is<sf::Event::Closed>()||sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
////			{
////				window.close();
////				sound1.stop();
////			}
////			
////			if (const auto& key = event->getIf<sf::Event::KeyPressed>())
////			{
////				if (key->code==sf::Keyboard::Key::W && cdirection!=down)
////				{
////					nextdirection = up;
////				}
////				if (key->code==sf::Keyboard::Key::S && cdirection!=up)
////				{
////					nextdirection = down;
////				}
////				if (key->code == sf::Keyboard::Key::D && cdirection != left1)
////				{
////					nextdirection = right1;
////				}
////				if (key->code == sf::Keyboard::Key::A && cdirection != right1)
////				{
////					nextdirection = left1;
////				}
////				if (key->code == sf::Keyboard::Key::Space && endgame)
////				{
////					snake.clear();
////
////					Block head(10, 10, sf::Color(0,220,0));
////					snake.push_back(head);
////
////					food.clear();
////					nofood = true;
////					sound1.stop();
////					sound1.play();
////					cdirection = none;
////					nextdirection = none;
////					score = 0;
////					
////					endgame = false;
////				}
////			}
////
////			
////				
////			
////		}
////
////		if (nofood && !snake.empty() && food.empty())
////		{
////			while (true)
////			{
////				int i = rand() % 20 , j = rand() % 20;
////
////				Block foodtemp(i, j, sf::Color::Red);
////				if (check(snake, foodtemp))
////				{
////					
////				}
////				else
////				{
////					Block foodtemp(i, j, sf::Color::Red);
////					food.push_back(foodtemp);
////					nofood = false;
////					break;
////				}
////			}
////
////
////		}
////
////		
////			if (!endgame && !snake.empty())
////			{
////				duration += dt;
////				if (duration >= 0.2)
////				{
////					cdirection = nextdirection;
////					int hi = snake[0].geti(), hj = snake[0].getj();
////					snake[0].setpositon(cdirection);
////
////					for (int i = 1; i < snake.size(); i++)
////					{
////						int bi = snake[i].geti(), bj = snake[i].getj();
////						snake[i].setbody(hi, hj);
////						hi = bi, hj = bj;
////
////					}
////
////
////					duration -= 0.2;
////				}
////			}
////		
////
////		// collision withe food 
////		if (!nofood)
////		{
////			if (snake[0].geti() == food[0].geti() && snake[0].getj()==food[0].getj())
////			{
////				food.erase(food.begin());
////				nofood = true;
////				addbody = true;
////				score += 10;
////				eatsound.play();
////			}
////		}
////		if (addbody)
////		{
////			Block temp(lasti, lastj, sf::Color(0,100,0));
////			snake.push_back(temp);
////			addbody = false;
////		}
////
////		if (!endgame && snake.size()>=3)
////		{
////			for (int i = 1; i < snake.size() ; ++i)
////			{
////				if (bodycollision(snake[0], snake[i]))
////				{
////					endgame = true;
////					food.clear();
////					nofood = true;
////					snake.clear();
////					break;
////				}
////				
////			}
////		}
////		
////
////		// display section :: 
////		window.clear();
////		for (auto&b:background)
////		{
////			window.draw(b.getblock());
////		}
////		for (auto& f : food)
////		{
////			window.draw(f.getblock());
////		}
////		for (auto& s : snake)
////		{
////			window.draw(s.getblock());
////		}
////		if (endgame)
////		{
////			window.draw(endscore);
////			window.draw(endgametext);
////			window.draw(exit);
////			
////		}
////		window.draw(point);
////		window.display();
////	}
////	return 0;
////}