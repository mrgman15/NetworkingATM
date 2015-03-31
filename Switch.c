Server and client
Do while into a switch
 
The switch statement delegates information to be split
Parse through
 
Initially go into a while loop looking for quit.  PArseing done through another while then hit a switch for codes
 

printf ("Please enter 1 to create a new account or 2 to log into an existing account");
    scanf ("%i", %starter);

if (starter == 1)
    printf ("Please enter your first name: ");
        scanf ("%s", %firstName);
    printf ("Please enter your last name: ");
        scanf ("%s", %lastName);
    printf ("Please choose a four digit pin: ");
        scanf ("%i", %Pin);
    // check for four digits
    printf ("Please enter your drivers license: ");
        scanf ("%i", %DL);
    // check for 8 digits
    printf ("Please enter your social security number: ");
        scanf ("%I", %SN);
    printf ("Please enter your email address");
        scant ("%s", &EmailAddress)


    //create bullshit

if (starter == 2)

    printf ("Please enter your first name: ");
        scanf ("%s", %firstName);
    printf ("Please enter your last name: ");
        scanf ("%s", %lastName);
    printf ("Please enter your pin: ");
        scanf ("%i", %Pin);
    //send log in
    //wait for log in

    while ( input!= quit)
        {
            printf ("")
            printf ("Please enter your command.  Deposit, withdrawl, check balance, buy stamps, recent transactions quit");
            scanf ("%s" %Choice);

                if (choice == deposit)
                    {
                        printf ("How much would you like to despoit?");
                            scanf ("%d" %amount);
                    }

                if (choice == withdrawl)
                    {
                        printf ("How much would you like to withdrawl?");
                            scanf ("%d" %amount);
                    }
                if (choice == check balance)
                    {

                    }
                if (choice == buy stamps)
                    {
                        printf ("How many stamps would you like to buy?");
                            scanf ("%d" %amount);
                    }
                if (choice == recent transactions)
                    {
                        printf ("How many previous transactions would you like to see?  1-5");
                            scanf ("%d" %amount);
                    }
                if (choice == quit)
                    {

                    }
        }


switch(message[0])
{
	case ‘101’ :
		//create account
		break;
	case ‘201’ :
		//Authentication
		break;
	case ‘301’ :
		//Deposit
		break;
	case ‘302’ :
		//When machine is full
		break;
	case ‘401’ :
		//Withdraw
		break;
	case ‘402’ :
		//When machine is empty
		break;
	case ‘501’ :
		//Asks for balance
		break;
	case ‘601’ :
		//Show most recent 1-5 transactions
		break;
	case ‘701’ :
		//User wants to buy stamps and needs to verify if enough money
		break;
	case ‘702’ :
		//Machine out of stamps
		break;
	case ‘801’ :
		//Logout exit
		break;
	default :
		 message = "Invalid Request";
		write(sock , message , strlen(message));
}



Client Switch
message[1];
printf(“%s”, message[1]);

switch(message[0]){
	case ‘103’ :
		//Account creation failed show which invalid entry
		printf(“Account creation failed.\n”);
		break;
	case ‘104’ :
		//Account was created log in
		printf(“Account was created and you are now logged in. \n”);
		break;
	case ‘105’ :
		//Account already exists
		printf(“Account already exists. \n”);
		break;
	case ‘203’ :
		//Failed attempt to authenticate
		printf(“Authentication failed. \n”);
		buffer = 
		break;
	case ‘204’ :
		//Exceeded number of authentication attempts
		printf(“Exceeded number of authentication attempts. \n”);
		printf(“You will be disconnected. \n”);
		close(sockfd);
		break;
	case ‘205’ :
		//Authentication succeeded 
		printf(“Authentication succeeded! \n”);
		printf(“You are now logged in! \n”);
		break;
	case ‘303’ :
		// Deposit worked, New balance is returned
		printf(“Deposit succesful! \n”);
		printf(“Your new balance is $%s. \n”, message[1]);
		break;
	case ‘304’ :
		//Deposit failed
		printf(“Deposit failed! \n”);
		printf(“Your balance is still $%s. \n”, message[1]);
		break;

	case ‘305’ :
		//Atm is full so return deposit
		printf(“Deposit failed!. \n “);
		printf(“Your deposit has been returned \n“);
		printf(“Your balance is still $%s. \n”, message[1]);
		break;		
		
	case ‘403’ :
		//Withdraw is successful
		printf(“Your withdraw was successful! \n“);
		printf(“Your new balance is $%s. \n”, message[1]);
		break;

	case ‘404’ :
		//Withdraw was unsuccessful because of insufficient funds
		printf(“Your withdrawal was unsuccessful! \n“);
		printf(“Insufficient funds. \n“);
		printf(“Your balance is still $%s. \n”, message[1]);
		break;

	case ‘405’ :
		//Not enough money in ATM for withdrawal
		printf(“Your withdrawal was unsuccessful! \n“);
		printf(“Not enough money in ATM for that withdrawal amount. \n“);
	break;

	case ‘503’ :
		//Returns balance request
		printf(“Your balance is $%s. \n”, message[1]);
	break;

	case ‘603’ :
		//Transactions ????	
	break;

	case ‘703’ :
		//Not enough money for stamps
		printf(“Transaction failed. \n”);
		printf(“Insufficient funds to buy stamps. \n”);
		break;

	case ‘704’ :
	//Buying stamps was successful return new balance
		printf(“Your transaction was successful! \n“);
		printf(“Your new balance is $%s. \n”, message[1]);
		break;

	case ‘705’ :
		//Not enough stamps left
		printf(“Not enough stamps available for this transaction. \n”);
	
	case ‘803’ :
		//Successful logout
		printf(“Logout successful \n”);
		printf(“You are now logged out. \n”);

	case ‘908’ :
		//Missing entries
		printf(“Your request was missing entries. \n”);
		printf(“Please try again. \n”);	
	break;

	case ‘909’ :
		//Unknown error code received
		printf(“Unknown error occurred. \n”);
		printf(“Please try again. \n”);
		break;

	default :
		 printf(“Error: Invalid Request. \n”);
}


printf(“ \n”);

Things to do:
Parsing function - Jordan
Switch Statements - Gerardo
Database interaction - Chaz
Form Validation Functions - Jordan 







Parser:
char message[10][80];
int i,k,j;
j=0;
i=0;
k=0;
while(buffer[j] != EOL){
if(buffer[j] == ‘ ‘){
	i++;
	k=0;
	j++;
}
else{
message[i][k] = buffer[j];
j++;
}
}
