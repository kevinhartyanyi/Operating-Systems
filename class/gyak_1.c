#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	char* str = malloc(10);
	if(!stpcpy(str, "123456789"))
	{
		printf("Hiba\n");
	}
	printf("%s\n", str);
	if(realloc(str,80))
{
printf("Hiba\n");
}

if(realloc(str,100000000000))
{
printf("Hiba\n");
}
else
{
printf("%s")
}

return 0;
}
