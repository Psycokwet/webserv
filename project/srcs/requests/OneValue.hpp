#ifndef ONEVALUE_HPP
# define ONEVALUE_HPP

# include <iostream>
# include "ALeaf.hpp"
# include <string>

class OneValue : public ALeaf
{

	public:

		OneValue();
		OneValue( OneValue const & src );
		~OneValue();

		OneValue &		operator=( OneValue const & rhs );
		bool isValid(int) const;
		std::ostream & print( std::ostream & o) const;

	private:
		int _value;

};


#endif /* ******************************************************* ONEVALUE_H */