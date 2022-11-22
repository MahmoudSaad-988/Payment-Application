#include <stdio.h>
#include <string.h>
#include"application.h"
#include <unistd.h>
#include"../Server/server.h"
#include"../Terminal/terminal.h"
#include<time.h>
#include <stdlib.h>


void appstart(void) {
    ST_terminalData_t terminalData;
	ST_transaction_t transactionData;
	ST_cardData_t cardData;


	while (getCardHolderName(&cardData) == WRONG_NAME)
		printf("\n\t\tWRONG_NAME\n\nTry Again-");

	while (getCardPAN(&cardData) == WRONG_PAN)
		printf("\n\t\tWRONG_PAN\n\nTry Again-");

	while (getCardExpiryDate(&cardData) == WRONG_EXP_DATE)
		printf("\n\t\tWRONG_EXP_DATE\n\nTry Again-");


  while (getTransactionDate(&terminalData) == WRONG_DATE)
		printf("\n\t\tWRONG_DATE\n\nRe-");
 

   
	if (isCardExpired(&cardData, &terminalData)) {
		printf("\n\t\tThe Card is Expired.\n\n");
		return;
	}

	while (setMaxAmount(&terminalData) == INVALID_MAX_AMOUNT)
		printf("\n\t\tINVALID_MAX_AMOUNT\n\nRe-");

	while (getTransactionAmount(&terminalData) == INVALID_AMOUNT)
		printf("\n\t\tINVALID_AMOUNT\n\nRe-");


	transactionData.cardHolderData = cardData;
	transactionData.terminalData = terminalData;
	EN_transState_t transStatus = recieveTransactionData(&transactionData);


	if (transStatus == DECLINED_STOLEN_CARD)
		printf("\n\t\tStolen Card\n\n");
	else if (transStatus == DECLINED_INSUFFECIENT_FUND)
		printf("\n\t\tDECLINED_INSUFFECIENT_FUND\n\n");
	else if (transStatus == INTERNAL_SERVER_ERROR)
		printf("\n\t\tINTERNAL_SERVER_ERROR\n\n");
	else if (transStatus == DECLINED_EXEED_MAX_AMOUNT)
		printf("\n\t\tDECLINED_EXEED_MAX_AMOUNT Max Amount is  : 30000\n\n");
  
  else
  {printf("\n\t\tApproved Transaction \n");
  sleep(2);
		 
  time(&terminalData);
      printf("\n\This Transaction took place at this (date and time): %s", ctime(&terminalData));
    int i;    
      sleep(1);
    printf(" Refrence Number of this Transaction  : \t");   
      
        srand(time(0));
        printf(" %d \n", rand());  
    }
  

}
