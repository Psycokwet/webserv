#ifndef AGRAMMAR_HPP
# define AGRAMMAR_HPP

# include <iostream>
# include <string>
# include <map>
# include <list>
# include <vector>

class AGrammar
{

	public:
		static std::map<std::string, AGrammar*> _dictionnary_rules;
		static std::string request;
		static unsigned index;
		static reset(); //Set request, index_x, index_y at 0;
		static feed(char*); // add chars to the request

		AGrammar( AGrammar const & src );
		~AGrammar();
		virtual std::ostream & print( std::ostream & o) const;

		AGrammar &		operator=( AGrammar const & rhs );

		int consume();

	private:
		AGrammar();
		std::list<std::string> _tokens;

};

std::ostream &			operator<<( std::ostream & o, AGrammar const & i );

#endif /* ******************************************************** AGRAMMAR_H */