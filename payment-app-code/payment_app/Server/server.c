#include "server.h"
#include<stdio.h>
#include<string.h>
#pragma warning (disable:4996)
#include <unistd.h>
 #include <stdlib.h>

ST_accountsDB_t accountsDB[255] = {
	{98328, "1234567891234567891"},
	{62510, "9876543219876543210"},
	{38614, "8068631554425884100"},
	{24944, "3544227951277778870"},
	{79915, "1431209889878681918"},
	{23266, "4947584051717646138"},
	{12366, "1065486947567638746"},
	{84985, "3358117730571319758"},
	{52948, "5946795381425590126"},
	{69087, "5055654547391478750"},
	{26990, "9249001981460233515"}
};

static uint32_t trans_index = 0;

EN_transState_t recieveTransactionData(ST_transaction_t* transaction) {

	if (isValidAccount(&transaction->cardHolderData) == ACCOUNT_NOT_FOUND) {
		transaction->transState = DECLINED_STOLEN_CARD;
		saveTransaction(transaction);
		return DECLINED_STOLEN_CARD;
	}
	if (isAmountAvailable(&transaction->terminalData) == LOW_BALANCE) {
		transaction->transState = DECLINED_INSUFFECIENT_FUND;
		saveTransaction(transaction);
		return DECLINED_INSUFFECIENT_FUND;
	}
	if (isBelowMaxAmount(&transaction->terminalData) == EXCEED_MAX_AMOUNT) {
		transaction->transState = DECLINED_EXEED_MAX_AMOUNT;
		saveTransaction(transaction);
		return DECLINED_EXEED_MAX_AMOUNT;
	}
	if (saveTransaction(transaction) == SAVING_FAILED) {
		transaction->transState = INTERNAL_SERVER_ERROR;
		return INTERNAL_SERVER_ERROR;
	}
	transaction->transState = APPROVED;
	return APPROVED;
}


EN_serverError_t isValidAccount(ST_cardData_t* cardData) {
	for (uint32_t loc_i = 0; loc_i < 11; loc_i++) {
		if (!strncmp(cardData->primaryAccountNumber, accountsDB[loc_i].primaryAccountNumber, 19)) {
			trans_index = loc_i;
			return SERVER_OK;
		}
	}
	return ACCOUNT_NOT_FOUND;
}


EN_serverError_t isAmountAvailable(ST_terminalData_t* terminalData) {
	if (terminalData->transAmount > accountsDB[trans_index].balance)
		return LOW_BALANCE;
	return SERVER_OK;
}


EN_serverError_t saveTransaction(ST_transaction_t* transaction) {
	FILE* ptr;
 printf("-Balance BEFORE Transaction = %.2f\n", accountsDB[trans_index].balance);
	ptr = fopen("./TransactionsDB.txt", "a");
	if(!ptr)
		return SAVING_FAILED;
	fprintf(ptr, "##############################TRANSACTION##############################\n\n");
	fprintf(ptr, "-Card Holder Name:- %s", transaction->cardHolderData.cardHolderName);
	fprintf(ptr, "-Card PAN:- %s\n", transaction->cardHolderData.primaryAccountNumber);
	fprintf(ptr, "-Card Expiry Date:- %s\n", transaction->cardHolderData.cardExpirationDate);
	fprintf(ptr, "-Transaction Date:- %s", transaction->terminalData.transactionDate);
	fprintf(ptr, "-Transaction Max Amount:- %.2f\n", transaction->terminalData.maxTransAmount);
	fprintf(ptr, "-Transaction Amount:- %.2f\n", transaction->terminalData.transAmount);
	if (!(transaction->transState == DECLINED_STOLEN_CARD)) {
		fprintf(ptr, "-Balance BEFORE Transaction = %.2f\n", accountsDB[trans_index].balance);
		if (!(transaction->transState == DECLINED_EXEED_MAX_AMOUNT) && !(transaction->transState == DECLINED_INSUFFECIENT_FUND))
			accountsDB[trans_index].balance -= transaction->terminalData.transAmount;
		fprintf(ptr, "-New Balance = % .2f\n", accountsDB[trans_index].balance);
	}
	fprintf(ptr, "-Transaction State:- ");
	if (transaction->transState == DECLINED_INSUFFECIENT_FUND)
		fprintf(ptr, "DECLINED_INSUFFECIENT_FUND\n");
	else if (transaction->transState == DECLINED_STOLEN_CARD)
		fprintf(ptr, "DECLINED_STOLEN_CARD\n");
	else if (transaction->transState == DECLINED_EXEED_MAX_AMOUNT)
		fprintf(ptr, "DECLINED_EXEED_MAX_AMOUNT\n");
	else
		fprintf(ptr, "APPROVED\n");
	fprintf(ptr, "\n#######################################################################\n\n");
	fclose(ptr);
  sleep(1);
  printf("-New Balance = % .2f\n", accountsDB[trans_index].balance);
	return SERVER_OK;
  
}

