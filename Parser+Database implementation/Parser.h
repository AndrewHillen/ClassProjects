#ifndef PARSER_H
#define PARSER_H

#include "DatalogProgram.h"
#include "Parameter.h"
#include "Rule.h"
#include "Predicate.h"
#include "Token.h"

#include <vector>

class Parser
{
private:
	std::vector<Token> tokens;
	int tokenPosition = 0;
	DatalogProgram dProgram;

public:
	Parser(std::vector<Token> t) : tokens(t) {};

	void Begin()
	{
		try
		{
			Parse();
		}
		catch (Token t)
		{
			std::cout << "Failure!\n  " << t.ToString() << std::endl;
		}
	}
	DatalogProgram RetrieveDatalog()
	{
		return dProgram;
	}

	void Parse()
	{
		std::vector<Predicate> schemes;
		std::vector<Predicate> facts;
		std::vector<Rule_> rules;
		std::vector<Predicate> queries;
		while (tokens.at(tokenPosition).GetType() != THEEND)
		{
			if (tokens.at(tokenPosition).GetType() == SCHEMES)
			{
				//Do something with SCHEMES?
				tokenPosition++;
				if (tokens.at(tokenPosition).GetType() == COLON)
				{
					//Do something with COLON?
					tokenPosition++;
					SchemeList(schemes);
				}
				else
				{
					throw(tokens.at(tokenPosition));
				}
			}

			else if (tokens.at(tokenPosition).GetType() == FACTS)
			{
				if (schemes.size() == 0) { throw(tokens.at(tokenPosition)); }
				//Do something with FACTS?
				tokenPosition++;
				if (tokens.at(tokenPosition).GetType() == COLON)
				{
					//Do something with COLON?
					tokenPosition++;
					
					FactList(facts);
				}
				else
				{
					throw(tokens.at(tokenPosition));
				}
			}
			else if (tokens.at(tokenPosition).GetType() == RULES)
			{
				//Do something with RULES?
				tokenPosition++;
				if (tokens.at(tokenPosition).GetType() == COLON)
				{
					//Do something with COLON?
					tokenPosition++;
					RuleList(rules);
				}
				else
				{
					throw(tokens.at(tokenPosition));
				}
			}
			else if (tokens.at(tokenPosition).GetType() == QUERIES)
			{
				tokenPosition++;
				if (tokens.at(tokenPosition).GetType() == COLON)
				{
					//Do something with COLON?
					tokenPosition++;
					QueryList(queries);
				}
				else
				{
					throw(tokens.at(tokenPosition));
				}
				if (queries.size() == 0) { throw(tokens.at(tokenPosition)); }
			}
		}
		dProgram.SetSchemes(schemes);
		dProgram.SetFacts(facts);
		dProgram.SetRules(rules);
		dProgram.SetQueries(queries);
		//dProgram.ToString();
	}


	void SchemeList(std::vector<Predicate> &schemes)
	{
		if (tokens.at(tokenPosition).GetType() == ID)
		{
			std::string theID = tokens.at(tokenPosition).TokenString();
			std::vector<Parameter> params;
			Scheme(params);
			schemes.push_back(Predicate(theID, params));
			switch (tokens.at(tokenPosition).GetType())
			{
			case SCHEMES: throw (tokens.at(tokenPosition));
			case FACTS: return; break;
			case RULES: return; break;
			case QUERIES: return; break;
			}
			SchemeList(schemes);
		}
		else
		{
			throw (tokens.at(tokenPosition));
		}
	}

	void Scheme(std::vector<Parameter> &params)
	{
		if (tokens.at(tokenPosition).GetType() == ID)
		{
			//Do something with head ID
			tokenPosition++;
			if (tokens.at(tokenPosition).GetType() == LEFT_PAREN)
			{
				//Probably nothing with Paren
				tokenPosition++;
                if (tokens.at(tokenPosition).GetType() == RIGHT_PAREN) { throw (tokens.at(tokenPosition)); }
				IDList(params);
			}
			else
			{
				throw (tokens.at(tokenPosition));
			}

		}
		//Do something with all the tokens. Combine into string and put in Pred vector? Increment Scheme count wherever

	}

