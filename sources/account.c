#include "../header.h" //go one step back directory to find the header file 
//shift and delete is a helperfunction to ease the code and avoid multiple writing 
// shift elements left to overwrite the index that must be deleted 
void shift_and_delete(int index) {
    for (int i = index; i < accountCount - 1; i++) {
        accounts[i] = accounts[i + 1];
    }
    accountCount--;//decrease the amount 
}
//feature 5
void addaccount(){
    if(accountCount >= MAX){ // check if we ran out of space for new accounts 
        printf(RED "you reached the max amount of accounts\n" RESET);
        return ; // stop the function here
    }
    char id_to_be_checked[20];
    printf(BLUE "\nAdd a new account\n" RESET);
    printf("enter account number (10 digits): "); //get the new account number
    scanf("%s", id_to_be_checked);
    //validate number length/digits
    if (!isvalidnumber(id_to_be_checked) || strlen(id_to_be_checked) != 10) {
        printf(RED "account number must be exactly 10 digits.\n" RESET);
        return; // stop the function here
    }
    //loop through all existing accounts to see if this number is already taken
    for(int i = 0; i < accountCount; i++){
        if(strcmp(accounts[i].acc_num, id_to_be_checked) == 0){
            printf(RED "account number '%s' is already taken\n" RESET, id_to_be_checked);
            return ; // stop the function here 
        }
    }
    // after checking assign the new account number 
    strcpy(accounts[accountCount].acc_num, id_to_be_checked);
    // get name
    printf("enter name: ");
    scanf(" %[^\n]s", accounts[accountCount].name);
    // get email
    printf("enter E-mail: ");
    scanf("%s", accounts[accountCount].email);
    // Validate email
    if (!isvalidemail(accounts[accountCount].email)) {
        printf(RED "email must contain '@' and '.'\n" RESET);
        return;
    }
    // get balance
    printf("enter initial balance: ");
    if(scanf("%lf", &accounts[accountCount].balance) != 1){
        printf(RED "invaild balance\n" RESET);
        return ; // stop the function here
    }
    // get mobile
    printf("Enter Mobile Number: ");
    scanf("%s", accounts[accountCount].mobile);
    // Validate mobile
    if (!isvalidphone(accounts[accountCount].mobile)) {
        printf(RED "mobile number must be only digits\n" RESET);
        return;
    }

    //we use the time.h library to get the current time 
    time_t t = time(NULL);
    struct tm now = *localtime(&t);
    accounts[accountCount].date_opened.day = now.tm_mday;
    accounts[accountCount].date_opened.month = now.tm_mon + 1; 
    accounts[accountCount].date_opened.year = now.tm_year + 1900; 
    //set the activity 
    strcpy(accounts[accountCount].status, "active");
    //initialize limits
    accounts[accountCount].daily_withdrawal = 0.0;
    accounts[accountCount].last_trans_date = accounts[accountCount].date_opened;
    //save the new account in the text file in the append mode
    FILE *fp = fopen("data/accounts.txt", "a");
    if(fp != NULL){
        fprintf(fp, "\n%s,%s,%s,%.2f,%s,%d-%d,%s", // %.2f for cleaner file format
            accounts[accountCount].acc_num,
            accounts[accountCount].name,
            accounts[accountCount].email,
            accounts[accountCount].balance,
            accounts[accountCount].mobile,
            accounts[accountCount].date_opened.month,
            accounts[accountCount].date_opened.year,
            accounts[accountCount].status
        );
        fclose(fp);
    } else {
        printf(RED "could not add the new account to the file\n" RESET);
    }
    //increase the global variable of accounts count 
    accountCount++; 
    printf(GREEN "Welcome %s to CIB \n" RESET, accounts[accountCount-1].name);
}
//feature 6
void deleteaccount(){
    char id[20];
    int index = -1; //Use -1 as standard 'not found' flag
    //take accont number from the user
    printf(BLUE "\nDelete Account\n" RESET);
    printf(YELLOW "enter account number: " RESET);
    scanf("%s", id);
    // check if account exists (Using helper from validation.c is also possible)
    index = accountexists(id); 
    //checking account availablity
    if(index == -1){
        printf(RED "account number not found\n" RESET);
        return;
    }
    // check if balance is zero before deleting 
    if(accounts[index].balance > 0){
        printf(RED "cannot delete account because balance must be zero (Current: %.2f)\n" RESET, accounts[index].balance);
        return;
    }
    // CALL THE HELPER function that deletes the account by shifting the indeces 
    shift_and_delete(index);
    // Save changes to file immediately
    savechanges();
    printf(GREEN "account deleted successfully\n" RESET);
}
//feature 7 
void modifyaccount(){
    char id[20];
    int index = -1;
    //get account number 
    printf(BLUE "\nModify Account\n" RESET);
    printf(YELLOW "enter account number to modify: " RESET);
    scanf("%s", id);
    index = accountexists(id);
    if(index == -1){
        printf(RED "account not found\n" RESET);
        return ; 
    }
    printf(GREEN "account found. please enter new information:\n" RESET);
    // modify name
    printf("enter new name: ");
    scanf(" %[^\n]s", accounts[index].name);
    // modify email
    printf("enter new email: ");
    scanf("%s", accounts[index].email); 
    if (!isvalidemail(accounts[index].email)) {
        printf(RED "invalid email update rejected\n" RESET);
        return;
    }
    // modify mobile
    printf("enter new mobile: ");
    scanf("%s", accounts[index].mobile); 
    if (!isvalidphone(accounts[index].mobile)) {
        printf(RED "invalid mobile update rejected\n" RESET);
        return;
    }
    // save changes in the main text file 
    savechanges();
    printf(GREEN "account details updated successfully\n" RESET);
}

