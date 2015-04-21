#include <stdio.h>
#include <string.h>
#include <curses.h>

void replaceWords( char filename[] );

int main()
{
	char filename[20] = "data.txt";
	replaceWords(filename);
	
return 0;
}

void replaceWords( char filename[] )
{
	char Buffer[4095];
    char *TemporaryFileName = "temp.txt";
    char Find[20];
    char Replace[20];
    int line = 0;
    FILE *Input,*Output;
	
    Input = fopen(filename, "r");
    Output = fopen(TemporaryFileName, "w");
 
    printf("Enter the word to find :");
    gets(Find);
    printf("Enter the New word to replace :");
    gets(Replace);
 
 
    printf("Find:    %s\n", Find);
    printf("Replace: %s\n", Replace);
 
    // For each line...
    while(NULL != fgets(Buffer, 4095, Input))
    {
        char *Stop = NULL;
        char *Start = Buffer;
 
        while(1)
        {
 
            /*getting the first location of the source string*/
            Stop = strstr(Start, Find);
            if(NULL == Stop)
            {
                fwrite(Start, 1, strlen(Start), Output);
                break;
            }
            printf("@found at Line  <%d>\n",line);
            /*writing data till the source string location*/
            fwrite(Start, 1, Stop - Start, Output);
            /*writing Replacement string to the output file**/
            fwrite(Replace, 1, strlen(Replace), Output);
            /*moving the pointer to the next char of source string*/
            Start = Stop + strlen(Find);
        }
        line++;
    }
 
    fclose(Input);
    fclose(Output);
    /*Remove the old file*/
    remove(filename);
    /*rename the temporary file*/
    rename(TemporaryFileName, filename);
	
	
	
}
