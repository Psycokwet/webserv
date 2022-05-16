#include "../../includes/webserv.h"


unsigned short ft_bitswap16(unsigned short y)
{
    y = y >> 8 | y << 8;
    return y;
}
//La fonction ntohs() convertit un entier court non signé netshort depuis l’ordre des octets du réseau vers celui de l’hôte.
unsigned short ft_ntohs(unsigned short i)
{
    if (BYTE_ORDER == BIG_ENDIAN)
        return(i);
    else if (BYTE_ORDER == LITTLE_ENDIAN)
        return (ft_bitswap16(i));
    return (0);
}

unsigned int ft_bitswap32(unsigned int y)
{
    y = (y & BYTE_0) >> 24 | (y & BYTE_3) << 24 |
        (y & BYTE_1) >> 8 | (y & BYTE_2) << 8;
    return (y);
}
//La fonction ntohl() convertit l’entier non signé netlong depuis l’ordre des octets du réseau vers celui de l’hôte.
unsigned int ft_ntohl(unsigned int i)
{
    if (BYTE_ORDER == BIG_ENDIAN)
        return (i);
    else if (BYTE_ORDER == LITTLE_ENDIAN)
        return (ft_bitswap32(i));
    return (0);
}