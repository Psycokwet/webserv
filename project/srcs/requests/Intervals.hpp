#ifndef INTERVALS_HPP
# define INTERVALS_HPP

# include <iostream>
# include "ALeaf.hpp"
# include <string>

class Intervals : public ALeaf
{

	public:

		Intervals();
		Intervals( Intervals const & src );
		~Intervals();

		Intervals &		operator=( Intervals const & rhs );
		bool isValid(int) const;
		std::ostream & print( std::ostream & o) const;

	private:
		int _start
		int _end;

};


#endif /* ******************************************************* INTERVALS_H */