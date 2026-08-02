#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<string>
#include<vector>
using namespace std;

const int w = 1000;
const int h = 650;
float detorad = 3.14159265f / 180.f;

// ============================================================
// MUSIC MANAGER
// ============================================================
class MusicManager
{
private:
	sf::Music music;
	int currentTrack;
	float volume;
public:
	MusicManager()
	{
		currentTrack = 0;
		volume = 50.f;
	}
	void loadAndPlay(int index)
	{
		if (index < 0 || index > 4) return;
		currentTrack = index;
		string filename = "assets/music" + to_string(currentTrack + 1) + ".ogg";
		if (music.openFromFile(filename))
		{
			music.setVolume(volume);
			music.setLooping(true);
			music.play();
		}
	}
	void next()
	{
		currentTrack = (currentTrack + 1) % 5;
		loadAndPlay(currentTrack);
	}
	void prev()
	{
		currentTrack = ((currentTrack - 1) + 5) % 5;
		loadAndPlay(currentTrack);
	}
	void setVolume(float v)
	{
		volume = v;
		if (volume < 0) volume = 0;
		if (volume > 100) volume = 100;
		music.setVolume(volume);
	}
	float getVolume() { return volume; }
	int getTrack() { return currentTrack; }
};

// ============================================================
// SIMPLE BUTTON
// ============================================================
class Button
{
private:
	sf::RectangleShape box;
	sf::Text label;
public:
	//Button() : label(sf::Font()) {}
	Button(sf::Font& font, const string& txt, sf::Vector2f pos, sf::Vector2f size, unsigned int charSize = 22)
		: label(font)
	{
		box.setSize(size);
		box.setPosition(pos);
		box.setFillColor(sf::Color(40, 40, 80, 220));
		box.setOutlineColor(sf::Color(100, 150, 255));
		box.setOutlineThickness(2);
		label.setString(txt);
		label.setCharacterSize(charSize);
		label.setFillColor(sf::Color::White);
		sf::FloatRect tb = label.getLocalBounds();
		label.setOrigin({ tb.position.x + tb.size.x / 2.f, tb.position.y + tb.size.y / 2.f });
		label.setPosition({ pos.x + size.x / 2.f, pos.y + size.y / 2.f });
	}
	bool isHovered(sf::Vector2i mouse)
	{
		return box.getGlobalBounds().contains(sf::Vector2f((float)mouse.x, (float)mouse.y));
	}
	void setHover(bool h)
	{
		if (h) box.setFillColor(sf::Color(70, 90, 160, 240));
		else   box.setFillColor(sf::Color(40, 40, 80, 220));
	}
	void draw(sf::RenderWindow& win) { win.draw(box); win.draw(label); }
};

// ============================================================
// HEALTH LABEL - shows remaining HP for 1.5 sec above entity
// ============================================================
class HealthLabel
{
private:
	sf::Text text;
	float timer;
	float duration;
	bool active;
public:
	HealthLabel(sf::Font& font) : text(font)
	{
		timer = 0; duration = 1.5f; active = false;
		text.setCharacterSize(14);
		text.setFillColor(sf::Color::Yellow);
	}
	void show(sf::Vector2f pos, int hp)
	{
		text.setString("HP:" + to_string(hp));
		text.setPosition({ pos.x - 10, pos.y - 22 });
		timer = 0; active = true;
	}
	void update(float dt)
	{
		if (active) { timer += dt; if (timer >= duration) active = false; }
	}
	void draw(sf::RenderWindow& win) { if (active) win.draw(text); }
};

// ============================================================
// PLAYER
// ============================================================
class player
{
private:
	sf::RectangleShape player1;
	sf::Texture spaceship;
	float angle, dx, dy, speed, rotationalspeed;
	int health, maxHealth;
public:
	player()
	{
		spaceship.loadFromFile("assets/spaceship.png");
		player1.setSize({ 50,50 });
		player1.setTexture(&spaceship);
		player1.setTextureRect(sf::IntRect({ 38,38 }, { 44,46 }));
		player1.setPosition({ 475,300 });
		angle = 0; dx = 0; dy = 0; speed = 2.5f; rotationalspeed = 180;
		health = 700; maxHealth = 700;
	}
	void setHealth(int h) { health = h; }
	int getHealth() { return health; }
	int getMaxHealth() { return maxHealth; }
	void takeDamage(int dmg) { health -= dmg; if (health < 0) health = 0; }
	bool isDead() { return health <= 0; }
	void resetFull()
	{
		player1.setPosition({ 475,300 });
		angle = 0; dx = 0; dy = 0;
		health = 700;
	}
	void setposition() { player1.setPosition({ 475,300 }); }
	void moveplayer(float dt)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			player1.setTextureRect(sf::IntRect({ 38,38 }, { 41,45 }));
			dx += cos((angle - 90) * detorad) * speed * dt;
			dy += sin((angle - 90) * detorad) * speed * dt;
			player1.move({ dx,dy });
		}
		else
		{
			dx *= 0.95f;
			dy *= 0.95f;
			player1.setTextureRect(sf::IntRect({ 37,0 }, { 42,40 }));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			player1.setTextureRect(sf::IntRect({ 85,38 }, { 32,46 }));
			angle += rotationalspeed * dt;
			player1.setRotation(sf::degrees(angle));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			angle -= rotationalspeed * dt;
			player1.setRotation(sf::degrees(angle));
			player1.setTextureRect(sf::IntRect({ 1,38 }, { 32,46 }));
		}
		if (player1.getPosition().x <= -50) player1.setPosition({ 1000, player1.getPosition().y });
		if (player1.getPosition().x >= 1050) player1.setPosition({ 0, player1.getPosition().y });
		if (player1.getPosition().y <= -50) player1.setPosition({ player1.getPosition().x, 649 });
		if (player1.getPosition().y >= 650)  player1.setPosition({ player1.getPosition().x, 0 });
	}
	sf::RectangleShape getshapepalyer() { return player1; }
	void drawplayer(sf::RenderWindow& win) { win.draw(player1); }
	void drawHealthBar(sf::RenderWindow& win)
	{
		float barW = 200.f, barH = 14.f;
		float bx = 10.f, by = (float)h - 30.f;
		sf::RectangleShape bg({ barW, barH });
		bg.setPosition({ bx, by });
		bg.setFillColor(sf::Color(80, 0, 0));
		sf::RectangleShape fg({ barW * ((float)health / (float)maxHealth), barH });
		fg.setPosition({ bx, by });
		fg.setFillColor(sf::Color(0, 220, 0));
		win.draw(bg); win.draw(fg);
	}
	float getdx() { return dx; }
	float getdy() { return dy; }
	sf::Vector2f getplayerposi() { return player1.getPosition(); }
	float getangle() { return angle; }
};

// ============================================================
// BULLET 1 (F key) power 70
// ============================================================
class bullet
{
	sf::RectangleShape bullet1;
	sf::Vector2f velocity2;
	float bx, by;
	int power;
public:
	bullet(float x, float y, sf::Texture& fire, player& obj, float dt)
	{
		bullet1.setSize({ 37,42 });
		bullet1.setTexture(&fire);
		bullet1.setTextureRect(sf::IntRect({ 0,0 }, { 32,54 }));
		bullet1.setPosition({ x,y });
		bullet1.setRotation(sf::degrees(obj.getangle()));
		velocity2 = { 0.f,0.f };
		bx = cos((obj.getangle() - 90) * detorad) * 400;
		by = sin((obj.getangle() - 90) * detorad) * 400;
		power = 70;
	}
	int getPower() { return power; }
	void movebullet(float dt) { bullet1.move({ bx * dt, by * dt }); }
	void drawbullet(sf::RenderWindow& bulletwindow) { bulletwindow.draw(bullet1); }
	void setvelocity() { velocity2 = sf::Vector2f(0, -100); }
	sf::Vector2f getposition() { return bullet1.getPosition(); }
	sf::RectangleShape& getshape() { return bullet1; }
};

// ============================================================
// BULLET 2 (G key) power 150
// ============================================================
class bullet2
{
	sf::RectangleShape bullet1;
	sf::Vector2f velocity2;
	float bx, by;
	int power;
public:
	bullet2(float x, float y, sf::Texture& fire, player& obj, float dt)
	{
		bullet1.setSize({ 37,42 });
		bullet1.setTexture(&fire);
		bullet1.setTextureRect(sf::IntRect({ 0,0 }, { 32,54 }));
		bullet1.setPosition({ x,y });
		bullet1.setRotation(sf::degrees(obj.getangle()));
		velocity2 = { 0.f,0.f };
		bx = cos((obj.getangle() - 90) * detorad) * 350;
		by = sin((obj.getangle() - 90) * detorad) * 350;
		power = 150;
	}
	int getPower() { return power; }
	void movebullet(float dt) { bullet1.move({ bx * dt, by * dt }); }
	void drawbullet(sf::RenderWindow& bulletwindow) { bulletwindow.draw(bullet1); }
	void setvelocity() { velocity2 = sf::Vector2f(0, -100); }
	sf::Vector2f getposition() { return bullet1.getPosition(); }
	sf::RectangleShape& getshape() { return bullet1; }
};

// ============================================================
// ENEMY BULLET 1 power 50 (fires2.png)
// ============================================================
class enemybullet
{
	sf::RectangleShape bullet1;
	sf::Vector2f velocity2;
	float length, bx, by;
	sf::Vector2f direction;
	float speed;
	int power;
public:
	enemybullet(sf::Texture& fire, sf::Vector2f playerposi, sf::Vector2f spawnpos)
	{
		bullet1.setSize({ 37,42 });
		bullet1.setTexture(&fire);
		bullet1.setTextureRect(sf::IntRect({ 4,2 }, { 27,52 }));
		bullet1.setPosition(spawnpos);
		bullet1.setRotation(sf::degrees(90));
		velocity2 = { 0.f,0.f };
		speed = 240.f; power = 50;
		direction = { playerposi.x - bullet1.getPosition().x, playerposi.y - bullet1.getPosition().y };
		length = sqrt(direction.x * direction.x + direction.y * direction.y);
		if (length > 0) direction /= length;
		bx = direction.x * speed;
		by = direction.y * speed;
	}
	int getPower() { return power; }
	void movebullet(float dt) { bullet1.move({ bx * dt, by * dt }); }
	void drawbullet(sf::RenderWindow& bulletwindow) { bulletwindow.draw(bullet1); }
	sf::Vector2f getposition() { return bullet1.getPosition(); }
	sf::RectangleShape& getshape() { return bullet1; }
};

// ============================================================
// ENEMY BULLET 2 power 70 (fires3.png) enemy8 only
// ============================================================
class enemybullet2
{
	sf::RectangleShape bullet1;
	float bx, by, length;
	sf::Vector2f direction;
	float speed;
	int power;
public:
	enemybullet2(sf::Texture& fire, sf::Vector2f playerposi, sf::Vector2f spawnpos)
	{
		bullet1.setSize({ 37,42 });
		bullet1.setTexture(&fire);
		bullet1.setTextureRect(sf::IntRect({ 0,0 }, { 32,54 }));
		bullet1.setPosition(spawnpos);
		bullet1.setRotation(sf::degrees(90));
		speed = 240.f; power = 70;
		direction = { playerposi.x - bullet1.getPosition().x, playerposi.y - bullet1.getPosition().y };
		length = sqrt(direction.x * direction.x + direction.y * direction.y);
		if (length > 0) direction /= length;
		bx = direction.x * speed;
		by = direction.y * speed;
	}
	int getPower() { return power; }
	void movebullet(float dt) { bullet1.move({ bx * dt, by * dt }); }
	void drawbullet(sf::RenderWindow& bulletwindow) { bulletwindow.draw(bullet1); }
	sf::Vector2f getposition() { return bullet1.getPosition(); }
	sf::RectangleShape& getshape() { return bullet1; }
};

