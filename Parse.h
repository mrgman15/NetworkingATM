#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char** parse(char* buffer){
	int i, k=0, j=0;
	char** message = malloc(10 * sizeof(char*));
	for(i=0;i<80;i++){
		message[i] = malloc(80 * sizeof(char));
	}
	i=0;
	while(buffer[j] != '\0'){
		if(buffer[j] == ' '){
			message[i][k] = '\0';
			i++;
			k=0;
			j++;
		}
		else{
			message[i][k] = buffer[j];
			j++;
			k++;
		}
	}
	message[i][k] = '\0';
	return message;
}

int validateName(char* name){
	int i;
	while(name[i] != '\0'){
		if(!((name[i]>='a'&& name[i]<='z') || (name[i]>='A' && name[i]<='Z'))){
			return 1;
		}
		i++;
	}
	if(i>20) return 1;
	return 0;
}

int validatePin(char* pin){
	int i;
	while(pin[i] != '\0'){
		if(!(pin[i]>='0'&& pin[i]<='9')){
			return 1;
		}
		i++;
	}
	if(i != 4) return 1;
	return 0;		
}

int validateDL(char* DL){
	int i;
	while(DL[i] != '\0'){
		if(!(DL[i]>='0'&& DL[i]<='9')){
			return 1;
		}
		i++;
	}
	if(i != 8) return 1;
	return 0;	
}

int validateSSN(char* SSN){
	int i;
	while(SSN[i] != '\0'){
		if(!(SSN[i]>='0'&& SSN[i]<='9')){
			return 1;
		}
		i++;
	}
	if(i != 9) return 1;
	return 0;	
}

int validateEmail(char* email){
	int i,symbol=0,period=0;
	while(email[i] != '\0'){
		if(!((email[i]>='0'&& email[i]<='9') || (email[i]>='a'&& email[i]<='z') || (email[i]>='A' && email[i]<='Z')) && email[i] != '.' && email[i] != '@'){
			return 1;
		}
		if(email[i] == '@') symbol++;
		if(email[i] == '.' && symbol == 1) period++;
		i++;
	}
	if((i>40) || (symbol <1 || symbol > 1) || (period<1)) return 1;
	return 0;
}

int validateAmount(char* amount){
	int i;
	while(amount[i] != '\0'){
		if(!(amount[i]>='0'&& amount[i]<='9')){
			return 1;
		}
		i++;
	}
	if(i > 10) return 1;
	return 0;	
}

int validateTransactions(char* num){
	int i;
	while(num[i] != '\0'){
		if(!(num[i]>='0'&& num[i]<='5')){
			return 1;
		}
		i++;
	}
	if(i != 1) return 1;
	return 0;	
}

void putToFile(char* fileName, char** info){
	FILE* account = fopen(fileName, "w");
	int i=0;
	while(info[i][0]>= '0' && info[i][0] <= 'z'){
		fprintf(account,"%s ",info[i]);
		i++;
	}
	fclose(account);
}

char* createFileName(char** info){
	char* fileName = malloc(80 * sizeof(char));
	sprintf(fileName,"%s-%s-%s",info[0],info[1],info[2]);
	return fileName;
}

