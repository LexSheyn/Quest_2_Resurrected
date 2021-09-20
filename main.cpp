#include "stdafx.h"
#include "Game.h"

int main()
{
	setlocale(LC_ALL, ".1251"); // Печать кириллицы
	SetConsoleCP(1251); // Ввод с консоли в кодировке 1251
	SetConsoleOutputCP(1251); // Вывод на консоль в кодировке 1251, шрифт консоли Lucida Console или Consolas

	//Creating Game object
	Game game;

	//Game loop
	game.run();

	return 0;
}