
#ifndef TOKEN_H
#define TOKEN_H
enum types {
	COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY,
	ADD, SCHEMES, FACTS, RULES, QUERIES, ID, STRING, COMMENT, UNDEFINED, THEEND, NOEENTRY, NEWLINE
};


#include <string>
#include <sstream>

class Token
{
private:
	int lineNumber;
	std::string theToken;
	int tokenType;

public:

	Token(int line = 0, std::string token = "null", int type = 0) : lineNumber(line), theToken(token), tokenType(type) {}
	std::string ToString()
	{
		std::ostringstream out;
		out << "(" << TypeToString() << ",\"" << theToken << "\"," << lineNumber << ")\n";
		return out.str();
	}

	int GetType()
	{
		return tokenType;
	}

	std::string TypeToString()
	{
		switch (tokenType)
		{
			//{COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY,
			//ADD, SCHEMES, FACTS, RULES, QUERIES, ID, STRING, COMMENT, UNDEFINED, THEEND, NOEENTRY

		case COMMA:			return "COMMA"; break;
		case PERIOD:		return "PERIOD"; break;
		case Q_MARK:		return "Q_MARK"; break;
		case LEFT_PAREN:	return "LEFT_PAREN"; break;
		case RIGHT_PAREN:	return "RIGHT_PAREN"; break;
		case COLON:			return "COLON"; break;
		case COLON_DASH:	return "COLON_DASH"; break;
		case MULTIPLY:		return "MULTIPLY"; break;
		case ADD:			return "ADD"; break;
		case SCHEMES:		return "SCHEMES"; break;
		case FACTS:			return "FACTS"; break;
		case RULES:			return "RULES"; break;
		case QUERIES:		return "QUERIES"; break;
		case ID:			return "ID"; break;
		case STRING:		return "STRING"; break;
		case COMMENT:		return "COMMENT"; break;
		case UNDEFINED:		return "UNDEFINED"; break;
		case THEEND:		return "EOF"; break;

		}
		return "ERROR";
	}

	std::string TokenString()
	{
		return theToken;
	}

};











#endif