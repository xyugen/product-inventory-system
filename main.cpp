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

// GLOBALS
User current_user;
bool is_logged = false;

FILE* openFile(const char* fileName, const char* mode)
{
    FILE *fp = fopen(fileName, mode);
    if (fp == nullptr) printf("Cannot find file %s!\n", fileName);

    return fp;
}

// TOOLS
bool checkProductID(int id)
{
    FILE* fp = openFile(PFNAME, "r");
    if (fp == NULL)
    {
        return false;
    }

    Product product;
    while (fread(&product, sizeof(product), 1, fp))
    {
        if (id == product.id)
        {
            fclose(fp);
            return true;
        }
    }

    fclose(fp);
    return false;
}

bool checkUserID(int id)
{
    FILE* fp = openFile(UFNAME, "r");
    if (fp == NULL)
    {
        return false;
    }

    User user;
    while (fread(&user, sizeof(user), 1, fp))
    {
        if (id == user.id)
        {
            fclose(fp);
            return true;
        }
    }

    fclose(fp);
    return false;
}

bool checkUsername(char* username)
{
    FILE* fp = openFile(UFNAME, "r");
    if (fp == NULL)
    {
        return false;
    }

    User user;
    while (fread(&user, sizeof(user), 1, fp))
    {
        if (!strcmp(username, user.username))
        {
            fclose(fp);
            return true;
        }
    }

    fclose(fp);
    return false;
}

bool checkProductName(char* productName)
{
    FILE* fp = openFile(PFNAME, "r");
    if (fp == NULL)
    {
        return false;
    }

    Product product;
    while (fread(&product, sizeof(product), 1, fp))
    {
        if (!strcmp(productName, product.name))
        {
            fclose(fp);
            return true;
        }
    }

    fclose(fp);
    return false;
}

int latestUserID()
{
    FILE* fp = openFile(UFNAME, "r");

    User user;
    int lastID;
    while (fread(&user, sizeof(user), 1, fp))
    {
        if (user.id > lastID)
        {
            lastID = user.id;
        }
    }

    fclose(fp);
    return lastID;
}

int latestProductID()
{
    FILE* fp = openFile(PFNAME, "r");

    Product product;
    int lastID;
    while (fread(&product, sizeof(product), 1, fp))
    {
        if (product.id > lastID)
        {
            lastID = product.id;
        }
    }

    fclose(fp);
    return lastID;
}

// Removes any new line
void rmNewline(char* input)
{
    int l = strlen(input);
    if (input[l - 1] == '\n')
    {
        input[l - 1] = '\0';
    }
}


// USER MANAGEMENT
void signup()
{
    FILE* fp = openFile(UFNAME, "a");
    if (fp == NULL)
    {
        return;
    }

    printf("Creating file...\n\n"
           "\t\tProduct Inventory System\n"
           "\t\t\tSign Up\n"
           "----------------------------------------------\n"
           "Enter admin username: ");
    fgets(current_user.username, MAX_LEN, stdin);
    rmNewline(current_user.username);

    printf("Enter admin password: ");
    fgets(current_user.password, MAX_LEN, stdin);
    rmNewline(current_user.password);

    current_user.id = 1;
    current_user.is_admin = true;

    fwrite(&current_user, sizeof(current_user), 1, fp);

    fclose(fp);
}

void login()
{
    FILE* fp = openFile(UFNAME, "r");

    while (!checkUserID(1))
    {
        signup();
    }

    printf("\tProduct Inventory System\n"
           "\t\tLogin\n"
           "----------------------------------------------\n"
           "Enter username: ");
    fgets(current_user.username, MAX_LEN, stdin);
    rmNewline(current_user.username);

    printf("Enter password: ");
    fgets(current_user.password, MAX_LEN, stdin);
    rmNewline(current_user.password);

    User user;
    while (fread(&user, sizeof(user), 1, fp))
    {
        bool is_user = !strcmp(current_user.username, user.username) && !strcmp(current_user.password, user.password);
        if (is_user)
        {
            is_logged = true;
            current_user.is_admin = user.is_admin;
            return;
        }
    }

    fclose(fp);
}

