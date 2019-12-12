#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<std::vector<int>> parseFile(std::string name)
{
	std::vector<std::vector<int>> result;
	std::string line;
	std::ifstream myfile(name);
	if (myfile.is_open())
	{
		while (std::getline(myfile, line))
		{
			std::vector<int> parsedLine;
			for (int i = 0; i < line.size(); i += 2)
			{
				if (line[i] == 'm')
				{
					parsedLine.push_back(9);
				}
				else if (line[i] == '*')
				{
					parsedLine.push_back(-1);
				}
				else
				{
					parsedLine.push_back(line[i] - '0');
				}
			}
			result.push_back(parsedLine);
		}
		myfile.close();
	}
	return result;
}

void printMap(std::vector<std::vector<int>> map)
{
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			switch (map[i][j])
			{
			case -1:
				std::cout << "*\t";
				break;
			case 9:
				std::cout << "m\t";
				break;
			case 10:
				std::cout << "E\t";
				break;
			default:
				std::cout << map[i][j] << "\t";
				break;
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

void countNeighbours(int i, int j, std::vector<std::vector<int>> map, int &mines, int &undefined)
{
	mines = 0;
	undefined = 0;
	for (int y = i - 1; y <= i + 1; y++)
	{
		for (int x = j - 1; x <= j + 1; x++)
		{
			if (map[y][x] == -1)
			{
				undefined++;
			}
			else if (map[y][x] == 9)
			{
				mines++;
			}
		}
	}
}

bool consistencyCheck(std::vector<std::vector<int>> map)
{
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			if ((map[i][j] >= 0) && (map[i][j] <= 8))
			{
				std::vector<std::vector<int>> window;
				for (int y = i - 1; y <= i + 1; y++)
				{
					std::vector<int> line;
					if ((y < 0) || (y >= map.size()))
					{
						for (int k = 0; k < 3; k++)
						{
							line.push_back(-2);
						}
					}
					else
					{
						for (int x = j - 1; x <= j + 1; x++)
						{
							if ((x < 0) || (x >= map[i].size()))
							{
								line.push_back(-2);
							}
							else
							{
								line.push_back(map[y][x]);
							}
						}
					}
					window.push_back(line);
				}
				int mines, undefined;
				countNeighbours(1, 1, window, mines, undefined);
				if (mines > map[i][j])
				{
					return false;
				}
				else if ((mines + undefined) < map[i][j])
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool countCheck(std::vector<std::vector<int>> map)
{
	int mines = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (map[i][j] == 9)
			{
				mines++;
			}
		}
	}
	return (mines == map[1][1]);
}

bool canBeSolved(std::vector<std::vector<int>> map, int vars)
{
	if (vars != 0)
	{
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (map[i][j] == -1)
				{
					map[i][j] = 9;
					if (canBeSolved(map, vars - 1))
					{
						return true;
					}
					map[i][j] = 10;
					return canBeSolved(map, vars - 1);
				}
			}
		}
	}
	else
	{
		for (int i = 1; i <= 5; i++)
		{
			for (int j = 1; j <= 5; j++)
			{
				if (map[i][j] != 9 && map[i][j] != -2 && map[i][j] != 10)
				{
					std::vector<std::vector<int>> window;
					for (int y = i - 1; y <= i + 1; y++)
					{
						std::vector<int> line;
						for (int x = j - 1; x <= j + 1; x++)
						{
							line.push_back(map[y][x]);
						}
						window.push_back(line);
					}
					if (!countCheck(window))
					{
						return false;
					}
				}
			}
		}
		return true;
	}
}

int cellCheck(std::vector<std::vector<int>> map)
{
	int undefined = -1;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (map[i][j] == -1)
			{
				undefined++;
			}
		}
	}
	map[3][3] = 9;
	bool canBeMine = canBeSolved(map, undefined);
	map[3][3] = 10;
	bool canBeEmpty = canBeSolved(map, undefined);
	if (canBeEmpty && !canBeMine)
	{
		return 10;
	}
	else if (canBeMine && !canBeEmpty)
	{
		return 9;
	}
	else if (canBeMine && canBeEmpty)
	{
		return -1;
	}
	else
	{
		return -3;
	}
}

bool singleRun(std::vector<std::vector<int>> &map, bool &inconsistent)
{
	inconsistent = false;
	bool updated = false;
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			if (map[i][j] < 0)
			{
				std::vector<std::vector<int>> window;
				for (int y = i - 3; y <= i + 3; y++)
				{
					std::vector<int> line;
					if ((y < 0) || (y >= map.size()))
					{
						for (int k = 0; k < 7; k++)
						{
							line.push_back(-2);
						}
					}
					else
					{
						for (int x = j - 3; x <= j + 3; x++)
						{
							if ((x < 0) || (x >= map[i].size()))
							{
								line.push_back(-2);
							}
							else
							{
								line.push_back(map[y][x]);
							}
						}
					}
					window.push_back(line);
				}
				map[i][j] = cellCheck(window);
				if (map[i][j] == -3)
				{
					inconsistent = true;
					return false;
				}
				else if (map[i][j] != -1)
				{
					updated = true;
				}
			}
		}
	}
	return updated;
}

int main(int argc, char *argv[])
{
	std::vector<std::vector<int>> map = parseFile(argv[1]);
	printMap(map);
	if (!consistencyCheck(map))
	{
		std::cout << "Map is inconsistent\n";
		return 0;
	}
	bool inconsistency;
	while (singleRun(map, inconsistency));
	if (inconsistency)
	{
		std::cout << "Map is inconsistent\n";
	}
	else
	{
		printMap(map);
	}
}