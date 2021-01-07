#pragma once

#include <Windows.h>
#include <map>
#include <set>
#include <vector>
#include "CScreen.h"

#define WIDTH  20
#define HEIGHT 20
//#define EMPTY '0'
//#define ROCK  '1'
//#define BOMB  '2'
//#define APPLE '3'






class gamemap
{
	
public:
	gamemap();
	~gamemap();
	char mp[HEIGHT][WIDTH];
	
	friend class Robot;
private:

};






class Robot
{
public:
	//map<int, int> localmap ;
	std::set<std::pair<int, int> > visited;    //открытые места карты, что в €чейках- смотрим по глобальной карте
	std::pair<int, int> cur;                   //текуща€ позици€ робота
	std::set<std::pair<int, int> > apples;     //€блоки
	std::set<std::pair<int, int> > bombs;      //бомбы
	//std::map<int, std::pair<int, int> > nodes; //номер хода, координаты
	int appleCount = 0;                        // общий счетчик €блок
	int appleGrab = 0;                         // кол-во собранных €блок
	int newMap = 0;                            //флаг на обновление карты
	bool iamLive = true;
	Robot();
	~Robot();
	int makestep(int move, gamemap& map);   //l/r/u/d/ возвращает 2 если бомба и 1 если не может пройти
	void scan(gamemap& map);  // 4 клетки вокруг в карту, флаг новой карты дл€ передачи общей
	void grab(gamemap &map);   //вз€ть ресурс, счетчик ресурсов увеличить
	void demining(gamemap& map); //разминирование
	

private:

};

class Game
{
private:
	bool run;
	int mode;
	gamemap gmap;
	Robot scout;
	CScreen screen;
	//Parser comand;
	//std::vector <std::pair< mode*, Robot*>> rob;
public:
	Game();
	int Execute();
	void showMap(gamemap gmap);
	void showCeil(std::pair<int, int> coord1, std::pair<int, int> coord2);
	void Exit();
};




class pathfinder
{
public:
	pathfinder();
	~pathfinder();

private:

};


class Parser
{
public:
	Parser();
	~Parser();

private:

};


