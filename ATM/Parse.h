#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

char** parse(char* buffer){
	int i, k=0, j=0;
	char** message = malloc(10 * sizeof(char*));
	for(i=0;i<10;i++){
		message[i] = malloc(26* sizeof(char));
	}
	i=0;
	while(buffer[j] != '\n' && buffer[j] != '\0'){
		if(buffer[j] == ' '){
			message[i][k] = ' ';
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

//Opens file, retrieves information, returns array of strings containing account info
char** parseFile(char* fileName){
	FILE* input = fopen(fileName, "r");
	char string[261];
	char c;
	int i = 0;
	while ((c = getc(input)) != EOF){
		string[i++] = c;
    }
	char** message = parse(string);
	fclose(input);
	return message;
}


int validateName(char* name){
	return 0;
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
	return 0;
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
	return 0;
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
	return 0;
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
	return 0;
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
	return 0;
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
	return 0;
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

void createFile(char* fileName, char** info){
	FILE* account = fopen(fileName, "w");
	int i=1;
	while(info[i][0]>= '0' && info[i][0] <= 'z'){
		fprintf(account,"%s ",info[i]);
		i++;
	}
	fprintf(account, "%s ", "0");
	fclose(account);
}

char* createFileName(char** info){
	char* fileName = malloc(80 * sizeof(char));
	sprintf(fileName,"%s-%s",info[1],info[3]);
	return fileName;
}

char* createFileName2(char** info){
	char* fileName = malloc(80 * sizeof(char));
	sprintf(fileName,"%s-%s",info[1],info[2]);
	return fileName;
}

int doesFileExist(char *filename)
{
  struct stat   buffer;
  return (stat (filename, &buffer) == 0);
}

int validateCreate(char** input){
	return 0;
	printf("");
	if(validateName(input[1]) == 1) return 1;
	printf("");
	if(validateName(input[2]) == 1) return 1;
	printf("");
	if(validatePin(input[3]) == 1) return 1;
	printf("");
	if(validateDL(input[4]) == 1) return 1;
	printf("");
	if(validateSSN(input[5]) == 1) return 1;
	printf("");
	if(validateEmail(input[6]) == 1) return 1;
	printf("");
	return 0;
}

int validateLogin(char** input){
	return 0;
	//printf("val0\n");
	if(validateName(input[1]) == 1){
		return 1;
	}
	printf("");
	if(validatePin(input[2]) == 1) return 1;
	return 0;
}

void addTransaction(char** input, char* transactionType, char* info){
	int i = 7;
	strcat(transactionType,info);
	while(input[i] != '\0'){
		i++;
	}

	if(i == 11){
		input[7] = input[8];
		input[8] = input[9];
		input[9] = input[10];
		input[10] = input[11];
		input[11] = transactionType;
	}

	else{
		input[i] = transactionType;
	}
}
