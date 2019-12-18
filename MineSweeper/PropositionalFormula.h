#pragma once
#include <string>
#include <vector>

class PropositionalFormula //Variables
{
protected:
	bool Sign;
	std::string Name;

public:
	PropositionalFormula(bool sign, std::string name);
	void negate();
	bool getSign();
	virtual int getValue();
	virtual std::string getFormula();
	virtual PropositionalFormula* getSimplified();
	virtual std::vector<PropositionalFormula*> getChildren();
	virtual std::string getAlias(int &newValue, bool sign);
	virtual int getType()
	{
		return 0;
	}
};

class PropositionalAnd : public PropositionalFormula
{
private:
	bool Defined;
	std::string Alias;
	std::vector<PropositionalFormula*> Conjuncts;

public:
	PropositionalAnd();
	void addConjunct(PropositionalFormula* conjunct);
	virtual int getValue() override;
	virtual std::string getFormula() override;
	virtual PropositionalFormula* getSimplified() override;
	virtual std::vector<PropositionalFormula*> getChildren() override;
	virtual std::string getAlias(int &newValue, bool sign) override;
	virtual int getType() override
	{
		return 1;
	}
};

class PropositionalOr : public PropositionalFormula
{
private:
	bool Defined;
	std::string Alias;
	std::vector<PropositionalFormula*> Disjuncts;

public:
	PropositionalOr();
	void addDisjunct(PropositionalFormula* disjunct);
	virtual int getValue() override;
	virtual std::string getFormula() override;
	virtual PropositionalFormula* getSimplified() override;
	virtual std::vector<PropositionalFormula*> getChildren() override;
	virtual std::string getAlias(int &newValue, bool sign) override;
	virtual int getType() override
	{
		return 2;
	}
};