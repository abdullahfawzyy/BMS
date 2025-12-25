#include "../header.h"

// --- FEATURE 4: QUERY ACCOUNT ---
void queryaccount(){
    char id_to_find[20]; // variable to store the id user types
    int index = -1;      // store the location of the account

    printf(BLUE "\nQuery Account Details\n" RESET);
    printf(YELLOW "enter the account number to search for: " RESET);
    scanf("%s", id_to_find); // get the id from the user

    // Refactoring: Use the helper function we made in validation.c
    // This makes the code cleaner and reuses logic
    index = accountexists(id_to_find);

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
        // if the index is still -1, it wasn't found
        printf(RED "\nError: The account number '%s' was not found.\n" RESET, id_to_find);
    }
}

// --- FEATURE 9: ADVANCED SEARCH ---
void advancedsearch(){
    char keyword[50]; // user input to be searched 
    int matches = 0;  // counter for results
    
    printf(BLUE "\nAdvanced Search\n" RESET);
    printf(YELLOW "enter keyword (Name, ID, or Email): " RESET);
    // Refactoring: " %[^\n]s" allows reading strings with spaces (e.g. "Mohamed Ali")
    scanf(" %[^\n]s", keyword);
    
    // Convert keyword to lowercase for case-insensitive search
    char lowerKeyword[50];
    strcpy(lowerKeyword, keyword);
    tolowerstring(lowerKeyword); // Helper from validation.c

    printf(CYAN "\n--- Search Results ---\n" RESET);
    
    // loop through all accounts
    for(int i = 0; i < accountCount; i++){
        // Create temp copies to convert to lowercase for comparison
        // We do not modify the actual data, just the copies
        char tempName[100];
        strcpy(tempName, accounts[i].name);
        tolowerstring(tempName);

        char tempEmail[100];
        strcpy(tempEmail, accounts[i].email);
        tolowerstring(tempEmail);

        // Check matches:
        // 1. Name (Case Insensitive)
        // 2. Email (Case Insensitive)
        // 3. ID (Direct match)
        // 4. Mobile (Direct match)
        if(strstr(tempName, lowerKeyword) != NULL || 
           strstr(tempEmail, lowerKeyword) != NULL || 
           strstr(accounts[i].mobile, keyword) != NULL || 
           strstr(accounts[i].acc_num, keyword) != NULL) {
            
            printf(GREEN "Match #%d:\n" RESET, matches + 1);
            printf("Account Number : %s\n", accounts[i].acc_num);
            printf("Name           : %s\n", accounts[i].name);
            printf("E-mail         : %s\n", accounts[i].email);
            printf("Balance        : %.2f $\n", accounts[i].balance);
            printf("Status         : %s\n", accounts[i].status);
            printf("--------------------------------\n");
            matches++; // count this match
        }
    }

    // summary message
    if(matches == 0){
        printf(RED "No accounts found matching '%s'\n" RESET, keyword);
    } 
}

// --- SORTING HELPER 1: BY NAME ---
void sortbyname(){
    for(int i = 0; i < accountCount - 1; i++){
        for(int j = 0; j < accountCount - i - 1; j++){
            // strcmp > 0 means that the first name comes after the second alphabetically
            // We use strcasecmp or just convert to lower manually if strict sorting needed, 
            // but standard strcmp is fine for this level.
            if(strcmp(accounts[j].name, accounts[j+1].name) > 0){
                // swap using a temp variable struct 
                Account temp = accounts[j];
                accounts[j] = accounts[j+1];
                accounts[j+1] = temp;
            }
        }
    }
    printf(GREEN "Sorted by Name successfully.\n" RESET);
}

// --- SORTING HELPER 2: BY BALANCE ---
void sortbybalance(){
    for(int i = 0; i < accountCount - 1; i++){
        for(int j = 0; j < accountCount - i - 1; j++){
            // Swap if current is LESS than next (Descending Order - Richest first)
            // Or use > for Ascending. Let's assume user wants Ascending (Lowest to Highest).
            if(accounts[j].balance > accounts[j+1].balance){
                Account temp = accounts[j];
                accounts[j] = accounts[j+1];
                accounts[j+1] = temp;
            }
        }
    }
    printf(GREEN "Sorted by Balance successfully.\n" RESET);
}

// --- SORTING HELPER 3: BY DATE ---
void sortbydate(){
    for(int i = 0; i < accountCount - 1; i++){
        for(int j = 0; j < accountCount - i - 1; j++){
            int swap = 0;
            // compare the years first
            if(accounts[j].date_opened.year > accounts[j+1].date_opened.year){
                swap = 1;
            }
            // if years are equal, compare months
            else if(accounts[j].date_opened.year == accounts[j+1].date_opened.year && 
                    accounts[j].date_opened.month > accounts[j+1].date_opened.month){
                swap = 1;
            }
            
            // if the preceding account is greater/newer, swap 
            if(swap){
                Account temp = accounts[j];
                accounts[j] = accounts[j+1];
                accounts[j+1] = temp;
            }
        }
    }
    printf(GREEN "Sorted by Date Opened successfully.\n" RESET);
}

// --- FEATURE 15: PRINT SORTED ---
void printsorted(){
    int choice;
    printf(BLUE "\nPrint Sorted Accounts\n" RESET);
    printf("1. Sort by Name\n");
    printf("2. Sort by Balance\n");
    printf("3. Sort by Date Opened\n");
    printf(YELLOW "Enter choice: " RESET);
    
    if (scanf("%d", &choice) != 1) {
        printf(RED "Invalid input.\n" RESET);
        return;
    }

    // call the function based on the desired choice 
    if(choice == 1) sortbyname();
    else if(choice == 2) sortbybalance();
    else if(choice == 3) sortbydate();
    else {
        printf(RED "Invalid choice. Printing unsorted list.\n" RESET);
    }

    printf(CYAN "\n--- All Accounts Data ---\n" RESET);
    // print all accounts in the format of query 
    for(int i = 0; i < accountCount; i++){
        printf("\nAccount Number : %s\n", accounts[i].acc_num);
        printf("Name           : %s\n", accounts[i].name);
        printf("E-mail         : %s\n", accounts[i].email);
        printf("Balance        : %.2f $\n", accounts[i].balance);
        printf("Mobile         : %s\n", accounts[i].mobile);
        printf("Date Opened    : %d-%d\n", accounts[i].date_opened.month, accounts[i].date_opened.year);
        printf("Status         : %s\n", accounts[i].status);
        printf("--------------------------------");
    }
    printf("\n");
}