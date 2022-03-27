#ifndef ACTIONFORKEY_HPP
# define ACTIONFORKEY_HPP

# include <iostream>
# include <string>
# include "ConfigConsumer.hpp"

class ConfigConsumer;
class ActionForKey
{

	public:

		ActionForKey(int min = 0 , int max = 0, void (ConfigConsumer::*consume)(void*) const = NULL);
		ActionForKey( ActionForKey const & src );
		~ActionForKey();

		ActionForKey &		operator=( ActionForKey const & rhs );
		bool isValid(int level) const;

	private:
		int _min_level;
		int _max_level;
		void (ConfigConsumer::*_consume)(void*) const;

};

std::ostream &			operator<<( std::ostream & o, ActionForKey const & i );

#endif /* **************************************************** ACTIONFORKEY_H */