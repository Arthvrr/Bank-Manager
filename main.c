#include "main.h"

void open_account(int account_id){
}

float get_amount(int account_id){    
}

void deposit(int account_id, float amount){

}

void withdraw(int account_id, float amount){
}

void make_transfer(int account_src,float amount, int account_dest){

}

//fonction simulant un client créant un compte et effectuant un dépot + retrait aléatoire
void *client_thread(void *arg) {
    return NULL;
}


int main(int argc, char* argv[]){

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_accounts> <num_clients>\n", argv[0]);
        return 1;
    }

    num_accounts = atoi(argv[1]);  //nombre de comptes
    num_clients = atoi(argv[2]);   //nombre de clients

    if (num_accounts <= 0 || num_clients <= 0){
        fprintf(stderr,"Le nombre de compte et/ou clients doit être positif !\n");
        return 1;
    }

    return 0;


}