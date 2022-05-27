#ifndef STATEMENTS_HPP
#define STATEMENTS_HPP

#include <iostream>
#include <string>
#include "GrammarVariables.hpp"
class Statements
{

public:
	Statements(int deepness = -1, std::string type = "", GrammarVariables *gv = NULL);
	Statements(Statements const &src);
	~Statements();

	Statements &operator=(Statements const &rhs);
	bool isTheRightClosingStatement(std::string type, GrammarVariables *gv);
	std::ostream &print(std::ostream &o) const;
	void setMustBeValid();
	void isValid();

private:
	std::string _type;
	bool _mustBeValid;
	GrammarVariables *_gv;
	int _deepness;
};

std::ostream &operator<<(std::ostream &o, Statements const &i);

#endif /* ****************************************************** STATEMENTS_H */