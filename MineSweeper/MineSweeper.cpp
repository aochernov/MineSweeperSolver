#include <iostream>
#include <fstream>
#include <algorithm>
#include "SAT.h"

std::vector<std::vector<std::string>> parseFile(std::string name, int border = 0)
{
	std::vector<std::vector<std::string>> result;
	int undefinedCounter = 1;
	std::string line;
	std::ifstream myfile(name);
	if (myfile.is_open())
	{
		std::getline(myfile, line);
		std::vector<std::string> emptyLine(2 * border, " ");
		for (int i = 0; i < line.size(); i += 2)
		{
			emptyLine.push_back(" ");
		}
		for (int i = 0; i < border; i++)
		{
			result.push_back(emptyLine);
		}
		do {
			std::vector<std::string> parsedLine;
			for (int i = 0; i < border; i++)
			{
				parsedLine.push_back(" ");
			}
			for (int i = 0; i < line.size(); i += 2)
			{
				if (line[i] == '*')
				{
					parsedLine.push_back("x" + std::to_string(undefinedCounter));
					undefinedCounter++;
				}
				else
				{
					parsedLine.push_back({ line[i] });
				}
			}
			for (int i = 0; i < border; i++)
			{
				parsedLine.push_back(" ");
			}
			result.push_back(parsedLine);
		} while (std::getline(myfile, line));
		for (int i = 0; i < border; i++)
		{
			result.push_back(emptyLine);
		}
		myfile.close();
	}
	return result;
}

void printMap(std::vector<std::vector<std::string>> &map, int border = 0)
{
	for (int i = border; i < map.size() - border; i++)
	{
		for (int j = border; j < map[i].size() - border; j++)
		{
			if (map[i][j][0] == 'x')
			{
				std::cout << "*\t";
			}
			else
			{
				std::cout << map[i][j] << "\t";
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

PropositionalFormula* cellConsistencyFormula(std::vector<std::string> &neighbours, int value)
{
	std::vector<bool> permutation = { false, false, false, false, false, false, false, false };
	for (int i = 0; i < value; i++)
	{
		permutation[7 - i] = true;
	}
	PropositionalOr* disjunction = new PropositionalOr();
	do {
		PropositionalAnd* conjunction = new PropositionalAnd();
		for (int i = 0; i < 8; i++)
		{
			conjunction->addConjunct(new PropositionalFormula(permutation[i], neighbours[i]));
		}
		disjunction->addDisjunct(conjunction);
	} while (std::next_permutation(permutation.begin(), permutation.end()));
	return disjunction;
}

std::vector<std::string> getNeighbours(std::vector<std::vector<std::string>> &map, int i, int j)
{
	std::vector<std::string> neighbours;
	for (int y = i - 1; y <= i + 1; y++)
	{
		for (int x = j - 1; x <= j + 1; x++)
		{
			if ((y == i) && (x == j))
			{
				continue;
			}
			else if (map[y][x][0] == 'x')
			{
				neighbours.push_back(map[y][x]);
			}
			else if (map[y][x][0] == 'm')
			{
				neighbours.push_back("1");
			}
			else
			{
				neighbours.push_back("0");
			}
		}
	}
	return neighbours;
}

bool consistencyCheck(std::vector<std::vector<std::string>> &map)
{
	for (int i = 3; i < map.size() - 3; i++)
	{
		for (int j = 3; j < map[i].size() - 3; j++)
		{
			if (map[i][j][0] >= '0' && map[i][j][0] <= '8')
			{
				std::vector<std::string> neighbours = getNeighbours(map, i, j);
				if (!SAT(cellConsistencyFormula(neighbours, map[i][j][0] - '0')->getSimplified()).solve())
				{
					return false;
				}
			}
		}
	}
	return true;
}

PropositionalFormula* getFormula(std::vector<std::vector<std::string>> &map, int y, int x)
{
	PropositionalAnd* conjunction = new PropositionalAnd();
	for (int i = y - 2; i <= y + 2; i++)
	{
		for (int j = x - 2; j <= x + 2; j++)
		{
			if (map[i][j][0] >= '0' && map[i][j][0] <= '8')
			{
				std::vector<std::string> neighbours = getNeighbours(map, i, j);
				conjunction->addConjunct(cellConsistencyFormula(neighbours, map[i][j][0] - '0'));
			}
		}
	}
	return conjunction;
}

void cellCheck(std::vector<std::vector<std::string>> &map, int y, int x)
{
	std::string undefined = map[y][x];
	map[y][x] = "m";
	bool canBeMine = SAT(getFormula(map, y, x)->getSimplified()).solve();
	map[y][x] = "E";
	bool canBeEmpty = SAT(getFormula(map, y, x)->getSimplified()).solve();
	if (!canBeEmpty && canBeMine)
	{
		map[y][x] = "m";
	}
	else if (canBeEmpty && canBeMine)
	{
		map[y][x] = undefined;
	}
	else if (!canBeEmpty && !canBeMine)
	{
		map[y][x] = "U";
	}
}

bool singleRun(std::vector<std::vector<std::string>> &map, bool &isConsistent)
{
	isConsistent = true;
	bool updated = false;
	for (int i = 3; i < map.size() - 3; i++)
	{
		for (int j = 3; j < map[i].size() - 3; j++)
		{
			if (map[i][j][0] == 'x')
			{
				cellCheck(map, i, j);
				if (map[i][j][0] == 'U')
				{
					isConsistent = false;
					return false;
				}
				else if (map[i][j][0] != 'x')
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
	std::vector<std::vector<std::string>> map = parseFile(argv[1], 3);
	printMap(map, 3);
	bool isConsistent = consistencyCheck(map);
	while (isConsistent && singleRun(map, isConsistent));
	if (isConsistent)
	{
		printMap(map, 3);
	}
	else
	{
		std::cout << "Map is inconsistent\n";
	}
}