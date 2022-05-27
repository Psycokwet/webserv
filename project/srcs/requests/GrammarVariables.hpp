#ifndef GRAMMARVARIABLES_HPP
#define GRAMMARVARIABLES_HPP

#include <iostream>
#include <string>
#include <vector>

#include "../../includes/enumFactory.h"
#define E_VAR_TYPE_ENUM(XX)       \
	XX(NO_VAR_TYPE, = 0x00000001) \
	XX(KEY, = 0x00000010)         \
	XX(VALUE, = 0x00000100)       \
	XX(ONLY_VALUE, = 0x00001000)

DECLARE_ENUM(e_var_type, E_VAR_TYPE_ENUM)

class GrammarVariables
{

public:
	GrammarVariables(std::string name = "", std::vector<std::string> tokens = std::vector<std::string>(), e_var_type type = NO_VAR_TYPE);
	GrammarVariables(GrammarVariables const &src);
	~GrammarVariables();

	GrammarVariables &operator=(GrammarVariables const &rhs);
	std::ostream &print(std::ostream &o) const;
	std::string getName() const;
	e_var_type getType() const;
	std::vector<std::string> &getTokens();

private:
	std::string _name;
	std::vector<std::string> _tokens;
	e_var_type _type;
};

std::ostream &operator<<(std::ostream &o, GrammarVariables const &i);

#endif /* ************************************************ GRAMMARVARIABLES_H */