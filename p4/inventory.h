/**
 * @file inventory.h
 * @author David Mond (dmmond)
 * Contains all prototypes for record, inventory, and order. Also contains the structs for record, inventory, order, and orderitem.
*/

#include <stdbool.h>

//Length of Artist Array for records
#define ARTIST 32
//Length of Title Array for records
#define TITLE 32
//Length of Genre Array for records 
#define GENRE 14

typedef struct {
    //ID of the record
    int id;
    //Char array for the artist in the record
    char artist[ARTIST];
    //Char array for the title in the record
    char title[TITLE];
    //Char array for the genre in the record
    char genre[GENRE];
    int copies;
} Record;

typedef struct {
    //Point to the pointer to the records
    Record **records;
    int count;
    //Capacity of the inventory
    int capacity;
} Inventory;

typedef struct {
    //Pointer to the Record
    Record *record;
    //Number of copies of the Record in the order
    int copies;
} OrderItem;

typedef struct {
    //Array of pointers to OrderItems
    OrderItem **items;
    int count;
    //Capacity of the order array
    int capacity; 
} Order;

/**
* Create a new inventory and records using dynamic memory
* @return returns the inventory that was created
*/
Inventory *makeInventory();

/**
* Free the current inventory and the records in it.
* @param inventory the inventory to be freed
*/
void freeInventory(Inventory *inventory);

/**
* Read records from the filename, move them into inventory.
* @param filename file to read the records from
* @param inventory inventory to read the records into
*/
void readRecords(const char *filename, Inventory *inventory);

/**
* Sort records in the inventory by a specific format.
* @param inventory current inventory to get the records from
* @param compare format to compare the records by
*/
void sortRecords(Inventory *inventory, int (*compare)(const void *, const void *));

/**
* Lists the records in the current format with the id, artist, title, genre, and copies.
* @param inventory inventory to get the records from
* @param test record check to compare with the str parameter to print out the current record
* @param str compares with the test parameter to print current record
*/
void listRecords(Inventory *inventory, bool (*test)(const Record *, const char *), const char *str);

/**
* Creates a new order item, starting capacity of 5, uses dynamic allocated memory for the items
* @return returns the order item that was created
*/
Order *createOrder();

/**
* Adds record to order, uses the current inventory and parameters of the id and copies to get the record
* @param order the order item that the record is being added too
* @param inventory current inventory to access the records
* @param id id of the record being added
* @param copies copies of the record being added
*/
void addRecordToOrder(Order *order, Inventory *inventory, int id, int copies);

/**
* Removes the record from the current order item
* @param order current order to remove record from
* @param id id of the record that is being removed
* @param copies copies of the record that is being removed
*/
void removeRecordFromOrder(Order *order, int id, int copies);

/**
* Checkout command that checks out the order from the current inventory
* @param order order that is being checked out
* @param inventory current inventory that contains the order being checked out, reduce when checked out
*/
void checkoutOrder(Order *order, Inventory *inventory);

/**
* Free the order and each individual order item
* @param order order that is being freed
*/
void freeOrder(Order *order);

/**
* Compares by ID for the 2 records
* @param a first record for ID to be compared
* @param b second record for ID to be compared
* @return returns an int to see which parameter goes first
*/
int compareById(const void *a, const void *b);

/**
* Compare artist by artist, then title, then ID. Returns an integer to see which is greater to help sort.
* @param a first artist/title/ID
* @param b second artist/title/ID
*/
int compareByArtistTitleId(const void *a, const void *b);

/**
* Tests the genre with a given record.
* @param record record that is being tested
* @param genre genre that is being tested with the record.
* @return returns true if the genre was successfully tested, false if not
*/
bool testGenre(const Record *record, const char *genre);

/**
* List the current order in the correct format and header
* @param order Order that is being listed
*/
void listOrder(Order *order);

/**
* Defines the prototype for helper method to process commands.
* @param inventory Inventory to process the commands for list, checkout, etc.
*/
void processCommands(Inventory *inventory);