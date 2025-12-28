#include <stdio.h>
#include "../header.h" //go one step back directory to find the header file 
Account accounts[MAX]; 
int accountCount = 0;

int main() {
    if(!login()){
        printf(RED "Please double check the username and password\n" RESET);
        printf(RED "Access denied to the bank system\n" RESET);
        return 1;
    }

    printf(GREEN "Welcome Bank Staff member. You have access to the bank system.\n" RESET);
    
    loadaccounts();
    // Don't forget this! It ensures limits are correct on startup.
    reset_daily_limits(); 

    int choice; 
    while(1) {
        printf(BLUE "\n------- BANK STAFF MENU -------\n" RESET);
        printf("1.  Add New Account\n");
        printf("2.  Delete Account\n");
        printf("3.  Modify Account\n");
        printf("4.  Query (Search)\n");
        printf("5.  Advanced Search\n");
        printf("6.  Change Status\n");
        printf("7.  Withdraw\n");
        printf("8.  Deposit\n");
        printf("9.  Transfer\n");
        printf("10. Report\n");
        printf("11. Print (Sorted)\n");
        printf("12. Delete Multiple Accounts\n");
        printf("13. Quit\n");
        printf(YELLOW "Enter your choice: " RESET);
        
        // Safe input handling
        if (scanf("%d", &choice) != 1) {
            // Clean the buffer safely
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf(RED "Invalid input! Please enter a number.\n" RESET);
            choice = 0; // Set to 0 to trigger default case
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
            case 0: break; // Handled by the if check above
            default:
                printf(RED "Please enter a number from 1 to 13.\n" RESET);
        }

        // Only show the "Press 1 to return" prompt if a valid function ran
        if (choice >= 1 && choice <= 12) {
             wait_for_user();
        }
        
    } // End of While Loop
    return 0;
}