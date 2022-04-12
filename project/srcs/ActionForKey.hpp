#ifndef ACTIONFORKEY_HPP
# define ACTIONFORKEY_HPP

# include <iostream>
# include <string>
# include "ConfigConsumer.hpp"

class ConfigConsumer;
class ActionForKey
{

	public:

		ActionForKey(int min = 0 , int max = 0, std::string parent1 = "", std::string parent2="");
		ActionForKey( ActionForKey const & src );
		~ActionForKey();

		ActionForKey &		operator=( ActionForKey const & rhs );
		bool isValid(int level, std::string parents) const;
		int getMinLevel() const;
		int getMaxLevel() const;
		std::string getParents1() const;
		std::string getParents2() const;

	private:
		int _min_level;
		int _max_level;
		std::string _parents1;
		std::string _parents2;

};

std::ostream &			operator<<( std::ostream & o, ActionForKey const & i );

#endif /* **************************************************** ACTIONFORKEY_H */