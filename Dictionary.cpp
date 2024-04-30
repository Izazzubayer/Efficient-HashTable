/*
 * Dictionary.cpp
 * 
 * Class Description: Dictionary data collection ADT class.
 *                    The implementation of this class is based on 
 *                    the hashing strategy and the open addressing 
 *                    collision resolution strategy called linear probing hashing. 
 *            
 * Class Invariant: Each element stored in this Dictionary is unique (no duplications allowed).
 *
 * Author: Izaz Zubayer (301386899) & Zoe Stanley (301427509)
 * Last Modification: April 2024
 */

// You can add more #include statements if you wish.

#include <iostream>
#include <cstring>
#include "Dictionary.h"  
#include "Profile.h"
#include "UnableToInsertException.h"
#include "ElementAlreadyExistsException.h"
#include "EmptyDataCollectionException.h"
#include "ElementDoesNotExistException.h"

using std::cout;
using std::endl;
using std::__cxx11::stoul;

// Constructor
Dictionary::Dictionary() {}

// Destructor
// Description: Destruct a Dictionary object, releasing heap-allocated memory.
// Precondition: Dictionary exists. 
// Postcondition: Dictionary and all its elements are freed from memory, pointers are handled (no mem leaks).
Dictionary::~Dictionary() {
  // Release all heap memory
  if ( hashTable != nullptr ) { 
    for ( unsigned int i = 0; i < CAPACITY; i++ ) 
      if ( hashTable[i] != nullptr ) {      
        delete hashTable[i]; // Delete object of Profile class on heap
        hashTable[i] = nullptr;
      }
    delete [] hashTable;
    hashTable = nullptr;	
  }
}

// Description: Returns the total element count currently stored in Dictionary.
// Precondition: Dictionary exists. 
// Postcondition: Dictionary remains unchanged.
unsigned int Dictionary::getElementCount() const {
   return elementCount;
}

// Description: Returns the CAPACITY (size) of the Dictionary.
// Precondition: There is a dictionary upon which to call method. 
// Postcondition: Dictionary remains unchanged.
unsigned int Dictionary::getCapacity() const{
   return CAPACITY;
}

// Hash Function
// Description: Hashes the given indexingKey to get a hash index based on Truncation, as this
//              produces the best spread and performance for our tests. 
// Calculating Time Efficiency: max(O(1),O(1),O(1),O(1)) = O(1)
// Final Time Efficiency: O(1)
// My Space Efficiency: O(1) 
// Space Efficiency: O(1)

// hashfunction based on truncation
// Precondition: a valid indexing key is provided (not confirmed, as they're given by a valid file). 
// Postcondition: a valid hash key is outputted.
unsigned int Dictionary::hashFunction( string indexingKey) {
  unsigned long hashValue = stoul(indexingKey);
  // This is a personalized linear probing hash function
  // The hash index is calculated by dividing the hash value by 16 and then modding it by the CAPACITY
  // With this, more thab 50% of hash table has been probed without collisions.
  unsigned int hashIndex = (hashValue/16) % CAPACITY; //O(1)
  return hashIndex;
}


// Description: Inserts an element into the Dictionary and increments "elementCount".
//              For this Assignment 5, you do not have to expand the hashTable when it is full. 
// Precondition: newElement must not already be in in the Dictionary.  
// Exception: Throws UnableToInsertException if we cannot insert newElement in the Dictionary.
//            For example, if the operator "new" fails, or Dictionary is full (temporary solution).
// Exception: Throws ElementAlreadyExistsException if newElement is already in the Dictionary. 
void Dictionary::insert( Profile * newElement )  {

   // Have we allocated memory for the hashTable yet?
   if ( elementCount == 0 ) {
      hashTable = new Profile*[CAPACITY];
      if (hashTable == nullptr) throw UnableToInsertException("In insert(): new failed.");
      for ( unsigned int i = 0; i < CAPACITY; i++ ) 
        hashTable[i] = nullptr; // Initialize each cell of hashTable, i.e., each pointer to nullptr.
   }

   // If Dictionary is full - to do: expand the Dictionary when full!
   // For this Assignment 5, you do not have to expand the Dictionary when it is full. 
   if ( elementCount == CAPACITY ) 
     throw UnableToInsertException("In insert(): Dictionary is full.");
   
   // Call hash function using indexing key to get hash index
   unsigned int hashIndex = hashFunction(newElement->getUserName());

   // Keep hashing and probing until no more collisions using Linear Collision Resolution Stratetgy
   unsigned int i = 0;
   unsigned int count = 0;
   while ( hashTable[(hashIndex + i)%CAPACITY] != nullptr ) {   
     // If newElement not already in Dictionary
     if ( *(hashTable[(hashIndex + i)%CAPACITY]) == *newElement ) {
	     // newElement was found? Then it already exists in the Dictionary, throw exception.
	     throw ElementAlreadyExistsException("In insert(): newElement already in Dictionary.");
	   }
     count++;
	   i++;
     if ( count == CAPACITY ) 
		   throw UnableToInsertException("In insertHelper(): Dictionary is full.");
   }
   
   // Insert new elem into the hashTable at hashIndex
   hashTable[hashIndex] = newElement; 
/* 
	 cout << "In insert: newElement = " << *newElement << "\t address of newElement = " 
	   << newElement << " *(hashTable[hashIndex]) = " << *(hashTable[hashIndex]) << endl;
*/

  // increment elementcount
  elementCount++;
    
  return;
}


// Description: Returns a pointer to the target element if found.
// Postcondition: Dictionary remains unchanged.
// Exception: Throws EmptyDataCollectionException if the Dictionary is empty.
// Exception: Throws ElementDoesNotExistException if target is not found in the Dictionary.
Profile* Dictionary::get(Profile& target) {
  if ( elementCount == 0 ) 
     throw EmptyDataCollectionException("In get(): Dictionary is empty.");

  // Call hash function using indexing key to get hash index
  unsigned int hashIndex = hashFunction(target.getUserName());

  // Keep hashing and probing until no more collisions using Linear Collision Resolution Stratetgy
  unsigned int i = 0;
  unsigned int count = 0;
  while ( hashTable[(hashIndex + i)%CAPACITY] != nullptr ) {   
    // If target found in Dictionary
    if ( *(hashTable[(hashIndex + i)%CAPACITY]) == target ) 
      return hashTable[(hashIndex + i)%CAPACITY];
    count++;
    i++;
    if ( count == CAPACITY ) 
      throw ElementDoesNotExistException("In get(): target not found in Dictionary.");
  }
  throw ElementDoesNotExistException("In get(): target not found in Dictionary.");
}

   
// Description: Prints all elements stored in the Dictionary (unsorted).
// Postcondition: Dictionary remains unchanged.
// Exception: Throws EmptyDataCollectionException if the Dictionary is empty.
void Dictionary::printDictionary()  const {
  if ( elementCount == 0 ) 
     throw EmptyDataCollectionException("In printDictionary(): hashTable is empty.");

  cout << endl << "Printing the Dictionary with " << this->elementCount << " elements: " << endl;
  for (unsigned int index = 0; index < CAPACITY; index++) {
    if ( hashTable[index] != nullptr ) 
		cout << "HashTable[" << index << "] = " << *(hashTable[index]) << endl; 
  }   
  return;
}