#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for a Book
struct Book {
    char title[50];
    char author[50];
    char ISBN[13];
    int year;
};


//adding abook
void addBook(struct Book books[], int *count) {
    printf("Enter book title: ");
    scanf(" %[^\n]", books[*count].title);
    printf("Enter author: ");
    scanf(" %[^\n]", books[*count].author);
    printf("Enter ISBN: ");
    scanf("%s", books[*count].ISBN);
    printf("Enter publication year: ");
    scanf("%d", &books[*count].year);
    (*count)++;
    printf("Book added successfully!\n");
}


//being able to view all books
void viewBooks(struct Book books[], int count) {
    printf("\nLibrary Books:\n");
    for (int i = 0; i < count; i++) {
        printf("Title: %s\n", books[i].title);
        printf("Author: %s\n", books[i].author);
        printf("ISBN: %s\n", books[i].ISBN);
        printf("Year: %d\n\n", books[i].year);
    }
}


//searching book by title
void searchBook(struct Book books[], int count, char *title) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(books[i].title, title) == 0) {
            printf("Book found:\n");
            printf("Title: %s\n", books[i].title);
            printf("Author: %s\n", books[i].author);
            printf("ISBN: %s\n", books[i].ISBN);
            printf("Year: %d\n", books[i].year);
            found = 1;
            break;
        }
    }
    if (!found) printf("Book not found.\n");
}

//deleting a book by title
void deleteBook(struct Book books[], int *count, char *title) {
    int found = 0;
    for (int i = 0; i < *count; i++) {
        if (strcmp(books[i].title, title) == 0) {
            for (int j = i; j < *count - 1; j++) {
                books[j] = books[j + 1];
            }
            (*count)--;
            found = 1;
            printf("Book deleted successfully.\n");
            break;
        }
    }
    if (!found) printf("Book not found.\n");
}

int main() {
    struct Book books[100];
    int count = 0;
    int choice;
    char title[50];

    do {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. View Books\n");
        printf("3. Search Book\n");
        printf("4. Delete Book\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addBook(books, &count);
                break;
            case 2:
                viewBooks(books, count);
                break;
            case 3:
                printf("Enter the title of the book to search: ");
                scanf(" %[^\n]", title);
                searchBook(books, count, title);
                break;
            case 4:
                printf("Enter the title of the book to delete: ");
                scanf(" %[^\n]", title);
                deleteBook(books, &count, title);
                break;
            case 5:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 5);

    return 0;
}

