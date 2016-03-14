#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "Personage.h"
#include "Maps.h" //Подключение карты

using namespace std;
using namespace sf;

//Создание нужных прототипов классов
Texture personage;
Personage Player(personage);
Clock timers;
Image imageb;
Texture map_g;
Sprite s_map;
Font font;

//Обьявление глобальных переменных
float CurentFrame = 0;
float timer;
int HeroDirect = 0;
bool isMenu = true;


void Menu(RenderWindow &window);
void Game(RenderWindow &window);
void levelComplete(RenderWindow &window);

int main()
{
	RenderWindow window(sf::VideoMode(640, 672), "SFML Application", Style::None);

	Menu(window);

	return 0;
}

void Menu(RenderWindow & window)
{
	int dirMenu = 0;

	Texture menu1;
	Texture menu2;
	Texture fon;

	menu1.loadFromFile("game.png");
	menu2.loadFromFile("exit.png");
	fon.loadFromFile("fon.jpg");
	Sprite menu1_spr(menu1);
	Sprite menu2_spr(menu2);
	Sprite fon_spr(fon);
	menu1_spr.setPosition(224, 32);
	menu2_spr.setPosition(224, 96);
	fon_spr.setPosition(0, 0);
	cout << "Menu";
	window.clear();

	while (isMenu)
	{
		menu1_spr.setColor(Color::White);
		menu2_spr.setColor(Color::White);
		

		if (IntRect(224, 32, 192, 64).contains(Mouse::getPosition(window))) dirMenu = 1;
		else if (IntRect(224, 96, 192, 64).contains(Mouse::getPosition(window))) dirMenu = 2;

		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			if (dirMenu < 2) dirMenu++;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (dirMenu > 1) dirMenu--;
		}

		if (dirMenu == 1) menu1_spr.setColor(Color::Red);
		else if (dirMenu == 2) menu2_spr.setColor(Color::Red);

		if (Mouse::isButtonPressed(Mouse::Left) || Keyboard::isKeyPressed(Keyboard::Space))
		{
			if (dirMenu == 1)
			{
				isMenu = false;
				Player.inGame = true;
			}
			else if (dirMenu == 2) 
			{
				isMenu = false;
				window.close();
			}
		}

		window.draw(fon_spr);
		window.draw(menu1_spr);
		window.draw(menu2_spr);
		window.display();
	}
	Game(window);
}

void Game(RenderWindow & window)
{
	Levels(Player.level);
	personage.loadFromFile("personage.png");
	imageb.loadFromFile("Map.png");
	map_g.loadFromImage(imageb);
	s_map.setTexture(map_g);

	Texture fon;
	fon.loadFromFile("fon.jpg");
	Sprite fon_spr(fon);
	fon_spr.setPosition(0, 0);

	font.loadFromFile("font.otf");
	Text score("", font, 26);
	score.setPosition(5, 0);
	score.setColor(Color::White);

	Player.sprite.setPosition(Player.rect.left - 16, Player.rect.top + 16);

	while (Player.inGame) {

		timer = timers.getElapsedTime().asMicroseconds();
		timers.restart();
		timer /= 800;

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left)) Player.Update(timer, 1);
		else if (Keyboard::isKeyPressed(Keyboard::Right)) Player.Update(timer, 2);
		else if (Keyboard::isKeyPressed(Keyboard::Up)) Player.Update(timer, 3);
		else if (Keyboard::isKeyPressed(Keyboard::Down)) Player.Update(timer, 4);
		else if (Keyboard::isKeyPressed(Keyboard::B)) Player.createBomb();
		else if (Keyboard::isKeyPressed(Keyboard::V)) Player.delBomb();
		else if (Keyboard::isKeyPressed(Keyboard::Escape)) break;

		window.clear();

		//Функция для отрисовки карты
		for (int i(0); i < MAP_WIDTH; i++)
		{
			for (int j(0); j < MAP_HEIGHT; j++)
			{
				if (Map[i][j] == '*') s_map.setTextureRect(IntRect(0, 0, 32, 32));
				else if (Map[i][j] == '+') s_map.setTextureRect(IntRect(32, 0, 32, 32));
				else if (Map[i][j] == '_') s_map.setTextureRect(IntRect(64, 0, 32, 32));
				else if (Map[i][j] == 'S') s_map.setTextureRect(IntRect(96, 0, 32, 32));
				else if (Map[i][j] == 'F') s_map.setTextureRect(IntRect(128, 0, 32, 32));
				else if (Map[i][j] == 'b') s_map.setTextureRect(IntRect(160, 0, 32, 32));
				else if (Map[i][j] == 'B') s_map.setTextureRect(IntRect(192, 0, 32, 32));
				else if (Map[i][j] == 'V') s_map.setTextureRect(IntRect(224, 0, 32, 32));
				s_map.setPosition(j * 32, (i * 32) + 32);

				window.draw(s_map);
			}
		}
		ostringstream Score;
		Score << Player.UserScore << "  Level: " << Player.level << "   Bomb: " << Player.UserBomb;
		score.setString(L"Очки: " + Score.str());
		window.draw(score);
		window.draw(Player.sprite);
		window.display();
	}
	levelComplete(window);
	Player.inGame = false;
}

void levelComplete(RenderWindow &window)
{
	//Нужные переменные
	int dirMenu = 0;
	bool tap = false;

	Texture menu1;
	Texture menu2;
	Texture fon;

	menu1.loadFromFile("next-lvl.png");
	menu2.loadFromFile("exit.png");
	fon.loadFromFile("fon.jpg");
	Sprite menu1_spr(menu1);
	Sprite menu2_spr(menu2);
	Sprite fon_spr(fon);
	menu1_spr.setPosition(176, 32);
	menu2_spr.setPosition(224, 96);
	fon_spr.setPosition(0, 0);

	while (!Player.inGame)
	{
		menu1_spr.setColor(Color::White);
		menu2_spr.setColor(Color::White);

		if (MAx_LVL > Player.level)
		{
			if (IntRect(176, 32, 288, 40).contains(Mouse::getPosition(window))) dirMenu = 1;
			else if (IntRect(224, 96, 192, 64).contains(Mouse::getPosition(window))) dirMenu = 2;
		}
		else if (IntRect(224, 96, 192, 64).contains(Mouse::getPosition(window))) dirMenu = 2;
		
		if (Keyboard::isKeyPressed(Keyboard::Down)) 
		{
			if(dirMenu < 2) dirMenu++;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (dirMenu > 1) dirMenu--;
		}

		if (dirMenu == 1) menu1_spr.setColor(Color::Red);
		else if (dirMenu == 2) menu2_spr.setColor(Color::Red);

		if (Keyboard::isKeyPressed(Keyboard::Space) || Mouse::isButtonPressed(Mouse::Left))
		{
			if (dirMenu == 1)
			{
				Player.inGame = true;
				Levels(Player.level++);
				Game(window);
			}
			else if (dirMenu == 2)
			{
				Player.inGame = true;
				window.close();
			}
		}

		window.clear();

		window.draw(fon_spr);
		if(MAx_LVL > Player.level) window.draw(menu1_spr);
		window.draw(menu2_spr);

		window.display();
	}
}