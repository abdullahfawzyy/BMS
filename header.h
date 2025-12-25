#ifndef BANK_H
#define BANK_H
//required libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //for the multiple delete function to get the current time
#include <ctype.h>
//color codes ANSII codes
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
//system constants
#define MAX 1000
#define TRANS_LIMIT 10000.0
#define DAILY_LIMIT 50000.0
//Structs
//date struct
typedef struct {
    int month;
    int year;
    int day;
} Date;
//account struct
typedef struct {
    char acc_num[20];
    char name[100];
    char email[100];
    double balance;
    char mobile[20];
    Date date_opened;
    char status[20];
    Date last_trans_date;
    double daily_withdrawal;
} Account;
//bank stuff user struct
typedef struct {
    char username[50];
    char password[50];
} User;
//global variables
extern Account accounts[MAX];
extern int accountCount;
//main functions prototypes (to link the main with the the rest of the codes)
int login();
void loadaccounts();
void addaccount();
void deleteaccount();
void modifyaccount();
void queryaccount();
void advancedsearch();
void changestatus();
void withdraw();
void deposit();
void transfer();
void report();
void printsorted();
void deletemultiple();
void quit();
//multiple used functions or helper functions
int isvalidemail(const char *email);//the the email has @ sign
int isvalidphone(const char *phone);//check phone is all numbers
int isvalidnumber(const char *str);//check the number is all numbers
int accountexists(const char *accNum); //account existance  
void tolowerstring(char *str); //to have better advanced search
void savechanges();//saves the changes to the accoutns file by rewriting the whole file
void savetransaction(char *accNum, char *type, double amount);//saves transactions in the file of the each user
#endif