void adduser()
{
    FILE* fp = openFile(UFNAME, "a");

    User newUser;
    printf("NEW USER\n");

    printf("Enter username: ");
    getchar();
    fgets(newUser.username, MAX_LEN, stdin);
    rmNewline(newUser.username);

    // Check if username already exists
    if (checkUsername(newUser.username))
    {
        printf("Username already exists!");
        return;
    }

    printf("Enter password: ");
    fgets(newUser.password, MAX_LEN, stdin);
    rmNewline(newUser.password);

    int c;
    printf("Is an admin?\n"
           "[1] Yes\n"
           "[0] No (default)\n"
           ">> ");
    scanf("%d", &c);

    switch (c) {
        case 1:
            newUser.is_admin = true;
            break;
        case 2:
        default:
            newUser.is_admin = false;
            break;
    }

    newUser.id = latestUserID() + 1;

    fwrite(&newUser, sizeof(newUser), 1, fp);

    printf("New user created successfully:\n"
           "User ID: %d\n"
           "Username: %s\n"
           "Password: %s\n"
           "Admin? %d\n", newUser.id, newUser.username, newUser.password, newUser.is_admin);

    fclose(fp);
}


// PRODUCT MANAGEMENT
void addProduct()
{
    FILE* fp = openFile(PFNAME, "a");
    if (fp == nullptr) return;

    Product newProduct;
    printf("NEW PRODUCT\n");

    printf("Enter product name: ");
    getchar();
    fgets(newProduct.name, MAX_LEN, stdin);
    rmNewline(newProduct.name);

    if (checkProductName(newProduct.name))
    {
        printf("Product already exists!\n");
        return;
    }

    printf("Enter quantity: ");
    scanf("%d", &newProduct.quantity);

    printf("Enter price: ");
    scanf("%f", &newProduct.price);

    newProduct.id = latestProductID() + 1;

    fwrite(&newProduct, sizeof(newProduct), 1, fp);
    printf("Product successfully added!\n");

    fclose(fp);
}

void readProduct()
{
    FILE* fp = openFile(PFNAME, "r");
    if (fp == nullptr) return;

    Product product;
    printf("\t\tPRODUCT LIST\n"
           "ID\t\tName\t\t\tQuantity\t\tPrice\n");
    while (fread(&product, sizeof(product), 1, fp))
    {
        printf("%d\t\t%s\t\t\t%4d\t\t$%.2f\n", product.id, product.name, product.quantity, product.price);
    }

    fclose(fp);
}

void updateProduct()
{
    FILE* fp = openFile(PFNAME, "r");
    FILE* temp = openFile(PTFNAME, "w");
    if ((fp == nullptr) || (temp == nullptr)) return;

    Product newProduct;
    printf("Enter product ID: ");
    scanf("%d", &newProduct.id);

    if (!checkProductID(newProduct.id))
    {
        printf("Product does not exist!\n");
        return;
    }

    printf("Enter new product name: ");
    getchar();
    fgets(newProduct.name, MAX_LEN, stdin);
    rmNewline(newProduct.name);

    printf("Enter product quantity: ");
    scanf("%d", &newProduct.quantity);

    printf("Enter product price: ");
    scanf("%f", &newProduct.price);


    Product product;
    while (fread(&product, sizeof(product), 1, fp))
    {
        if (product.id == newProduct.id)
            fwrite(&newProduct, sizeof(newProduct), 1, temp);
        else fwrite(&product, sizeof(product), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(PFNAME);
    rename(PTFNAME, PFNAME);

    printf("Product successfully updated!\n");
}

void menu ()
{
    char last[MAX_LEN] = "\n";
    if (current_user.is_admin) {
        strcpy(last, "[9] Add a user\n\n");
    }
    printf("\tProduct Inventory System\n"
           "\t\tMenu\n"
           "----------------------------------------------\n"
           "[1] Add a product\n"
           "[2] See product list\n"
           "[3] Update a product\n"
           "[4] Remove a product\n"
           "%s"
           "[0] Exit\n"
           "----------------------------------------------\n", last);
}

int main() {
    int c;

    do {
        login();
    } while (!is_logged);

    do {
        menu();

        printf("Choice: ");
        scanf("%d", &c);

        switch (c) {
            case 1:
                addProduct();
                break;
            case 2:
                readProduct();
                break;
            case 3:
                updateProduct();
                break;
            case 4:
                break;
            case 9:
                if (current_user.is_admin) {
                    adduser();
                } else printf("Invalid input...\n");
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid input...");
                break;
        }
    } while (c != 0);

    return 0;
}