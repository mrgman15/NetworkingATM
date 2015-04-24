#include "proxy3.h"

int main(){
  char string[] = "Ape something ape ApE APe aPe aPE ape\n";
  printf("Original String: %s\n",string);
  profanityFilter(string);
  printf("Filtered String: %s\n",string);


  return 0;
}
