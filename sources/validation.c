#include "../header.h" //go one step back directory to find the header file 

// returns index if accout exists 
int accountexists(const char *accNum) {
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].acc_num, accNum) == 0) {
            return i; //found at index i 
        }
    }
    return -1; //not found
}
//search of @ or . 
int isvalidemail(const char *email) {
    if (strchr(email, '@') && strchr(email, '.')) {
        return 1; // Valid
    }
    return 0; // Invalid
}
//search of digits and must be of a length of 10 
int isvalidphone(const char *phone) {
    // Check length (optional, but good for "Validate all entered data" rule)
    if (strlen(phone) < 10 || strlen(phone) > 15) return 0;
    
    // Check if all characters are digits
    for (int i = 0; i < strlen(phone); i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

//only contains digits 
int isvalidnumber(const char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}
//lowercase of non sensitive searching 
void tolowerstring(char *str) {
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}
void wait_for_user() {
    printf("Press 1 to return to Main Menu\n");
    printf("Or type 2 to Exit: ");
    // 1. Clear the input buffer (eats up leftover 'Enter' keys from previous scans)
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    // 2. Wait for new input
    char input = getchar(); 
    // 3. Logic: If they typed '0', exit. Anything else returns to menu.
    if (input == '2') {
        printf(RED "Exiting...\n");
        exit(0); 
    }
}