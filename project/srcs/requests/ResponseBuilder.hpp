#ifndef RESPONSEBUILDER_HPP
# define RESPONSEBUILDER_HPP

# include <iostream>
# include <string>

class ResponseBuilder
{

	public:

		ResponseBuilder();
		ResponseBuilder( ResponseBuilder const & src );
		~ResponseBuilder();

		ResponseBuilder &		operator=( ResponseBuilder const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, ResponseBuilder const & i );

#endif /* ************************************************* RESPONSEBUILDER_H */