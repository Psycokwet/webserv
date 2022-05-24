#ifndef ASERVERITEM_HPP
# define ASERVERITEM_HPP

# include <iostream>
# include <string>

class AServerItem
{

	protected:
		AServerItem &		operator=( AServerItem const & rhs );
		AServerItem( AServerItem const & src );
		AServerItem();


	public:
		virtual ~AServerItem();
		virtual std::ostream & print( std::ostream & o) const;

};

std::ostream &			operator<<( std::ostream & o, AServerItem const & i );

#endif /* ***************************************************** ASERVERITEM_H */