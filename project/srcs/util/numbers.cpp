#include "numbers.hpp"

bool isIntNumber(std::string value)
{
	unsigned int i = 0;
	while(isdigit(value[i])) i++;
	if (i != strlen(value.c_str()))
		return false;
	return true;
}

bool isItTwoDigitHexa(std::string str)
{
	if (str.size() != 2 || !isxdigit(str.at(0)) || !isxdigit(str.at(1)))
		return false;
	return true;
}

int getIntNumberWithGuard(std::string value)
{
	if (isIntNumber(value) == false)
		throw InvalidIntValueError();
	long int size = atol(value.c_str());
	if (size > INT_MAX)
		throw InvalidIntValueError();
	else
		return size;
}

int getIntNumberWithoutGuard(std::string str)
{
	return strtol(str.c_str(), NULL, 10);
}

int getTwoDigitHexaWithoutGuard(std::string str)
{
	return strtol(str.c_str(), NULL, 16);
}