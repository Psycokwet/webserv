#ifndef NUMBER_HPP
# define NUMBER_HPP
# include <cstring>
# include <string.h>
# include <ctype.h>
# include <algorithm>
# include <fstream>
# include <climits>

bool isIntNumber(std::string value);
bool isItTwoDigitHexa(std::string str);
int getIntNumberWithGuard(std::string value);
int getIntNumberWithoutGuard(std::string str);
int getTwoDigitHexaWithoutGuard(std::string str);

class InvalidIntValueError : public std::exception
{
	public:
	virtual const char *what() const throw()
	{
		return "ERROR: invalid int value from string";
	}
};

#endif