#include <iostream>
#include <string>
#include <map>

#include <sstream>

#include <iostream>
#include <limits> // numberic_limits
#include <cstring>
#include <set>
int main ()
{
  std::set<int> myset;
  std::set<int>::iterator it;
  std::pair<std::set<int>::iterator,bool> ret;

  // set some initial values:
  for (int i=1; i<=5; ++i) myset.insert(i*10);    // set: 10 20 30 40 50

  ret = myset.insert(20);               // no new element inserted

  if (ret.second==false) 
	std::cout << "Can not insert " << *ret.first << std::endl;
}
// void	ft_skip_spacenl(const char *str, int *i)
// {
// 	while ((str[*i] == ' ' || str[*i] == '\t' || str[*i] == '\n')
// 	|| (str[*i] == '\r' || str[*i] == '\v' || str[*i] == '\f'))
// 		(*i)++;
// }

// int	ft_atoi(const char *str)
// {
// 	int	i;
// 	int	sign;
// 	int	num;

// 	i = 0;
// 	sign = 1;
// 	num = 0;
// 	ft_skip_spacenl(str, &i);
// 	if (str[i] == '-')
// 		sign = -1;
// 	if (str[i] == '-' || str[i] == '+')
// 		i++;
// 	while (str[i] >= '0' && str[i] <= '9')
// 		num = num * 10 + (str[i++] - '0');
// 	return (num * sign);
// }


// unsigned int	strToIp(std::string strIp) {
// 	size_t  sep = 0;
// 	unsigned int   n;
// 	unsigned char  m[4];
// 	size_t  start = 0;
// 	if (strIp == "localhost")
// 		strIp = "127.0.0.1";
// 	for (unsigned int i = 3 ; i != std::numeric_limits<uint32_t>::max(); i--) {
// 		sep = strIp.find_first_of('.', sep);
// 		std::string str = strIp.substr(start, sep);
//         std::cout << "\nsub str: " << str;
// 		n = ft_atoi(str.c_str());
//         std::cout << "\t n = " << n;
// 		m[i] = static_cast<unsigned char>(n);
//         std::cout << "\t m[" << i << "] = " << m[i];
// 		sep++;
// 		start = sep;
// 	}
// 	unsigned final = *(reinterpret_cast<unsigned int *>(m));
// 	return final;
// }

// uint32_t getDecimalValueOfIPV4_String(const char* ipAddress)
// {
//     uint8_t ipbytes[4]={};
//     int i =0;
//     int8_t j=3;
//     while (ipAddress+i && i<strlen(ipAddress))
//     {
//        char digit = ipAddress[i];
//        if (isdigit(digit) == 0 && digit!='.'){
//            return 0;
//        }
//         j=digit=='.'?j-1:j;
//         ipbytes[j]= ipbytes[j]*10 + atoi(&digit);

//         i++;
//     }

//     uint32_t a = ipbytes[0];
//     uint32_t b =  ( uint32_t)ipbytes[1] << 8;
//     uint32_t c =  ( uint32_t)ipbytes[2] << 16;
//     uint32_t d =  ( uint32_t)ipbytes[3] << 24;
//     return a+b+c+d;
// }


// int main() {
//     std::string s = "127.0.0.1";
//     unsigned int result = strToIp(s);
//     u_int32_t r2 = getDecimalValueOfIPV4_String(s.c_str());

//     std::cout << "\nValue of IP address in unsigned int: " << result << std::endl;
//     std::cout << "\nValue of IP address in unsigned int: " << r2 << std::endl;
// }