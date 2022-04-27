#ifndef ACTIONFORKEY_HPP
# define ACTIONFORKEY_HPP

# include <iostream>
# include <string>
# include "ConfigConsumer.hpp"
# include "../server/AServerItem.hpp"

class ConfigConsumer;
class ActionForKey
{

	public:

		ActionForKey(
				int min = 0, // ! min, max is the allowed Deepness level of the directive.
				int max = 0, // ! min = 0, max = 2 for the key "server" meaning: server can be at the deepness < 2 and > 4.
				std::vector<std::string> parents = std::vector<std::string>());
		ActionForKey( ActionForKey const & src );
		virtual ~ActionForKey();

		ActionForKey &		operator=( ActionForKey const & rhs );
		bool isValid(int level, std::string *parent) const;
		int getMinLevel() const;
		int getMaxLevel() const;
		std::vector<std::string> getParents() const;

	private:
		int 							_min_level;
		int 							_max_level;
		std::vector<std::string>		_parents;
		AServerItem *(*_consume)(Node *, AServerItem *); // ! function pointer to an appropriate function
};

std::ostream &			operator<<( std::ostream & o, ActionForKey const & i );

#endif /* **************************************************** ACTIONFORKEY_H */