// Robots.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <Windows.h>
#include <iostream>
#include "Robots.h"
#include <fstream>
#include <string>
#include "optionparser.h"
#include <conio.h>

#define WIDTH  20
#define HEIGHT 20

#define EMPTY '0'
#define ROCK  '1'
#define BOMB  '2'
#define APPLE '3'

#define UP        0
#define DOWN      1
#define LEFT      2
#define RIGHT     3
#define SCAN      4
#define GRAB      5
#define GRAB_ALL  6

#define MANUAL_MODE 0
#define SCAN_MODE   1
#define AUTO_MODE   2

int sTOi(std::string str) {
	int res = 0;

	for (unsigned int i = 0; i < str.length(); i++) {
		if (str[i] == '\n')
			break;
		else
			res = res * 10 + (int)(str[i] - '0');

	}
	return res;
}
Game::Game()
{
	run = true;
	mode = MANUAL_MODE;
	
}

int Game::Execute()
{
	//

	scout.scan(gmap);
	showMap(gmap);
	screen.pos_str(0, 20, "Robots,  WELCOME: (u/d/l/r/mode: scan/auto/manual ");
	
	while (run) {
		int step = 0;
		std::string input;
		int comand = 100;
		screen.pos(50, 20);
		screen.cursor_show(true);
		std::getline(std::cin, input);
		screen.cursor_show(false);
		screen.pos(50, 20);
		if (mode == MANUAL_MODE) {
			if (input == "u" || input == "U")
				comand = UP;
			else if (input == "d" || input == "D")
				comand = DOWN;
			else if (input == "l" || input == "L")
				comand = LEFT;
			else if (input == "r" || input == "R")
				comand = RIGHT;
			else if (input == "scan" || input == "Scan" || input == "SCAN")
				comand = SCAN;
			else if (input == "grab" || input == "Grab" || input == "GRAB")
				comand = GRAB;
			else if (input.substr(0, 8) == "set_mode" || input.substr(0, 8) == "Set_mode" || input.substr(0, 8) == "SET_MODE") {
				if (input.substr(9, 4) == "scan" || input.substr(9, 4) == "Scan" || input.substr(9, 4) == "SCAN") {
					mode = SCAN_MODE;
					step = sTOi(input.substr(13));
				}
				if (input.substr(0, 4) == "auto" || input.substr(0, 4) == "Auto" || input.substr(0, 4) == "AUTO") {
					comand = GRAB_ALL;
				}

			}

		}
		else if (mode == SCAN_MODE) {
			if (input.substr(0, 4) == "scan" || input.substr(0, 4) == "Scan" || input.substr(0, 4) == "SCAN") {
				step = sTOi(input.substr(5));
				comand = SCAN;
			}
			if (input.substr(0, 8) == "set_mode" || input.substr(0, 8) == "Set_mode" || input.substr(0, 8) == "SET_MODE") {
				if (input.substr(9, 6) == "manual" || input.substr(9, 6) == "Manual" || input.substr(9, 6) == "MANUAL") {
					mode = MANUAL_MODE;

				}
				if (input.substr(9, 4) == "auto" || input.substr(0, 4) == "Auto" || input.substr(0, 4) == "AUTO") {
					comand = GRAB_ALL;
				}
			}

		}
		else if (mode == AUTO_MODE) {
			if (input.substr(0, 4) == "auto" || input.substr(0, 4) == "Auto" || input.substr(0, 4) == "AUTO") {
				comand = GRAB_ALL;
			}
			if (input.substr(0, 8) == "set_mode" || input.substr(0, 8) == "Set_mode" || input.substr(0, 8) == "SET_MODE") {
				if (input.substr(9, 6) == "manual" || input.substr(9, 6) == "Manual" || input.substr(9, 6) == "MANUAL") {
					mode = MANUAL_MODE;

				}
				if (input.substr(9, 4) == "scan" || input.substr(9, 4) == "Scan" || input.substr(9, 4) == "SCAN") {
					mode = SCAN_MODE;
					comand = SCAN;
					step = sTOi(input.substr(13));
				}

			}
		}



		if (comand == LEFT || comand == RIGHT || comand == UP || comand == DOWN) {
			std::pair<int, int> coord1 = scout.cur;
			int res = scout.makestep(comand, gmap);
			if (res == 2) {
				run = false;  //подорвался
				screen.pos_str(50, 20, "You are lost Robot-scout ");
				scout.cur = { 0,0 };
			}
				
			//отрисовка 2 позиций- прежней и текущей
			showCeil(coord1, scout.cur);
		}
		else if (comand == SCAN) {
			if (mode == MANUAL_MODE) {
				scout.scan(gmap);
				showMap(gmap);
			}
			else {
				for (int i = 0; i < step; i++) {
					//создать копию открытой карты и идти в неизведанные районы
					scout.scan(gmap);
					showMap(gmap);
				}

			}


		}
		screen.pos_str(0, 20, "Robots,  WELCOME: (u/d/l/r/mode: scan/auto/manual                                               ");
	}

	
	
	


	return 0;
}

void Game::Exit()
{
	run = false;
}