	void IDList(std::vector<Parameter> &params)
	{
		if (tokens.at(tokenPosition).GetType() == ID)
        {
            params.push_back(Parameter(tokens.at(tokenPosition).TokenString()));
    		//Verify that they are all IDs
    		tokenPosition++; //This may be an undesired effect
        }
		//Verify that they are all IDs
		if (tokens.at(tokenPosition).GetType() == COMMA)
		{
			//Create double comma security
			tokenPosition++;
			IDList(params);
		}

		else if (tokens.at(tokenPosition).GetType() == RIGHT_PAREN)
		{
			//Probably nothing with RParen
			tokenPosition++;
			return;
		}

		else
		{
			throw (tokens.at(tokenPosition));
		}
	}
	//FACTS------------------------------------

	void FactList(std::vector<Predicate> &facts)
	{	
		if (tokens.at(tokenPosition).GetType() == RULES) { return; }
		if (tokens.at(tokenPosition).GetType() == ID)
		{
			std::string theID = tokens.at(tokenPosition).TokenString();
			std::vector<Parameter> params;
			Fact(params);
			facts.push_back(Predicate(theID, params));
			switch (tokens.at(tokenPosition).GetType())
			{
			case SCHEMES: throw (tokens.at(tokenPosition));
			case FACTS: throw (tokens.at(tokenPosition));
			case RULES: return; break;
			case QUERIES: throw (tokens.at(tokenPosition));
			}
			FactList(facts);
			return;
		}
		if (tokens.at(tokenPosition).GetType() == THEEND)
		{
			return;
		}
		else
		{

			throw (tokens.at(tokenPosition));
		}
	}

	//String set goes with facts
	void Fact(std::vector<Parameter> &params)
	{
		if (tokens.at(tokenPosition).GetType() == ID)
		{
			//Do something with head ID
			tokenPosition++;
			if (tokens.at(tokenPosition).GetType() == LEFT_PAREN)
			{
				//Probably nothing with Paren
				tokenPosition++;
                if (tokens.at(tokenPosition).GetType() == RIGHT_PAREN) { throw (tokens.at(tokenPosition)); }
				StringList(params);
			}
			else
			{
				throw (tokens.at(tokenPosition));
			}

		}
		//Do something with all the tokens. Combine into string and put in Pred vector? Increment Fact count wherever

	}

	void StringList(std::vector<Parameter> &params)
	{
		//Add strings to set
		if (tokens.at(tokenPosition).GetType() == STRING)
		{
			params.push_back(Parameter(tokens.at(tokenPosition).TokenString()));
			tokenPosition++;
			//Add strings to set
			StringList(params);
		}
		else if (tokens.at(tokenPosition).GetType() == COMMA)
		{
			tokenPosition++;
			//Add double comma protection
			StringList(params);
		}
		else if (tokens.at(tokenPosition).GetType() == RIGHT_PAREN)
		{
			//Probably nothing with RParen
			tokenPosition++;
			if (tokens.at(tokenPosition).GetType() == THEEND)
			{
				return;
			}
			if (tokens.at(tokenPosition).GetType() != PERIOD)
			{
				throw (tokens.at(tokenPosition));
			}
			tokenPosition++;
			return;
		}

		else
		{
			throw (tokens.at(tokenPosition));
		}
	}

	// RULES-----------------------
	/*
	rule			->	headPredicate COLON_DASH predicate predicateList PERIOD
	headPredicate	->	ID LEFT_PAREN ID idList RIGHT_PAREN
	predicate		->	ID LEFT_PAREN parameter parameterList RIGHT_PAREN
	*/
	void RuleList(std::vector<Rule_> &rules)
	{
		if (tokens.at(tokenPosition).GetType() == QUERIES) { return; }
		if (tokens.at(tokenPosition).GetType() == ID)
		{
			Rule(rules);
			switch (tokens.at(tokenPosition).GetType())
			{
			case SCHEMES: throw (tokens.at(tokenPosition));
			case FACTS: throw (tokens.at(tokenPosition));
			case RULES: throw (tokens.at(tokenPosition));
			case QUERIES: return; break;
			}
			RuleList(rules);
		}
		else
		{
			throw (tokens.at(tokenPosition));
		}
	}

