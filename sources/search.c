#include "../header.h" //go one step back directory to find the header file 
//feature 3 query 
void queryaccount(){
    char id_to_find[20]; // variable to store the id user types
    int index = -1;      // store the location of the account
    printf(BLUE "\nQuery Account Details\n" RESET);
    printf(YELLOW "enter the account number to search for: " RESET);
    scanf("%19s", id_to_find); // get the id from the user
    index = accountexists(id_to_find);//search for the account 
    if(index != -1){
        // if match is found, print all details
        printf(GREEN "\n--- Account Found ---\n" RESET);
        printf("Account Number : %s\n", accounts[index].acc_num);
        printf("Name           : %s\n", accounts[index].name);
        printf("E-mail         : %s\n", accounts[index].email);
        printf("Balance        : %.2f $\n", accounts[index].balance);
        printf("Mobile         : %s\n", accounts[index].mobile);
        printf("Date Opened    : %d-%d\n", accounts[index].date_opened.month, accounts[index].date_opened.year);
        printf("Status         : %s\n", accounts[index].status);
    } else {
        // if the index is still -1 then it wasnt found
        printf(RED "\nError: The account number '%s' was not found.\n" RESET, id_to_find);
    }
}
//feature 4
void advancedsearch(){
    char keyword[50]; // user input to be searched 
    int matches = 0;  // counter for results
    printf(BLUE "\nAdvanced Search\n" RESET);
    printf(YELLOW "enter keyword (Name, ID, or Email): " RESET);
    //%[^\n]s allows reading strings with spaces (mohamed ali)
    scanf(" %49[^\n]s", keyword);
    //convert keyword to lowercase for case insensitive search
    char lowerKeyword[50];
    strcpy(lowerKeyword, keyword);
    tolowerstring(lowerKeyword);//helper function 
    printf(CYAN "\nSearch Results\n" RESET);
    // loop through all accounts
    for(int i = 0; i < accountCount; i++){
        //create temp copies to convert to lowercase for comparison
        //we do not modify the actual data, just the copies
        char tempName[100];
        strcpy(tempName, accounts[i].name);
        tolowerstring(tempName);
        char tempEmail[100];
        strcpy(tempEmail, accounts[i].email);
        tolowerstring(tempEmail);
        //try to find matching info related to the keyword not case sensitive 
        if(strstr(tempName, lowerKeyword) != NULL || 
           strstr(tempEmail, lowerKeyword) != NULL || 
           strstr(accounts[i].mobile, keyword) != NULL || 
           strstr(accounts[i].acc_num, keyword) != NULL) {
            matches++;
            printf(GREEN "Match #%d:\n" RESET, matches);
            printf("Account Number : %s\n", accounts[i].acc_num);
            printf("Name           : %s\n", accounts[i].name);
            printf("E-mail         : %s\n", accounts[i].email);
            printf("Balance        : %.2f $\n", accounts[i].balance);
            printf("Status         : %s\n", accounts[i].status);
            printf("--------------------------------\n");
           
        }
    }
    // summary message
    if(matches == 0){
        printf(RED "No accounts found matching '%s'\n" RESET, keyword);
    } 
}

//sort by name using bubble sort and using strcmp for comparing alphabetically 
void sortbyname(){
    for(int i = 0; i < accountCount - 1; i++){
        for(int j = 0; j < accountCount - i - 1; j++){
            //strcmp > 0 means that the first name comes after the second alphabetically
            if(strcmp(accounts[j].name, accounts[j+1].name) > 0){
                //swap using a temp variable struct 
                Account temp = accounts[j];
                accounts[j] = accounts[j+1];
                accounts[j+1] = temp;
            }
        }
    }
    printf(GREEN "sorted by Name successfully\n" RESET);
}
//sort by balance asendingly bubble sort also 
void sortbybalance(){
    for(int i = 0; i < accountCount - 1; i++){
        for(int j = 0; j < accountCount - i - 1; j++){
            if(accounts[j].balance > accounts[j+1].balance){
                Account temp = accounts[j];
                accounts[j] = accounts[j+1];
                accounts[j+1] = temp;
            }
        }
    }
    printf(GREEN "sorted by Balance successfully\n" RESET);
}

//sort by date abit more complex by also asendengly 
void sortbydate(){
    for(int i = 0; i < accountCount - 1; i++){
        for(int j = 0; j < accountCount - i - 1; j++){
            int swap = 0;
            // 1. Check Year
            if(accounts[j].date_opened.year > accounts[j+1].date_opened.year){
                swap = 1;
            }
            // 2. If Year is equal, Check Month
            else if(accounts[j].date_opened.year == accounts[j+1].date_opened.year && 
                    accounts[j].date_opened.month > accounts[j+1].date_opened.month){
                swap = 1;
            }
            // 3. If Month is ALSO equal, Check Day (NEW)
            else if(accounts[j].date_opened.year == accounts[j+1].date_opened.year && 
                    accounts[j].date_opened.month == accounts[j+1].date_opened.month &&
                    accounts[j].date_opened.day > accounts[j+1].date_opened.day){
                swap = 1;
            }

            if(swap){
                Account temp = accounts[j];
                accounts[j] = accounts[j+1];
                accounts[j+1] = temp;
            }
        }
    }
    printf(GREEN "Sorted by Date Opened successfully\n" RESET);
}
//helper function to sort by status 
void sortbystatus(){
    for(int i = 0; i < accountCount - 1; i++){
        for(int j = 0; j < accountCount - i - 1; j++){
            //sorting alphabetically 
            if(strcmp(accounts[j].status, accounts[j+1].status) > 0){
                Account temp = accounts[j];
                accounts[j] = accounts[j+1];
                accounts[j+1] = temp;
            }
        }
    }
    printf(GREEN "Sorted by Status successfully.\n" RESET);
}
//print sorted 
void printsorted(){
    int choice;
    printf(BLUE "\nPrint Sorted Accounts\n" RESET);
    printf("1. Sort by Name\n");
    printf("2. Sort by Balance\n");
    printf("3. Sort by Date Opened\n");
    printf("4. Sort by Status\n");
    printf(YELLOW "Enter choice: " RESET);
    if (scanf("%d", &choice) != 1) {
        printf(RED "Invalid input.\n" RESET);
        return;
    }
    // call the function based on the desired choice 
    if(choice == 1) sortbyname();
    else if(choice == 2) sortbybalance();
    else if(choice == 3) sortbydate();
    else if(choice == 4) sortbystatus();
    else {
        printf(RED "Invalid choice. Printing unsorted list.\n" RESET);
    }
    printf(CYAN "\nall Accounts Data \n" RESET);
    // print all accounts in the format of query 
    for(int i = 0; i < accountCount; i++){
        printf("\nAccount Number : %s\n", accounts[i].acc_num);
        printf("Name           : %s\n", accounts[i].name);
        printf("E-mail         : %s\n", accounts[i].email);
        printf("Balance        : %.2f $\n", accounts[i].balance);
        printf("Mobile         : %s\n", accounts[i].mobile);
        printf("Date Opened    : %d-%d-%d\n", accounts[i].date_opened.day, accounts[i].date_opened.month, accounts[i].date_opened.year);
        printf("Status         : %s\n", accounts[i].status);
        printf("--------------------------------");
    }
    printf("\n");
}