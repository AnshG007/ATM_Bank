#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct customer {
    int accno;
    char *name;
    float balance;
    struct customer *next;
    struct customer *prev;
};

struct trans {
    int accno;
    char trans_type;
    float amount;
};

struct customer *head = NULL;
struct customer *tail = NULL;

// Function to read customer data from file and create the linked list
void readData() {
    FILE *file = fopen("CUSTOMER.DAT", "r");
    if (file == NULL) {
        printf("Error opening file: CUSTOMER.DAT\n");
        exit(1);
    }

    while (!feof(file)) {
        struct customer *newCustomer = (struct customer *)malloc(sizeof(struct customer));
        if (fscanf(file, "%d", &newCustomer->accno) != 1) {
            break;
        }

        char nameBuffer[100];
        if (fscanf(file, "%99s", nameBuffer) != 1) {
            break;
        }
        newCustomer->name = strdup(nameBuffer);

        if (fscanf(file, "%f", &newCustomer->balance) != 1) {
            break;
        }

        if (head == NULL) {
            head = newCustomer;
        } else {
            tail->next = newCustomer;
            newCustomer->prev = tail;
        }
        tail = newCustomer;
    }

    // Make the list circular
    if (head != NULL) {
        head->prev = tail;
        tail->next = head;
    }

    fclose(file);
}

// Function to write customer data back to the file
void writeData() {
    FILE *file = fopen("CUSTOMER.DAT", "w");
    if (file == NULL) {
        printf("Error opening file: CUSTOMER.DAT\n");
        exit(1);
    }

    struct customer *current = head;

    do {
        fprintf(file, "%d %s %.2f\n", current->accno, current->name, current->balance);
        current = current->next;
    } while (current != head);

    fclose(file); // Close the file after writing
}

// Function to update customer data based on the deposit transaction
void deposit(struct trans *transaction) {
    printf("Enter Transaction Amount: ");
    scanf("%f", &transaction->amount);
    if (transaction->amount < 0) {
        printf("Invalid transaction amount. Amount cannot be negative.\n");
        return;
    }
    struct customer *current = head;
    do {
        if (current->accno == transaction->accno) {
            current->balance += transaction->amount;
            printf("\n");
            printf("Transaction successful!\n");
            printf("Account details for Accno %d:\n", current->accno);
            printf("Name: %s\n", current->name);
            printf("Balance: %.2f Rs.\n", current->balance);
            writeData(); // Update the file with new data
            return;
        }
        current = current->next;
    } while (current != head);
    printf("Error: Account with Accno %d not found.\n", transaction->accno);
}

// Function to update customer data based on the withdrawal transaction
void withdraw(struct trans *transaction) {
    printf("Enter Transaction Amount: ");
    scanf("%f", &transaction->amount);
    if (transaction->amount < 0) {
        printf("Invalid transaction amount. Amount cannot be negative.\n");
        return;
    }
    struct customer *current = head;
    do {
        if (current->accno == transaction->accno) {
            float remaining_balance = current->balance - transaction->amount;
            if (remaining_balance >= 100.0) {
                current->balance = remaining_balance;
                printf("\n");
                printf("Transaction successful!\n");
                printf("Account details for Accno %d:\n", current->accno);
                printf("Name: %s\n", current->name);
                printf("Balance: %.2f Rs.\n", current->balance);
                writeData(); // Update the file with new data
            } else {
                printf("Error: Account balance cannot go below 100 Rs.\n");
            }
            return;
        }
        current = current->next;
    } while (current != head);
    printf("Error: Account with Accno %d not found.\n", transaction->accno);
}

// Function to update customer data based on the balance inquiry transaction
void balanceEnquiry(struct trans *transaction) {
    struct customer *current = head;
    do {
        if (current->accno == transaction->accno) {
            printf("\n");
            printf("Account details for Accno %d:\n", current->accno);
            printf("Name: %s\n", current->name);
            printf("Balance: %.2f Rs.\n", current->balance);
            return;
        }
        current = current->next;
    } while (current != head);
    printf("Error: Account with Accno %d not found.\n", transaction->accno);
}

// Function to find a customer by account number
struct customer *findCustomer(int accno) {
    struct customer *current = head;
    do {
        if (current->accno == accno) {
            return current;
        }
        current = current->next;
    } while (current != head);
    return NULL; // Account not found
}

int main() {
    readData();
    int choice = 1;
    struct trans transaction;

    while (choice) {
        printf("\n1. Transaction (D for deposit, W for withdraw, B for balance enquiry)\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nTransaction:\n");
                printf("Enter Account Number: ");
                scanf("%d", &transaction.accno);
                struct customer *cust = findCustomer(transaction.accno);
                if (cust == NULL) {
                    printf("Error: Account with Accno %d not found.\n", transaction.accno);
                } else {
                    printf("Enter Transaction Type (D for deposit, W for withdraw, B for balance enquiry): ");
                    scanf(" %c", &transaction.trans_type);
                    if (transaction.trans_type == 'D') {
                        deposit(&transaction);
                    } else if (transaction.trans_type == 'W') {
                        withdraw(&transaction);
                    } else if (transaction.trans_type == 'B') {
                        balanceEnquiry(&transaction);
                    } else {
                        printf("Invalid transaction type.\n");
			
                    }
                }
                break;
            case 0:
                printf("Exiting the application. Thank you!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;

