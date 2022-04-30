#ifndef GRAMMARVARIABLES_HPP
# define GRAMMARVARIABLES_HPP

# include <iostream>
# include <string>
# include <vector>

class GrammarVariables
{

	public:

		GrammarVariables(std::string name = "", std::vector<std::string> tokens = std::vector<std::string> ());
		GrammarVariables(GrammarVariables const & src);
		~GrammarVariables();

		GrammarVariables &		operator=( GrammarVariables const & rhs );
		std::ostream &			print( std::ostream & o) const;
		std::string				getName() const;

	private:
		std::string _name;
		std::vector<std::string> _tokens;

};

std::ostream &			operator<<( std::ostream & o, GrammarVariables const & i );

#endif /* ************************************************ GRAMMARVARIABLES_H */