#include <stdio.h>
#include <string.h>
#include <curses.h>

void replace( char swearFile[], char siteFile[] );
void replaceWords( char siteFile[], char badWord[] );

int main()
{
  char swearFile[20] = "swearFile.txt";
  char string[100] = "www.example.com.txt";

  replace(swearFile,string);

return 0;
}


void replace( char swearFile[], char siteFile[] )
{
    FILE *f;
    char s1[200],*p;
    char swearWord[30];
    f = fopen(swearFile, "r");
    /* loops through the file of bad words */
    while (fgets(s1, 200, f))
    {

      while (fgets(s1, 200, f))
        {

    p=strtok(s1, ";\n");
    strcpy(swearWord,p);
    /*on each word searches the html file to replace it*/
    do
    {
        
        replaceWords(siteFile,swearWord);
    }    
    while(p=strtok(NULL,";\n"));
	
}

	}
  fclose(f);


}

void replaceWords( char filename[] , char badWord[] )
{
	char Buffer[4095];
    char *TemporaryFileName = "temp.txt";
    char Find[30];
    char Replace[30];
    FILE *Input,*Output;
	
    Input = fopen(filename, "r");
    Output = fopen(TemporaryFileName, "w");
    
    
    strcpy(Find,badWord);
    strcpy(Replace,"CENSORED");
 
 
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
            /*writing data till the source string location*/
            fwrite(Start, 1, Stop - Start, Output);
            /*writing Replacement string to the output file**/
            fwrite(Replace, 1, strlen(Replace), Output);
            /*moving the pointer to the next char of source string*/
            Start = Stop + strlen(Find);
        }
        
    }
 
    fclose(Input);
    fclose(Output);
    /*Remove the old file*/
    remove(filename);
    /*rename the temporary file*/
    rename(TemporaryFileName, filename);
	
	
	
}
