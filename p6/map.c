/**
 * @file map.c
 * @author David Mond (dmmond)
 * Contains all the map functions. Can make a map, get the map size, set a map, remove a map, and more.
*/

#include "map.h"
#include <stdlib.h>
#include "value.h"

/** Lowest-numbered symbol ina  key. */
#define FIRST_SYM '!'

/** Number of possible symbols in a key. */
#define SYM_COUNT ( '~' - '!' + 1 )

/** Short name for the node used to build this tree. */
typedef struct NodeStruct Node;

/** Node in the trie data structure. */
struct NodeStruct {
  /** If the substring to the root of the tree up to this node is a
      key, this is the value that goes with it. */
  Value *val;
  
  /** Array of pointers to child nodes. */
  Node *child[ SYM_COUNT ];
};

/** Representation of a trie implementation of a map. */
struct MapStruct {
  /** Root node of this tree. */
  Node *root;
};

/** 
* Create a new trie
* @return returns the trie that was dynamically created
*/
Map *makeMap() 
{
  Map *m = malloc(sizeof(Map));
  m->root = NULL; 
  return m;
}

/**
* Helper function to recursively count the number of key/value pairs in the trie.
* @param node pointer to a node which represents the trie
* @return returns the number of pairs in the trie
*/
static int countKeys(Node *node) 
{
  if (node == NULL) return 0;
  //Assign only if not null
  int count = (node->val != NULL); 
  for (int i = 0; i < SYM_COUNT; i++)
    count += countKeys(node->child[i]);
  return count;
}

/**
* Return the number of key/value pairs in the map by using the count keys helper function
* @param m pointer to the map 
* @return returns the size of the number of pairs in the maps
*/
int mapSize(Map *m) 
{
  return countKeys(m->root);
}

/**
* Helper function to recursively free a trie node
* @param node pointer to the trie that is being freed
*/
static void freeNode(Node *node) 
{
  if (node == NULL) return;
  if (node->val != NULL) destroyValue(node->val);
  for (int i = 0; i < SYM_COUNT; i++)
    freeNode(node->child[i]);
  free(node);
}

/**
* Free all the memory associated with the map.
* @param m pointer to the map that is being freed
*/
void freeMap(Map *m) 
{
  freeNode(m->root);
  free(m);
}

/**
* Helper function to create a new trie node
* @return returns the node that was created
*/
static Node *createNode() 
{
  Node *node = calloc(1, sizeof(Node));
  return node;
}

/**
* Helper function to insert or update a value in the trie
* @param node pointer to a pointer to the node that is being updated
* @param key key for the node
* @param value value for the node
*/
static void setHelper(Node **node, char const *key, Value *val) 
{
  if (!*node) *node = createNode();

  if (*key == '\0') {
    //Replace the value at this node
    if ((*node)->val) destroyValue((*node)->val);
    (*node)->val = val;
  } else {
    //recursion for setting
    setHelper(&((*node)->child[*key - FIRST_SYM]), key + 1, val);
  }
}

/**
* Sets the map with a key and value
* @param m pointer to the map to set
* @param key key for the map
* @param val value for the map
*/
void mapSet(Map *m, char const *key, Value *val) 
{
  setHelper(&(m->root), key, val);
}

/**
* Helper method for the get command to get the value for the map.
* @param node pointer to the node
* @param key pointer to the key for the node
* @return returns a value that the get command retrieves
*/
static Value *getHelper(Node *node, char const *key) 
{
  if (!node) return NULL;

  if (*key == '\0') {
    return node->val;
  } else {
    return getHelper(node->child[*key - FIRST_SYM], key + 1);
  }
}

/** 
* Gets the map and uses the helper function
* @param m pointer to the map
* @param key pointer to the key for the map
* @return returns a value that is retrieved by the get command
*/
Value *mapGet(Map *m, char const *key) 
{
  return getHelper(m->root, key);
}

/**
* Helper function to remove a value from the trie
* @param node pointer to pointer to node for the trie
* @param key key for the trie
* @return returns true if removed, false if not
*/
static bool removeHelper(Node **node, char const *key) 
{
  if (!*node) return false;

  if (*key == '\0') {
    if ((*node)->val) {
      destroyValue((*node)->val);
      (*node)->val = NULL;
      return true;
    }
    return false;
  } else {
    return removeHelper(&((*node)->child[*key - FIRST_SYM]), key + 1);
  }
}

/**
* Removes value from the trie and uses the helper function
* @param m pointer to the trie
* @param key pointer to the key for the trie
* @return returns true if value removed, false if not
*/
bool mapRemove(Map *m, char const *key) 
{
  return removeHelper(&(m->root), key);
}
