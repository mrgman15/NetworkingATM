#include "Parse.h"

int main(){
	char buffer[256] = "Jordan Sanders 1235 12346546 123123123 jms0666@unt.edu";
	char** message1 = parse(buffer);

	int i,error;

	for(i = 0; i<6; i++){
		//Parse test
		//printf("%s\n",message1[i]);

		//Validation Test
		//*
		error = validateEmail(message1[i]);
		if(error == 1){
			printf("%s is not an email\n", message1[i]);
		}
		else printf("%s is an email\n", message1[i]);
		//*/
	}
	return 0;
}
