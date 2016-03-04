#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <String>

using namespace sf;
using namespace std;

const int MAP_HEIGHT = 20; //высота карты
const int MAP_WIDTH = 20; //Ўирина карты

String Map[MAP_HEIGHT]
{
	"                    ",
	"                    ",
	"                    ",
	"                    ",
	"                    ",
	"                    ",
	"                    ",
	"                    ",
	"                    ",
	"                    ",
	"                    ",
	"                    ",
	"                    ",
	"                    ",
	"                    ",
	"                    ",
	"                    ",
	"                    ",
	"                    ",
	"                    ",
}; 

ifstream level;

void Levels(int lvl)
{
	switch (lvl)
	{
	case 1 :
		level.open("level1.lvl", ios::in);
		break;
	case 2:
		level.open("level2.lvl", ios::in);
		cout << "Level Up";
		break;
	default: cout << "”ровень не загружен!";
	}

	int height = 0;
	int width = 0;
	level >> height;
	level >> width;
	cout << height << ' ' << width << '\n';
	for (int i(0); i < height; i++)
	{
		for (int j(0); j < width; j++)
		{
			char a;
			level >> a;
			cout << a << ' ';
			Map[i][j] = a;
		}
		cout << '\n';
	}
	level.close();
}