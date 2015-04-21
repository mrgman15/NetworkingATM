#include <stdio.h>
#include <string.h>


void blacklist( char fileName[], char siteName[] );

int main()
{
	char filename[20] = "blacklist.txt";
  char string[100] = "www.my.unt.edu";
	blacklist(filename,string);
  
	
return 0;
}


void blacklist( char fileName[], char siteName[] )
{
    FILE *f;
    char s1[200],*p;
    f = fopen(fileName, "r");
    while (fgets(s1, 200, f))
    {

      while (fgets(s1, 200, f))
        {

    p=strtok(s1, ";\n");

    do
    {
        if (strcmp(p, siteName) == 0)
		{
			printf("This site is blacklisted: %s\n", p);
		}
	
    }
    while(p=strtok(NULL,";\n"));
	
}

	}
  fclose(f);

}
