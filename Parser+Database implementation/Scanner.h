#ifndef SCANNER_H
#define SCANNER_H
#include "Token.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cctype>

class Scanner
{
private:
    std::string inputfile;
	int currentLine;
	int tokenLine;
	std::vector<Token> theTokens;


public:
	Scanner(std::string inputs, int line = 1, int otherLine = 1) : inputfile(inputs), currentLine(line), tokenLine(otherLine) {}

	std::string tester()
	{
		std::ifstream in(inputfile);

		char identifier;

		while (in.peek() != EOF)
		{
			//std::cout << "I did something\n";
			identifier = in.peek();
			//FIXME put commented choices in if statements
			switch (identifier)
			{
			case ',':		Punctuation(in); break;
			case '.':		Punctuation(in); break;
			case '?':		Punctuation(in); break;
			case ':':		Punctuation(in); break;
			case '(':		Symbols(in); break;
			case ')':		Symbols(in); break;
			case '*':		Symbols(in); break;
			case '+':		Symbols(in); break;
			case 'S':		Words(in); break;
			case 'F':		Words(in); break;
			case 'R':		Words(in); break;
			case 'Q':		Words(in); break;
			case '\'':		StringComments(in); break;
			case '#':		StringComments(in); break;
			case '\n':		AdvanceLine(in); break;
				//case UNDEFINED:		return "UNDEFINED"; break;
			default:		Undefined(in); break;
			}

		}
		Ending(in);



		return "worked";
	}
	//COMMA, PERIOD, Q_MARK, COLON, COLON_DASH,
	void Punctuation(std::ifstream& inputs)
	{
		char p = inputs.get();
		std::string theToken = "";
		if (p == ',')
		{
			theToken += p;
			tokenLine = currentLine;
			Token thisToken(tokenLine, theToken, COMMA);
			theTokens.push_back(thisToken);
		}
		else if (p == '.')
		{
			theToken += p;
			tokenLine = currentLine;
			Token thisToken(tokenLine, theToken, PERIOD);
			theTokens.push_back(thisToken);
		}
		else if (p == '?')
		{
			theToken += p;
			tokenLine = currentLine;
			Token thisToken(tokenLine, theToken, Q_MARK);
			theTokens.push_back(thisToken);
		}
		else if (p == ':')
		{
			theToken += p;
			tokenLine = currentLine;
			if (inputs.peek() == '-')
			{
				p = inputs.get();
				theToken += p;
				Token thisToken(tokenLine, theToken, COLON_DASH);
				theTokens.push_back(thisToken);
			}
			else
			{
				Token thisToken(tokenLine, theToken, COLON);
				theTokens.push_back(thisToken);
			}
		}
	}
	//LEFT_PAREN, RIGHT_PAREN, MULTIPLY, ADD
	void Symbols(std::ifstream& inputs)
	{
		char p = inputs.get();
		std::string theToken = "";
		if (p == '(')
		{
			theToken += p;
			tokenLine = currentLine;
			Token thisToken(tokenLine, theToken, LEFT_PAREN);
			theTokens.push_back(thisToken);
		}
		else if (p == ')')
		{
			theToken += p;
			tokenLine = currentLine;
			Token thisToken(tokenLine, theToken, RIGHT_PAREN);
			theTokens.push_back(thisToken);
		}
		else if (p == '*')
		{
			theToken += p;
			tokenLine = currentLine;
			Token thisToken(tokenLine, theToken, MULTIPLY);
			theTokens.push_back(thisToken);
		}
		else if (p == '+')
		{
			theToken += p;
			tokenLine = currentLine;
			Token thisToken(tokenLine, theToken, ADD);
			theTokens.push_back(thisToken);
		}
	}
	//SCHEMES, FACTS, RULES, QUERIES, ID,
	void Words(std::ifstream& inputs)
	{
		std::string schemes = "Schemes";
		std::string facts = "Facts";
		std::string rules = "Rules";
		std::string queries = "Queries";

		char p = inputs.get();
		int type;
		tokenLine = currentLine;
		std::string theToken = "";
		bool id = false;

		if (p == 'S')
		{
			theToken += p;
			for (unsigned int i = 1; i < schemes.size(); i++)
			{
				p = inputs.peek();
				if (p == schemes.at(i))
				{
					p = inputs.get();
					theToken += p;
				}
				else
				{
					id = true;
					p = inputs.peek();
					while (isalpha(inputs.peek()) || isalnum(inputs.peek()))
					{
						//std::cout << p << std::endl;
						p = inputs.get();
						theToken += p;
					}
					i = schemes.size();
				}
			}
			type = SCHEMES;
		}
		else if (p == 'F')
		{
			theToken += p;
			for (unsigned int i = 1; i < facts.size(); i++)
			{
				p = inputs.peek();
				if (p == facts.at(i))
				{
					p = inputs.get();
					theToken += p;
				}
				else
				{
					id = true;
					p = inputs.peek();
					while (isalpha(inputs.peek()) || isalnum(inputs.peek()))
					{
						p = inputs.get();
						theToken += p;
					}
					i = facts.size();
				}
			}
			type = FACTS;
		}
		else if (p == 'R')
		{
			theToken += p;
			for (unsigned int i = 1; i < rules.size(); i++)
			{
				p = inputs.peek();
				if (p == rules.at(i))
				{
					p = inputs.get();
					theToken += p;
				}
				else
				{
					id = true;
					p = inputs.peek();
					while (isalpha(inputs.peek()) || isalnum(inputs.peek()))
					{
						p = inputs.get();
						theToken += p;
					}
					i = rules.size();
				}
			}
			type = RULES;
		}
		else if (p == 'Q')
		{
			theToken += p;
			for (unsigned int i = 1; i < queries.size(); i++)
			{
				p = inputs.peek();
				if (p == queries.at(i))
				{
					p = inputs.get();
					theToken += p;
				}
				else
				{
					id = true;
					p = inputs.peek();
					while (isalpha(inputs.peek()) || isalnum(inputs.peek()))
					{
						p = inputs.get();
						theToken += p;
					}
					i = queries.size();
				}
			}
			type = QUERIES;
		}
		else
		{
			theToken += p;
			p = inputs.peek();
			while (isalpha(inputs.peek()) || isalnum(inputs.peek()))
			{
				p = inputs.get();
				theToken += p;
			}
			id = true;

		}

		if (id)
		{
			type = ID;
		}

		Token thisToken(tokenLine, theToken, type);
		theTokens.push_back(thisToken);



	}
	//STRING, COMMENT
	void StringComments(std::ifstream& inputs)
	{
		std::string theToken = "";
		int type = NOEENTRY;
		tokenLine = currentLine;
		char p;
		p = inputs.peek();
		bool isUndefined = false;

		if (p == '\'')
		{
			bool keepgoing = true;
			type = STRING;
			do //while (p != '\'' && (inputs.peek() != EOF || isspace(inputs.peek())) )
			{
				//std::cout << p << std::endl;
				p = inputs.get();
				//std::cout << p << std::endl;
				if (p == '\n')
				{
					currentLine++;
				}
				theToken += p;
                if(inputs.peek() == '\'')
                {
                    p = inputs.get();
                    theToken += p;
                    break;
                }
				if (p != '\'' && inputs.peek() == EOF)
				{
					isUndefined = true;
					break;
				}
				if (!keepgoing && inputs.peek() != '\'')
				{
					break;
				}
				keepgoing = true;
				if (inputs.peek() == '\'')
				{
					keepgoing = false;
				}

			} while (!(p == '\'' && inputs.peek() == '\'') && inputs.peek() != EOF);
		}

		else if (p == '#')
		{
			type = COMMENT;
			p = inputs.get();
			if (inputs.peek() == '|')
			{
				bool whileWorked = false;
				theToken += p;
				p = inputs.get();
				theToken += p;
				do //while (p != '|' && (inputs.peek() != EOF || inputs.peek() != '#'))
				{
					p = inputs.get();
					if (p == '\n')
					{
						currentLine++;
					}
					theToken += p;

					if (p == '|' && inputs.peek() == '#')
					{
						whileWorked = true;
					}

					if (inputs.peek() == EOF)
					{
						isUndefined = true;
						break;
					}

				} while (p != '|' && (inputs.peek() != EOF || inputs.peek() != '#'));
				if (whileWorked)
				{
					theToken += inputs.get();
				}
			}

			else
			{
				theToken += p;
				while (inputs.peek() != '\n' && inputs.peek() != EOF)
				{
					p = inputs.get();
					theToken += p;
				}
			}
			return;
		}

		if (isUndefined)
		{
			type = UNDEFINED;
		}

		Token thisToken(tokenLine, theToken, type);
		theTokens.push_back(thisToken);

	}
	//EOF
	void Ending(std::ifstream& inputs)
	{
		std::string theToken = "";
		int type = THEEND;
		tokenLine = currentLine;
		Token thisToken(tokenLine, theToken, type);
		theTokens.push_back(thisToken);


	}
	void AdvanceLine(std::ifstream& inputs)
	{
		inputs.get();
		currentLine++;
	}

	void Undefined(std::ifstream& inputs)
	{
		if (isalpha(inputs.peek()))
		{
			Words(inputs);
			return;
		}

		else if (isspace(inputs.peek()))
		{
			inputs.get();
			return;
		}

		else
		{
			char p;
			std::string theToken = "";
			int type = UNDEFINED;
			tokenLine = currentLine;
			while (!isspace(inputs.peek()))
			{
				p = inputs.get();
				theToken += p;
			}
			Token thisToken(tokenLine, theToken, type);
			theTokens.push_back(thisToken);
			return;
		}

	}

	void output()
	{
		int tokenCounter = 0;
		for (unsigned int i = 0; i < theTokens.size(); i++)
		{
			std::cout << theTokens.at(i).ToString();
			tokenCounter++;
		}
		std::cout << "Total Tokens: " << tokenCounter << std::endl;
	}

	std::vector<Token> RetrieveVector()
	{
		return theTokens;
	}
};













#endif
