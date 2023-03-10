#include <stdio.h>
#include <string.h>
/*
 * PRODUCT INVENTORY SYSTEM
 * CREATOR: XYUGEN
 * LICENCE: MIT
 */

#define PFNAME "products.txt"
#define PTFNAME "products.temp.txt"
#define UFNAME "users.txt"
#define UTFNAME "users.temp.txt"
#define MAX_LEN 100

typedef struct {
    int id;
    char name[MAX_LEN];
    int quantity;
    float price;
} Product;

typedef struct {
    int id;
    char username[MAX_LEN], password[MAX_LEN];
    bool is_admin;
} User;

User current_user;

void menu ()
{
    char last[MAX_LEN] = "\n";
    if (current_user.is_admin) {
        strcpy(last, "[9] Add a user\n\n");
    }
    printf("\t\t\tProduct Inventory System\n"
           "----------------------------------------\n"
           "[1] Add a product\n"
           "[2] See product list\n"
           "[3] Update a product\n"
           "[4] Remove a product\n"
           "%s"
           "[0] Exit\n"
           "----------------------------------------\n", last);
}

int main() {
    int c;

    do {
        menu();
    } while (c != 0);

    return 0;
}
