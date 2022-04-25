#ifndef ONESTRING_HPP
# define ONESTRING_HPP

# include <iostream>
# include "ALeaf.hpp"
# include <string>

class OneString : public ALeaf
{

	public:

		OneString(std::string value);
		OneString( OneString const & src );
		~OneString();

		OneString &		operator=( OneString const & rhs );
		bool isValid(int) const;
		std::ostream & print( std::ostream & o) const;

	private:
		std::string _value;

};


#endif /* ******************************************************* ONESTRING_H */