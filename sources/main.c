#include <stdio.h>
#include "../header.h"
Account accounts[MAX]; 
int accountCount = 0;
int main() {
    // login first as a bank staff
    // if login returns 0 stop the program to avoid non bank staff to access the system 
    if(!login()){
        printf(RED "please double check the username and passsword\n" RESET);
        printf(RED "access denied to the bank system\n" RESET);
        return 1;
    }
    printf(GREEN "Welcome Bank Staff member now you have access to the bank system\n" RESET);
    loadaccounts();//load all accounts once logged in
    int choice; //user choices based on the menu 
    //main menu 
    //while 1 "true" causes an infinite loop until the the program ends intentionally 
    while(1) {
        printf(BLUE "BANK MENU\n" RESET);
        printf("1.  Add New Account\n");
        printf("2.  Delete Account\n");
        printf("3.  Modify Account\n");
        printf("4.  Query (Search)\n");
        printf("5.  Advanced Search\n");
        printf("6.  Change Status\n");
        printf("7.  Withdraw\n");
        printf("8.  Deposit\n");
        printf("9.  Transfer\n");
        printf("10. Report \n");
        printf("11. Print (Sorted)\n");
        printf("12. Delete multiple Acccounts\n");
        printf("13. Quit\n");
        printf(YELLOW "Enter your choice: " RESET);
        char garbage[100]; // used to store undesired input form the user 
        if (scanf("%d", &choice) != 1) {
            //if the user accidentally added a character or a string it goes to the default case 
            scanf("%s", garbage); //read the  text and put it into the garbage variable to clear the input line and contiue 
            //why do i need to get rid of it because if i didnt do so ill go through an infinite loop  
            choice = 0; // reset choice to 0 so the loop continues safely
        }
         switch(choice) {
            case 1: addaccount(); break;
            case 2: deleteaccount(); break;
            case 3: modifyaccount(); break;
            case 4: queryaccount(); break;
            case 5: advancedsearch(); break;
            case 6: changestatus(); break;
            case 7: withdraw(); break;
            case 8: deposit(); break;
            case 9: transfer(); break;
            case 10: report(); break;
            case 11: printsorted(); break;
            case 12: deletemultiple(); break;
            case 13: quit(); break;
            default:
                printf(RED "Invalid choice! Please enter a number from 1 to 12.\n" RESET);//if invalid choice it wil start the loop again `
}
    }
    return 0;
}