// ============================================================
// EXPLOSION ANIMATION
// ============================================================
class explosion2
{
private:
	sf::RectangleShape shape;
	int currentfram, totalfram;
	float timer, frameDuration;
public:
	explosion2(sf::Texture& copy)
	{
		shape.setTexture(&copy);
		shape.setTextureRect(sf::IntRect({ 23,15 }, { 191,169 }));
		shape.setSize({ 50,50 });
		shape.setOrigin({ 25,25 });
		totalfram = 65;
		timer = 0;
		currentfram = 0;
		frameDuration = 0.1f;
	}
	bool update(float dt)
	{
		timer += dt;
		if (timer >= frameDuration)
		{
			if (currentfram >= totalfram) { currentfram = 0; return false; }
			shape.setTextureRect(sf::IntRect({ 15 + (currentfram * 192),20 }, { 194,140 }));
			currentfram++;
			timer = 0;
		}
		return true;
	}
	void setposition(sf::Vector2f enemyposition) { shape.setPosition(enemyposition); }
	void setsize(sf::RectangleShape other) { shape.setSize(other.getSize()); }
	void displayblast2(sf::RenderWindow& win) { win.draw(shape); }
};

// ============================================================
// ENEMY CLASS
// ============================================================
class enime
{
private:
	sf::RectangleShape enemy;
	sf::Vector2f enemyvelocity;
	explosion2 blast;
	float angle, speed;
	bool blastshow;
	float y;
	int hitcount, fram;
	float timer;
	int health, maxHealth;
	HealthLabel* hlabel;
public:
	vector<enemybullet>  bullet;
	vector<enemybullet2> bullet2;

	enime(sf::Texture& text, float x, sf::Texture& copy, sf::Font& font, int hp = 100, float startY = -50) : blast(copy)
	{
		y = startY;
		enemy.setTexture(&text);
		hitcount = 0;
		enemy.setTextureRect(sf::IntRect({ 21,21 }, { 370,438 }));
		enemy.setSize({ 50,50 });
		enemy.setOrigin({ 25.f,25.f });
		enemy.setPosition({ x,y });
		enemyvelocity = sf::Vector2f(0, 18);
		timer = 0; fram = 0;
		angle = 0; speed = 20; blastshow = false;
		health = hp; maxHealth = hp;
		hlabel = new HealthLabel(font);
	}
	~enime() { delete hlabel; }

	void takeDamage(int dmg)
	{
		health -= dmg;
		if (health < 0) health = 0;
		hlabel->show(enemy.getPosition(), health);
	}
	int getHealth() { return health; }
	bool isDead() { return health <= 0; }

	void rotateenemy(float dt) { angle += speed * dt; enemy.setRotation(sf::degrees(angle)); }
	void enemymove(float dt) { enemy.move(enemyvelocity * dt); }
	void sethitcount(int i) { hitcount = i; }
	int gethitcount() { return hitcount; }
	void animationenemy(float dt, int totalfram, int x, int y2, int width, int height)
	{
		timer += dt;
		if (timer >= 0.5f)
		{
			enemy.setTextureRect(sf::IntRect({ x + (fram * width),y2 }, { width,height }));
			++fram; timer = 0;
			if (fram >= totalfram) fram = 0;
		}
	}
	void setbool(bool decision) { blastshow = decision; }
	bool getbool() { return blastshow; }
	void setblastsize(sf::RectangleShape other) { blast.setsize(other); }
	void drawenemy(sf::RenderWindow& window)
	{
		if (blastshow) { blast.displayblast2(window); }
		else { window.draw(enemy); }
	}
	void drawHealthBar(sf::RenderWindow& win)
	{
		if (blastshow) return;
		sf::Vector2f pos = enemy.getPosition();
		sf::Vector2f sz = enemy.getSize();
		float bw = sz.x, bh = 5.f;
		float bx2 = pos.x - sz.x / 2.f;
		float by2 = pos.y - sz.y / 2.f - 8.f;
		sf::RectangleShape bg({ bw, bh });
		bg.setPosition({ bx2, by2 });
		bg.setFillColor(sf::Color(100, 0, 0));
		sf::RectangleShape fg({ bw * ((float)health / (float)maxHealth), bh });
		fg.setPosition({ bx2, by2 });
		fg.setFillColor(sf::Color(255, 80, 0));
		win.draw(bg); win.draw(fg);
	}
	void updateLabel(float dt) { if (hlabel) hlabel->update(dt); }
	void drawLabel(sf::RenderWindow& win) { if (hlabel) hlabel->draw(win); }
	void startanimation(float dt) { blastshow = blast.update(dt); }
	void setblastposition() { blast.setposition(enemy.getPosition()); }
	sf::Vector2f getposition() { return enemy.getPosition(); }
	sf::RectangleShape& getshape() { return enemy; }
	void setsizeofenemy(float x2, float y2) { enemy.setSize(sf::Vector2f(x2, y2)); }
};

// ============================================================
// GAME STATE
// ============================================================
// 0 = MainMenu  1 = MapSelect  2 = InfoPage  3 = SettingsPage
// 4 = Playing   5 = Paused     6 = EndGame
int gamestate = 0;

// ============================================================
// GAME STATS
// ============================================================
int  gscore = 0;
int  gkilled = 0;
int  gshotsFired = 0;
int  gshotsHit = 0;
int  gdamageDealt = 0;
float gtimePlayed = 0.f;

void resetStats()
{
	gscore = 0; gkilled = 0; gshotsFired = 0;
	gshotsHit = 0; gdamageDealt = 0; gtimePlayed = 0.f;
}

// ============================================================
// MAP SELECT
// ============================================================
struct MapOption
{
	string filename;
	sf::Texture tex;
	sf::RectangleShape img;
	sf::RectangleShape highlight;
	bool selected;
	MapOption() : selected(false) {}
};

