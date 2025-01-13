#include "main.h"

int transactions_num = 0;
int num_accounts = 0;
BankAccount *accounts = NULL; //liste des comptes avec leur montant

void open_account(int account_id){
    
    //si id du compte incorrect
    if (account_id < 0 || account_id >= num_accounts) {
        fprintf(stderr, "Erreur : ID de compte invalide (%d).\n", account_id);
        return;
    }

    accounts[account_id].amount = 0.0;  //solde initial du compte
    pthread_mutex_init(&accounts[account_id].mutex_account, NULL);  //initialiser le mutex
    printf("Compte créé avec l'ID %d. Solde initial: %.2f\n", account_id, accounts[account_id].amount);
}

float get_amount(int account_id){

    //si id du compte incorrect
    if (account_id < 0 || account_id >= num_accounts) {
        fprintf(stderr, "Erreur : ID de compte invalide (%d).\n", account_id);
        return -1.0; //retourne une valeur négative pour indiquer une erreur
    }

    pthread_mutex_lock(&accounts[account_id].mutex_account);
    float balance = accounts[account_id].amount;
    pthread_mutex_unlock(&accounts[account_id].mutex_account);
    printf("Affichage du montant du compte numéro %d: %.2f\n", account_id, balance);
    transactions_num++;
    return balance;
}

void deposit(int account_id, float amount){

    //si id du compte incorrect
    if (account_id < 0 || account_id >= num_accounts) {
        fprintf(stderr, "Erreur : ID de compte invalide (%d).\n", account_id);
        return;
    }

    //si montant négatif
    if (amount <= 0) {
        fprintf(stderr, "Erreur : Le montant du dépôt doit être positif.\n");
        return;
    }

    pthread_mutex_lock(&accounts[account_id].mutex_account);
    accounts[account_id].amount += amount;
    printf("Dépôt de %.2f effectué sur le compte %d. Nouveau solde: %.2f\n", amount, account_id, accounts[account_id].amount);
    transactions_num++;
    pthread_mutex_unlock(&accounts[account_id].mutex_account);
}

void withdraw(int account_id, float amount){

    //si id du compte incorrect
    if (account_id < 0 || account_id >= num_accounts) {
        fprintf(stderr, "Erreur : ID de compte invalide (%d).\n", account_id);
        return;
    }

    //si montant négatif
    if (amount <= 0) {
        fprintf(stderr, "Erreur : Le montant du retrait doit être positif.\n");
        return;
    }

    printf("Tentative de retrait de %.2f effectué sur le compte %d.\n", amount, account_id);
    pthread_mutex_lock(&accounts[account_id].mutex_account);

    //si montant de retrait voulu est supérieur au montant disponible
    if (accounts[account_id].amount < amount){
        fprintf(stderr, "Solde Insuffisant.\n");
        pthread_mutex_unlock(&accounts[account_id].mutex_account); //on libère le mutex pour éviter le deadlock
        return;
    }
    accounts[account_id].amount -= amount;
    printf("Retrait de %.2f effectué sur le compte %d. Nouveau solde: %.2f\n", amount, account_id, accounts[account_id].amount);
    transactions_num++;
    pthread_mutex_unlock(&accounts[account_id].mutex_account);
}

void make_transfer(int account_src,float amount, int account_dest){

    //si id du compte source incorrect
    if (account_src < 0 || account_src >= num_accounts) {
        fprintf(stderr, "Erreur : ID de compte invalide (%d).\n", account_src);
        return;
    }

    //si id du compte destination incorrect
    if (account_dest < 0 || account_dest >= num_accounts) {
        fprintf(stderr, "Erreur : ID de compte invalide (%d).\n", account_dest);
        return;
    }

    //si montant négatif
    if (amount <= 0) {
        fprintf(stderr, "Erreur : Le montant du transfert doit être positif.\n");
        return;
    }

    //si compte src = compte dest
    if (account_src == account_dest) {
        fprintf(stderr, "Erreur : Transfert impossible, même compte source et destination.\n");
        return;
    }

    printf("Tentative de virement de %.2f effectué du compte %d vers le compte %d.\n", amount, account_src,account_dest);
    pthread_mutex_lock(&accounts[account_src].mutex_account); //on lock les 2 mutex des 2 comptes
    pthread_mutex_lock(&accounts[account_dest].mutex_account);

    //si montant de retrait voulu est supérieur au montant disponible
    if (accounts[account_src].amount < amount){
        fprintf(stderr, "Solde Insuffisant pour transfert.\n");
        pthread_mutex_unlock(&accounts[account_src].mutex_account);
        pthread_mutex_unlock(&accounts[account_dest].mutex_account);
        return;
    }

    accounts[account_src].amount -= amount;
    accounts[account_dest].amount += amount;
    printf("Virement de %.2f effectué sur le compte %d vers le compte %d.\n", amount, account_src, account_dest);
    transactions_num++;
    pthread_mutex_unlock(&accounts[account_src].mutex_account);
    pthread_mutex_unlock(&accounts[account_dest].mutex_account);
}


//fonction simulant un client créant un compte et effectuant un dépot + retrait aléatoire
void *client_thread(void *arg) {
    int client_id = *(int *)arg;
    int operations = 1 + rand() % 10; //nombre aléatoire d'opérations (1 à 10)
    for (int i = 0; i < operations; i++) {
        int operation = rand() % 4; //choisir aléatoirement une opération (0 à 3)
        float random_amount = (rand() % 10000) / 100.0; //générer un montant aléatoire
        int random_account = rand() % num_accounts; //choisir un compte aléatoire

        switch (operation) {
            case 0:
                get_amount(client_id);
                break;
            case 1:
                deposit(client_id, random_amount);
                break;
            case 2:
                withdraw(client_id, random_amount);
                break;
            case 3:
                make_transfer(client_id, random_amount, random_account);
                break;
            default:
                fprintf(stderr, "Opération inconnue.\n");
        }
    }
    return NULL;
}


int main(int argc, char* argv[]){

    //INITIALISATION
    if (argc != 2) {
        fprintf(stderr, "Erreur : nombre de clients manquants !\n", argv[0]);
        return 1;
    }

    num_accounts = atoi(argv[1]);  //nombre de comptes

    if (num_accounts <= 0){
        fprintf(stderr,"Le nombre de clients doit être positif !\n");
        return 1;
    }

    //allocation de mémoire
    accounts = (BankAccount *)malloc(num_accounts * sizeof(BankAccount));
    if (accounts == NULL) {
        fprintf(stderr, "Erreur : Allocation mémoire pour les comptes échouée.\n");
        return 1;
    }

    srand(time(NULL));
    pthread_t threads[num_accounts];
    int client_ids[num_accounts];

    //crée les différents threads et crée les comptes
    for (int i = 0; i < num_accounts; i++) {
        open_account(i);
        client_ids[i] = i;
        if (pthread_create(&threads[i], NULL, client_thread, &client_ids[i]) != 0) {
            fprintf(stderr, "Erreur lors de la création du thread %d.\n", i);
        }
    }

    //attendre que les threads finissent leur job
    for (int i = 0; i < num_accounts; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Nombre de transactions totales : %d\n",transactions_num);


    //libérer les ressources
    for (int i = 0; i < num_accounts; i++) {
        pthread_mutex_destroy(&accounts[i].mutex_account);
    }
    free(accounts);


    return 0;


}