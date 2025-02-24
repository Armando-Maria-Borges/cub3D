#include "libft.h"

char	err(char *str)
{
	while(*str)
		write(2, str++, 1);
	return (1);
}
