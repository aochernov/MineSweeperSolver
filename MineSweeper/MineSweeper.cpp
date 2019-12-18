#include <iostream>
#include <fstream>
#include <algorithm>
#include "SAT.h"

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

PropositionalFormula* getFormula(std::vector<std::vector<int>> &map)
{
	PropositionalAnd* conjunction = new PropositionalAnd();
	for (int i = 1; i <= 5; i++)
	{
		for (int j = 1; j <= 5; j++)
		{
			if (map[i][j] >= 0 && map[i][j] <= 8)
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
						else if (map[y][x] > 100)
						{
							neighbours.push_back("x" + std::to_string(map[y][x] % 100));
						}
						else if (map[y][x] == 9)
						{
							neighbours.push_back("1");
						}
						else
						{
							neighbours.push_back("0");
						}
					}
				}
				conjunction->addConjunct(cellConsistencyFormula(neighbours, map[i][j]));
			}
		}
	}
	return conjunction;
}

int cellCheck(std::vector<std::vector<int>> map)
{
	map[3][3] = 9;
	int variable = 101;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (map[i][j] == -1)
			{
				map[i][j] = variable;
				variable++;
			}
		}
	}
	bool canBeMine = SAT(getFormula(map)->getSimplified()).solve();
	map[3][3] = 10;
	bool canBeEmpty = SAT(getFormula(map)->getSimplified()).solve();
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

bool singleRun(std::vector<std::vector<int>> &map, bool &isConsistent)
{
	isConsistent = true;
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
					isConsistent = false;
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

bool consistencyCheck(std::vector<std::vector<int>> map)
{
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			if ((map[i][j] >= 0) && (map[i][j] <= 8))
			{
				int counter = 1;
				std::vector<std::string> neighbours;
				for (int y = i - 1; y <= i + 1; y++)
				{
					for (int x = j - 1; x <= j + 1; x++)
					{
						if ((y == i) && (x == j))
						{
							continue;
						}
						else if ((y < 0) || (x < 0) || (y > map.size()) || (x > map[i].size()))
						{
							neighbours.push_back("0");
						}
						else if (map[y][x] == -1)
						{
							neighbours.push_back("x" + std::to_string(counter));
							counter++;
						}
						else if (map[y][x] == 9)
						{
							neighbours.push_back("1");
						}
						else
						{
							neighbours.push_back("0");
						}
					}
				}
				if (!SAT(cellConsistencyFormula(neighbours, map[i][j])->getSimplified()).solve())
				{
					return false;
				}
			}
		}
	}
	return true;
}

int main(int argc, char *argv[])
{
	std::vector<std::vector<int>> map = parseFile(argv[1]);
	printMap(map);
	bool isConsistent = consistencyCheck(map);
	while (isConsistent && singleRun(map, isConsistent));
	if (isConsistent)
	{
		printMap(map);
	}
	else
	{
		std::cout << "Map is inconsistent\n";
	}
}