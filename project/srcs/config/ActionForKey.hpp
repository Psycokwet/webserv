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
				int min = 0,
				int max = 0,
				std::vector<std::string> parents = std::vector<std::string>(),
				AServerItem *(consume)(Node *node, AServerItem *currentServerItem) = NULL);
		ActionForKey( ActionForKey const & src );
		virtual ~ActionForKey();

		ActionForKey &		operator=( ActionForKey const & rhs );
		bool isValid(int level, std::string *parent) const;
		int getMinLevel() const;
		int getMaxLevel() const;
		std::vector<std::string> getParents() const;
		AServerItem *consume(Node *node, AServerItem *currentServerItem) const;

		class ConsumerNotDefined : public std::exception
        {
            public:
                virtual const char *what() const throw()
				{
					return "Consumer function cannot be used since it haven't been defined.";
				}
        };

	private:
		int 			_min_level;
		int 			_max_level;
		std::vector<std::string>		_parents;
		AServerItem *(*_consume)(Node *node, AServerItem *currentServerItem);
};

std::ostream &			operator<<( std::ostream & o, ActionForKey const & i );

#endif /* **************************************************** ACTIONFORKEY_H */