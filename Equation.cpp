#include "Equation.hpp"

Equation::Equation(std::string equation)
	: equation(equation)
{
	
	printf("%s\n", equation.c_str());
	bool hasErrors = checkForMismatchedCharacters(equation, '(', ')');
	if (hasErrors)
	{
		printf("%s\n", getErrorLog().c_str());
	}
	else
	{
		componentList = makeComponentList(equation);
		printComponentList();
		getParsedEquation();
	}
}

const std::string& Equation::getEquation()
{
	return equation;
}

const std::vector<Equation::Pair> Equation::getComponents()
{
	return componentList;
}

const std::string& Equation::getErrorLog()
{
	return errorLog;
}

bool Equation::checkForMismatchedCharacters(std::string source, const char c1, const char c2)
{
	unsigned int startIndex = -1, endIndex = -1;

	char* list = (char*)source.c_str();
	char c, k;

	char* trimmedList = nullptr;
	int trimmedListSize = -1;

	for (unsigned int i = 0; i < source.size(); i++)
	{
		c = list[i];

		if (c == c1)
		{	
			startIndex = i;
			int nSkips = 0;
			for (unsigned int j = i + 1; j < source.size(); j++)
			{
				k = list[j];
				if (k == c1)
				{
					nSkips++;
				}
				if (k == c2 && nSkips == 0)
				{
					endIndex = j;
					nSkips = 0;
					break;
				}
				else if (k == c2 && nSkips > 0)
				{
					nSkips--;
				}
			}

			if (startIndex > endIndex)
			{
				errorLog += "One or more of the characters couldn't be found.";
				return true;
			}

			trimmedListSize = endIndex - startIndex;

			if (trimmedListSize < 0)
			{
				errorLog += "One or more of the characters could not be found.";
				return true;
			}

			trimmedList = new char[trimmedListSize];
			for (int m = 0; m < trimmedListSize - 1; m++)
			{
				trimmedList[m] = list[m + startIndex + 1];
			}
			break;
		}
	}

	if (trimmedList == nullptr)
	{
		return true;
	}
	std::string trimmed = std::string(trimmedList, trimmedListSize - 1);

	char z;
	for (unsigned int l = 0; l < trimmed.size(); l++)
	{
		z = trimmed.c_str()[l];
		if (z == c1 || z == c2)
		{
			errorLog += "Character mistmatch error";
			return true;
		}
	}
	checkForMismatchedCharacters(trimmed, c1, c2);
	return false;
}

Equation::PairList Equation::makeComponentList(std::string source)
{
	PairList temp;
	char* list = (char*)source.c_str();

	for (unsigned int i = 0; i < source.size(); i++)
	{
		if (list[i] != ' ')
		{
			if (list[i] == 'y' || list[i] == 'Y')
			{
				pushPair(temp, Equation::Component::OUTPUT, "y");
			}

			if (list[i] == '=')
			{
				pushPair(temp, Equation::Component::EQUALS, "=");
			}

			if (list[i] == 'x' || list[i] == 'X')
			{
				pushPair(temp, Equation::Component::VARIABLE, "x");
			}

			if (list[i] == '+' || list[i] == '-' || list[i] == '/' || list[i] == '*' || list[i] == '%')
			{
				pushPair(temp, Equation::Component::OPERATOR, std::string(1, list[i]));
			}
			if (i + 2 <= source.size() && (list[i] == 's' || list[i] == 'S') && (list[i + 1] == 'i' || list[i + 1] == 'I') && (list[i + 2] == 'n' || list[i + 2] == 'N'))
			{
				pushPair(temp, Equation::Component::FUNCTION, "sin" + Parser::getFunctionInnards(i, source));
				i += strlen(Parser::getFunctionInnards(i, source).c_str()) + strlen("sin");
			}

			if (i + 2 <= source.size() && (list[i] == 'c' || list[i] == 'C') && (list[i + 1] == 'o' || list[i + 1] == 'O') && (list[i + 2] == 's' || list[i + 2] == 'S'))
			{
				pushPair(temp, Equation::Component::FUNCTION, "cos" + Parser::getFunctionInnards(i, source));
				i += strlen(Parser::getFunctionInnards(i, source).c_str()) + strlen("cos");
			}

			if (i + 2 <= source.size() && (list[i] == 't' || list[i] == 'T') && (list[i + 1] == 'a' || list[i + 1] == 'A') && (list[i + 2] == 'n' || list[i + 2] == 'N'))
			{
				pushPair(temp, Equation::Component::FUNCTION, "tan" + Parser::getFunctionInnards(i, source));
				i += strlen(Parser::getFunctionInnards(i, source).c_str()) + strlen("tan");
			}

			int startDigit = -1, endDigit = -1;
			while (std::isdigit(list[i]))
			{
				if (startDigit < 0)
				{
					startDigit = i;
				}
				i++;
			}
			if (startDigit > 0)
			{
				endDigit = i;
				pushPair(temp, Equation::Component::CONSTANT, source.substr(startDigit, endDigit - startDigit));
				i--;
			}

			if (list[i] == '(')
			{
				std::string expression = Parser::getWholeExpression(source, '(', ')', i);
				pushPair(temp, Equation::Component::EXPRESSION, expression);
				i += strlen(expression.c_str());
			}
		}
	}

	return temp;
}