// ============================================================
// GAME CLASS
// ============================================================
class game
{
public:
	void run()
	{
		sf::RenderWindow spacegame(sf::VideoMode({ w,h }), "SPACE INVADER");
		spacegame.setFramerateLimit(60);
		srand(time(0));

		// ---- FONT ----
		sf::Font logofont;
		logofont.openFromFile("assets/font2.otf");

		// ---- MUSIC ----
		MusicManager mm;
		mm.loadAndPlay(0);

		// ---- LOGO / MENU BACKGROUND ----
		sf::Texture logobackground;
		logobackground.loadFromFile("assets/background8.png");
		sf::RectangleShape logoback;
		logoback.setTexture(&logobackground);
		logoback.setSize({ 1000,650 });
		logoback.setPosition({ 0,0 });

		// ---- GAME BACKGROUNDS (loaded after map select) ----
		sf::Texture lv1BgTex, lv2BgTex, lv3BgTex;
		sf::RectangleShape lv1Bg, lv2Bg, lv3Bg;
		lv1Bg.setSize({ 1000,650 }); lv2Bg.setSize({ 1000,650 }); lv3Bg.setSize({ 1000,650 });

		// ---- MAP SELECT DATA ----
		// level1 options
		MapOption lv1opt[3];
		string lv1files[3] = { "assets/background1.png","assets/background2.png","assets/background3.png" };
		for (int i = 0; i < 3; i++)
		{
			lv1opt[i].filename = lv1files[i];
			lv1opt[i].tex.loadFromFile(lv1files[i]);
			lv1opt[i].img.setSize({ 220,150 });
			lv1opt[i].img.setTexture(&lv1opt[i].tex);
			lv1opt[i].img.setPosition({ 100.f + i * 280.f, 250 });
			lv1opt[i].highlight.setSize({ 226,156 });
			lv1opt[i].highlight.setPosition({ 97.f + i * 280.f, 247 });
			lv1opt[i].highlight.setFillColor(sf::Color::Transparent);
			lv1opt[i].highlight.setOutlineColor(sf::Color::Transparent);
			lv1opt[i].highlight.setOutlineThickness(3);
			lv1opt[i].selected = false;
		}
		// level2 options
		MapOption lv2opt[3];
		string lv2files[3] = { "assets/background4.png","assets/background5.jpeg","assets/background6.jpeg" };
		for (int i = 0; i < 3; i++)
		{
			lv2opt[i].filename = lv2files[i];
			lv2opt[i].tex.loadFromFile(lv2files[i]);
			lv2opt[i].img.setSize({ 220,150 });
			lv2opt[i].img.setTexture(&lv2opt[i].tex);
			lv2opt[i].img.setPosition({ 100.f + i * 280.f, 250 });
			lv2opt[i].highlight.setSize({ 226,156 });
			lv2opt[i].highlight.setPosition({ 97.f + i * 280.f, 247 });
			lv2opt[i].highlight.setFillColor(sf::Color::Transparent);
			lv2opt[i].highlight.setOutlineColor(sf::Color::Transparent);
			lv2opt[i].highlight.setOutlineThickness(3);
			lv2opt[i].selected = false;
		}
		// level3 options
		MapOption lv3opt[3];
		string lv3files[3] = { "assets/background0.jpg","assets/background7.png","assets/background9.png" };
		for (int i = 0; i < 3; i++)
		{
			lv3opt[i].filename = lv3files[i];
			lv3opt[i].tex.loadFromFile(lv3files[i]);
			lv3opt[i].img.setSize({ 220,150 });
			lv3opt[i].img.setTexture(&lv3opt[i].tex);
			lv3opt[i].img.setPosition({ 100.f + i * 280.f, 250 });
			lv3opt[i].highlight.setSize({ 226,156 });
			lv3opt[i].highlight.setPosition({ 97.f + i * 280.f, 247 });
			lv3opt[i].highlight.setFillColor(sf::Color::Transparent);
			lv3opt[i].highlight.setOutlineColor(sf::Color::Transparent);
			lv3opt[i].highlight.setOutlineThickness(3);
			lv3opt[i].selected = false;
		}
		string lv1chosen = "assets/background1.png";
		string lv2chosen = "assets/background4.png";
		string lv3chosen = "assets/background0.jpg";
		int mapSelectLevel = 1; // which level we are choosing for

		// ---- MAIN MENU BUTTONS ----
		Button btnStart(logofont, "Start", { 400,260 }, { 200,55 });
		Button btnInfo(logofont, "Info", { 400,330 }, { 200,55 });
		Button btnSettingsMain(logofont, "Settings", { 400,400 }, { 200,55 });
		Button btnExitMain(logofont, "Exit", { 400,470 }, { 200,55 });
		sf::Text mainTitle(logofont);
		mainTitle.setString("SPACE INVADER");
		mainTitle.setCharacterSize(52);
		mainTitle.setFillColor(sf::Color::Cyan);
		mainTitle.setPosition({ 220,160 });

		// ---- MAP SELECT UI ----
		Button btnConfirm(logofont, "Confirm", { 430,450 }, { 140,46 });
		sf::Text mapPrompt(logofont);
		mapPrompt.setCharacterSize(28);
		mapPrompt.setFillColor(sf::Color::White);
		mapPrompt.setPosition({ 60,160 });

		// ---- INFO PAGE ----
		Button btnInfoBack(logofont, "Back", { 430,600 }, { 140,40 });
		float infoScrollY = 0;
		string infoStr =
			"=== STORY ===\n"
			"Year 2147. An alien armada emerges from deep space,\n"
			"threatening all of humanity. You are the last pilot\n"
			"of the elite Space Guard. In your modified starfighter\n"
			"you must battle through three waves of deadly enemies\n"
			"to save Earth.\n\n"
			"=== CONTROLS ===\n"
			"W         - Thrust forward\n"
			"A         - Rotate left\n"
			"D         - Rotate right\n"
			"F         - Fire Bullet 1  (Power: 70 , Cooldown: 0.5s)\n"
			"G         - Fire Bullet 2  (Power: 150, Cooldown: 1.5s)\n"
			"ESC       - Quit game\n"
			"PAUSE BTN - Pause / Resume (top-left)\n\n"
			"=== MAPS ===\n"
			"Level 1: background1, background2, background3\n"
			"Level 2: background4, background5, background6\n"
			"Level 3: background0, background7, background9\n\n"
			"=== ENEMIES ===\n"
			"Enemy 1 - HP: 100  | Basic descender\n"
			"Enemy 2 - HP: 100  | Basic descender\n"
			"Enemy 3 - HP: 150  | Tougher descender\n"
			"Enemy 4 - HP: 150  | Tougher descender\n"
			"Enemy 5 - HP: 100  | Basic descender\n"
			"Enemy 6 - HP: 100  | Rotating spinner\n"
			"Enemy 7 - HP: 300  | Boss shoots bullets (Power: 50)\n"
			"Enemy 8 - HP: 500  | Final Boss 2 types (P:50 and P:70)\n\n"
			"=== PLAYER SHIP ===\n"
			"Health: 700\n"
			"Bullet 1 Power: 70   | Bullet 2 Power: 150\n"
			"Enemy collision or reaching bottom: -50 HP to player\n\n"
			"=== SCORING ===\n"
			"Enemy 1/2/5/6 : +50\n"
			"Enemy 3/4     : +100\n"
			"Enemy 7       : +200\n"
			"Enemy 8       : +500\n";
		sf::Text infoText(logofont);
		infoText.setString(infoStr);
		infoText.setCharacterSize(16);
		infoText.setFillColor(sf::Color::White);
		infoText.setPosition({ 30, 50 });
		sf::RectangleShape infoScrollBar({ 12, 530.f });
		infoScrollBar.setPosition({ 980, 55 });
		infoScrollBar.setFillColor(sf::Color(60, 60, 60));
		sf::RectangleShape infoScrollThumb({ 12, 80 });
		infoScrollThumb.setFillColor(sf::Color(150, 150, 255));

		// ---- SETTINGS PAGE (standalone) ----
		Button btnStPrev(logofont, "< Prev Track", { 350,230 }, { 300,50 });
		Button btnStNext(logofont, "Next Track >", { 350,300 }, { 300,50 });
		Button btnStVolDown(logofont, "Volume -", { 350,370 }, { 145,50 });
		Button btnStVolUp(logofont, "Volume +", { 505,370 }, { 145,50 });
		Button btnStBack(logofont, "Back", { 350,450 }, { 300,50 });
		sf::Text stTitle(logofont);
		stTitle.setCharacterSize(36); stTitle.setFillColor(sf::Color::White);
		stTitle.setString("Settings"); stTitle.setPosition({ 390,140 });
		sf::Text stTrackLabel(logofont);
		stTrackLabel.setCharacterSize(22); stTrackLabel.setFillColor(sf::Color::Cyan);
		stTrackLabel.setPosition({ 350,185 });
		sf::Text stVolLabel(logofont);
		stVolLabel.setCharacterSize(22); stVolLabel.setFillColor(sf::Color::Cyan);
		stVolLabel.setPosition({ 350,520 });
		bool settingsFromMain = true;

		// ---- PAUSE MENU ----
		sf::RectangleShape pauseBtn({ 60,30 });
		pauseBtn.setPosition({ 10,10 });
		pauseBtn.setFillColor(sf::Color(30, 30, 70, 200));
		pauseBtn.setOutlineColor(sf::Color(100, 150, 255));
		pauseBtn.setOutlineThickness(2);
		sf::Text pauseBtnTxt(logofont);
		pauseBtnTxt.setString("II");
		pauseBtnTxt.setCharacterSize(18);
		pauseBtnTxt.setFillColor(sf::Color::White);
		pauseBtnTxt.setPosition({ 28,12 });

		Button btnPauseResume(logofont, "Resume", { 350,200 }, { 300,50 });
		Button btnPauseRestart(logofont, "Restart", { 350,270 }, { 300,50 });
		Button btnPauseSettings(logofont, "Settings", { 350,340 }, { 300,50 });
		Button btnPauseExit(logofont, "Exit", { 350,410 }, { 300,50 });

		// pause - settings sub panel
		bool pauseSettingsOpen = false;
		Button btnPsPrev(logofont, "< Prev Track", { 350,220 }, { 300,50 });
		Button btnPsNext(logofont, "Next Track >", { 350,290 }, { 300,50 });
		Button btnPsVolDown(logofont, "Volume -", { 350,360 }, { 145,50 });
		Button btnPsVolUp(logofont, "Volume +", { 505,360 }, { 145,50 });
		Button btnPsBack(logofont, "Back", { 350,430 }, { 300,50 });
		sf::Text psTrackLabel(logofont);
		psTrackLabel.setCharacterSize(20); psTrackLabel.setFillColor(sf::Color::Cyan);
		psTrackLabel.setPosition({ 350,175 });
		sf::Text psVolLabel(logofont);
		psVolLabel.setCharacterSize(20); psVolLabel.setFillColor(sf::Color::Cyan);
		psVolLabel.setPosition({ 350,500 });

		// ---- HUD ----
		sf::Text scoreTxt(logofont);
		scoreTxt.setCharacterSize(22);
		scoreTxt.setFillColor(sf::Color::White);
		scoreTxt.setPosition({ 780,10 });
		sf::Text hpTxt(logofont);
		hpTxt.setCharacterSize(16);
		hpTxt.setFillColor(sf::Color::Green);
		hpTxt.setPosition({ 10,(float)h - 50 });

		// ---- LEVEL LOGO ----
		sf::Text logoText(logofont);
		logoText.setCharacterSize(55);
		logoText.setFillColor(sf::Color::White);
		logoText.setPosition({ 300,300 });
		bool logo1 = false, logo2 = false, logo3 = false;
		float logoTimer = 0, logoCooldown = 5.f;

		// ---- PLAYER ----
		player space;
		space.setposition();
		sf::Clock clock;

		// ---- PLAYER BULLETS ----
		vector<bullet>  firess;
		vector<bullet2> firess2;
		sf::Texture bulletTexture;
		bulletTexture.loadFromFile("assets/fires.png");
		sf::Texture bulletTexture2;
		bulletTexture2.loadFromFile("assets/fires3.png");
		float cooldown = 0.5f, timer = 0.f;
		float cooldown2 = 1.5f, timer2 = 0.f;

		// ---- ENEMY TEXTURES ----
		sf::Texture enemy1tex; enemy1tex.loadFromFile("assets/enemy1.png");
		sf::Texture enemy2tex; enemy2tex.loadFromFile("assets/enemy2.png");
		sf::Texture enemy3tex; enemy3tex.loadFromFile("assets/enemy3.png");
		sf::Texture enemy4tex; enemy4tex.loadFromFile("assets/enemy4.png");
		sf::Texture enemy5tex; enemy5tex.loadFromFile("assets/enemy5.png");
		sf::Texture enemy6tex; enemy6tex.loadFromFile("assets/enemy6.png");
		sf::Texture enemy7tex; enemy7tex.loadFromFile("assets/enemy7.png");
		sf::Texture enemy8tex; enemy8tex.loadFromFile("assets/enemy8.png");
		sf::Texture blast2tex; blast2tex.loadFromFile("assets/blast2.png");
		sf::Texture ebullet;   ebullet.loadFromFile("assets/fires2.png");
		sf::Texture ebullet3;  ebullet3.loadFromFile("assets/fires3.png");

		// ---- ENEMY VECTORS ----
		vector<enime*> alliens1, alliens2, alliens3, alliens4;
		vector<enime*> alliens5, alliens6, alliens7, alliens8;

		// ---- ENEMY TIMERS ----
		float enemy1timer = 0, enemy1cooldown = 14; int enemy1count = 0;
		float enemy2timer = 0, enemy2cooldown = 17; int enemy2count = 0;
		float enemy3timer = 0, enemy3cooldown = 10; int enemy3count = 0;
		float enemy4timer = 0, enemy4cooldown = 18; int enemy4count = 0;
		float enemy5timer = 0, enemy5cooldown = 7.5f; int enemy5count = 0;
		float enemy6timer = 0, enemy6cooldown = 6.f;  int enemy6count = 0;
		int enemy7count = 0, enemy8count = 0;
		float ecooldown = 2.f, etimer = 0.f;
		float e8cooldown1 = 1.5f, e8timer1 = 0.f;
		float e8cooldown2 = 2.25f, e8timer2 = 0.f;

		// ---- LEVEL FLAGS ----
		bool levelone = false, leveltwo = false, levelthree = false, endgame = false;
		bool lv2phase1 = true, lv2phase2 = false;
		bool lv3phase1 = true, lv3phase2 = false, lv3phase3 = false;
		int lv3enemy7spawned = 0;

		// ---- HELPER: clear all enemies and reset counters ----
		// (called inline with repeated code like original style)

		bool mouseClick = false;

		// ================================================================
		// MAIN LOOP
		// ================================================================
		while (spacegame.isOpen())
		{
			float dt = clock.restart().asSeconds();
			if (dt > 0.1f) dt = 0.1f;

			timer += dt;
			timer2 += dt;
			mouseClick = false;

			sf::Vector2i mousePos = sf::Mouse::getPosition(spacegame);

			// ---- EVENTS ----
			while (const std::optional event = spacegame.pollEvent())
			{
				if (event->is<sf::Event::Closed>())
					spacegame.close();

				if (event->is<sf::Event::MouseButtonPressed>())
					mouseClick = true;

				if (const auto* mw = event->getIf<sf::Event::MouseWheelScrolled>())
				{
					if (gamestate == 2)
					{
						infoScrollY -= mw->delta * 20.f;
						if (infoScrollY < 0) infoScrollY = 0;
					}
				}

				if (const auto* kp = event->getIf<sf::Event::KeyPressed>())
				{
					if (kp->code == sf::Keyboard::Key::Escape)
						spacegame.close();

					if (gamestate == 4)
					{
						if (kp->code == sf::Keyboard::Key::F && timer >= cooldown)
						{
							bullet release(space.getplayerposi().x, space.getplayerposi().y - 10, bulletTexture, space, dt);
							release.setvelocity();
							firess.push_back(release);
							timer = 0;
							gshotsFired++;
						}
						if (kp->code == sf::Keyboard::Key::G && timer2 >= cooldown2)
						{
							bullet2 release2(space.getplayerposi().x, space.getplayerposi().y - 10, bulletTexture2, space, dt);
							release2.setvelocity();
							firess2.push_back(release2);
							timer2 = 0;
							gshotsFired++;
						}
					}
				}
			}

			// ================================================================
			// MAIN MENU  (gamestate == 0)
			// ================================================================
			if (gamestate == 0)
			{
				spacegame.clear();
				spacegame.draw(logoback);
				spacegame.draw(mainTitle);

				btnStart.setHover(btnStart.isHovered(mousePos));
				btnInfo.setHover(btnInfo.isHovered(mousePos));
				btnSettingsMain.setHover(btnSettingsMain.isHovered(mousePos));
				btnExitMain.setHover(btnExitMain.isHovered(mousePos));

				btnStart.draw(spacegame);
				btnInfo.draw(spacegame);
				btnSettingsMain.draw(spacegame);
				btnExitMain.draw(spacegame);
				spacegame.display();

				if (mouseClick)
				{
					if (btnStart.isHovered(mousePos))
					{
						mapSelectLevel = 1;
						gamestate = 1;
					}
					else if (btnInfo.isHovered(mousePos))
					{
						infoScrollY = 0;
						gamestate = 2;
					}
					else if (btnSettingsMain.isHovered(mousePos))
					{
						settingsFromMain = true;
						gamestate = 3;
					}
					else if (btnExitMain.isHovered(mousePos))
					{
						spacegame.close();
					}
				}
				continue;
			}

			// ================================================================
			// MAP SELECT  (gamestate == 1)
			// ================================================================
			if (gamestate == 1)
			{
				spacegame.clear();
				sf::RectangleShape blackbg({ 1000,650 });
				blackbg.setFillColor(sf::Color::Black);
				spacegame.draw(blackbg);

				if (mapSelectLevel == 1) mapPrompt.setString("Level 1 - Choose a Map:");
				else if (mapSelectLevel == 2) mapPrompt.setString("Level 2 - Choose a Map:");
				else mapPrompt.setString("Level 3 - Choose a Map:");
				spacegame.draw(mapPrompt);

				// draw the 3 options for current level
				if (mapSelectLevel == 1)
				{
					for (int i = 0; i < 3; i++)
					{
						bool hov = lv1opt[i].img.getGlobalBounds().contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y));
						if (hov || lv1opt[i].selected)
							lv1opt[i].highlight.setOutlineColor(sf::Color::Yellow);
						else
							lv1opt[i].highlight.setOutlineColor(sf::Color::Transparent);
						spacegame.draw(lv1opt[i].img);
						spacegame.draw(lv1opt[i].highlight);
						if (hov && mouseClick)
						{
							for (int j = 0; j < 3; j++) lv1opt[j].selected = false;
							lv1opt[i].selected = true;
							lv1chosen = lv1opt[i].filename;
						}
					}
				}
				else if (mapSelectLevel == 2)
				{
					for (int i = 0; i < 3; i++)
					{
						bool hov = lv2opt[i].img.getGlobalBounds().contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y));
						if (hov || lv2opt[i].selected)
							lv2opt[i].highlight.setOutlineColor(sf::Color::Yellow);
						else
							lv2opt[i].highlight.setOutlineColor(sf::Color::Transparent);
						spacegame.draw(lv2opt[i].img);
						spacegame.draw(lv2opt[i].highlight);
						if (hov && mouseClick)
						{
							for (int j = 0; j < 3; j++) lv2opt[j].selected = false;
							lv2opt[i].selected = true;
							lv2chosen = lv2opt[i].filename;
						}
					}
				}
				else
				{
					for (int i = 0; i < 3; i++)
					{
						bool hov = lv3opt[i].img.getGlobalBounds().contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y));
						if (hov || lv3opt[i].selected)
							lv3opt[i].highlight.setOutlineColor(sf::Color::Yellow);
						else
							lv3opt[i].highlight.setOutlineColor(sf::Color::Transparent);
						spacegame.draw(lv3opt[i].img);
						spacegame.draw(lv3opt[i].highlight);
						if (hov && mouseClick)
						{
							for (int j = 0; j < 3; j++) lv3opt[j].selected = false;
							lv3opt[i].selected = true;
							lv3chosen = lv3opt[i].filename;
						}
					}
				}

				btnConfirm.setHover(btnConfirm.isHovered(mousePos));
				btnConfirm.draw(spacegame);
				spacegame.display();

				if (mouseClick && btnConfirm.isHovered(mousePos))
				{
					if (mapSelectLevel < 3)
					{
						mapSelectLevel++;
					}
					else
					{
						// all maps chosen - load them and start game
						lv1BgTex.loadFromFile(lv1chosen);
						lv1Bg.setTexture(&lv1BgTex);
						lv2BgTex.loadFromFile(lv2chosen);
						lv2Bg.setTexture(&lv2BgTex);
						lv3BgTex.loadFromFile(lv3chosen);
						lv3Bg.setTexture(&lv3BgTex);

						// reset all game data
						resetStats();
						space.resetFull();
						firess.clear(); firess2.clear();
						for (int i = 0; i < (int)alliens1.size(); i++) delete alliens1[i]; alliens1.clear();
						for (int i = 0; i < (int)alliens2.size(); i++) delete alliens2[i]; alliens2.clear();
						for (int i = 0; i < (int)alliens3.size(); i++) delete alliens3[i]; alliens3.clear();
						for (int i = 0; i < (int)alliens4.size(); i++) delete alliens4[i]; alliens4.clear();
						for (int i = 0; i < (int)alliens5.size(); i++) delete alliens5[i]; alliens5.clear();
						for (int i = 0; i < (int)alliens6.size(); i++) delete alliens6[i]; alliens6.clear();
						for (int i = 0; i < (int)alliens7.size(); i++) delete alliens7[i]; alliens7.clear();
						for (int i = 0; i < (int)alliens8.size(); i++) delete alliens8[i]; alliens8.clear();
						enemy1timer = 0; enemy1count = 0; enemy2timer = 0; enemy2count = 0;
						enemy3timer = 0; enemy3count = 0; enemy4timer = 0; enemy4count = 0;
						enemy5timer = 0; enemy5count = 0; enemy6timer = 0; enemy6count = 0;
						enemy7count = 0; enemy8count = 0;
						etimer = 0; e8timer1 = 0; e8timer2 = 0;
						levelone = true; leveltwo = false; levelthree = false; endgame = false;
						lv2phase1 = true; lv2phase2 = false;
						lv3phase1 = true; lv3phase2 = false; lv3phase3 = false;
						lv3enemy7spawned = 0;
						logo1 = true; logo2 = false; logo3 = false;
						logoTimer = 0; timer = 0; timer2 = 0;
						pauseSettingsOpen = false;
						gamestate = 4;
						clock.restart();
					}
				}
				continue;
			}

			// ================================================================
			// INFO PAGE  (gamestate == 2)
			// ================================================================
			if (gamestate == 2)
			{
				spacegame.clear();
				sf::RectangleShape blackbg2({ 1000,650 });
				blackbg2.setFillColor(sf::Color::Black);
				spacegame.draw(blackbg2);

				// scrollable view for info text
				sf::View infoView(sf::FloatRect({ 0, infoScrollY }, { (float)w, (float)h - 60 }));
				infoView.setViewport(sf::FloatRect({ 0, 0.07f }, { 1.f, 0.85f }));
				spacegame.setView(infoView);
				spacegame.draw(infoText);
				spacegame.setView(spacegame.getDefaultView());

				// scroll thumb
				float totalH = infoText.getGlobalBounds().size.y;
				float ratio = infoScrollY / (totalH > (float)h ? totalH - (float)h + 80.f : 1.f);
				if (ratio > 1) ratio = 1;
				infoScrollThumb.setPosition({ 980, 55 + ratio * (530.f - 80.f) });
				spacegame.draw(infoScrollBar);
				spacegame.draw(infoScrollThumb);

				btnInfoBack.setHover(btnInfoBack.isHovered(mousePos));
				btnInfoBack.draw(spacegame);
				spacegame.display();

				if (mouseClick && btnInfoBack.isHovered(mousePos))
					gamestate = 0;
				continue;
			}

			// ================================================================
			// SETTINGS PAGE  (gamestate == 3)
			// ================================================================
			if (gamestate == 3)
			{
				spacegame.clear();
				spacegame.draw(logoback);
				stTrackLabel.setString("Now Playing: music" + to_string(mm.getTrack() + 1) + ".ogg");
				stVolLabel.setString("Volume: " + to_string((int)mm.getVolume()));

				btnStPrev.setHover(btnStPrev.isHovered(mousePos));
				btnStNext.setHover(btnStNext.isHovered(mousePos));
				btnStVolDown.setHover(btnStVolDown.isHovered(mousePos));
				btnStVolUp.setHover(btnStVolUp.isHovered(mousePos));
				btnStBack.setHover(btnStBack.isHovered(mousePos));

				spacegame.draw(stTitle);
				spacegame.draw(stTrackLabel);
				spacegame.draw(stVolLabel);
				btnStPrev.draw(spacegame);
				btnStNext.draw(spacegame);
				btnStVolDown.draw(spacegame);
				btnStVolUp.draw(spacegame);
				btnStBack.draw(spacegame);
				spacegame.display();

				if (mouseClick)
				{
					if (btnStPrev.isHovered(mousePos))    mm.prev();
					if (btnStNext.isHovered(mousePos))    mm.next();
					if (btnStVolDown.isHovered(mousePos)) mm.setVolume(mm.getVolume() - 10.f);
					if (btnStVolUp.isHovered(mousePos))   mm.setVolume(mm.getVolume() + 10.f);
					if (btnStBack.isHovered(mousePos))
					{
						if (settingsFromMain) gamestate = 0;
						else gamestate = 5;
					}
				}
				continue;
			}

			// ================================================================
			// END GAME  (gamestate == 6)
			// ================================================================
			if (gamestate == 6)
			{
				spacegame.clear();
				spacegame.draw(logoback);

				sf::Text t1(logofont); t1.setCharacterSize(44); t1.setFillColor(sf::Color::Cyan);
				t1.setString("GAME COMPLETE!"); t1.setPosition({ 260,60 }); spacegame.draw(t1);

				sf::Text t2(logofont); t2.setCharacterSize(24); t2.setFillColor(sf::Color::White);
				t2.setString("Score            : " + to_string(gscore)); t2.setPosition({ 220,155 }); spacegame.draw(t2);
				sf::Text t3(logofont); t3.setCharacterSize(24); t3.setFillColor(sf::Color::White);
				t3.setString("Enemies Destroyed: " + to_string(gkilled)); t3.setPosition({ 220,200 }); spacegame.draw(t3);
				sf::Text t4(logofont); t4.setCharacterSize(24); t4.setFillColor(sf::Color::White);
				t4.setString("Shots Fired      : " + to_string(gshotsFired)); t4.setPosition({ 220,245 }); spacegame.draw(t4);
				float acc = gshotsFired > 0 ? (float)gshotsHit / (float)gshotsFired * 100.f : 0.f;
				sf::Text t5(logofont); t5.setCharacterSize(24); t5.setFillColor(sf::Color::White);
				t5.setString("Accuracy         : " + to_string((int)acc) + "%"); t5.setPosition({ 220,290 }); spacegame.draw(t5);
				sf::Text t6(logofont); t6.setCharacterSize(24); t6.setFillColor(sf::Color::White);
				t6.setString("Damage Dealt     : " + to_string(gdamageDealt)); t6.setPosition({ 220,335 }); spacegame.draw(t6);
				int mins = (int)gtimePlayed / 60;
				int secs = (int)gtimePlayed % 60;
				sf::Text t7(logofont); t7.setCharacterSize(24); t7.setFillColor(sf::Color::White);
				t7.setString("Time Played      : " + to_string(mins) + "m " + to_string(secs) + "s"); t7.setPosition({ 220,380 }); spacegame.draw(t7);

				Button btnMenu(logofont, "Main Menu", { 400,470 }, { 200,50 });
				btnMenu.setHover(btnMenu.isHovered(mousePos));
				btnMenu.draw(spacegame);
				spacegame.display();

				if (mouseClick && btnMenu.isHovered(mousePos))
					gamestate = 0;
				continue;
			}

			// ================================================================
			// PAUSED  (gamestate == 5)
			// ================================================================
			if (gamestate == 5)
			{
				spacegame.clear();
				spacegame.draw(logoback);

				// dim overlay
				sf::RectangleShape dim({ 1000,650 });
				dim.setFillColor(sf::Color(0, 0, 0, 160));
				spacegame.draw(dim);

				if (!pauseSettingsOpen)
				{
					btnPauseResume.setHover(btnPauseResume.isHovered(mousePos));
					btnPauseRestart.setHover(btnPauseRestart.isHovered(mousePos));
					btnPauseSettings.setHover(btnPauseSettings.isHovered(mousePos));
					btnPauseExit.setHover(btnPauseExit.isHovered(mousePos));
					btnPauseResume.draw(spacegame);
					btnPauseRestart.draw(spacegame);
					btnPauseSettings.draw(spacegame);
					btnPauseExit.draw(spacegame);

					if (mouseClick)
					{
						if (btnPauseResume.isHovered(mousePos))
						{
							gamestate = 4;
							pauseSettingsOpen = false;
							clock.restart();
						}
						else if (btnPauseRestart.isHovered(mousePos))
						{
							// full reset then go to map select level 1 logo
							resetStats();
							space.resetFull();
							firess.clear(); firess2.clear();
							for (int i = 0; i < (int)alliens1.size(); i++) delete alliens1[i]; alliens1.clear();
							for (int i = 0; i < (int)alliens2.size(); i++) delete alliens2[i]; alliens2.clear();
							for (int i = 0; i < (int)alliens3.size(); i++) delete alliens3[i]; alliens3.clear();
							for (int i = 0; i < (int)alliens4.size(); i++) delete alliens4[i]; alliens4.clear();
							for (int i = 0; i < (int)alliens5.size(); i++) delete alliens5[i]; alliens5.clear();
							for (int i = 0; i < (int)alliens6.size(); i++) delete alliens6[i]; alliens6.clear();
							for (int i = 0; i < (int)alliens7.size(); i++) delete alliens7[i]; alliens7.clear();
							for (int i = 0; i < (int)alliens8.size(); i++) delete alliens8[i]; alliens8.clear();
							enemy1timer = 0; enemy1count = 0; enemy2timer = 0; enemy2count = 0;
							enemy3timer = 0; enemy3count = 0; enemy4timer = 0; enemy4count = 0;
							enemy5timer = 0; enemy5count = 0; enemy6timer = 0; enemy6count = 0;
							enemy7count = 0; enemy8count = 0;
							etimer = 0; e8timer1 = 0; e8timer2 = 0;
							levelone = true; leveltwo = false; levelthree = false; endgame = false;
							lv2phase1 = true; lv2phase2 = false;
							lv3phase1 = true; lv3phase2 = false; lv3phase3 = false;
							lv3enemy7spawned = 0;
							logo1 = true; logo2 = false; logo3 = false;
							logoTimer = 0; timer = 0; timer2 = 0;
							pauseSettingsOpen = false;
							gamestate = 4;
							clock.restart();
						}
						else if (btnPauseSettings.isHovered(mousePos))
						{
							pauseSettingsOpen = true;
						}
						else if (btnPauseExit.isHovered(mousePos))
						{
							gamestate = 0;
							pauseSettingsOpen = false;
						}
					}
					// pause button also resumes
					if (mouseClick && pauseBtn.getGlobalBounds().contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y)))
					{
						gamestate = 4;
						pauseSettingsOpen = false;
						clock.restart();
					}
				}
				else
				{
					// pause settings sub panel
					psTrackLabel.setString("Track: music" + to_string(mm.getTrack() + 1) + ".ogg");
					psVolLabel.setString("Volume: " + to_string((int)mm.getVolume()));

					btnPsPrev.setHover(btnPsPrev.isHovered(mousePos));
					btnPsNext.setHover(btnPsNext.isHovered(mousePos));
					btnPsVolDown.setHover(btnPsVolDown.isHovered(mousePos));
					btnPsVolUp.setHover(btnPsVolUp.isHovered(mousePos));
					btnPsBack.setHover(btnPsBack.isHovered(mousePos));

					spacegame.draw(psTrackLabel);
					spacegame.draw(psVolLabel);
					btnPsPrev.draw(spacegame);
					btnPsNext.draw(spacegame);
					btnPsVolDown.draw(spacegame);
					btnPsVolUp.draw(spacegame);
					btnPsBack.draw(spacegame);

					if (mouseClick)
					{
						if (btnPsPrev.isHovered(mousePos))    mm.prev();
						if (btnPsNext.isHovered(mousePos))    mm.next();
						if (btnPsVolDown.isHovered(mousePos)) mm.setVolume(mm.getVolume() - 10.f);
						if (btnPsVolUp.isHovered(mousePos))   mm.setVolume(mm.getVolume() + 10.f);
						if (btnPsBack.isHovered(mousePos))    pauseSettingsOpen = false;
					}
				}

				spacegame.display();
				continue;
			}

			// ================================================================
			// PLAYING  (gamestate == 4)
			// ================================================================

			// pause button click
			if (mouseClick && pauseBtn.getGlobalBounds().contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y)))
			{
				gamestate = 5;
				pauseSettingsOpen = false;
				continue;
			}

			gtimePlayed += dt;

			// ---- LOGO DISPLAY ----
			if (logo1 || logo2 || logo3)
			{
				logoTimer += dt;
				if (logoTimer >= logoCooldown) { logo1 = false; logo2 = false; logo3 = false; logoTimer = 0; }
				spacegame.clear();
				spacegame.draw(logoback);
				if (logo1)logoText.setString("LEVEL - [1]"); if (logo2)logoText.setString("LEVEL - [2]"); if (logo3)logoText.setString("LEVEL - [3]");
				spacegame.draw(logoText);
				spacegame.display();
				continue;
			}

			// ============================================================
			// LEVEL ONE - enemies 1,2,5,6  (11 kills)
			// ============================================================
			if (levelone)
			{
				enemy1timer += dt;
				if (enemy1timer >= enemy1cooldown && enemy1count <= 2)
				{
					float enemy1randomx = rand() % 900 + 50;
					enime* obj = new enime(enemy1tex, enemy1randomx, blast2tex, logofont, 100);
					alliens1.push_back(obj);
					enemy1timer = 0; enemy1count++;
				}

				enemy2timer += dt;
				if (enemy2timer >= enemy2cooldown && enemy2count <= 2)
				{
					float enemy2randomx = rand() % 900 + 50;
					enime* obj = new enime(enemy2tex, enemy2randomx, blast2tex, logofont, 100);
					alliens2.push_back(obj);
					enemy2timer = 0; enemy2count++;
				}

				enemy5timer += dt;
				if (enemy5timer >= enemy5cooldown && enemy5count <= 2)
				{
					float enemy5randomx = rand() % 900 + 50;
					enime* obj = new enime(enemy5tex, enemy5randomx, blast2tex, logofont, 100);
					alliens5.push_back(obj);
					enemy5timer = 0; enemy5count++;
				}

				enemy6timer += dt;
				if (enemy6timer >= enemy6cooldown && enemy6count <= 5)
				{
					float enemy6randomx = rand() % 900 + 50;
					enime* obj = new enime(enemy6tex, enemy6randomx, blast2tex, logofont, 100);
					alliens6.push_back(obj);
					enemy6timer = 0; enemy6count++;
				}

				if (gkilled >= 11 && alliens1.empty() && alliens2.empty() && alliens5.empty() && alliens6.empty())
				{
					levelone = false; leveltwo = true;
					lv2phase1 = true; lv2phase2 = false;
					enemy1count = 0; enemy2count = 0; enemy3count = 0; enemy4count = 0;
					enemy5count = 0; enemy6count = 0; enemy7count = 0; enemy8count = 0;
					enemy1timer = 0; enemy2timer = 0; enemy3timer = 0; enemy4timer = 0;
					enemy5timer = 0; enemy6timer = 0; etimer = 0; e8timer1 = 0; e8timer2 = 0;
					for (int i = 0; i < (int)alliens1.size(); i++) delete alliens1[i]; alliens1.clear();
					for (int i = 0; i < (int)alliens2.size(); i++) delete alliens2[i]; alliens2.clear();
					for (int i = 0; i < (int)alliens5.size(); i++) delete alliens5[i]; alliens5.clear();
					for (int i = 0; i < (int)alliens6.size(); i++) delete alliens6[i]; alliens6.clear();
					logo2 = true; logoTimer = 0;
					logoText.setString(" LEVEL - [ 2 ] ");
				}
			}

			// ============================================================
			// LEVEL TWO
			// phase1: enemies 1,2,3,4,5,6 (17 kills)
			// phase2: one enemy7 boss
			// ============================================================
			else if (leveltwo)
			{
				if (lv2phase1)
				{
					enemy1timer += dt;
					if (enemy1timer >= enemy1cooldown && enemy1count <= 2)
					{
						float rx = rand() % 900 + 50;
						alliens1.push_back(new enime(enemy1tex, rx, blast2tex, logofont, 100));
						enemy1timer = 0; enemy1count++;
					}
					enemy2timer += dt;
					if (enemy2timer >= enemy2cooldown && enemy2count <= 2)
					{
						float rx = rand() % 900 + 50;
						alliens2.push_back(new enime(enemy2tex, rx, blast2tex, logofont, 100));
						enemy2timer = 0; enemy2count++;
					}
					enemy3timer += dt;
					if (enemy3timer >= enemy3cooldown && enemy3count <= 3)
					{
						float rx = rand() % 900 + 50;
						alliens3.push_back(new enime(enemy3tex, rx, blast2tex, logofont, 150));
						enemy3timer = 0; enemy3count++;
					}
					enemy4timer += dt;
					if (enemy4timer >= enemy4cooldown && enemy4count <= 3)
					{
						float rx = rand() % 900 + 50;
						alliens4.push_back(new enime(enemy4tex, rx, blast2tex, logofont, 150));
						enemy4timer = 0; enemy4count++;
					}
					enemy5timer += dt;
					if (enemy5timer >= enemy5cooldown && enemy5count <= 2)
					{
						float rx = rand() % 900 + 50;
						alliens5.push_back(new enime(enemy5tex, rx, blast2tex, logofont, 100));
						enemy5timer = 0; enemy5count++;
					}
					enemy6timer += dt;
					if (enemy6timer >= enemy6cooldown && enemy6count <= 5)
					{
						float rx = rand() % 900 + 50;
						alliens6.push_back(new enime(enemy6tex, rx, blast2tex, logofont, 100));
						enemy6timer = 0; enemy6count++;
					}

					if (gkilled >= 28 && alliens1.empty() && alliens2.empty()
						&& alliens3.empty() && alliens4.empty() && alliens5.empty() && alliens6.empty())
					{
						lv2phase1 = false; lv2phase2 = true;
						enemy7count = 0; etimer = 0;
					}
				}

				if (lv2phase2)
				{
					if (enemy7count == 0)
					{
						enime* obj = new enime(enemy7tex, 200, blast2tex, logofont, 300);
						obj->setsizeofenemy(600, 100);
						alliens7.push_back(obj);
						enemy7count++;
					}
					if (!alliens7.empty() && !alliens7[0]->getbool())
					{
						etimer += dt;
						if (etimer >= ecooldown)
						{
							sf::Vector2f spawnpos = { alliens7[0]->getposition().x + 280, alliens7[0]->getposition().y + 80 };
							enemybullet bulletobj(ebullet, space.getplayerposi(), spawnpos);
							alliens7[0]->bullet.push_back(bulletobj);
							etimer = 0;
						}
					}
					if (gkilled >= 29 && alliens7.empty())
					{
						leveltwo = false; levelthree = true;
						lv3phase1 = true; lv3phase2 = false; lv3phase3 = false;
						lv3enemy7spawned = 0;
						enemy1count = 0; enemy2count = 0; enemy3count = 0; enemy4count = 0;
						enemy5count = 0; enemy6count = 0; enemy7count = 0; enemy8count = 0;
						enemy1timer = 0; enemy2timer = 0; enemy3timer = 0; enemy4timer = 0;
						enemy5timer = 0; enemy6timer = 0; etimer = 0; e8timer1 = 0; e8timer2 = 0;
						for (int i = 0; i < (int)alliens1.size(); i++) delete alliens1[i]; alliens1.clear();
						for (int i = 0; i < (int)alliens2.size(); i++) delete alliens2[i]; alliens2.clear();
						for (int i = 0; i < (int)alliens3.size(); i++) delete alliens3[i]; alliens3.clear();
						for (int i = 0; i < (int)alliens4.size(); i++) delete alliens4[i]; alliens4.clear();
						for (int i = 0; i < (int)alliens5.size(); i++) delete alliens5[i]; alliens5.clear();
						for (int i = 0; i < (int)alliens6.size(); i++) delete alliens6[i]; alliens6.clear();
						for (int i = 0; i < (int)alliens7.size(); i++) delete alliens7[i]; alliens7.clear();
						for (int i = 0; i < (int)alliens8.size(); i++) delete alliens8[i]; alliens8.clear();
						logo3 = true; logoTimer = 0;
						logoText.setString(" LEVEL - [ 3 ] ");
					}
				}
			}

			// ============================================================
			// LEVEL THREE
			// phase1: enemies 1,2,3,4,5,6
			// phase2: two enemy7
			// phase3: enemy8 final boss
			// ============================================================
			else if (levelthree)
			{
				if (lv3phase1)
				{
					enemy1timer += dt;
					if (enemy1timer >= enemy1cooldown && enemy1count <= 2)
					{
						float rx = rand() % 900 + 50;
						alliens1.push_back(new enime(enemy1tex, rx, blast2tex, logofont, 100));
						enemy1timer = 0; enemy1count++;
					}
					enemy2timer += dt;
					if (enemy2timer >= enemy2cooldown && enemy2count <= 2)
					{
						float rx = rand() % 900 + 50;
						alliens2.push_back(new enime(enemy2tex, rx, blast2tex, logofont, 100));
						enemy2timer = 0; enemy2count++;
					}
					enemy3timer += dt;
					if (enemy3timer >= enemy3cooldown && enemy3count <= 3)
					{
						float rx = rand() % 900 + 50;
						alliens3.push_back(new enime(enemy3tex, rx, blast2tex, logofont, 150));
						enemy3timer = 0; enemy3count++;
					}
					enemy4timer += dt;
					if (enemy4timer >= enemy4cooldown && enemy4count <= 3)
					{
						float rx = rand() % 900 + 50;
						alliens4.push_back(new enime(enemy4tex, rx, blast2tex, logofont, 150));
						enemy4timer = 0; enemy4count++;
					}
					enemy5timer += dt;
					if (enemy5timer >= enemy5cooldown && enemy5count <= 2)
					{
						float rx = rand() % 900 + 50;
						alliens5.push_back(new enime(enemy5tex, rx, blast2tex, logofont, 100));
						enemy5timer = 0; enemy5count++;
					}
					enemy6timer += dt;
					if (enemy6timer >= enemy6cooldown && enemy6count <= 5)
					{
						float rx = rand() % 900 + 50;
						alliens6.push_back(new enime(enemy6tex, rx, blast2tex, logofont, 100));
						enemy6timer = 0; enemy6count++;
					}

					if (gkilled >= 46 && alliens1.empty() && alliens2.empty()
						&& alliens3.empty() && alliens4.empty() && alliens5.empty() && alliens6.empty())
					{
						lv3phase1 = false; lv3phase2 = true;
						lv3enemy7spawned = 0; etimer = 0;
					}
				}

				if (lv3phase2)
				{
					if (lv3enemy7spawned == 0)
					{
						enime* o1 = new enime(enemy7tex, 100, blast2tex, logofont, 300);
						o1->setsizeofenemy(350, 100);
						alliens7.push_back(o1);
						enime* o2 = new enime(enemy7tex, 550, blast2tex, logofont, 300);
						o2->setsizeofenemy(350, 100);
						alliens7.push_back(o2);
						lv3enemy7spawned = 2;
					}
					etimer += dt;
					if (etimer >= ecooldown)
					{
						for (int i = 0; i < (int)alliens7.size(); ++i)
						{
							if (!alliens7[i]->getbool())
							{
								sf::Vector2f spawnpos = { alliens7[i]->getposition().x + 150, alliens7[i]->getposition().y + 80 };
								enemybullet bulletobj(ebullet, space.getplayerposi(), spawnpos);
								alliens7[i]->bullet.push_back(bulletobj);
							}
						}
						etimer = 0;
					}
					if (gkilled >= 48 && alliens7.empty())
					{
						lv3phase2 = false; lv3phase3 = true;
						enemy8count = 0; e8timer1 = 0; e8timer2 = 0;
					}
				}

				if (lv3phase3)
				{
					if (enemy8count == 0)
					{
						enime* obj = new enime(enemy8tex, 300, blast2tex, logofont, 500);
						obj->setsizeofenemy(400, 100);
						alliens8.push_back(obj);
						enemy8count++;
					}
					if (!alliens8.empty() && !alliens8[0]->getbool() && alliens8[0]->getposition().y >= 120)
					{
						e8timer1 += dt;
						if (e8timer1 >= e8cooldown1)
						{
							sf::Vector2f spawnpos = { alliens8[0]->getposition().x + 200, alliens8[0]->getposition().y + 80 };
							enemybullet bulletobj(ebullet, space.getplayerposi(), spawnpos);
							alliens8[0]->bullet.push_back(bulletobj);
							e8timer1 = 0;
						}
						e8timer2 += dt;
						if (e8timer2 >= e8cooldown2)
						{
							sf::Vector2f spawnpos2 = { alliens8[0]->getposition().x + 200, alliens8[0]->getposition().y + 80 };
							enemybullet2 bulletobj2(ebullet3, space.getplayerposi(), spawnpos2);
							alliens8[0]->bullet2.push_back(bulletobj2);
							e8timer2 = 0;
						}
					}
					if (gkilled >= 49 && alliens8.empty())
					{
						levelthree = false; endgame = true;
						gamestate = 6;
						continue;
					}
				}
			}
			else if (endgame)
			{
				gamestate = 6;
				continue;
			}

			// ============================================================
			// MOVEMENT
			// ============================================================
			for (int i = 0; i < (int)alliens1.size(); ++i) alliens1[i]->enemymove(dt);
			for (int i = 0; i < (int)alliens2.size(); ++i) alliens2[i]->enemymove(dt);
			for (int i = 0; i < (int)alliens3.size(); ++i) alliens3[i]->enemymove(dt);
			for (int i = 0; i < (int)alliens4.size(); ++i) alliens4[i]->enemymove(dt);
			for (int i = 0; i < (int)alliens5.size(); ++i) alliens5[i]->enemymove(dt);
			for (int i = 0; i < (int)alliens6.size(); ++i) { alliens6[i]->enemymove(dt); alliens6[i]->rotateenemy(dt); }
			for (int i = 0; i < (int)alliens7.size(); ++i) if (alliens7[i]->getposition().y <= 110) alliens7[i]->enemymove(dt);
			for (int i = 0; i < (int)alliens8.size(); ++i) if (alliens8[i]->getposition().y < 120)  alliens8[i]->enemymove(dt);

			// ============================================================
			// ANIMATION + LABEL UPDATE
			// ============================================================
			for (int k = 0; k < (int)alliens1.size(); ++k) { alliens1[k]->animationenemy(dt, 2, 21, 21, 370, 438); alliens1[k]->updateLabel(dt); }
			for (int k = 0; k < (int)alliens2.size(); ++k) { alliens2[k]->animationenemy(dt, 2, 40, 86, 360, 287); alliens2[k]->updateLabel(dt); }
			for (int k = 0; k < (int)alliens3.size(); ++k) { alliens3[k]->animationenemy(dt, 2, 3, 26, 386, 186);  alliens3[k]->updateLabel(dt); }
			for (int k = 0; k < (int)alliens4.size(); ++k) { alliens4[k]->animationenemy(dt, 1, 1, 33, 200, 180);  alliens4[k]->updateLabel(dt); }
			for (int k = 0; k < (int)alliens5.size(); ++k) { alliens5[k]->animationenemy(dt, 2, 6, 28, 196, 168);  alliens5[k]->updateLabel(dt); }
			for (int k = 0; k < (int)alliens6.size(); ++k) { alliens6[k]->animationenemy(dt, 1, 0, 4, 55, 54);     alliens6[k]->updateLabel(dt); }
			for (int k = 0; k < (int)alliens7.size(); ++k) { alliens7[k]->animationenemy(dt, 1, 16, 8, 386, 194);  alliens7[k]->updateLabel(dt); }
			for (int k = 0; k < (int)alliens8.size(); ++k) { alliens8[k]->animationenemy(dt, 1, 145, 30, 563, 649); alliens8[k]->updateLabel(dt); }

			// ============================================================
			// LOWER WALL - trigger blast and reduce player HP by 50
			// ============================================================
			for (int i = (int)alliens1.size() - 1; i >= 0; --i) if (alliens1[i]->getposition().y >= 600 && !alliens1[i]->getbool()) { alliens1[i]->setblastsize(alliens1[i]->getshape()); alliens1[i]->setblastposition(); alliens1[i]->sethitcount(1); alliens1[i]->setbool(true); space.takeDamage(50); }
			for (int i = (int)alliens2.size() - 1; i >= 0; --i) if (alliens2[i]->getposition().y >= 600 && !alliens2[i]->getbool()) { alliens2[i]->setblastsize(alliens2[i]->getshape()); alliens2[i]->setblastposition(); alliens2[i]->sethitcount(1); alliens2[i]->setbool(true); space.takeDamage(50); }
			for (int i = (int)alliens3.size() - 1; i >= 0; --i) if (alliens3[i]->getposition().y >= 600 && !alliens3[i]->getbool()) { alliens3[i]->setblastsize(alliens3[i]->getshape()); alliens3[i]->setblastposition(); alliens3[i]->sethitcount(1); alliens3[i]->setbool(true); space.takeDamage(50); }
			for (int i = (int)alliens4.size() - 1; i >= 0; --i) if (alliens4[i]->getposition().y >= 600 && !alliens4[i]->getbool()) { alliens4[i]->setblastsize(alliens4[i]->getshape()); alliens4[i]->setblastposition(); alliens4[i]->sethitcount(1); alliens4[i]->setbool(true); space.takeDamage(50); }
			for (int i = (int)alliens5.size() - 1; i >= 0; --i) if (alliens5[i]->getposition().y >= 600 && !alliens5[i]->getbool()) { alliens5[i]->setblastsize(alliens5[i]->getshape()); alliens5[i]->setblastposition(); alliens5[i]->sethitcount(1); alliens5[i]->setbool(true); space.takeDamage(50); }
			for (int i = (int)alliens6.size() - 1; i >= 0; --i) if (alliens6[i]->getposition().y >= 600 && !alliens6[i]->getbool()) { alliens6[i]->setblastsize(alliens6[i]->getshape()); alliens6[i]->setblastposition(); alliens6[i]->sethitcount(1); alliens6[i]->setbool(true); space.takeDamage(50); }
			for (int i = (int)alliens7.size() - 1; i >= 0; --i) if (alliens7[i]->getposition().y >= 600 && !alliens7[i]->getbool()) { alliens7[i]->setblastsize(alliens7[i]->getshape()); alliens7[i]->setblastposition(); alliens7[i]->sethitcount(1); alliens7[i]->setbool(true); space.takeDamage(50); }
			for (int i = (int)alliens8.size() - 1; i >= 0; --i) if (alliens8[i]->getposition().y >= 600 && !alliens8[i]->getbool()) { alliens8[i]->setblastsize(alliens8[i]->getshape()); alliens8[i]->setblastposition(); alliens8[i]->sethitcount(1); alliens8[i]->setbool(true); space.takeDamage(50); }

			// ============================================================
			// PLAYER-ENEMY BODY COLLISION - reduce player HP by 50
			// ============================================================
			for (int i = (int)alliens1.size() - 1; i >= 0; --i) if (!alliens1[i]->getbool() && space.getshapepalyer().getGlobalBounds().findIntersection(alliens1[i]->getshape().getGlobalBounds())) { alliens1[i]->setblastsize(alliens1[i]->getshape()); alliens1[i]->setblastposition(); alliens1[i]->sethitcount(1); alliens1[i]->setbool(true); space.takeDamage(50); }
			for (int i = (int)alliens2.size() - 1; i >= 0; --i) if (!alliens2[i]->getbool() && space.getshapepalyer().getGlobalBounds().findIntersection(alliens2[i]->getshape().getGlobalBounds())) { alliens2[i]->setblastsize(alliens2[i]->getshape()); alliens2[i]->setblastposition(); alliens2[i]->sethitcount(1); alliens2[i]->setbool(true); space.takeDamage(50); }
			for (int i = (int)alliens3.size() - 1; i >= 0; --i) if (!alliens3[i]->getbool() && space.getshapepalyer().getGlobalBounds().findIntersection(alliens3[i]->getshape().getGlobalBounds())) { alliens3[i]->setblastsize(alliens3[i]->getshape()); alliens3[i]->setblastposition(); alliens3[i]->sethitcount(1); alliens3[i]->setbool(true); space.takeDamage(50); }
			for (int i = (int)alliens4.size() - 1; i >= 0; --i) if (!alliens4[i]->getbool() && space.getshapepalyer().getGlobalBounds().findIntersection(alliens4[i]->getshape().getGlobalBounds())) { alliens4[i]->setblastsize(alliens4[i]->getshape()); alliens4[i]->setblastposition(); alliens4[i]->sethitcount(1); alliens4[i]->setbool(true); space.takeDamage(50); }
			for (int i = (int)alliens5.size() - 1; i >= 0; --i) if (!alliens5[i]->getbool() && space.getshapepalyer().getGlobalBounds().findIntersection(alliens5[i]->getshape().getGlobalBounds())) { alliens5[i]->setblastsize(alliens5[i]->getshape()); alliens5[i]->setblastposition(); alliens5[i]->sethitcount(1); alliens5[i]->setbool(true); space.takeDamage(50); }
			for (int i = (int)alliens6.size() - 1; i >= 0; --i) if (!alliens6[i]->getbool() && space.getshapepalyer().getGlobalBounds().findIntersection(alliens6[i]->getshape().getGlobalBounds())) { alliens6[i]->setblastsize(alliens6[i]->getshape()); alliens6[i]->setblastposition(); alliens6[i]->sethitcount(1); alliens6[i]->setbool(true); space.takeDamage(50); }

			// ============================================================
			// BULLET MOVEMENT
			// ============================================================
			for (int i = 0; i < (int)firess.size(); ++i) firess[i].movebullet(dt);
			for (int i = 0; i < (int)firess2.size(); ++i) firess2[i].movebullet(dt);

			for (int k = 0; k < (int)alliens7.size(); ++k)
			{
				for (int i = 0; i < (int)alliens7[k]->bullet.size(); ++i) alliens7[k]->bullet[i].movebullet(dt);
				for (int i = (int)alliens7[k]->bullet.size() - 1; i >= 0; --i)
					if (alliens7[k]->bullet[i].getposition().y >= 650)
						alliens7[k]->bullet.erase(alliens7[k]->bullet.begin() + i);
			}
			if (!alliens8.empty())
			{
				for (int i = 0; i < (int)alliens8[0]->bullet.size(); ++i)  alliens8[0]->bullet[i].movebullet(dt);
				for (int i = (int)alliens8[0]->bullet.size() - 1; i >= 0; --i)
					if (alliens8[0]->bullet[i].getposition().y >= 650)
						alliens8[0]->bullet.erase(alliens8[0]->bullet.begin() + i);
				for (int i = 0; i < (int)alliens8[0]->bullet2.size(); ++i) alliens8[0]->bullet2[i].movebullet(dt);
				for (int i = (int)alliens8[0]->bullet2.size() - 1; i >= 0; --i)
					if (alliens8[0]->bullet2[i].getposition().y >= 650)
						alliens8[0]->bullet2.erase(alliens8[0]->bullet2.begin() + i);
			}

			// ============================================================
			// ENEMY BULLETS HIT PLAYER
			// ============================================================
			for (int k = 0; k < (int)alliens7.size(); ++k)
			{
				for (int i = (int)alliens7[k]->bullet.size() - 1; i >= 0; --i)
				{
					if (space.getshapepalyer().getGlobalBounds().findIntersection(alliens7[k]->bullet[i].getshape().getGlobalBounds()))
					{
						space.takeDamage(alliens7[k]->bullet[i].getPower());
						alliens7[k]->bullet.erase(alliens7[k]->bullet.begin() + i);
					}
				}
			}
			if (!alliens8.empty())
			{
				for (int i = (int)alliens8[0]->bullet.size() - 1; i >= 0; --i)
					if (space.getshapepalyer().getGlobalBounds().findIntersection(alliens8[0]->bullet[i].getshape().getGlobalBounds()))
					{
						space.takeDamage(alliens8[0]->bullet[i].getPower()); alliens8[0]->bullet.erase(alliens8[0]->bullet.begin() + i);
					}
				for (int i = (int)alliens8[0]->bullet2.size() - 1; i >= 0; --i)
					if (space.getshapepalyer().getGlobalBounds().findIntersection(alliens8[0]->bullet2[i].getshape().getGlobalBounds()))
					{
						space.takeDamage(alliens8[0]->bullet2[i].getPower()); alliens8[0]->bullet2.erase(alliens8[0]->bullet2.begin() + i);
					}
			}

			// ============================================================
			// SPACESHIP BULLET 1 vs ENEMIES
			// ============================================================
			for (int i = (int)firess.size() - 1; i >= 0; --i)
			{
				bool bulletdead = false;
				if (firess[i].getposition().y <= -10) { firess.erase(firess.begin() + i); continue; }

				if (!bulletdead)
				{
					for (int j = (int)alliens1.size() - 1; j >= 0; --j)
					{
						if (alliens1[j]->getbool()) continue;
						if (alliens1[j]->getshape().getGlobalBounds().findIntersection(firess[i].getshape().getGlobalBounds()))
						{
							alliens1[j]->takeDamage(firess[i].getPower());
							gdamageDealt += firess[i].getPower(); gshotsHit++;
							if (alliens1[j]->isDead()) { alliens1[j]->setblastsize(alliens1[j]->getshape()); alliens1[j]->setblastposition(); alliens1[j]->sethitcount(1); alliens1[j]->setbool(true); gscore += 50; }
							firess.erase(firess.begin() + i); bulletdead = true; break;
						}
					}
				}
				if (!bulletdead)
				{
					for (int j = (int)alliens2.size() - 1; j >= 0; --j)
					{
						if (alliens2[j]->getbool()) continue;
						if (alliens2[j]->getshape().getGlobalBounds().findIntersection(firess[i].getshape().getGlobalBounds()))
						{
							alliens2[j]->takeDamage(firess[i].getPower());
							gdamageDealt += firess[i].getPower(); gshotsHit++;
							if (alliens2[j]->isDead()) { alliens2[j]->setblastsize(alliens2[j]->getshape()); alliens2[j]->setblastposition(); alliens2[j]->sethitcount(1); alliens2[j]->setbool(true); gscore += 50; }
							firess.erase(firess.begin() + i); bulletdead = true; break;
						}
					}
				}
				if (!bulletdead)
				{
					for (int j = (int)alliens3.size() - 1; j >= 0; --j)
					{
						if (alliens3[j]->getbool()) continue;
						if (alliens3[j]->getshape().getGlobalBounds().findIntersection(firess[i].getshape().getGlobalBounds()))
						{
							alliens3[j]->takeDamage(firess[i].getPower());
							gdamageDealt += firess[i].getPower(); gshotsHit++;
							if (alliens3[j]->isDead()) { alliens3[j]->setblastsize(alliens3[j]->getshape()); alliens3[j]->setblastposition(); alliens3[j]->sethitcount(1); alliens3[j]->setbool(true); gscore += 100; }
							firess.erase(firess.begin() + i); bulletdead = true; break;
						}
					}
				}
				if (!bulletdead)
				{
					for (int j = (int)alliens4.size() - 1; j >= 0; --j)
					{
						if (alliens4[j]->getbool()) continue;
						if (alliens4[j]->getshape().getGlobalBounds().findIntersection(firess[i].getshape().getGlobalBounds()))
						{
							alliens4[j]->takeDamage(firess[i].getPower());
							gdamageDealt += firess[i].getPower(); gshotsHit++;
							if (alliens4[j]->isDead()) { alliens4[j]->setblastsize(alliens4[j]->getshape()); alliens4[j]->setblastposition(); alliens4[j]->sethitcount(1); alliens4[j]->setbool(true); gscore += 100; }
							firess.erase(firess.begin() + i); bulletdead = true; break;
						}
					}
				}
				if (!bulletdead)
				{
					for (int j = (int)alliens5.size() - 1; j >= 0; --j)
					{
						if (alliens5[j]->getbool()) continue;
						if (alliens5[j]->getshape().getGlobalBounds().findIntersection(firess[i].getshape().getGlobalBounds()))
						{
							alliens5[j]->takeDamage(firess[i].getPower());
							gdamageDealt += firess[i].getPower(); gshotsHit++;
							if (alliens5[j]->isDead()) { alliens5[j]->setblastsize(alliens5[j]->getshape()); alliens5[j]->setblastposition(); alliens5[j]->sethitcount(1); alliens5[j]->setbool(true); gscore += 50; }
							firess.erase(firess.begin() + i); bulletdead = true; break;
						}
					}
				}
				if (!bulletdead)
				{
					for (int j = (int)alliens6.size() - 1; j >= 0; --j)
					{
						if (alliens6[j]->getbool()) continue;
						if (alliens6[j]->getshape().getGlobalBounds().findIntersection(firess[i].getshape().getGlobalBounds()))
						{
							alliens6[j]->takeDamage(firess[i].getPower());
							gdamageDealt += firess[i].getPower(); gshotsHit++;
							if (alliens6[j]->isDead()) { alliens6[j]->setblastsize(alliens6[j]->getshape()); alliens6[j]->setblastposition(); alliens6[j]->sethitcount(1); alliens6[j]->setbool(true); gscore += 50; }
							firess.erase(firess.begin() + i); bulletdead = true; break;
						}
					}
				}
				if (!bulletdead)
				{
					for (int j = (int)alliens7.size() - 1; j >= 0; --j)
					{
						if (alliens7[j]->getbool()) continue;
						if (alliens7[j]->getshape().getGlobalBounds().findIntersection(firess[i].getshape().getGlobalBounds()))
						{
							alliens7[j]->takeDamage(firess[i].getPower());
							gdamageDealt += firess[i].getPower(); gshotsHit++;
							if (alliens7[j]->isDead()) { alliens7[j]->setblastsize(alliens7[j]->getshape()); alliens7[j]->setblastposition(); alliens7[j]->sethitcount(1); alliens7[j]->setbool(true); gscore += 200; }
							firess.erase(firess.begin() + i); bulletdead = true; break;
						}
					}
				}
				if (!bulletdead)
				{
					for (int j = (int)alliens8.size() - 1; j >= 0; --j)
					{
						if (alliens8[j]->getbool()) continue;
						if (alliens8[j]->getshape().getGlobalBounds().findIntersection(firess[i].getshape().getGlobalBounds()))
						{
							alliens8[j]->takeDamage(firess[i].getPower());
							gdamageDealt += firess[i].getPower(); gshotsHit++;
							if (alliens8[j]->isDead()) { alliens8[j]->setblastsize(alliens8[j]->getshape()); alliens8[j]->setblastposition(); alliens8[j]->sethitcount(1); alliens8[j]->setbool(true); gscore += 500; }
							firess.erase(firess.begin() + i); bulletdead = true; break;
						}
					}
				}
				if (bulletdead) continue;
			}

			// ============================================================
			// SPACESHIP BULLET 2 vs ENEMIES
			// ============================================================
			for (int i = (int)firess2.size() - 1; i >= 0; --i)
			{
				bool bulletdead = false;
				if (firess2[i].getposition().y <= -10) { firess2.erase(firess2.begin() + i); continue; }

				if (!bulletdead)
				{
					for (int j = (int)alliens1.size() - 1; j >= 0; --j)
					{
						if (alliens1[j]->getbool()) continue;
						if (alliens1[j]->getshape().getGlobalBounds().findIntersection(firess2[i].getshape().getGlobalBounds()))
						{
							alliens1[j]->takeDamage(firess2[i].getPower());
							gdamageDealt += firess2[i].getPower(); gshotsHit++;
							if (alliens1[j]->isDead()) { alliens1[j]->setblastsize(alliens1[j]->getshape()); alliens1[j]->setblastposition(); alliens1[j]->sethitcount(1); alliens1[j]->setbool(true); gscore += 50; }
							firess2.erase(firess2.begin() + i); bulletdead = true; break;
						}
					}
				}
				if (!bulletdead)
				{
					for (int j = (int)alliens2.size() - 1; j >= 0; --j)
					{
						if (alliens2[j]->getbool()) continue;
						if (alliens2[j]->getshape().getGlobalBounds().findIntersection(firess2[i].getshape().getGlobalBounds()))
						{
							alliens2[j]->takeDamage(firess2[i].getPower());
							gdamageDealt += firess2[i].getPower(); gshotsHit++;
							if (alliens2[j]->isDead()) { alliens2[j]->setblastsize(alliens2[j]->getshape()); alliens2[j]->setblastposition(); alliens2[j]->sethitcount(1); alliens2[j]->setbool(true); gscore += 50; }
							firess2.erase(firess2.begin() + i); bulletdead = true; break;
						}
					}
				}
				if (!bulletdead)
				{
					for (int j = (int)alliens3.size() - 1; j >= 0; --j)
					{
						if (alliens3[j]->getbool()) continue;
						if (alliens3[j]->getshape().getGlobalBounds().findIntersection(firess2[i].getshape().getGlobalBounds()))
						{
							alliens3[j]->takeDamage(firess2[i].getPower());
							gdamageDealt += firess2[i].getPower(); gshotsHit++;
							if (alliens3[j]->isDead()) { alliens3[j]->setblastsize(alliens3[j]->getshape()); alliens3[j]->setblastposition(); alliens3[j]->sethitcount(1); alliens3[j]->setbool(true); gscore += 100; }
							firess2.erase(firess2.begin() + i); bulletdead = true; break;
						}
					}
				}
				if (!bulletdead)
				{
					for (int j = (int)alliens4.size() - 1; j >= 0; --j)
					{
						if (alliens4[j]->getbool()) continue;
						if (alliens4[j]->getshape().getGlobalBounds().findIntersection(firess2[i].getshape().getGlobalBounds()))
						{
							alliens4[j]->takeDamage(firess2[i].getPower());
							gdamageDealt += firess2[i].getPower(); gshotsHit++;
							if (alliens4[j]->isDead()) { alliens4[j]->setblastsize(alliens4[j]->getshape()); alliens4[j]->setblastposition(); alliens4[j]->sethitcount(1); alliens4[j]->setbool(true); gscore += 100; }
							firess2.erase(firess2.begin() + i); bulletdead = true; break;
						}
					}
				}
				if (!bulletdead)
				{
					for (int j = (int)alliens5.size() - 1; j >= 0; --j)
					{
						if (alliens5[j]->getbool()) continue;
						if (alliens5[j]->getshape().getGlobalBounds().findIntersection(firess2[i].getshape().getGlobalBounds()))
						{
							alliens5[j]->takeDamage(firess2[i].getPower());
							gdamageDealt += firess2[i].getPower(); gshotsHit++;
							if (alliens5[j]->isDead()) { alliens5[j]->setblastsize(alliens5[j]->getshape()); alliens5[j]->setblastposition(); alliens5[j]->sethitcount(1); alliens5[j]->setbool(true); gscore += 50; }
							firess2.erase(firess2.begin() + i); bulletdead = true; break;
						}
					}
				}
				if (!bulletdead)
				{
					for (int j = (int)alliens6.size() - 1; j >= 0; --j)
					{
						if (alliens6[j]->getbool()) continue;
						if (alliens6[j]->getshape().getGlobalBounds().findIntersection(firess2[i].getshape().getGlobalBounds()))
						{
							alliens6[j]->takeDamage(firess2[i].getPower());
							gdamageDealt += firess2[i].getPower(); gshotsHit++;
							if (alliens6[j]->isDead()) { alliens6[j]->setblastsize(alliens6[j]->getshape()); alliens6[j]->setblastposition(); alliens6[j]->sethitcount(1); alliens6[j]->setbool(true); gscore += 50; }
							firess2.erase(firess2.begin() + i); bulletdead = true; break;
						}
					}
				}
				if (!bulletdead)
				{
					for (int j = (int)alliens7.size() - 1; j >= 0; --j)
					{
						if (alliens7[j]->getbool()) continue;
						if (alliens7[j]->getshape().getGlobalBounds().findIntersection(firess2[i].getshape().getGlobalBounds()))
						{
							alliens7[j]->takeDamage(firess2[i].getPower());
							gdamageDealt += firess2[i].getPower(); gshotsHit++;
							if (alliens7[j]->isDead()) { alliens7[j]->setblastsize(alliens7[j]->getshape()); alliens7[j]->setblastposition(); alliens7[j]->sethitcount(1); alliens7[j]->setbool(true); gscore += 200; }
							firess2.erase(firess2.begin() + i); bulletdead = true; break;
						}
					}
				}
				if (!bulletdead)
				{
					for (int j = (int)alliens8.size() - 1; j >= 0; --j)
					{
						if (alliens8[j]->getbool()) continue;
						if (alliens8[j]->getshape().getGlobalBounds().findIntersection(firess2[i].getshape().getGlobalBounds()))
						{
							alliens8[j]->takeDamage(firess2[i].getPower());
							gdamageDealt += firess2[i].getPower(); gshotsHit++;
							if (alliens8[j]->isDead()) { alliens8[j]->setblastsize(alliens8[j]->getshape()); alliens8[j]->setblastposition(); alliens8[j]->sethitcount(1); alliens8[j]->setbool(true); gscore += 500; }
							firess2.erase(firess2.begin() + i); bulletdead = true; break;
						}
					}
				}
				if (bulletdead) continue;
			}

			// ============================================================
			// BLAST ANIMATION + ERASE + KILL COUNT
			// ============================================================
			for (int i = 0; i < (int)alliens1.size(); ++i) if (alliens1[i]->getbool()) { alliens1[i]->startanimation(dt); if (!alliens1[i]->getbool()) { delete alliens1[i]; alliens1.erase(alliens1.begin() + i); gkilled++; } }
			for (int i = 0; i < (int)alliens2.size(); ++i) if (alliens2[i]->getbool()) { alliens2[i]->startanimation(dt); if (!alliens2[i]->getbool()) { delete alliens2[i]; alliens2.erase(alliens2.begin() + i); gkilled++; } }
			for (int i = 0; i < (int)alliens3.size(); ++i) if (alliens3[i]->getbool()) { alliens3[i]->startanimation(dt); if (!alliens3[i]->getbool()) { delete alliens3[i]; alliens3.erase(alliens3.begin() + i); gkilled++; } }
			for (int i = 0; i < (int)alliens4.size(); ++i) if (alliens4[i]->getbool()) { alliens4[i]->startanimation(dt); if (!alliens4[i]->getbool()) { delete alliens4[i]; alliens4.erase(alliens4.begin() + i); gkilled++; } }
			for (int i = 0; i < (int)alliens5.size(); ++i) if (alliens5[i]->getbool()) { alliens5[i]->startanimation(dt); if (!alliens5[i]->getbool()) { delete alliens5[i]; alliens5.erase(alliens5.begin() + i); gkilled++; } }
			for (int i = 0; i < (int)alliens6.size(); ++i) if (alliens6[i]->getbool()) { alliens6[i]->startanimation(dt); if (!alliens6[i]->getbool()) { delete alliens6[i]; alliens6.erase(alliens6.begin() + i); gkilled++; } }
			for (int i = 0; i < (int)alliens7.size(); ++i) if (alliens7[i]->getbool()) { alliens7[i]->startanimation(dt); if (!alliens7[i]->getbool()) { delete alliens7[i]; alliens7.erase(alliens7.begin() + i); gkilled++; } }
			for (int i = 0; i < (int)alliens8.size(); ++i) if (alliens8[i]->getbool()) { alliens8[i]->startanimation(dt); if (!alliens8[i]->getbool()) { delete alliens8[i]; alliens8.erase(alliens8.begin() + i); gkilled++; } }

			// ============================================================
			// PLAYER DEAD CHECK
			// ============================================================
			if (space.isDead())
			{
				gamestate = 6;
				continue;
			}

			// ============================================================
			// SPACESHIP MOVEMENT
			// ============================================================
			space.moveplayer(dt);

			// ============================================================
			// DRAWING
			// ============================================================
			spacegame.clear();

			if (levelone)   spacegame.draw(lv1Bg);
			else if (leveltwo)   spacegame.draw(lv2Bg);
			else if (levelthree) spacegame.draw(lv3Bg);
			else spacegame.draw(lv1Bg);

			for (int i = 0; i < (int)firess.size(); ++i) firess[i].drawbullet(spacegame);
			for (int i = 0; i < (int)firess2.size(); ++i) firess2[i].drawbullet(spacegame);

			for (int k = 0; k < (int)alliens7.size(); ++k)
				for (int i = 0; i < (int)alliens7[k]->bullet.size(); ++i)
					alliens7[k]->bullet[i].drawbullet(spacegame);

			if (!alliens8.empty())
			{
				for (int i = 0; i < (int)alliens8[0]->bullet.size(); ++i) alliens8[0]->bullet[i].drawbullet(spacegame);
				for (int i = 0; i < (int)alliens8[0]->bullet2.size(); ++i) alliens8[0]->bullet2[i].drawbullet(spacegame);
			}

			for (int i = 0; i < (int)alliens1.size(); ++i) { alliens1[i]->drawenemy(spacegame); alliens1[i]->drawHealthBar(spacegame); alliens1[i]->drawLabel(spacegame); }
			for (int i = 0; i < (int)alliens2.size(); ++i) { alliens2[i]->drawenemy(spacegame); alliens2[i]->drawHealthBar(spacegame); alliens2[i]->drawLabel(spacegame); }
			for (int i = 0; i < (int)alliens3.size(); ++i) { alliens3[i]->drawenemy(spacegame); alliens3[i]->drawHealthBar(spacegame); alliens3[i]->drawLabel(spacegame); }
			for (int i = 0; i < (int)alliens4.size(); ++i) { alliens4[i]->drawenemy(spacegame); alliens4[i]->drawHealthBar(spacegame); alliens4[i]->drawLabel(spacegame); }
			for (int i = 0; i < (int)alliens5.size(); ++i) { alliens5[i]->drawenemy(spacegame); alliens5[i]->drawHealthBar(spacegame); alliens5[i]->drawLabel(spacegame); }
			for (int i = 0; i < (int)alliens6.size(); ++i) { alliens6[i]->drawenemy(spacegame); alliens6[i]->drawHealthBar(spacegame); alliens6[i]->drawLabel(spacegame); }
			for (int i = 0; i < (int)alliens7.size(); ++i) { alliens7[i]->drawenemy(spacegame); alliens7[i]->drawHealthBar(spacegame); alliens7[i]->drawLabel(spacegame); }
			for (int i = 0; i < (int)alliens8.size(); ++i) { alliens8[i]->drawenemy(spacegame); alliens8[i]->drawHealthBar(spacegame); alliens8[i]->drawLabel(spacegame); }

			space.drawplayer(spacegame);
			space.drawHealthBar(spacegame);

			hpTxt.setString("HP: " + to_string(space.getHealth()));
			spacegame.draw(hpTxt);

			scoreTxt.setString("Score: " + to_string(gscore));
			spacegame.draw(scoreTxt);

			spacegame.draw(pauseBtn);
			spacegame.draw(pauseBtnTxt);

			spacegame.display();
		}

		// cleanup
		for (int i = 0; i < (int)alliens1.size(); i++) delete alliens1[i];
		for (int i = 0; i < (int)alliens2.size(); i++) delete alliens2[i];
		for (int i = 0; i < (int)alliens3.size(); i++) delete alliens3[i];
		for (int i = 0; i < (int)alliens4.size(); i++) delete alliens4[i];
		for (int i = 0; i < (int)alliens5.size(); i++) delete alliens5[i];
		for (int i = 0; i < (int)alliens6.size(); i++) delete alliens6[i];
		for (int i = 0; i < (int)alliens7.size(); i++) delete alliens7[i];
		for (int i = 0; i < (int)alliens8.size(); i++) delete alliens8[i];
	}
};

int main()
{
	game obj;
	obj.run();
	return 0;
}