	void Rule(std::vector<Rule_> &rules)
	{
		if (tokens.at(tokenPosition).GetType() == ID)
		{
			HeadPred(rules);
			if (tokens.at(tokenPosition).GetType() != PERIOD) { throw(tokens.at(tokenPosition)); }
			//Do something with Period.
			tokenPosition++;
		}
	}

	void HeadPred(std::vector<Rule_>& rules)
	{

		if (tokens.at(tokenPosition).GetType() == ID)
		{
			std::string theID = tokens.at(tokenPosition).TokenString();
			std::vector<Parameter> params;
			//Do something with ID
			tokenPosition++;
			if (tokens.at(tokenPosition).GetType() != LEFT_PAREN) { throw(tokens.at(tokenPosition)); }
			//Do something with LParen
			tokenPosition++;
            if (tokens.at(tokenPosition).GetType() == RIGHT_PAREN) { throw (tokens.at(tokenPosition)); }
			ParameterList(params);
			Predicate headPred(theID, params);
			//Do something with Rparen
			tokenPosition++;


			if (tokens.at(tokenPosition).GetType() != COLON_DASH)
			{
				throw (tokens.at(tokenPosition));
			}
			//Do something with colon-Dash
			tokenPosition++;

			std::vector<Predicate> preds;
			PredicateList(preds);
			rules.push_back(Rule_(headPred, preds));

			//Headpred and predicate vector
		}

		else
		{
			throw (tokens.at(tokenPosition));
		}

	}

	void PredicateList(std::vector<Predicate> &preds)
	{
		if (tokens.at(tokenPosition).GetType() == ID)
		{
			std::string theID = tokens.at(tokenPosition).TokenString();
			std::vector<Parameter> params;
			tokenPosition++;
			if (tokens.at(tokenPosition).GetType() != LEFT_PAREN) { throw(tokens.at(tokenPosition)); }
			//Do something with LParen
			tokenPosition++;
            if (tokens.at(tokenPosition).GetType() == RIGHT_PAREN) { throw (tokens.at(tokenPosition)); }
			ParameterList(params);
			//Do something with Rparen
			tokenPosition++;
			preds.push_back(Predicate(theID, params));
			PredicateList(preds);
		}

		else if (tokens.at(tokenPosition).GetType() == COMMA)
		{
			//Do something with COMMA
			tokenPosition++;
			if (tokens.at(tokenPosition).GetType() != ID) { throw (tokens.at(tokenPosition)); }
			PredicateList(preds);
		}

		else if (tokens.at(tokenPosition).GetType() == PERIOD || tokens.at(tokenPosition).GetType() == Q_MARK)
		{
			//Punctuation NOT handled here.
			return;
		}

		else
		{
			throw (tokens.at(tokenPosition));
		}

		return;
	}

	void ParameterList(std::vector<Parameter> &params)
	{
		if (tokens.at(tokenPosition).GetType() == STRING)
		{
			//Add isConstant = true here
			params.push_back(Parameter(tokens.at(tokenPosition).TokenString(), true));
			tokenPosition++;
		}

		else if (tokens.at(tokenPosition).GetType() == ID)
		{
			params.push_back(Parameter(tokens.at(tokenPosition).TokenString()));
			tokenPosition++;
		}

		else if (tokens.at(tokenPosition).GetType() == LEFT_PAREN)
		{
			std::ostringstream expressionParam;
            Expression(expressionParam);
            if (tokens.at(tokenPosition).GetType() == COMMA && tokens.at(tokenPosition + 1).GetType() != ID) { throw (tokens.at(tokenPosition)); }
			params.push_back(Parameter(expressionParam.str()));
			//tokenPosition++;
		}
		//FIXME: Add expressions


		if (tokens.at(tokenPosition).GetType() != COMMA && tokens.at(tokenPosition).GetType() != RIGHT_PAREN)
		{
			throw (tokens.at(tokenPosition));
		}

		else if (tokens.at(tokenPosition).GetType() == COMMA)
		{
			//Do something with Comma.
			tokenPosition++;
			if (!(tokens.at(tokenPosition).GetType() == ID || tokens.at(tokenPosition).GetType() == STRING || tokens.at(tokenPosition).GetType() == LEFT_PAREN)) //Add expressions to this too
			{ 
				throw (tokens.at(tokenPosition));
			}
			ParameterList(params);
		}

		else
		{
            if (tokens.at(tokenPosition + 1).GetType() == RIGHT_PAREN) { throw (tokens.at(tokenPosition)); }
			//Rparen NOT handled.
			return;
		}

	}

