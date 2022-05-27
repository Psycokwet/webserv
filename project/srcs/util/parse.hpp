#ifndef PARSE_HPP
# define PARSE_HPP
# include <cstring>
# include <string.h>
# include <ctype.h>
# include <algorithm>
# include <fstream>
void ltrim(std::string &s);
void rtrim(std::string &s);
void trim(std::string &s);
int isReallyBlank(int c);

#define PREPARE_AND_SKIP_EMPTY_LIGNES(str)                       \
	std::replace_if(str.begin(), str.end(), isReallyBlank, ' '); \
	trim(str);                                                   \
	if (str == "" || str[0] == ';')                              \
		continue;
#endif