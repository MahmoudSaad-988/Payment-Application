
#include <stdio.h>
#include <string.h>
#pragma warning (disable:4996)
#pragma warning (disable:6031)
#include <ctype.h>
#include"terminal.h"
#define MAXDATE 10
#include<time.h>
#include<stdlib.h>


EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
    strcpy(termData->transactionDate, "");
    char dd[11];
    char mm[11];
    char yyyy[11];

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    sprintf(dd, "%02d", tm.tm_mday);
    strcat(termData->transactionDate, dd);
    strcat(termData->transactionDate, "/");
    sprintf(mm, "%02d", (tm.tm_mon + 1));
    strcat(termData->transactionDate, mm);
    strcat(termData->transactionDate, "/");
    sprintf(yyyy, "%d", (tm.tm_year + 1900));
    strcat(termData->transactionDate, yyyy);

    printf("local date: ");
    puts(termData->transactionDate);

    // good formate (DD/MM/YYYY)
    if (termData->transactionDate == NULL)
    {
        strcpy(termData->transactionDate, "\0");
        return WRONG_DATE;
    }
    else if (strlen(termData->transactionDate) < 10)
    {
        strcpy(termData->transactionDate, "\0");
        return WRONG_DATE;
    }
    else if ((termData->transactionDate[0] < 48) || (termData->transactionDate[0] > 57)) // first D should be (0:9)
    {
        strcpy(termData->transactionDate, "\0");
        return WRONG_DATE;
    }
    else if ((termData->transactionDate[1] < 48) || (termData->transactionDate[1] > 57)) // second D should be (0:9)
    {
        strcpy(termData->transactionDate, "\0");
        return WRONG_DATE;
    }
    else if (termData->transactionDate[2] != 47) // check '/' position
    {
        strcpy(termData->transactionDate, "\0");
        return WRONG_DATE;
    }
    else if ((termData->transactionDate[3] < 48) || (termData->transactionDate[3] > 57)) // first M should be (0:9)
    {
        strcpy(termData->transactionDate, "\0");
        return WRONG_DATE;
    }
    else if ((termData->transactionDate[4] < 48) || (termData->transactionDate[4] > 57)) // second M should be (0:9)
    {
        strcpy(termData->transactionDate, "\0");
        return WRONG_DATE;
    }
    else if (termData->transactionDate[5] != 47) // check '/' position
    {
        strcpy(termData->transactionDate, "\0");
        return WRONG_DATE;
    }
    else if ((termData->transactionDate[6] < 48) || (termData->transactionDate[6] > 57)) // first Y should be (0:9)
    {
        strcpy(termData->transactionDate, "\0");
        return WRONG_DATE;
    }
    else if ((termData->transactionDate[7] < 48) || (termData->transactionDate[7] > 57)) // second Y should be (0:9)
    {
        strcpy(termData->transactionDate, "\0");
        return WRONG_DATE;
    }
    else if ((termData->transactionDate[8] < 48) || (termData->transactionDate[8] > 57)) // third M should be (0:9)
    {
        strcpy(termData->transactionDate, "\0");
        return WRONG_DATE;
    }
    else if ((termData->transactionDate[9] < 48) || (termData->transactionDate[9] > 57)) // fourth M should be (0:9)
    {
        strcpy(termData->transactionDate, "\0");
        return WRONG_DATE;
    }
    else
    {
        
        return TERMINAL_OK;
      }
}


EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* terminalData) {
	uint32_t card_month, card_year, transaction_month, transaction_year;
	card_month = (cardData->cardExpirationDate[0] - '0') * 10 + (cardData->cardExpirationDate[1] - '0');
	card_year = (cardData->cardExpirationDate[3] - '0') * 10 + (cardData->cardExpirationDate[4] - '0') + 2000; 
  
	transaction_month = (terminalData->transactionDate[3] - '0') * 10 + (terminalData->transactionDate[4] - '0');
	transaction_year = (terminalData->transactionDate[6] - '0') * 1000 + (terminalData->transactionDate[7] - '0') * 100 + (terminalData->transactionDate[8] - '0') * 10 + (terminalData->transactionDate[9] - '0');
	if ((card_year == transaction_year) && (card_month < transaction_month)) 
		return WRONG_EXP_DATE;
	if(card_year < transaction_year)
		return WRONG_EXP_DATE;
	return TERMINAL_OK;
  }


EN_terminalError_t getTransactionAmount(ST_terminalData_t* terminalData) {
	float temp = 30000;
  printf("Max Amount of Transaction is : %f  \n  ",temp);
  temp=0;
	printf("Enter transaction amount:- ");
	scanf("%f", &temp);
	if (temp <= 0)
		return INVALID_AMOUNT;
	terminalData->transAmount = temp;
	return TERMINAL_OK;
}



EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* terminalData) {
	float temp_amount, temp_max;
	temp_amount = terminalData->transAmount;
	temp_max = terminalData->maxTransAmount;
	if (temp_amount > temp_max)
		return EXCEED_MAX_AMOUNT;
	return TERMINAL_OK;
}


EN_terminalError_t setMaxAmount(ST_terminalData_t* terminalData) {
	float temp = 30000;
	
	if (temp <= 0)
  
		return INVALID_AMOUNT;
  
	terminalData->maxTransAmount = temp;
	return TERMINAL_OK;
}
