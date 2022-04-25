#ifndef ALEAF_HPP
# define ALEAF_HPP

# include <iostream>
# include <string>
# include "AGrammar.hpp"

class ALeaf : public AGrammar
{

	public:
		ALeaf( ALeaf const & src );
		~ALeaf();
		virtual std::ostream & print( std::ostream & o) const;
		virtual bool isValid(int) const;

		ALeaf &		operator=( ALeaf const & rhs );

	private:
		ALeaf();

};


#endif /* *********************************************************** ALEAF_H */