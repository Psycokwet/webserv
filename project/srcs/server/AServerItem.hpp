#ifndef ASERVERITEM_HPP
# define ASERVERITEM_HPP

# include <iostream>
# include <string>
#include <ctype.h> //isalnum
#include <cstdlib> //atoi
#include <climits> // INT_MAX
# include "../config/Node.hpp"
#include "../../includes/ConfigDataStructures.hpp"


class AServerItem
{

	protected:
		AServerItem &		operator=( AServerItem const & rhs );
		AServerItem( AServerItem const & src );
		AServerItem();


	public:
		virtual ~AServerItem();
		virtual std::ostream & print( std::ostream & o) const;
      	virtual AServerItem *consume(Node *node);


      class MultipleDeclareError : public std::exception
      {
          public:
              virtual const char *what() const throw()
              {
                return "ERROR: Directive is multiple declared.";
              }
      };
      class IncompleteDirective : public std::exception
      {
          public:
              virtual const char *what() const throw()
              {
                return "ERROR: Directive doesn't have an argument, or too much args";
              }
      };

      class DuplicateUriError : public std::exception
      {
          public:
              virtual const char *what() const throw()
              {
                return "ERROR: Uri is duplicated.";
              }
      };
      class IncompleteTypeError : public std::exception
      {
          public:
              virtual const char *what() const throw()
              {
                return "ALocation childs should have this function reimplemented";
              }
      };
      class InvalidValueError : public std::exception
      {
          public:
              virtual const char *what() const throw()
              {
                return "ERROR: invalid value for directive";
              }
      };

};

std::ostream &			operator<<( std::ostream & o, AServerItem const & i );

#endif /* ***************************************************** ASERVERITEM_H */