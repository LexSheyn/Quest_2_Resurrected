#include "stdafx.h"
#include "Game.h"

int main()
{
	setlocale(LC_ALL, ".1251"); // ������ ���������
	SetConsoleCP(1251); // ���� � ������� � ��������� 1251
	SetConsoleOutputCP(1251); // ����� �� ������� � ��������� 1251, ����� ������� Lucida Console ��� Consolas

	//Creating Game object
	Game game;

	//Game loop
	game.run();

	return 0;
}