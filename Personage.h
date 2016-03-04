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
	int dx, dy;
	int UserScore;
	Sprite sprite;
	float leftFrame;
	float rightFrame;
	float upFrame;
	float downFrame;
	int level;
	FloatRect rect;
	bool GameStart = false;

	Personage(Texture &texture)
	{
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(32,0,32,32));
		rect = FloatRect(32, 32, 32, 32);
		dx = dy = 0.1;
		leftFrame = 0;
		rightFrame = 0;
		upFrame = 0;
		downFrame = 0;
		UserScore = 0;
		GameStart = true;
		level = 1;
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
		sprite.setPosition(rect.left, rect.top +32);
	}
	
	void Collision(int dir) //Функция обработки столкновения
	{

		for (int i = (rect.top) / 32; i < ((rect.top + rect.height) / 32); i++)
			for (int j = rect.left / 32; j < ((rect.left + rect.width) / 32); j++)
			{
				if (Map[i][j] == '*') //Столкновение с стенкой
				{
					if (dir == 1) rect.left = j * 32 + rect.width;
					if (dir == 2) rect.left = j * 32 - rect.width;
					if (dir == 3) rect.top = i * 32 + rect.height;
					if (dir == 4) rect.top = i * 32 - rect.height;
				}
				else if (Map[i][j] == '+') //Столкновение с блоком
				{
					if (dir == 1) rect.left = j * 32 + rect.width;
					if (dir == 2) rect.left = j * 32 - rect.width;
					if (dir == 3) rect.top = i * 32 + rect.height;
					if (dir == 4) rect.top = i * 32 - rect.height;
				}
				else if (Map[i][j] == 'S')
				{
					Map[i][j] = '_';
					UserScore++;
				}
				else if (Map[i][j] == 'F')
				{
					GameStart = false;
				}
				else if (Map[i][j] == 'B') //Столкновение с блоком
				{
					if (dir == 1) rect.left = j * 32 + rect.width;
					if (dir == 2) rect.left = j * 32 - rect.width;
					if (dir == 3) rect.top = i * 32 + rect.height;
					if (dir == 4) rect.top = i * 32 - rect.height;
				}
			}
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

	void Stats() {

	}
};