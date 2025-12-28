#include "../header.h" 
// feature 1
int login(){
    FILE *fp = fopen("data/users.txt", "r"); // try to open the users file 
    if(fp == NULL){
        printf(RED "users database file not found\n" RESET);
        return 0; 
    }

    //variables to hold file data and user input
    char fileuser[50], filepass[50];
    char userinput[50], passinput[50];
    int found = 0; //flag to track if the user is in the system 

    printf(BLUE   "\nCIB LOGIN\n" RESET);
    printf(GREEN  "Welcome to the Commercial International Bank\n" RESET);
-   
    //stops at the space as all the usernames and passwords has no spaces
    printf(YELLOW "Username:" RESET);
    if (fgets(userinput, sizeof(userinput), stdin) != NULL) {
        userinput[strcspn(userinput, " ")] = 0;
        userinput[strcspn(userinput, "\n")] = 0;
    }

    printf(YELLOW "Password:" RESET);
    if (fgets(passinput, sizeof(passinput), stdin) != NULL) {
        passinput[strcspn(passinput, " ")] = 0;  
        passinput[strcspn(passinput, "\n")] = 0; 
}

    while(fscanf(fp, "%s %s", fileuser, filepass) != EOF){
        // strcmp returns 0 only if they are equal
        if(strcmp(userinput, fileuser) == 0 && strcmp(passinput, filepass) == 0){
            found = 1; // set flag to true
            break;     // no need to continue
        }
    }

    fclose(fp); // close the file 
    return found; // return the result 
}