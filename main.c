#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 100
#define MAX_BORROWERS 50
#define MAX_LOANS 100

struct Book {
    int id;
    char title[50];
    char author[50];
    int is_issued;
};

struct Borrower {
    int id;
    char name[50];
};

struct Loan {
    int book_id;
    int borrower_id;
    time_t issue_date;
    time_t due_date;
    int is_returned;
};

struct Book books[MAX_BOOKS];
struct Borrower borrowers[MAX_BORROWERS];
struct Loan loans[MAX_LOANS];
int book_count = 0, borrower_count = 0, loan_count = 0;

// Function prototypes
void loadDatabase();
void saveDatabase();
void addBook();
void addBorrower();
void issueBook();
void returnBook();
void viewIssuedBooks();
void listBorrowers();
void calculateFine(struct Loan loan);
int findBorrowerById(int id);

int main() {
    loadDatabase();
    int choice;
    do {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. Add Borrower\n");
        printf("3. Issue Book\n");
        printf("4. Return Book\n");
        printf("5. View Issued Books\n");
        printf("6. List All Borrowers\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // clear newline

        switch (choice) {
            case 1: addBook(); break;
            case 2: addBorrower(); break;
            case 3: issueBook(); break;
            case 4: returnBook(); break;
            case 5: viewIssuedBooks(); break;
            case 6: listBorrowers(); break;
            case 0: printf("Exiting...\n"); saveDatabase(); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 0);

    return 0;
}

// Load data from .dat files
void loadDatabase() {
    FILE *fbook = fopen("books.dat", "rb");
    FILE *fborrower = fopen("borrowers.dat", "rb");
    FILE *floan = fopen("loans.dat", "rb");

    if (fbook) {
        fread(&book_count, sizeof(int), 1, fbook);
        fread(books, sizeof(struct Book), book_count, fbook);
        fclose(fbook);
    }
    if (fborrower) {
        fread(&borrower_count, sizeof(int), 1, fborrower);
        fread(borrowers, sizeof(struct Borrower), borrower_count, fborrower);
        fclose(fborrower);
    }
    if (floan) {
        fread(&loan_count, sizeof(int), 1, floan);
        fread(loans, sizeof(struct Loan), loan_count, floan);
        fclose(floan);
    }
}

// Save data to .dat files
void saveDatabase() {
    FILE *fbook = fopen("books.dat", "wb");
    FILE *fborrower = fopen("borrowers.dat", "wb");
    FILE *floan = fopen("loans.dat", "wb");

    fwrite(&book_count, sizeof(int), 1, fbook);
    fwrite(books, sizeof(struct Book), book_count, fbook);
    fclose(fbook);

    fwrite(&borrower_count, sizeof(int), 1, fborrower);
    fwrite(borrowers, sizeof(struct Borrower), borrower_count, fborrower);
    fclose(fborrower);

    fwrite(&loan_count, sizeof(int), 1, floan);
    fwrite(loans, sizeof(struct Loan), loan_count, floan);
    fclose(floan);
}

// Add a new book
void addBook() {
    if (book_count >= MAX_BOOKS) {
        printf("Book limit reached!\n");
        return;
    }

    books[book_count].id = book_count + 1;
    printf("Enter book title: ");
    fgets(books[book_count].title, sizeof(books[book_count].title), stdin);
    printf("Enter author: ");
    fgets(books[book_count].author, sizeof(books[book_count].author), stdin);
    books[book_count].is_issued = 0;
    book_count++;
    printf("Book added successfully.\n");
}

// Add a new borrower
void addBorrower() {
    if (borrower_count >= MAX_BORROWERS) {
        printf("Borrower limit reached!\n");
        return;
    }

    borrowers[borrower_count].id = borrower_count + 1;
    printf("Enter borrower name: ");
    fgets(borrowers[borrower_count].name, sizeof(borrowers[borrower_count].name), stdin);
    borrower_count++;
    printf("Borrower added successfully.\n");
}

// Issue a book to a borrower
void issueBook() {
    int book_id, borrower_id;
    printf("Enter book ID to issue: ");
    scanf("%d", &book_id);
    printf("Enter borrower ID: ");
    scanf("%d", &borrower_id);
    getchar(); // clear newline

    if (book_id <= 0 || book_id > book_count || borrower_id <= 0 || borrower_id > borrower_count) {
        printf("Invalid book or borrower ID.\n");
        return;
    }

    if (books[book_id - 1].is_issued) {
        printf("Book is already issued.\n");
        return;
    }

    books[book_id - 1].is_issued = 1;

    loans[loan_count].book_id = book_id;
    loans[loan_count].borrower_id = borrower_id;
    loans[loan_count].issue_date = time(NULL);
    loans[loan_count].due_date = loans[loan_count].issue_date + (7 * 24 * 60 * 60); // 1-week loan period
    loans[loan_count].is_returned = 0;
    loan_count++;
    printf("Book issued successfully.\n");
}

// Return a book
void returnBook() {
    int book_id, borrower_id;
    printf("Enter book ID to return: ");
    scanf("%d", &book_id);
    printf("Enter borrower ID: ");
    scanf("%d", &borrower_id);
    getchar(); // clear newline

    for (int i = 0; i < loan_count; i++) {
        if (loans[i].book_id == book_id && loans[i].borrower_id == borrower_id && !loans[i].is_returned) {
            loans[i].is_returned = 1;
            books[book_id - 1].is_issued = 0;
            calculateFine(loans[i]);
            printf("Book returned successfully.\n");
            return;
        }
    }

    printf("No active loan found for the given book and borrower.\n");
}

// Calculate and display fine if overdue
void calculateFine(struct Loan loan) {
    time_t now = time(NULL);
    if (now > loan.due_date) {
        int days_overdue = (now - loan.due_date) / (24 * 60 * 60);
        int fine = days_overdue * 5; // Assume a fine of 5 units per overdue day
        printf("Book is overdue by %d days. Fine: %d\n", days_overdue, fine);
    } else {
        printf("Book returned on time. No fine.\n");
    }
}

// View issued books with borrower details
void viewIssuedBooks() {
    printf("\nIssued Books:\n");
    printf("Book ID | Borrower Name | Issue Date | Due Date | Returned\n");
    for (int i = 0; i < loan_count; i++) {
        struct Borrower borrower = borrowers[loans[i].borrower_id - 1];
        struct tm *issue_date = localtime(&loans[i].issue_date);
        struct tm *due_date = localtime(&loans[i].due_date);
        printf("%7d | %13s | %02d-%02d-%04d | %02d-%02d-%04d | %s\n",
               loans[i].book_id, borrower.name,
               issue_date->tm_mday, issue_date->tm_mon + 1, issue_date->tm_year + 1900,
               due_date->tm_mday, due_date->tm_mon + 1, due_date->tm_year + 1900,
               loans[i].is_returned ? "Yes" : "No");
    }
}

// List all registered borrowers
void listBorrowers() {
    printf("\nList of Borrowers:\n");
    printf("Borrower ID | Name\n");
    for (int i = 0; i < borrower_count; i++) {
        printf("%11d | %s", borrowers[i].id, borrowers[i].name);
    }
}

