#pragma once
#include <string>
#include <vector>
#include <utility>
#include <cctype>

#include "Parser.hpp"

class Equation
{
public:

	enum Component
	{
		OUTPUT,
		EQUALS,
		OPERATOR,
		VARIABLE,
		CONSTANT,
		EXPRESSION,
		FUNCTION
	};

	typedef std::vector<std::pair<Equation::Component, std::string>> PairList;
	typedef std::pair<Equation::Component, std::string> Pair;

	Equation(std::string quation);
	const PairList getComponents();
	const std::string& getEquation();
	const std::string& getErrorLog();
	const std::string getParsedEquation();

private:
	PairList componentList;
	std::string equation;
	std::string errorLog;
	std::string componentString;
	
	PairList makeComponentList(std::string source);
	std::vector<std::pair<int, int>> expressionStartAndStop;
	std::string getComponentList();

	bool checkForMismatchedCharacters(std::string source, const char c1, const char c2);
	void printComponentList();
	void pushPair(PairList& list, Equation::Component c, std::string expression);
};
