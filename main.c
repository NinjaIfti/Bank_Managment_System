#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TRANSACTIONS 10

// Define the Account structure
typedef struct {
    float balance;
    char branch[50];
    char transactionHistory[MAX_TRANSACTIONS][100];
    int transactionCount;
} Account;

// Function to save the account data to the file
void saveAccountData(Account *account) {
    FILE *file = fopen("account.txt", "w");
    if (file == NULL) {
        printf("Error opening file for saving data.\n");
        return;
    }

    // Save branch name, balance, and transaction history to the file
    fprintf(file, "%s\n", account->branch);
    fprintf(file, "%.2f\n", account->balance);
    for (int i = 0; i < account->transactionCount; i++) {
        fprintf(file, "%s\n", account->transactionHistory[i]);
    }

    fclose(file); // Close the file
    printf("Account data saved successfully.\n");
}

// Function to load the account data from the file
int loadAccountData(Account *account) {
    FILE *file = fopen("account.txt", "r");
    if (file == NULL) {
        return 0; // No account exists, so return 0
    }

    // Read branch name, balance, and transaction history from the file
    fgets(account->branch, sizeof(account->branch), file);
    fscanf(file, "%f\n", &account->balance);

    account->transactionCount = 0;
    while (fgets(account->transactionHistory[account->transactionCount], sizeof(account->transactionHistory[account->transactionCount]), file)) {
        account->transactionHistory[account->transactionCount][strcspn(account->transactionHistory[account->transactionCount], "\n")] = 0; // Remove newline
        account->transactionCount++;
    }

    fclose(file); // Close the file
    return 1; // Account data loaded successfully
}

// Function to create a new account
void createAccount(Account *account) {
    printf("Welcome to IFIC Bank!\n");

    // Present branch options for first-time users
    printf("Please select a branch:\n");
    printf("1. Bashundhara Branch\n");
    printf("2. Uttara Branch\n");
    printf("3. Mirpur Branch\n");

    int branchChoice;
    printf("Enter the number of the branch you want to choose: ");
    scanf("%d", &branchChoice);

    switch (branchChoice) {
        case 1:
            strcpy(account->branch, "Bashundhara Branch");
            break;
        case 2:
            strcpy(account->branch, "Uttara Branch");
            break;
        case 3:
            strcpy(account->branch, "Mirpur Branch");
            break;
        default:
            printf("Invalid choice, defaulting to Bashundhara Branch.\n");
            strcpy(account->branch, "Bashundhara Branch");
            break;
    }

    // Set initial balance
    account->balance = 0.0;
    account->transactionCount = 0;

    // Save the new account data
    saveAccountData(account);
}

// Deposit function
void deposit(Account *account) {
    float amount;
    printf("Enter the amount to deposit: ");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("Invalid deposit amount.\n");
        return;
    }

    account->balance += amount;
    printf("Deposited %.2f. New balance: %.2f\n", amount, account->balance);

    // Save the transaction
    sprintf(account->transactionHistory[account->transactionCount], "Deposited %.2f", amount);
    account->transactionCount++;
    saveAccountData(account);
}

// Withdraw function
void withdraw(Account *account) {
    float amount;
    printf("Enter the amount to withdraw: ");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("Invalid withdrawal amount.\n");
        return;
    }

    if (amount > account->balance) {
        printf("Insufficient funds. Your current balance is %.2f.\n", account->balance);
        return;
    }

    account->balance -= amount;
    printf("Withdrew %.2f. New balance: %.2f\n", amount, account->balance);

    // Save the transaction
    sprintf(account->transactionHistory[account->transactionCount], "Withdrew %.2f", amount);
    account->transactionCount++;
    saveAccountData(account);
}

// Balance Inquiry function
void checkBalance(Account *account) {
    printf("Current balance: %.2f\n", account->balance);
}

// Transaction History function
void viewTransactionHistory(Account *account) {
    printf("Transaction History:\n");
    for (int i = 0; i < account->transactionCount; i++) {
        printf("%s\n", account->transactionHistory[i]);
    }
}

int main() {
    Account account;

    // Load account data if available
    if (loadAccountData(&account)) {
        printf("Welcome back to IFIC Bank! You are at the %s.\n", account.branch);
    } else {
        createAccount(&account);
    }

    // Main banking operations loop
    int choice;
    do {
        printf("\nBanking Operations:\n");
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Check Balance\n");
        printf("4. View Transaction History\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                deposit(&account);
                break;
            case 2:
                withdraw(&account);
                break;
            case 3:
                checkBalance(&account);
                break;
            case 4:
                viewTransactionHistory(&account);
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}
