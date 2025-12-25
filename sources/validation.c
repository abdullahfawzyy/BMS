#include "../header.h"

// --- Helper: Check if Account Exists ---
// Returns the index in the array if found, or -1 if not found.
int accountexists(const char *accNum) {
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].acc_num, accNum) == 0) {
            return i; // Found it at index i
        }
    }
    return -1; // Not found
}

// --- Helper: Validate Email ---
// Must contain '@' and '.'
int isvalidemail(const char *email) {
    if (strchr(email, '@') && strchr(email, '.')) {
        return 1; // Valid
    }
    return 0; // Invalid
}

// --- Helper: Validate Phone ---
// Must be digits only (and typically 11 digits for Egyptian numbers)
int isvalidphone(const char *phone) {
    // Check length (optional, but good for "Validate all entered data" rule)
    if (strlen(phone) < 10 || strlen(phone) > 15) return 0;
    
    // Check if all characters are digits
    for (int i = 0; i < strlen(phone); i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

// --- Helper: Validate Number String ---
// Ensures a string contains only digits (used for Account ID validation)
int isvalidnumber(const char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

// --- Helper: String to Lowercase ---
// Used for Advanced Search (Case Insensitive)
void tolowerstring(char *str) {
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}