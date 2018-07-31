#include <stdio.h>
#include "gets_s.h"
#include "strtok_r.h"

int main()
{
	char buf[1024];
	char delim[8];
	char* pstr[4] = {0};
	char* lastStr = buf;
	int i;
	
	printf("获取字符串...\n");
	gets_s (buf, sizeof (buf));
	
	printf("获取字符串分割符...\n");
	gets_s (delim, sizeof (delim));
	
	for (i = 0; i <= 3; i++) {
		pstr[i] = strtok_r(NULL, delim, &lastStr);
		printf ("%s\n", pstr[i]);
	}
	
	/*printf ("%s\n", lastStr);
	pstr[0] = strtok_r(NULL, delim, &lastStr);
	printf ("%s\n", lastStr);
	pstr[1] = strtok_r(NULL, delim, &lastStr);
	printf ("%s\n", lastStr);
	pstr[2] = strtok_r(NULL, delim, &lastStr);
	printf ("%s\n", lastStr);
	pstr[3] = strtok_r(NULL, delim, &lastStr);
	
	for (i = 0; i <= 0; i++) {
		printf ("%s\n", pstr[i]);
	}*/

	return 0;
}
