#pragma once
#include <string>

class Parser
{
public:
	Parser() = delete;

	static const std::string getWholeExpression(std::string source, char start, const char end)
	{
		std::string returnStr = "";
		int expressionStart = -1, expressionEnd = -1;
		int nSkips = 0;
		for (int i = 0; i < source.size(); i++)
		{
			if (source.c_str()[i] == '(')
			{
				expressionStart = i;
				for (int j = i + 1; j < source.size(); j++)
				{
					if (source.c_str()[j] == '(')
					{
						nSkips++;
					}

					if (source.c_str()[j] == ')' && nSkips > 0)
					{
						nSkips--;
					}

					if (source.c_str()[j] == ')' && nSkips == 0)
					{
						expressionEnd = j + 1;
					}
				}

				break;
			}
		}
		returnStr = source.substr(expressionStart, expressionEnd - expressionStart);
		return returnStr;
	}
	static const std::string getWholeExpression(std::string source, char start, const char end, int offset)
	{
		source = source.substr(offset);
		std::string returnStr = "";
		int expressionStart = -1, expressionEnd = -1;
		int nSkips = 0;
		for (int i = 0; i < source.size(); i++)
		{
			if (source.c_str()[i] == '(')
			{
				expressionStart = i;
				for (int j = i + 1; j < source.size(); j++)
				{
					if (source.c_str()[j] == '(')
					{
						nSkips++;
					}

					if (source.c_str()[j] == ')' && nSkips > 0)
					{
						nSkips--;
					}

					if (source.c_str()[j] == ')' && nSkips == 0)
					{
						expressionEnd = j + 1;
					}
				}

				break;
			}
		}
		returnStr = source.substr(expressionStart, expressionEnd - expressionStart);
		return returnStr;
	}
	static const std::string getFunctionInnards(int i, std::string source)
	{
		int index = i;
		int endIndex = -1;
		int startIndex = -1;
		std::string tempString = source.substr(index);
		endIndex = tempString.find(')') + 1;
		startIndex = tempString.find('(');
		std::string functionInnards = tempString.substr(startIndex, endIndex - startIndex);
		return functionInnards;
	};

};