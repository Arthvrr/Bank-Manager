#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/*
accounts = liste contenant tout les montant de chaque compte
chaque client est un thread qui ouvre un compte à son id
chaque client peut récupérer le montant qu'il possède, retirer ou déposer de l'argent sur son compte si c'est le sien
chaque client peut effectuer un virement vers un autre compte
*/


typedef struct {
    float amount;                
    pthread_mutex_t mutex_account;
} BankAccount;

//variables globales
int transactions_num; //nombre de transactions effectuées
extern int num_accounts; //nombre total de comptes bancaires
extern BankAccount *accounts; //pointeur vers le tableau des comptes bancaires


void open_account(int account_id);
float get_amount(int account_id);
void deposit(int account_id, float amount);
void withdraw(int account_id, float amount);
void make_transfer(int account_src, float amount, int account_dest);
void *client_thread(void *arg);

#endif // MAIN_H