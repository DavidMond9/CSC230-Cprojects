/**
 * @file vinyl.c
 * @author David Mond (dmmond)
 * Runs the main program and proccess all the commands. Also contains all the order aspects of the project.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventory.h"
#include "input.h"

//Argument Min Count
#define ARGCOUNT 2
//Length for the genre array
#define GENREARR 14
//Length for list command
#define LIST 4
//Length for list genre command
#define LISTGENRE 11
//Length for add command
#define ADD 3
//Multiplier to make array bigger
#define CAPACITYMULTIPLIER 2
//Capacity of Inventory
#define CAPACITY 5


/** 
* Main part of the program, creates the inventory, reads in the records, and calls the processCommands helper method.
* @param argc argument count for the command line
* @param argv argument array to hold the arguments
* @return return an integer for exit success or exit failure
*/
int main(int argc, char *argv[]) 
{
    Inventory *inventory = makeInventory();
    if (!inventory) {
        fprintf(stderr, "Error: Could not create inventory.\n");
        return EXIT_FAILURE;
    }

    if (argc < ARGCOUNT) {
        fprintf(stderr, "usage: vinyl record-file+\n");
        freeInventory(inventory);
        return EXIT_FAILURE;
    }
   //Read in the records and check for correct file
    for (int i = 1; i < argc; i++) {
        FILE *testFile = fopen(argv[i], "r");
        //Exit if any file does not exist
        if (!testFile) {
            fprintf(stderr, "Can't open file: file-that-doesnt-exist.txt\n");
            freeInventory(inventory);
            return EXIT_FAILURE;
        }
        fclose(testFile);
        readRecords(argv[i], inventory);
    }
    //Handle all commands for the input
    processCommands(inventory);
    //free inventory before exiting
    freeInventory(inventory); 
    return EXIT_SUCCESS;
}

/**
* Defines the prototype for helper method to process commands.
* @param inventory Inventory to process the commands for list, checkout, etc.
*/
void processCommands(Inventory *inventory) 
{
    char *line = NULL;
    //Create a new order
    Order *order = createOrder(); 

    while (true) { 
        printf("cmd> ");
        fflush(stdout);
        line = readLine(stdin);
        
        if (!line) 
        break; 

        printf("%s\n", line); 

        if (strcmp(line, "quit") == 0) {
            free(line);
            break;
        } else if (strncmp(line, "list", LIST) == 0) {
            if (strncmp(line, "list genre ", LISTGENRE) == 0) {
                char genre[GENREARR] = {0};
                sscanf(line + 11, "%12s", genre);
            //Sort records by artist, title, and ID before listing
                sortRecords(inventory, compareByArtistTitleId);
            //list records filtered by genre
                listRecords(inventory, testGenre, genre);
            } else if (strcmp(line, "list order") == 0) {
                //List current order
                listOrder(order);
            } else {
                if(strcmp(line, "list") != 0){
                   printf("Invalid command\n");
                }
                else {
                    sortRecords(inventory, compareById);
                    listRecords(inventory, NULL, NULL); 
                }
            }
        } else if (strncmp(line, "add", ADD) == 0) {
            int id, copies;
            if (sscanf(line, "add %d %d", &id, &copies) == 2) {
                addRecordToOrder(order, inventory, id, copies);
            } else {
                printf("Invalid command\n");
            }
        } else if (strncmp(line, "remove", 6) == 0) {
            int id, copies;
            if (sscanf(line, "remove %d %d", &id, &copies) == 2) {
                removeRecordFromOrder(order, id, copies);
            } else {
                printf("Invalid command\n");
            }
        } else if (strcmp(line, "checkout") == 0) {
            checkoutOrder(order, inventory);
            //Clear and free the order after checkout
            freeOrder(order);
            order = createOrder();
        } else {
            printf("Invalid command\n");
        }
        //freeing line after processing
        free(line); 
        printf("\n");
    }
    //free order at end
    freeOrder(order);
}

/**
* Creates a new order item, starting capacity of 5, uses dynamic allocated memory for the items
* @return returns the order item that was created
*/
Order *createOrder() 
{
    Order *order = (Order *)malloc(sizeof(Order));
    if (!order) 
    return NULL;
    order->count = 0;
    //starting capacity
    order->capacity = CAPACITY;
    //create order items
    order->items = (OrderItem **)malloc(sizeof(OrderItem *) * order->capacity);
    if (!order->items) {
        free(order);
        return NULL;
    }
    return order;
}

