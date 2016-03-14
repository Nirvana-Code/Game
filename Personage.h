#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include <stdio.h>
#include "Maps.h"

using namespace std;
using namespace sf;

class Personage
{
public:
	struct bomb
	{
		int x;
		int y;
	};

	bomb masBomb[400];
	int dx, dy;
	int UserScore;
	Sprite sprite;
	float leftFrame;
	float rightFrame;
	float upFrame;
	float downFrame;
	int level;
	int bombCount;
	FloatRect rect;
	int UserBomb;
	bool inGame = false; //Индикатор начала игры

	Personage(Texture &texture)
	{
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(32,0,32,32));

		rect = FloatRect(48, 48, 12, 12);
		leftFrame = 0;
		rightFrame = 0;
		upFrame = 0;
		downFrame = 0;
		UserScore = 0;
		inGame = true;
		level = 1;
		bombCount = 0;
		UserBomb = 2;
	}

	void Update(float time, int dir) //Функция для перемещения персонажа
	{
		if (dir == 1) //Зажата кнопка влево
		{
			rightFrame = 0;
			upFrame = 0;
			downFrame = 0;
			leftFrame += 0.005 * time;
			if (leftFrame > 3) leftFrame -= 3;
			sprite.setTextureRect(IntRect(int(leftFrame) * 32, 32, 32, 32));
			rect.left -= 0.05 * time;
		}
		else if (dir == 2) //Зажата кнопка вправо
		{
			leftFrame = 0;
			upFrame = 0;
			downFrame = 0;
			rightFrame += 0.005 * time;
			if (rightFrame > 3) rightFrame -= 3;
			sprite.setTextureRect(IntRect(int(rightFrame) * 32, 64, 32, 32));
			rect.left += 0.05 * time;
		}
		else if (dir == 3) //Зажата кнопка вверх
		{
			leftFrame = 0;
			rightFrame = 0;
			downFrame = 0;
			upFrame += 0.005 * time;
			if (upFrame > 3) upFrame -= 3;
			sprite.setTextureRect(IntRect(int(upFrame) * 32, 96, 32, 32));
			rect.top -= 0.05 * time;
		}
		else if (dir == 4) //Зажата кнопка вниз
		{
			leftFrame = 0;
			upFrame = 0;
			rightFrame = 0;
			downFrame += 0.005 * time;
			if (downFrame > 3) downFrame -= 3;
			sprite.setTextureRect(IntRect(int(downFrame) * 32, 0, 32, 32));
			rect.top += 0.05 * time;
		}
		Collision(dir);
		border(dir);
		sprite.setPosition(rect.left - 16, rect.top + 16);
	}
	
	void Collision(int dir) //Функция обработки столкновения
	{
		if (dir == 1)
		{
			int x = int((rect.left - rect.width - 8)/32);
			int wy = int((rect.top + rect.height)/32);
			int ny = int((rect.top - rect.height)/32);
			if(Map[wy][x] == '+' || Map[ny][x] == '+' || Map[wy][x] == '*' || Map[ny][x] == '*')
				rect.left = (x+1) * 32 + rect.width;
		}
		else if (dir == 2)
		{
			int x = int((rect.left + rect.width) / 32);
			int wy = int((rect.top - rect.height) / 32);
			int ny = int((rect.top + rect.height) / 32);
			if (Map[wy][x] == '+' || Map[ny][x] == '+' || Map[wy][x] == '*' || Map[ny][x] == '*')
				rect.left = x  * 32 - rect.width;
		}
		else if (dir == 3)
		{
			int y = int((rect.top - rect.height) / 32);
			int lx = int((rect.left - rect.width) / 32);
			int rx = int((rect.left + rect.width) / 32);
			if (Map[y][lx] == '+' || Map[y][rx] == '+' || Map[y][lx] == '*' || Map[y][rx] == '*')
				rect.top = (y + 1) * 32 + rect.height;
		}
		else if (dir == 4)
		{
			int y = int((rect.top + rect.height + 8) / 32);
			int lx = int((rect.left - rect.width) / 32);
			int rx = int((rect.left + rect.width) / 32);
			if (Map[y][lx] == '+' || Map[y][rx] == '+' || Map[y][lx] == '*' || Map[y][rx] == '*')
				rect.top = (y - 1) * 32 + rect.height;
		}

		if (Map[int(rect.top / 32)][int(rect.left / 32)] == 'S')
		{
			Map[int(rect.top / 32)][int(rect.left / 32)] = '_';
			UserScore++;
		} else if (Map[int(rect.top / 32)][int(rect.left / 32)] == 'F')
		{
			inGame = false;
		}

		cout << rect.top << ' ' << rect.left << '\n';
	}

	void border(int dir)
	{
		switch (dir)
		{
		case 2:
			{
				int y = rect.top / 32;
				int x = (rect.left / 32) - 1;
				if (Map[y][x] == 'b')
					Map[y][x] = 'B';
			}	
		}
	}

	void createBomb() //Функция будет создавать бомбу на поле
	{
		int x = int(rect.left / 32);
		int y = int(rect.top / 32);
		if (Map[y][x] == 'V')
		{
			UserBomb++;
			Map[y][x] = '_';
			masBomb[bombCount].x = 0;
			masBomb[bombCount].y = 0;
		}
		else
		{
			if (UserBomb > 0)
			{
				Map[y][x] = 'V';
				masBomb[bombCount].x = x;
				masBomb[bombCount].y = y;
				bombCount++;
			}
			UserBomb--;
		}
	}
	void delBomb() //Взврыв всех бомб на поле
	{
		if (bombCount)
		{
			for (int i(0); i < bombCount; i++)
			{
				int x = masBomb[i].x;
				int y = masBomb[i].y;
				if (x != 0 && y != 0)
				{
					if (Map[y - 1][x] == '+') Map[y - 1][x] = '_';
					if (Map[y - 1][x - 1] == '+') Map[y - 1][x - 1] = '_';
					if (Map[y + 1][x] == '+') Map[y + 1][x] = '_';
					if (Map[y + 1][x + 1] == '+') Map[y + 1][x + 1] = '_';
					if (Map[y - 1][x + 1] == '+') Map[y - 1][x + 1] = '_';
					if (Map[y + 1][x - 1] == '+') Map[y + 1][x - 1] = '_';
					if (Map[y][x - 1] == '+') Map[y][x - 1] = '_';
					if (Map[y][x + 1] == '+') Map[y][x + 1] = '_';
					Map[y][x] = '_';
					masBomb[i].x = 0;
					masBomb[i].y = 0;
				}
			}
			bombCount = 0;
		}
	}
};