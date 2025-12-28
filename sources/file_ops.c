#include "../header.h" 

void loadaccounts() {
    FILE *fp = fopen("data/accounts.txt", "r");
    // check if the file is missing 
    if (fp == NULL) {
        printf(RED "can not find the accounts.txt file that acts as the database\n" RESET);
        return; 
    }

    char line[500]; // temp storage for one line
    accountCount = 0; // reset count

    while (fgets(line, sizeof(line), fp)) {
        if (accountCount >= MAX) {
            printf(RED "Limit reached (Max %d users)\n" RESET, MAX);
            break;
        }

        if (strlen(line) < 5) continue; // skip empty lines
        line[strcspn(line, "\n")] = 0; // remove newline

        char *token = strtok(line, ",");
        
        // 1. Account Number
        if(token) strcpy(accounts[accountCount].acc_num, token);

        // 2. Name
        token = strtok(NULL, ",");
        if(token) strcpy(accounts[accountCount].name, token);

        // 3. Email
        token = strtok(NULL, ",");
        if(token) strcpy(accounts[accountCount].email, token);

        // 4. Balance
        token = strtok(NULL, ",");
        if(token) accounts[accountCount].balance = atof(token);

        // 5. Mobile
        token = strtok(NULL, ",");
        if(token) strcpy(accounts[accountCount].mobile, token);

        // 6. Date Opened (Format MM-YYYY)
        token = strtok(NULL, "-"); 
        if(token) accounts[accountCount].date_opened.day = atoi(token);
        
        token = strtok(NULL, "-");
        if(token) accounts[accountCount].date_opened.month = atoi(token);
        
        token = strtok(NULL, ",");
        if(token) accounts[accountCount].date_opened.year = atoi(token);

        // 7. Status
        token = strtok(NULL, ",");
        if(token) strcpy(accounts[accountCount].status, token);

        // --- NEW DATA LOADING ---

        // 8. Daily Withdrawal Limit
        token = strtok(NULL, ",");
        if(token) {
            accounts[accountCount].daily_withdrawal = atof(token);
        } else {
            // Default if data is missing
            accounts[accountCount].daily_withdrawal = 0.0;
        }

        // 9. Last Transaction Date (Format D-M-Y)
        // Note: The previous token ended with a comma, so we look for dashes now
        token = strtok(NULL, "-");
        if(token) accounts[accountCount].last_trans_date.day = atoi(token);
        else accounts[accountCount].last_trans_date.day = 1;

        token = strtok(NULL, "-");
        if(token) accounts[accountCount].last_trans_date.month = atoi(token);
        else accounts[accountCount].last_trans_date.month = 1;

        // The year is the last item, usually followed by nothing or newline
        token = strtok(NULL, ","); 
        if(token) accounts[accountCount].last_trans_date.year = atoi(token);
        else accounts[accountCount].last_trans_date.year = 2000;

        accountCount++;
    }
    fclose(fp);
    printf(GREEN "successfully Loaded %d accounts\n" RESET, accountCount);
}

//rewrite the text file 
void savechanges() {
    FILE *fp = fopen("data/accounts.txt", "w"); //write mode
    if (fp == NULL) {
        printf(RED "couldnt save the changes\n" RESET);
        return;
    }

for (int i = 0; i < accountCount; i++) {
        fprintf(fp, "%s,%s,%s,%.2f,%s,%d-%d-%d,%s,%.2f,%d-%d-%d\n",
            accounts[i].acc_num,
            accounts[i].name,
            accounts[i].email,
            accounts[i].balance,
            accounts[i].mobile,
            accounts[i].date_opened.day,
            accounts[i].date_opened.month,
            accounts[i].date_opened.year,
            accounts[i].status,
            accounts[i].daily_withdrawal,
            accounts[i].last_trans_date.day,
            accounts[i].last_trans_date.month,
            accounts[i].last_trans_date.year
        );
    }
    fclose(fp);
    printf(GREEN "all changes saved\n" RESET);
}
void savetransaction(char *accNum, char *type, double amount) {
    char filepath[100];
    //saves in the user file name 
    sprintf(filepath, "data/transactions/%s.txt", accNum); 
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
void quit() {
    printf(BLUE "Thank you for banking with us\n" RESET);
    exit(0);
}