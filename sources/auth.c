#include "../header.h"
int login(){
    FILE *fp = fopen("data/users.txt", "r");// try to open the users file 
    if(fp == NULL){// check if the file opened successfully
        printf(RED "users file that acts as a database is not found" RESET);// print error if file is missing
        return 0; 
    }
    // variables to hold file data and user input
    char fileuser[50], filepass[50];
    char userinput[50], passinput[50];
    int found = 0; //flag to track if the user is in the system 
    printf(BLUE   "CIB\n");
    printf(GREEN  "Welcome to the Commerical International Bank\n");
    printf(YELLOW "Username: \n" RESET);
    scanf("%s", userinput);
    printf(YELLOW "Password: \n" RESET);
    scanf("%s", passinput);
    while(fscanf(fp, "%s %s", fileuser, filepass) != EOF){// loop through the file line by line
        // check if input matches the file data "strcmp returns 0 only if they are equal"
        if(strcmp(userinput, fileuser) == 0 && strcmp(passinput, filepass) == 0){
            found = 1; // set flag to true
            break;     //no need to continue
        }
    }
    fclose(fp);// close the file 
    return found;// return the result 
}