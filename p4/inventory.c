/**
 * @file inventory.c
 * @author David Mond (dmmond)
 * Contains all commands for record and inventory, helps create inventory and records, sort, list, and read in inventory and records.
*/

#include "inventory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Length of buffer array
#define LENGTH 1024
//Capacity of Inventory
#define CAPACITY 5
//Length of Genre
#define LGENRE 12
//Length of Title and Artist
#define LTITLEARTIST 30
//Multiplier to make array bigger
#define CAPACITYMULTIPLIER 2
//Matcher for input values
#define THREE 3

/**
* Create a new inventory and records using dynamic memory
* @return returns the inventory that was created
*/
Inventory *makeInventory() {
    Inventory *inventory = (Inventory *)malloc(sizeof(Inventory));
    if (!inventory) return NULL;
    inventory->count = 0;
    inventory->capacity = CAPACITY;
    inventory->records = (Record **)malloc(sizeof(Record *) * inventory->capacity);
    if (!inventory->records) {
        free(inventory);
        return NULL;
    }
    return inventory;
}

/**
* Free the current inventory and the records in it.
* @param inventory the inventory to be freed
*/
void freeInventory(Inventory *inventory) {
    if (!inventory) return;
    for (int i = 0; i < inventory->count; i++) {
        free(inventory->records[i]);
    }
    free(inventory->records);
    free(inventory);
}

/**
* Read records from the filename, move them into inventory.
* @param filename file to read the records from
* @param inventory inventory to read the records into
*/
void readRecords(const char *filename, Inventory *inventory) {
    FILE *file = fopen(filename, "r");
    //Use a large buffer to ensure it can hold any line.
    char lineBuffer[LENGTH];
    while (fgets(lineBuffer, sizeof(lineBuffer), file)) {
        //Remove any trailing newline characters from the buffer.
        int i = 0;
        while (lineBuffer[i] != '\0') {
            if (lineBuffer[i] == '\n' || lineBuffer[i] == '\r') {
                lineBuffer[i] = '\0';
                break; 
            }
            i++;
        }

        Record *newRecord = (Record *)malloc(sizeof(Record));
        if (!newRecord) {
            continue; 
        }

        //Read the ID, genre, and copies from the line buffer.
        if (sscanf(lineBuffer, "%d %12s %d", &newRecord->id, newRecord->genre, &newRecord->copies) != THREE) {
            //The line doesn't match the expected format.
            fprintf(stderr, "Invalid record file: %s\n", filename);
            free(newRecord);
            exit(1);
        }

        newRecord->genre[LGENRE] = '\0';
        if (strlen(newRecord->genre) > LGENRE) {
            //Genre name is too long.
            fprintf(stderr, "Invalid record file: %s\n", filename);
            free(newRecord);
            exit(1);
        }

        //ID must be a positive integer
        if (newRecord->id <= 0) {
            fprintf(stderr, "Invalid record file: %s\n", filename);
            exit(1); 
        }
        //Number of copies cant be negative
        if (newRecord->copies < 0) {
            fprintf(stderr, "Invalid record file: %s\n", filename);
            exit(1); 
        }

        //Read the artist name.
        if (!fgets(newRecord->artist, sizeof(newRecord->artist), file)) {
            free(newRecord);
            continue;
        }
        i = 0;
        while (newRecord->artist[i] != '\0') {
            if (newRecord->artist[i] == '\n' || newRecord->artist[i] == '\r') {
                newRecord->artist[i] = '\0';
                break;
            }
            i++;
        }
        if (strlen(newRecord->artist) > LTITLEARTIST) {
            //Artist name is too long.
            fprintf(stderr, "Invalid record file: %s\n", filename);
            free(newRecord);
            exit(1);
        }


        if (strlen(newRecord->artist) == sizeof(newRecord->artist) - 1) { 
            int c;
            while ((c = fgetc(file)) != '\n' && c != EOF) {
                 //Loop until a newline or EOF
            }
        }
        //Read the title.
        if (!fgets(newRecord->title, sizeof(newRecord->title), file)) {
            free(newRecord);
            continue;
        }
        i = 0;
        while (newRecord->title[i] != '\0') {
            if (newRecord->title[i] == '\n' || newRecord->title[i] == '\r') {
                newRecord->title[i] = '\0';
                break; 
            }
            i++;
        }
        if (strlen(newRecord->title) > LTITLEARTIST) {
            //Title name is too long.
            fprintf(stderr, "Invalid record file: %s\n", filename);
            free(newRecord);
            exit(1);
        }

        //Check for existing record with the same ID but different genre, artist, or title.
        for (int i = 0; i < inventory->count; i++) {
            if (inventory->records[i]->id == newRecord->id &&
                (strcmp(inventory->records[i]->genre, newRecord->genre) != 0 ||
                 strcmp(inventory->records[i]->artist, newRecord->artist) != 0 ||
                 strcmp(inventory->records[i]->title, newRecord->title) != 0)) {
                fprintf(stderr, "Invalid record file: %s\n", filename);
                free(newRecord);
                exit(1);
            }
        }

        //Check for existing record with the same ID
        bool found = false;
        for (int i = 0; i < inventory->count; i++) {
            if (inventory->records[i]->id == newRecord->id) {
                //Existing record found, update the number of copies.
                inventory->records[i]->copies += newRecord->copies;
                found = true;
                break;
            }
        }

        if (!found) {
            //no matching record found, add the new record to inventory.
            if (inventory->count >= inventory->capacity) {
                //Increase the capacity of the inventory array
                inventory->capacity *= CAPACITYMULTIPLIER;
                Record **newArray = realloc(inventory->records, inventory->capacity * sizeof(Record *));
                if (!newArray) {
                    //free the new record and exit the loop
                    free(newRecord);
                    break;
                }
                inventory->records = newArray;
            }
            inventory->records[inventory->count++] = newRecord;
        } else {
            //The new record is not needed free memory.
            free(newRecord);
        }
    }

    fclose(file);
}