void Equation::printComponentList()
{
	std::string temp;
	int counter = 0;
	for (auto i : componentList)
	{
		for (unsigned int j = 0; j < expressionStartAndStop.size(); j++)
		{
			if (counter == std::get<0>(expressionStartAndStop[j]))
			{
				temp += "{";
			}
		}

		temp += "[";
		if (std::get<0>(i) == Equation::Component::OUTPUT)
		{
			temp += "OUTPUT : " + std::get<1>(i);
		}

		if (std::get<0>(i) == Equation::Component::CONSTANT)
		{
			temp += "CONSTANT : " + std::get<1>(i);
		}

		if (std::get<0>(i) == Equation::Component::VARIABLE)
		{
			temp += "VARIABLE : " + std::get<1>(i);
		}

		if (std::get<0>(i) == Equation::Component::FUNCTION)
		{
			temp += "FUNCTION : " + std::get<1>(i);
		}

		if (std::get<0>(i) == Equation::Component::OPERATOR)
		{
			temp += "OPERATOR : " + std::get<1>(i);
		}

		if (std::get<0>(i) == Equation::Component::EQUALS)
		{
			temp += "EQUALS : " + std::get<1>(i);
		}

		if (std::get<0>(i) == Equation::Component::EXPRESSION)
		{
			temp += "EXPRESSION : " + std::get<1>(i);
		}

		temp += "] ";

		for (unsigned int j = 0; j < expressionStartAndStop.size(); j++)
		{
			if (counter == std::get<1>(expressionStartAndStop[j]))
			{
				temp += "}";
			}
		}
		counter++;
	}
	componentString = temp;
	printf("%s\n", componentString.c_str());
}

std::string Equation::getComponentList()
{
	return componentString;
}

const std::string Equation::getParsedEquation()
{
	std::string code = getComponentList();
    unsigned int expressionStart = -1;
    
    while((expressionStart = (unsigned int)code.find("EXPRESSION")) > 0)
    {
        std::string codeSub = code.substr(expressionStart);
        unsigned int expressionEnd = (unsigned int)codeSub.find("]") + expressionStart;
        std::string expressionSnippet = code.substr(expressionStart, expressionEnd - expressionStart);
        
        char c;
        unsigned int lastParenthesis = -1;
        int offsetStart = (unsigned int)expressionSnippet.find("(");
        
        if (offsetStart < 0)
        {
            offsetStart = 0;
        }
        
        std::string finalizedExpression = code;
        int firstParenthesis = -1;
        
        for (int i = offsetStart + 1; i < expressionSnippet.size(); i++)
        {
            c = expressionSnippet.c_str()[i];
            if (c == '(')
            {
                lastParenthesis = i;
            }
            
            if (c == ')')
            {
                std::string functionSnippet = expressionSnippet.substr(lastParenthesis, (i + 1) - lastParenthesis);
                std::string newExpression = "[EXPRESSION : " + functionSnippet + "]";
                expressionSnippet = Parser::remove(expressionSnippet, lastParenthesis, i + 1);
                
                 firstParenthesis = (int)expressionSnippet.find_first_of("(");
                
                finalizedExpression = Parser::removeAndReplace(code, expressionStart + (lastParenthesis - 1), expressionStart + i + 1, newExpression);
                
                finalizedExpression = Parser::insert(finalizedExpression, expressionStart + (firstParenthesis - 1), " (");
                
                printf("%s\n", finalizedExpression.c_str());
                break;
            }
        }
        break;
    }
    
	return code;
}

void Equation::pushPair(PairList& list, Equation::Component c, std::string expression)
{
	Equation::Pair p = std::make_pair(c, expression);
	list.push_back(p);
}
