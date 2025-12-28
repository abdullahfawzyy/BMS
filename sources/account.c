#include "../header.h" //go one step back directory to find the header file 

// --- Helper: Ask to Save or Discard ---
void ask_to_save() {
    char choice[100];
    
    while(1) {
        printf(YELLOW "\nDo you want to save changes? (yes/no): " RESET);
        
        // 1. Read input with leading space (skips previous newline)
        scanf(" %99s", choice);

        // 2. Clear anything else on the line (e.g. if user typed "yes please")
        int c;
        while ((c = getchar()) != '\n' && c != EOF); 

        // 3. Check logic
        if (strcmp(choice, "yes") == 0 || strcmp(choice, "Yes") == 0) {
            savechanges(); 
            printf(GREEN "Changes saved successfully.\n" RESET);
            break; 
        } else if(strcmp(choice, "no") == 0 || strcmp(choice, "No") == 0){
            printf(RED "Changes discarded. Reloading original data...\n" RESET);
            accountCount = 0; 
            loadaccounts(); 
            break; 
        }
        else{
            printf(RED "Invalid Choice. Please type 'yes' or 'no'.\n" RESET);
        }
    }
}

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
    scanf("%19s", id_to_be_checked);
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
    scanf(" %99[^\n]s", accounts[accountCount].name);
    // get email
    printf("enter E-mail: ");
    scanf("%99s", accounts[accountCount].email);
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
    scanf("%19s", accounts[accountCount].mobile);
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

    // NOTE: Removed the direct file writing block here because ask_to_save() does it now.
    
    //increase the global variable of accounts count 
    accountCount++; 
    printf(GREEN "Welcome %s to CIB \n" RESET, accounts[accountCount-1].name);

    // Ask user to save or discard
    ask_to_save();
}

//feature 6
void deleteaccount(){
    char id[20];
    int index = -1; //Use -1 as standard 'not found' flag
    //take accont number from the user
    printf(BLUE "\nDelete Account\n" RESET);
    printf(YELLOW "enter account number: " RESET);
    scanf("%19s", id);
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
    
    // Ask user to save or discard
    ask_to_save();
    
    // Note: The success message is inside ask_to_save now
}

//feature 7 
void modifyaccount(){
    char id[20];
    int index = -1;
    //get account number 
    printf(BLUE "\nModify Account\n" RESET);
    printf(YELLOW "enter account number to modify: " RESET);
    scanf("%19s", id);
    index = accountexists(id);
    if(index == -1){
        printf(RED "account not found\n" RESET);
        return ; 
    }
    printf(GREEN "account found. please enter new information:\n" RESET);
    // modify name
    printf("enter new name: ");
    scanf(" %99[^\n]s", accounts[index].name);
    // modify email
    printf("enter new email: ");
    scanf("%99s", accounts[index].email); 
    if (!isvalidemail(accounts[index].email)) {
        printf(RED "invalid email update rejected\n" RESET);
        return;
    }
    // modify mobile
    printf("enter new mobile: ");
    scanf("%19s", accounts[index].mobile); 
    if (!isvalidphone(accounts[index].mobile)) {
        printf(RED "invalid mobile update rejected\n" RESET);
        return;
    }
    
    // Ask user to save or discard
    ask_to_save();
}

//feature 8
void changestatus(){
    char id[20]; //account number
    char new_status[20]; //new status sent by the user 
    int index = -1;
    printf(BLUE "\nChange Account Status\n" RESET);
    printf(YELLOW "enter account number: " RESET);
    scanf("%19s", id);//take number from the user 
    index = accountexists(id);//check availbility using the helper function 
    if(index == -1){
        printf(RED "account not found\n" RESET); 
        return ; 
    }
    printf("current status is: %s\n", accounts[index].status);//print current status 
    printf("enter new status (active/inactive): ");
    scanf("%19s", new_status);
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
    
    // Ask user to save or discard
    ask_to_save();
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
                long acc_days = (accounts[i].date_opened.year * 365)+(accounts[i].date_opened.month * 30)+accounts[i].date_opened.day;
                long now_days = (current_year * 365)+(current_month * 30)+now.tm_mday;
                if((now_days - acc_days) > 90){
                    shift_and_delete(i);
                    deleted_count++;
                }
            }
        }
    } else {
        printf(RED "invalid choice.\n" RESET);
        return;
    }

    if(deleted_count == 0) printf(RED "no matching accounts found\n" RESET);
    else {
        printf(GREEN "Removed %d account/s from memory.\n" RESET, deleted_count);
        // Ask user to save or discard
        ask_to_save();
    }
}