/**
* Adds record to order, uses the current inventory and parameters of the id and copies to get the record
* @param order the order item that the record is being added too
* @param inventory current inventory to access the records
* @param id id of the record being added
* @param copies copies of the record being added
*/
void addRecordToOrder(Order *order, Inventory *inventory, int id, int copies) 
{
    if (!order || !inventory) 
    return;

    //find the record in inventory
    Record *recordToAdd = NULL;
    for (int i = 0; i < inventory->count; i++) {
        if (inventory->records[i]->id == id) {
            recordToAdd = inventory->records[i];
            break;
        }
    }
    
    if (!recordToAdd) {
        printf("Invalid command\n");
        return;
    }

    if (recordToAdd->copies < copies) {
        //Not enough copies in inventory
        printf("Invalid command\n"); 
        return;
    }

    //Check if the record already exists in the order
    for (int i = 0; i < order->count; i++) {
        if (order->items[i]->record->id == id) {
            //Update the number of copies in the order
            order->items[i]->copies += copies;
            return;
        }
    }

    //record not in the order yet
    if (order->count >= order->capacity) {
        //resize the order array if necessary
        int newCapacity = order->capacity * CAPACITYMULTIPLIER;
        OrderItem **newItems = (OrderItem **)realloc(order->items, sizeof(OrderItem *) * newCapacity);
        if (!newItems) 
        return;
        order->items = newItems;
        order->capacity = newCapacity;
    }

    //Create a new order item
    OrderItem *newItem = (OrderItem *)malloc(sizeof(OrderItem));
    newItem->record = recordToAdd;
    newItem->copies = copies;
    order->items[order->count++] = newItem;
}

/**
* Removes the record from the current order item
* @param order current order to remove record from
* @param id id of the record that is being removed
* @param copies copies of the record that is being removed
*/
void removeRecordFromOrder(Order *order, int id, int copies) 
{
    if (!order) 
    return;

    for (int i = 0; i < order->count; i++) {
        if (order->items[i]->record->id == id) {
            //Found the record in the order
            if (order->items[i]->copies > copies) {
                order->items[i]->copies -= copies;
            } else {
                //Remove the item completely if not enough copies remain
                free(order->items[i]);
                for (int j = i; j < order->count - 1; j++) {
                    order->items[j] = order->items[j + 1];
                }
                order->count--;
            }
            return;
        }
    }
    printf("Invalid command\n");
}

/**
* Checkout command that checks out the order from the current inventory
* @param order order that is being checked out
* @param inventory current inventory that contains the order being checked out, reduce when checked out
*/
void checkoutOrder(Order *order, Inventory *inventory) 
{
    if (!order || !inventory) 
    return;

    //Go through each order item and reduce the inventory count
    for (int i = 0; i < order->count; i++) {
        int id = order->items[i]->record->id;
        int copies = order->items[i]->copies;

        //Find the matching record in inventory and reduce copies
        for (int j = 0; j < inventory->count; j++) {
            if (inventory->records[j]->id == id) {
                if (inventory->records[j]->copies >= copies) {
                    inventory->records[j]->copies -= copies;
                }
                break;
            }
        }
    }

    //After checkout, clear the order
    for (int i = 0; i < order->count; i++) {
        free(order->items[i]);
    }
    //Reset order count after checkout
    order->count = 0;
}

/**
* Free the order and each individual order item
* @param order order that is being freed
*/
void freeOrder(Order *order) 
{
    if (!order) return;
    for (int i = 0; i < order->count; i++) {
        //Free each individual order item
        free(order->items[i]);
    }
    //Free the array of pointers
    free(order->items);
    //Free the order
    free(order); 
}

/**
* List the current order in the correct format and header
* @param order Order that is being listed
*/
void listOrder(Order *order) 
{
    if (!order || order->count == 0) {
        printf("%-3s %-30s %-30s %-12s %6s\n", "ID", "Artist", "Title", "Genre", "Copies");
        return;
    }
    
    //Print header
    printf("%-3s %-30s %-30s %-12s %6s\n", "ID", "Artist", "Title", "Genre", "Copies");
    
    for (int i = 0; i < order->count; i++) {
        Record *rec = order->items[i]->record;
        int copies = order->items[i]->copies;
        printf("%-3d %-30s %-30s %-12s %6d\n",
               rec->id, rec->artist, rec->title, rec->genre, copies);
    }
}