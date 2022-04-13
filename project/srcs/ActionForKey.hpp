#ifndef ACTIONFORKEY_HPP
# define ACTIONFORKEY_HPP

# include <iostream>
# include <string>
# include "ConfigConsumer.hpp"

class ConfigConsumer;
class ActionForKey
{

	public:

		ActionForKey(int min = 0 , int max = 0, std::vector<std::string> parents=std::vector<std::string>());
		ActionForKey( ActionForKey const & src );
		~ActionForKey();

		ActionForKey &		operator=( ActionForKey const & rhs );
		bool isValid(int level, std::string parents) const;
		int getMinLevel() const;
		int getMaxLevel() const;
		std::vector<std::string> getParents() const;

	private:
		int 			_min_level;
		int 			_max_level;
		std::vector<std::string>		_parents;
};

std::ostream &			operator<<( std::ostream & o, ActionForKey const & i );

#endif /* **************************************************** ACTIONFORKEY_H */