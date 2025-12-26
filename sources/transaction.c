#include "../header.h" //go one step back directory to find the header file 
//appends a single line to the specific user's history file
void savetransaction(char *accNum, char *type, double amount) {
    char filepath[100];
    //saves in the user file name 
    sprintf(filepath, "data/%s.txt", accNum); 
    FILE *fp = fopen(filepath, "a");
    if (fp == NULL) {
        printf(RED "Error: Could not save transaction history.\n" RESET);
        return;
    }
    //get current date for the log
    time_t t = time(NULL);
    struct tm now = *localtime(&t);
    fprintf(fp, "%s %.2f $ Date: %d-%d-%d\n", type, amount, now.tm_mday, now.tm_mon + 1, now.tm_year + 1900);
    fclose(fp);
}
//feature 10 withdraw 
void withdraw(){
    char userid[20];
    double amount;
    int index = -1; //flag
    //get the account number first
    printf(BLUE "\nWithdraw\n" RESET);
    printf(YELLOW "enter account number: " RESET);
    scanf("%s", userid);
    //try to find it using helper
    index = accountexists(userid);
    if(index == -1){
        printf(RED "account not found\n" RESET);
        return ;
    }
    //check activity
    if(strcmp(accounts[index].status, "inactive") == 0){
        printf(RED "you can not withdraw from an inactive account\n" RESET);
        return ; //stop the function here
    }
    //take amount from the user
    printf(YELLOW "enter amount: " RESET);
    if(scanf("%lf", &amount) != 1){
        printf(RED "This is a very large number (invalid)\n" RESET);
        return ; //stop the function here
    }
    
    //single transaction limit 
    if(amount > TRANS_LIMIT){
        printf(RED "you can not exceed %.0f per single transaction\n" RESET, TRANS_LIMIT);
        return ; //stop the function here
    }
    //take care of the daily limit
    if(accounts[index].daily_withdrawal + amount > DAILY_LIMIT){
        printf(RED "you exceeded the daily limit of %.0f\n" RESET, DAILY_LIMIT);
        return ; // stop the function here
    }
    //check if the user has enough to withdraw
    if(amount > accounts[index].balance){
        printf(RED "insufficient balance\n" RESET);
        return ; // stop the function here
    }
    //if it passed all the previous test now we are able to perform the withdrawal
    //subtract the the amount from the user balance
    accounts[index].balance = accounts[index].balance - amount;
    //add the the amount to the daily limit 
    accounts[index].daily_withdrawal = accounts[index].daily_withdrawal + amount;
    //commit all the changes to the text file
    savechanges();
    //commit the changes to the specified file of that account 
    savetransaction(userid, "Withdraw", amount);
    //confirmation message
    printf(GREEN "transaction successful the new balance: %.2f $\n" RESET, accounts[index].balance);
}
//feature 11 deposit
void deposit(){
    char id[20];
    double amount;
    int index = -1;
    printf(BLUE "\nDeposit\n" RESET);
    //get user number 
    printf(YELLOW "enter account number: " RESET);
    scanf("%s", id);
    //try to find the account 
    index = accountexists(id);
    //like the withdrawal -1 is when no account found 
    if(index == -1){
        printf(RED "account not found\n" RESET);
        return ;
    }
    //check activity 
    if(strcmp(accounts[index].status, "inactive") == 0){
        printf(RED "can not deposit in an inactive account \n" RESET);
        return ;
    }
    //get amount from the user
    printf(YELLOW "enter amount: " RESET);
    scanf("%lf", &amount);
    //check max amount per transaction 
    if(amount > TRANS_LIMIT){
        printf(RED "error: max limit is %.0f $ per transaction.\n" RESET, TRANS_LIMIT);
        return ; 
    }
    //after it passed all that checks it is time to add it to the balance 
    accounts[index].balance = accounts[index].balance + amount;
    //save the changes to the accounts file 
    savechanges();
    //save the transactions in the specified files 
    savetransaction(id, "Deposit", amount); 
    printf(GREEN "transaction successful the new balance: %.2f $\n" RESET, accounts[index].balance);
}

//feature 12 transfer 
void transfer(){
    //create variable to store the sender and receiver account number 
    char sender_id[20], receiver_id[20];
    double amount;
    int sender_idx = -1, receiver_idx = -1;
    //get info 
    printf(BLUE "\nTransfer\n" RESET);
    printf(YELLOW "enter sender account number: " RESET);
    scanf("%s", sender_id);
    printf(YELLOW "enter receiver account number: " RESET);
    scanf("%s", receiver_id);
    //find both accounts first 
    sender_idx = accountexists(sender_id);
    receiver_idx = accountexists(receiver_id);
    //check if the accounts are not found 
    if(sender_idx == -1 || receiver_idx == -1){
        printf(RED "one or both accounts not found.\n" RESET);
        return ;
    }
    //check activity for both accounts 
    if(strcmp(accounts[sender_idx].status, "inactive") == 0 || 
       strcmp(accounts[receiver_idx].status, "inactive") == 0){
        printf(RED "warning: cannot transfer. one or both accounts are inactive.\n" RESET);
        return ;
    }
    //intake transfer amount 
    printf(YELLOW "enter amount to transfer: " RESET);
    scanf("%lf", &amount);
    //make sure that the sender has that type of money 
    if(accounts[sender_idx].balance < amount){
        printf(RED "sender has insufficient funds.\n" RESET);
        return ;
    }
    //do the necessary operations by reducing the amount from the sender and adding it to the receiver 
    accounts[sender_idx].balance -= amount;
    accounts[receiver_idx].balance += amount;
    //save changes to the accounts.txt file 
    savechanges();
    //save the transaction in the desired files of each account 
    savetransaction(sender_id, "Transfer Sent", amount);
    savetransaction(receiver_id, "Transfer Received", amount);
    printf(GREEN "transfer successful!\n" RESET);
}

//feature 13 report 
void report(){
    //account number 
    char id[20];
    char history[100][100];
    int transamount = 0;
    printf(BLUE "\nAccount Report\n" RESET);
    printf(YELLOW "enter account number: " RESET);
    scanf("%s", id);
    // check if account exists in the file first
    int index = accountexists(id);
    if(index == -1){
        printf(RED "account not found.\n" RESET);
        return ; // stop the function here
    }
    //create the file name (Updated to look in data/ folder)
    char filename[50]; 
    sprintf(filename, "data/%s.txt", id);
    // open the file in read mode 
    FILE *fp = fopen(filename, "r");
    //check if the file actually exists
    if(fp == NULL){
        printf(RED "no transactions found for this account\n" RESET);
        return ; // stop the function here
    }
    //add all the transactions to the history array 
    while(fgets(history[transamount], 100, fp) != NULL){
        transamount++; //transactions amount incremtnation 
        //can not add more than 100 as the size is 100x100 (100 lines maximum )
        if(transamount >= 100) break; 
    }
    fclose(fp);
    printf(CYAN "\nLast 5 Transactions for %s\n" RESET, accounts[index].name);
    //start from the 5th from down transaction 
    int start = transamount - 5;
    //if there is transaction less than 5 so the start will be negative 
    //so we will need to start from the first element  
    if(start < 0) start = 0; 
    //print all transactions
    for(int i = start; i < transamount; i++){
        printf("%s", history[i]);
    }
}