    void Expression(std::ostringstream &expressionParam)
    {
        expressionParam << tokens.at(tokenPosition).TokenString();
		tokenPosition++;
        if (tokens.at(tokenPosition).GetType() != ID && tokens.at(tokenPosition).GetType() != LEFT_PAREN && tokens.at(tokenPosition).GetType() != STRING) { throw (tokens.at(tokenPosition)); }


			while (tokens.at(tokenPosition).GetType() != RIGHT_PAREN)
			{
                if (tokens.at(tokenPosition).GetType() == COMMA) { throw (tokens.at(tokenPosition)); }
                if (tokens.at(tokenPosition).GetType() == LEFT_PAREN)
                {
                    if (tokens.at(tokenPosition + 1).GetType() != LEFT_PAREN && tokens.at(tokenPosition + 1).GetType() != ID && tokens.at(tokenPosition + 1).GetType() != STRING)
                    {
                        throw(tokens.at(tokenPosition + 1));
                    }
                }

                if (tokens.at(tokenPosition).GetType() == ID || tokens.at(tokenPosition).GetType() == STRING)
                {
                    if (tokens.at(tokenPosition + 1).GetType() != ADD && tokens.at(tokenPosition + 1).GetType() != MULTIPLY && tokens.at(tokenPosition + 1).GetType() != RIGHT_PAREN)
                    {
                        throw(tokens.at(tokenPosition + 1));
                    }
                }

                if (tokens.at(tokenPosition).GetType() == ADD || tokens.at(tokenPosition).GetType() == MULTIPLY)
                {
                    if (tokens.at(tokenPosition + 1).GetType() != LEFT_PAREN && tokens.at(tokenPosition + 1).GetType() != ID && tokens.at(tokenPosition + 1).GetType() != STRING)
                    {
                        throw(tokens.at(tokenPosition + 1));
                    }
                }
                if (tokens.at(tokenPosition).GetType() == LEFT_PAREN) { Expression(expressionParam); }
                if (tokens.at(tokenPosition).GetType() == COMMA) { throw (tokens.at(tokenPosition)); }
                if (tokens.at(tokenPosition).GetType() == RIGHT_PAREN) { break; }
                expressionParam << tokens.at(tokenPosition).TokenString();
			    tokenPosition++;
			}
            expressionParam << tokens.at(tokenPosition).TokenString();
			tokenPosition++;
            if (tokens.at(tokenPosition).GetType() == PERIOD || tokens.at(tokenPosition).GetType() == COMMA)
            {
                return;
            }
            if (tokens.at(tokenPosition).GetType() != RIGHT_PAREN && tokens.at(tokenPosition).GetType() != ADD && tokens.at(tokenPosition).GetType() != MULTIPLY)
            {
                throw(tokens.at(tokenPosition));
            }

    }

	void QueryList(std::vector<Predicate> &queries)
	{
		if (tokens.at(tokenPosition).GetType() == ID)
		{
			PredicateList(queries);
			if (tokens.at(tokenPosition).GetType() != Q_MARK) { throw(tokens.at(tokenPosition)); }
			//Do something with Qmark.
			tokenPosition++;
			QueryList(queries);
		}

		else if (tokens.at(tokenPosition).GetType() == THEEND)
		{
			return;
		}

		else
		{
			throw (tokens.at(tokenPosition));
		}
	}

};


















#endif
