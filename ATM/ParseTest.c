#include "Parse.h"

int main(){
	char buffer[256] = "Jordan Sanders 1234 12346546 123123123 jms0666@unt.edu";
	char** message1 = parse(buffer);

	int i,error;

	for(i = 0; i<6; i++){
		//Parse test
		//printf("%s\n",message1[i]);

		//Validation Test
		/*
		error = validateEmail(message1[i]);
		if(error == 1){
			printf("%s is not an email\n", message1[i]);
		}
		else printf("%s is an email\n", message1[i]);
		//*/
	}
	//char* file = createFileName(message1);
	message1 = parseFile("Jordan-1234");
	printf("%s\n",message1[6]);
	return 0;
}
