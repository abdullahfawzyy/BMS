#include "../header.h" //go one step back directory to find the header file 
void loadaccounts() {
    FILE *fp = fopen("data/accounts.txt", "r");
    //check if the file is missing 
    if (fp == NULL) {
        printf(RED "can not find the accounts.txt file that acts as the database\n" RESET);
        return; //stop here
    }
    char line[300]; // temp storage for one line
    accountCount = 0; //we will need to reset the count because if a user was recently added
    while (fgets(line, sizeof(line), fp)) {//read the file line by line
        if (accountCount >= MAX) {//dont exceed the maxium amount of users at a time (array size)
            printf(RED "can not load more account storage issue the Limit is %d)\n" RESET, MAX);
            break;
        }
        if (strlen(line) < 5) continue; //skip empty lines
        line[strcspn(line, "\n")] = 0;// it removes the \n after every line that causes a problem with the activity text so it removes it
        //example before we added this functionalility the activity text was "active\n"
        char *token = strtok(line, ",");// get the first piece before the first comma
        //cut account number 
        if(token) strcpy(accounts[accountCount].acc_num, token);
        // cut the name
        // NULL means keep cutting the same line we started with
        token = strtok(NULL, ",");
        if(token) strcpy(accounts[accountCount].name, token);
        //cut the email
        token = strtok(NULL, ",");
        if(token) strcpy(accounts[accountCount].email, token);
        //cut the balance 
        token = strtok(NULL, ",");
        if(token) accounts[accountCount].balance = atof(token);
        //cut the mobile 
        token = strtok(NULL, ",");
        if(token) strcpy(accounts[accountCount].mobile, token);
        //cut the date 
        token = strtok(NULL, "-"); //read up to the dash of the month 
        //atoi reads the first number and stops at the dash
        if(token) accounts[accountCount].date_opened.month = atoi(token);
        //we find a dash, read the number after the dash that resembels the year
        token = strtok(NULL, ","); //read the rest up to comma of the year 
        if(token) accounts[accountCount].date_opened.year = atoi(token);
        // all acounts day will be set to 1 
        accounts[accountCount].date_opened.day = 1;
        token = strtok(NULL, ",");
        if(token) strcpy(accounts[accountCount].status, token);
        //daily withdrawal initially set to zero 
        accounts[accountCount].daily_withdrawal = 0.0;
        accountCount++;
    }
    fclose(fp);
    printf(GREEN "success: Loaded %d accounts\n" RESET, accountCount);
}
//rewrite the text file 
void savechanges() {
    FILE *fp = fopen("data/accounts.txt", "w"); //write mode
    if (fp == NULL) {//cannot find the file
        printf(RED "couldnt save the changes\n" RESET);
        return;
    }
    for (int i = 0; i < accountCount; i++) {
        fprintf(fp, "%s,%s,%s,%.2f,%s,%d-%d,%s\n",
            accounts[i].acc_num,
            accounts[i].name,
            accounts[i].email,
            accounts[i].balance,
            accounts[i].mobile,
            accounts[i].date_opened.month,
            accounts[i].date_opened.year,
            accounts[i].status
        );
    }
    fclose(fp);
    printf(GREEN "all changes saved\n" RESET);
}
void quit() {
    printf(BLUE "Thank you for banking with us\n" RESET);
    exit(0);
}