void Game::showMap(gamemap gmap) {
	std::pair<int, int> scf;
	for (auto it = scout.visited.begin(); it != scout.visited.end(); it++) {
		scf = *it;		
		char symb = gmap.mp[scf.first][scf.second];
		if (symb == APPLE)
			screen.pos(scf.second, scf.first, 'A');
		if (symb == ROCK)
			screen.pos(scf.second, scf.first, 'R');
		if (symb == EMPTY)
			screen.pos(scf.second, scf.first, '_');
		if (symb == BOMB)
			screen.pos(scf.second, scf.first, 'B');
		if (scout.cur == scf)
			screen.pos(scf.second, scf.first, 1); //сам робот

	}
	
}

void Game::showCeil(std::pair<int, int> coord1, std::pair<int, int> coord2) {
	auto x    = coord1.first;
	auto y    = coord1.second;
	char symb = gmap.mp[x][y];
	if (symb == APPLE)
		screen.pos(y, x, 'A');
	if (symb == ROCK)
		screen.pos(y, x, 'R');
	if (symb == EMPTY)
		screen.pos(y, x, '_');
	if (symb == BOMB)
		screen.pos(y, x, 'B');
	if (scout.cur == coord1)
		screen.pos(y, x, 1); //сам робот
	x    = coord2.first;
	y    = coord2.second;
	symb = gmap.mp[x][y];
	if (symb == APPLE)
		screen.pos(y, x, 'A');
	if (symb == ROCK)
		screen.pos(y, x, 'R');
	if (symb == EMPTY)
		screen.pos(y, x, '_');
	if (symb == BOMB)
		screen.pos(y, x, 'B');
	if (scout.cur == coord2)
		screen.pos(y, x, 1); //сам робот
}


gamemap::gamemap() {
std::ifstream file("map.txt");
	
    for ( int i = 0; i < HEIGHT; i++) {
		for ( int j = 0; j < WIDTH; j++) {
			file >> mp[i][j];
			
		}
	}
	file.close();

}
gamemap::~gamemap()
{
}


Robot::Robot() {
	// стоит на поле 0,0
	
	cur = { 0, 0 };
	visited.insert(cur);
}

Robot::~Robot()
{
}

void Robot::scan(gamemap& map) {
  //с текущ позиции отсканировать 4 клетки, если возможно
	std::pair<int, int> scf= cur;
	int openPl = 0;

	if (--scf.first >= 0) 
		if (!visited.count(scf)) {
			visited.insert(scf);
			
			if (map.mp[scf.first][scf.second] == APPLE) {
				appleCount++;
				apples.insert(scf);
			}
			if (map.mp[scf.first][scf.second] == BOMB) {
				bombs.insert(scf);
			}
			openPl++;
		}
	scf = cur;
	if (++scf.first < HEIGHT) {
		if (!visited.count(scf)) {
			visited.insert(scf);
			if (map.mp[scf.first][scf.second] == APPLE) {
				appleCount++;
				apples.insert(scf);
			}
			if (map.mp[scf.first][scf.second] == BOMB) {
				bombs.insert(scf);
			}
			openPl++;
		}
	}
	scf = cur;
	if (--scf.second >= 0) {
		if (!visited.count(scf)) {
			visited.insert(scf);
			if (map.mp[scf.first][scf.second] == APPLE) {
				appleCount++;
				apples.insert(scf);
			}
			if (map.mp[scf.first][scf.second] == BOMB) {
				bombs.insert(scf);
			}
			openPl++;
		}
	}
	scf = cur;
	if (++scf.second < WIDTH) {
		if (!visited.count(scf)) {
			visited.insert(scf);
			if (map.mp[scf.first][scf.second] == APPLE) {
				appleCount++;
				apples.insert(scf);
			}
			if (map.mp[scf.first][scf.second] == BOMB) {
				bombs.insert(scf);
			}
			openPl++;
		}
	}

	if (openPl > 0)  //флаг на обновление
		newMap++;

}

void Robot::grab(gamemap &map) {
	if (map.mp[cur.first][cur.second] == APPLE) {
		//очистить от яблока
		map.mp[cur.first][cur.second] = EMPTY;
		appleGrab++;
		appleCount--;
		apples.erase(cur);
		
	}
	
}
void Robot::demining(gamemap& map) {
	if (map.mp[cur.first][cur.second] == BOMB) {
		//очистить от яблока
		map.mp[cur.first][cur.second] = EMPTY;
		bombs.erase(cur);

	}

}

int Robot:: makestep(int mode, gamemap& map) {  //0-OK, 1-нельзя сделать шаг, 2-bomb
	std::pair<int, int> scf = cur;

	switch (mode) {
	case LEFT:
		
		--scf.second;
		break;
	case RIGHT:
		//--перерисовка робота later
		++scf.second;
		break;
	case UP:
		//--перерисовка робота later
		--scf.first;
		break;
	case DOWN:
		//--перерисовка робота later
		++scf.first;
		break;
	}
	//==проверка на входимость в поле

	if (!visited.count(scf))
		return 1;
	if (map.mp[cur.first][cur.second] == BOMB) 
		return 2;
	if (map.mp[cur.first][cur.second] == ROCK)
		return 1;
	//--перерисовка робота later
	cur = scf;
	return 0;

}




pathfinder::pathfinder()
{
}

pathfinder::~pathfinder()
{
}
Parser::Parser()
{
}

Parser::~Parser()
{
}
