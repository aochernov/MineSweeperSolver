#include "SAT.h"

SAT::SAT(PropositionalFormula* formula)
{
	solution = formula->getValue();
	if (solution == -1)
	{
		tseytinTransform(formula);
	}
}

void SAT::tseytinTransform(PropositionalFormula* formula)
{
	int newVariable = 1;
	std::vector<PropositionalFormula*> children = {formula};
	CNF = { {formula->getAlias(newVariable, formula->getSign())} };
	while (children.size())
	{
		PropositionalFormula* child = children[0];
		children.erase(children.begin());
		switch (child->getType())
		{
		case 1: //AND
		{
			std::string commonDisjunct = child->getAlias(newVariable, false);
			std::vector<std::string> longDisjunction = {child->getAlias(newVariable, true)};
			std::vector<PropositionalFormula*> nextChildren = child->getChildren();
			for (PropositionalFormula* ch : nextChildren)
			{
				longDisjunction.push_back(ch->getAlias(newVariable, !ch->getSign()));
				CNF.push_back({commonDisjunct, ch->getAlias(newVariable, ch->getSign())});
			}
			CNF.push_back(longDisjunction);
			children.insert(children.end(), nextChildren.begin(), nextChildren.end());
			break;
		}
		case 2: //OR
		{
			std::string commonDisjunct = child->getAlias(newVariable, true);
			std::vector<std::string> longDisjunction = { child->getAlias(newVariable, false) };
			std::vector<PropositionalFormula*> nextChildren = child->getChildren();
			for (PropositionalFormula* ch : nextChildren)
			{
				longDisjunction.push_back(ch->getAlias(newVariable, ch->getSign()));
				CNF.push_back({commonDisjunct, ch->getAlias(newVariable, !ch->getSign())});
			}
			CNF.push_back(longDisjunction);
			children.insert(children.end(), nextChildren.begin(), nextChildren.end());
			break;
		}
		}
	}
}

std::map<std::string, int> SAT::getLiterals()
{
	std::map<std::string, int> literals;
	for (std::vector<std::string> conjunct : CNF)
	{
		for (std::string literal : conjunct)
		{
			if (literal[0] == '!')
			{
				std::string invLiteral = literal;
				invLiteral.erase(invLiteral.begin());
				literals.insert(std::pair<std::string, int>(invLiteral, -1));
			}
			else
			{
				literals.insert(std::pair<std::string, int>(literal, -1));
			}
		}
	}
	return literals;
}

bool SAT::DPLL(std::string chosen, std::map<std::string, int> literals, std::vector<std::vector<std::string>> cnf)
{
	std::string unit, unitInv;
	if (chosen.empty())
	{
		for (std::vector<std::string> conjunct : cnf)
		{
			if (conjunct.size() == 1)
			{
				unit = conjunct[0];
				break;
			}
		}
	}
	else
	{
		unit = chosen;
	}
	while (!unit.empty())
	{
		//Propagate
		if (unit[0] == '!')
		{
			unitInv = unit;
			unitInv.erase(unitInv.begin());
			literals[unitInv] = 0;
		}
		else
		{
			unitInv = "!" + unit;
			literals[unit] = 1;
		}
		//Elimination
		for (int i = cnf.size() - 1; i >= 0; i--)
		{
			if (std::find(cnf[i].begin(), cnf[i].end(), unit) != cnf[i].end())
			{
				cnf.erase(cnf.begin() + i);
			}
			else
			{
				std::vector<std::string>::iterator isFound = std::find(cnf[i].begin(), cnf[i].end(), unitInv);
				if (isFound != cnf[i].end())
				{
					cnf[i].erase(isFound);
				}
			}
		}
		unit.clear();
		for (std::vector<std::string> conjunct : cnf)
		{
			if (conjunct.size() == 1)
			{
				unit = conjunct[0];
				break;
			}
		}
	}
	if (cnf.empty())
	{
		return true;
	}
	for (std::vector<std::string> conjunct : cnf)
	{
		if (conjunct.empty())
		{
			return false;
		}
	}
	std::string unassignedLiteral;
	for (std::pair<std::string, int> literal : literals)
	{
		if (literal.second == -1)
		{
			unassignedLiteral = literal.first;
			break;
		}
	}
	literals.insert(std::pair<std::string, int>(unassignedLiteral, 0));
	if (DPLL("!" + unassignedLiteral, literals, cnf))
	{
		return true;
	}
	literals.insert(std::pair<std::string, int>(unassignedLiteral, 1));
	return DPLL(unassignedLiteral, literals, cnf);
	return true;
}

bool SAT::solve()
{
	if (solution == -1)
	{
		bool res = DPLL("", getLiterals(), CNF);
		return res;
	}
	else
	{
		return solution;
	}
}