//feature 8
void changestatus(){
    char id[20]; //account number
    char new_status[20]; //new status sent by the user 
    int index = -1;
    printf(BLUE "\nChange Account Status\n" RESET);
    printf(YELLOW "enter account number: " RESET);
    scanf("%s", id);//take number from the user 
    index = accountexists(id);//check availbility using the helper function 
    if(index == -1){
        printf(RED "account not found\n" RESET); 
        return ; 
    }
    printf("current status is: %s\n", accounts[index].status);//print current status 
    printf("enter new status (active/inactive): ");
    scanf("%s", new_status);
    // check if the new status is the same as the current one
    if(strcmp(accounts[index].status, new_status) == 0){
        printf(RED "the account is already %s\n" RESET, new_status); 
        return ; 
    }
    // check if the user typed a valid status
    if(strcmp(new_status, "active") != 0 && strcmp(new_status, "inactive") != 0){
        printf(RED "invalid status please type 'active' or 'inactive'\n" RESET);
        return ; 
    }
    // update status
    strcpy(accounts[index].status, new_status);
    // save changes to the main text file 
    savechanges();
    printf(GREEN "status changed to %s successfully\n" RESET, accounts[index].status);
}
//bonus feature 
void deletemultiple(){
    int choice; //deleting option 
    int deleted_count = 0; //number of deleted accounts 
    printf(BLUE "\nDelete Multiple Accounts\n" RESET);
    printf("1. Delete by Date (MM-YYYY)\n");
    printf("2. Delete Old Inactive Accounts (Zero Balance & > 90 Days)\n");
    printf(YELLOW "Enter choice: " RESET);//take the choice from the user 
    if (scanf("%d", &choice) != 1) {
         printf(RED "invalid input\n" RESET);
         return;
    }
    // Choice 1: Delete according to the date 
    if(choice == 1){
        int target_year, target_month;
        printf("Enter date (format MM YYYY): ");
        if(scanf("%d %d", &target_month, &target_year) != 2){
            printf(RED "invalid date format\n" RESET);
            return;
        }
        //looping backwards to avoid skipping accounts when an index shifts
        for(int i = accountCount - 1; i >= 0; i--){
            if(accounts[i].date_opened.year == target_year && 
               accounts[i].date_opened.month == target_month) {
                //cannot delete if they have money
                if (accounts[i].balance != 0) {
                     printf(RED "skipped %s (has balance: %.2f)\n" RESET, accounts[i].name, accounts[i].balance);
                     continue;
                }
                //call the helper function to shift and delete 
                shift_and_delete(i);
                deleted_count++;
            }
        }
        if(deleted_count == 0) printf(RED "No empty accounts found for that date.\n" RESET);
        else {
            savechanges();
            printf(GREEN "Success: Deleted %d accounts.\n" RESET, deleted_count);
        }
    } 
    //choice 2: delete old inactive accounts
    else if(choice == 2){
        //get current date
        time_t t = time(NULL);
        struct tm now = *localtime(&t);
        int current_year = now.tm_year + 1900;
        int current_month = now.tm_mon + 1;

        for(int i = accountCount - 1; i >= 0; i--){
            //status is inactive and balance is 0
            if(strcmp(accounts[i].status, "inactive") == 0 && accounts[i].balance == 0){
                //calculate time passed initilizing year zero as the begining then subtracting 
                long acc_days = (accounts[i].date_opened.year * 365) + (accounts[i].date_opened.month * 30);
                long now_days = (current_year * 365) + (current_month * 30);
                if((now_days - acc_days) > 90){
                    shift_and_delete(i);
                    deleted_count++;
                }
            }
        }
        if(deleted_count == 0) printf(RED "no matching old accounts found\n" RESET);
        else {
            savechanges();
            printf(GREEN "removed %d old accounts\n" RESET, deleted_count);
        }

    } else {
        printf(RED "invalid choice.\n" RESET);
    }
}