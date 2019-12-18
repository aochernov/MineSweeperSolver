#pragma once
#include "PropositionalFormula.h"
#include <map>

class SAT
{
private:
	std::vector<std::vector<std::string>> CNF;
	int solution;
	void tseytinTransform(PropositionalFormula* formula);
	std::map<std::string, int> getLiterals();
	bool DPLL(std::string chosen, std::map<std::string, int> literals, std::vector<std::vector<std::string>> cnf);

public:
	SAT(PropositionalFormula* formula);
	bool solve();
};