/**
* Sort records in the inventory by a specific format.
* @param inventory current inventory to get the records from
* @param compare format to compare the records by
*/
void sortRecords(Inventory *inventory, int (*compare)(const void *, const void *)) {
    if (inventory && inventory->records) {
        //Sort records using qsort
        qsort(inventory->records, inventory->count, sizeof(Record *), compare);
    }
}

/**
* Lists the records in the current format with the id, artist, title, genre, and copies.
* @param inventory inventory to get the records from
* @param test record check to compare with the str parameter to print out the current record
* @param str compares with the test parameter to print current record
*/
void listRecords(Inventory *inventory, bool (*test)(const Record *, const char *), const char *str) {
    if (!inventory) 
    return;

    // Print header
    printf("%-3s %-30s %-30s %-12s %6s\n", "ID", "Artist", "Title", "Genre", "Copies");

    for (int i = 0; i < inventory->count; i++) {
        //If matching or test is not present, print
        if (test == NULL || (test && test(inventory->records[i], str))) {
            Record *rec = inventory->records[i];
            printf("%-3d %-30s %-30s %-12s %6d\n", 
                rec->id, 
                rec->artist, 
                rec->title, 
                rec->genre, 
                rec->copies);
        }
    }
}

/**
* Compares by ID for the 2 records
* @param a first record for ID to be compared
* @param b second record for ID to be compared
* @return returns an int to see which parameter goes first
*/
int compareById(const void *a, const void *b) {
    const Record *ra = *(const Record **)a;
    const Record *rb = *(const Record **)b;
    //Compares by ID
    return ra->id - rb->id;
}

/**
* Compare artist by artist, then title, then ID. Returns an integer to see which is greater to help sort.
* @param a first artist/title/ID
* @param b second artist/title/ID
* @return returns an int to see which paramter goes first
*/
int compareByArtistTitleId(const void *a, const void *b) {
    const Record *ra = *(const Record **)a;
    const Record *rb = *(const Record **)b;
    //Compare by artist
    int artistCmp = strcmp(ra->artist, rb->artist);
    if (artistCmp != 0) 
    return artistCmp;
    //Compare by title
    int titleCmp = strcmp(ra->title, rb->title);
    if (titleCmp != 0) 
    return titleCmp;
    //Compare by ID if artist and title are the same
    return ra->id - rb->id;
}

/**
* Tests the genre with a given record.
* @param record record that is being tested
* @param genre genre that is being tested with the record.
* @return returns true if the genre was successfully tested, false if not
*/
bool testGenre(const Record *record, const char *genre) {
    return strcmp(record->genre, genre) == 0;
}