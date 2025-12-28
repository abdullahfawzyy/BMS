#include "../header.h" //go one step back directory to find the header file 
int confirm_transaction_save() {
    char choice[50];
    
    while(1) {
        printf(YELLOW "\nConfirm transaction and save? (yes/no): " RESET);
        
        // 1. Read input with leading space (skips previous newline)
        scanf(" %49s", choice);

        // 2. Clear buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF); 

        // 3. Check logic
        if (strcmp(choice, "yes") == 0 || strcmp(choice, "Yes") == 0) {
            savechanges();
            return 1; 
        } else if(strcmp(choice, "no") == 0 || strcmp(choice, "No") == 0){
            printf(RED "Transaction cancelled. Changes discarded.\n" RESET);
            accountCount = 0; 
            loadaccounts(); 
            return 0; 
        }
        else {
            printf(RED "Invalid Choice. Please type 'yes' or 'no'.\n" RESET);
        }
    }
}
// --- Helper: Maintenance Check (Run at Login) ---
// --- Helper: Maintenance Check (Run at Login) ---
// --- Helper: Maintenance Check (Run at Login) ---
void reset_daily_limits() {
    time_t t = time(NULL);
    struct tm now = *localtime(&t);
    int today_day = now.tm_mday;
    int today_month = now.tm_mon + 1;
    int today_year = now.tm_year + 1900;

    int reset_count = 0;

    printf(BLUE "Performing Daily Maintenance...\n" RESET);

    for (int i = 0; i < accountCount; i++) {
        // Check if the saved date is DIFFERENT from today
        if (accounts[i].last_trans_date.day != today_day ||
            accounts[i].last_trans_date.month != today_month ||
            accounts[i].last_trans_date.year != today_year) {
            
            // 1. Reset Limit
            accounts[i].daily_withdrawal = 0.0;
            
            // 2. FORCE UPDATE DATE TO TODAY (As requested)
            // This marks the account as "checked/reset" for the current day
            accounts[i].last_trans_date.day = today_day;
            accounts[i].last_trans_date.month = today_month;
            accounts[i].last_trans_date.year = today_year;

            reset_count++;
        }
    }

    if (reset_count > 0) {
        // Save these new dates and zero limits to the file
        savechanges(); 
        printf(GREEN "Maintenance Complete: Updated date & limit for %d accounts.\n" RESET, reset_count);
    } else {
        printf(GREEN "Maintenance Complete: All accounts are already up to date.\n" RESET);
    }
}
//appends a single line to the specific user's history file

//feature 10 withdraw 
void withdraw(){
    char userid[20];
    double amount;
    int index = -1; //flag
    reset_daily_limits();
    //get the account number first
    printf(BLUE "\nWithdraw\n" RESET);
    printf(YELLOW "enter account number: " RESET);
    scanf("%19s", userid);
    
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
    
    // --- SECURITY FIX: NEGATIVE CHECK ---
    if(amount < 0){
        printf(RED "you cannot withdraw a negative number\n" RESET);
        return;
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
    
    // --- LOGIC FIX: UPDATE DATE ---
    time_t t = time(NULL);
    struct tm now = *localtime(&t);
    accounts[index].last_trans_date.day = now.tm_mday;      
    accounts[index].last_trans_date.month = now.tm_mon + 1; 
    accounts[index].last_trans_date.year = now.tm_year + 1900;
    
    // --- CONFIRMATION STEP ---
    if(confirm_transaction_save()) {
        // Only write log if user said YES
        savetransaction(userid, "Withdraw", amount);
        //confirmation message
        printf(GREEN "transaction successful the new balance: %.2f $\n" RESET, accounts[index].balance);
    }
}

//feature 11 deposit
void deposit(){
    char id[20];
    double amount;
    int index = -1;
    printf(BLUE "\nDeposit\n" RESET);
    //get user number 
    printf(YELLOW "enter account number: " RESET);
    scanf("%19s", id);
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
    if(amount < 0){
        printf(RED "you cannot deposit a negative number\n");
        return;
    }
    //after it passed all that checks it is time to add it to the balance 
    accounts[index].balance = accounts[index].balance + amount;
    
    // --- CONFIRMATION STEP ---
    if(confirm_transaction_save()) {
        //save the transactions in the specified files 
        savetransaction(id, "Deposit", amount); 
        printf(GREEN "transaction successful the new balance: %.2f $\n" RESET, accounts[index].balance);
    }
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
    scanf("%19s", sender_id);
    printf(YELLOW "enter receiver account number: " RESET);
    scanf("%19s", receiver_id);
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
    if(amount < 0){
        printf(RED "you cannot transfer a negative number.\n");
        return ;
    }
    //do the necessary operations by reducing the amount from the sender and adding it to the receiver 
    accounts[sender_idx].balance -= amount;
    accounts[receiver_idx].balance += amount;
    
    // --- CONFIRMATION STEP ---
    if(confirm_transaction_save()) {
        //save the transaction in the desired files of each account 
        savetransaction(sender_id, "Transfer Sent", amount);
        savetransaction(receiver_id, "Transfer Received", amount);
        printf(GREEN "transfer successful!\n" RESET);
    }
}

//feature 13 report 
void report(){
    //account number 
    char id[20];
    char history[100][100];
    int transamount = 0;
    printf(BLUE "\nAccount Report\n" RESET);
    printf(YELLOW "enter account number: " RESET);
    scanf("%19s", id);
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