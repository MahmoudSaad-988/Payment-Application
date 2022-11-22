#include "card.h"
#include <stdio.h>
#include <string.h>
#define MaxName 24
#define MinName 20
#define MaxDate 5
#define MaxPAN 19
#define MinPAN 16
#pragma warning(disable : 4996)
#include <ctype.h>

EN_cardError_t getCardHolderName(ST_cardData_t *cardData) {
  if (!cardData)
    return WRONG_NAME;
  fseek(stdin, 0, SEEK_END);
  uint8_t temp[40];
  printf("Enter Card Holder Name [20-24 characters]:- ");
  fgets(temp, 40, stdin);
  if (strlen(temp) < MinName + 1 || strlen(temp) > MaxName + 1)
    return WRONG_NAME;
  for (uint32_t loc_i = 0; loc_i < strlen(temp) - 1; loc_i++) {
    if (!isalpha(temp[loc_i]) && temp[loc_i] != ' ')
      return WRONG_NAME;
  }
  temp[30] = '\0';
  strcpy(cardData->cardHolderName, temp);
  return CARD_OK;
}


EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
    printf("Enter Card Expiry Date: ");
    gets(cardData->cardExpirationDate);

    // good format (MM/YY)
    if (cardData->cardExpirationDate == NULL) // check null date
    {
        strcpy(cardData->cardExpirationDate, "\0");
        return WRONG_EXP_DATE;
    }
    else if (strlen(cardData->cardExpirationDate) != 5) // check date formate length
    {
        strcpy(cardData->cardExpirationDate, "\0");
        return WRONG_EXP_DATE;
    }
    else if ((cardData->cardExpirationDate[0] == 48) && (cardData->cardExpirationDate[1] == 48)) // check MM = 00
    {
        strcpy(cardData->cardExpirationDate, "\0");
        return WRONG_EXP_DATE;
    }
    else if ((cardData->cardExpirationDate[0] < 48) || (cardData->cardExpirationDate[0] > 49)) // first M should be (0:1)
    {
        strcpy(cardData->cardExpirationDate, "\0");
        return WRONG_EXP_DATE;
    }
    else if ((cardData->cardExpirationDate[1] < 49) || (cardData->cardExpirationDate[1] > 57)) // second M should be (1:9)
    {
        strcpy(cardData->cardExpirationDate, "\0");
        return WRONG_EXP_DATE;
    }
    else if ((cardData->cardExpirationDate[0] == 49) && (cardData->cardExpirationDate[1] > 50)) // no month over 12
    {
        strcpy(cardData->cardExpirationDate, "\0");
        return WRONG_EXP_DATE;
    }
    else if ((cardData->cardExpirationDate[3] < 48) || (cardData->cardExpirationDate[3] > 57)) // first Y should be (0:9)
    {
        strcpy(cardData->cardExpirationDate, "\0");
        return WRONG_EXP_DATE;
    }
    else if ((cardData->cardExpirationDate[4] < 48) || (cardData->cardExpirationDate[4] > 57)) // second Y should be (0:9)
    {
        strcpy(cardData->cardExpirationDate, "\0");
        return WRONG_EXP_DATE;
    }
    else
        return CARD_OK; // valid card holder name
}
// ------------------functions getC

EN_cardError_t getCardPAN(ST_cardData_t *cardData) {
  if (!cardData)
    return WRONG_PAN;
  fseek(stdin, 0, SEEK_END);
  uint8_t temp[30];
  printf("Enter Primary Account Number [MAX: 19 characters]:- ");
  fgets(temp, 30, stdin);
  if (strlen(temp) < MinPAN + 1 || strlen(temp) > MaxPAN + 1)
    return WRONG_PAN;
  for (uint32_t loc_i = 0; loc_i < strlen(temp) - 1; loc_i++) {
    if (!isdigit(temp[loc_i]))
      return WRONG_PAN;
  }
  temp[19] = '\0';
  strcpy(cardData->primaryAccountNumber, temp);
  return CARD_OK;
}
