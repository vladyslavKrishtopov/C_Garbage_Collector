#include "ObjectManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct MEMORY_CELL cell;
typedef struct MEMORY_CELL{
    Ref ref;
    ulong size;
    ulong refCount;
    void* cellAdress;
    cell* next;
}cell;


void* buffer; // pointer to the buffer
void* freePtr; // pointer to the free space in our buffer
ulong takenSpace; // counts how much space is currently taken
Ref refCounter; // counter for references
cell* top;

// Reference check
static void checkReference(Ref ref){
    ulong refCount = 0;
    cell* curr = top;
    while(curr != NULL){
        if(curr->ref == ref){
            refCount = curr->refCount;
        }
        curr = curr->next;
    }
    if(refCount <= 0 ){
        printf("Invalid reference exception with reference %lu, terminating process.\n\n", ref);
        exit(0);
    }
}
//-------------------

// Constructor
void initPool(){
    buffer = malloc(MEMORY_SIZE);
    freePtr = buffer;
    refCounter = 1;
    top = NULL;
    takenSpace = 0;
}
//-------------------

static void destroyList();

// Destructor
void destroyPool(){
    destroyList();
    free(buffer);
}

static void destroyList(){
    cell* curr = top;
    while(curr != NULL){
        cell* dummy = curr;
        curr = curr->next;
        free(dummy);
    }
}
//-------------------

// Insert
void compact();
Ref createCell(ulong);

Ref insertObject( ulong size ){
    Ref newObjRef = 0;
    if(MEMORY_SIZE-takenSpace < size) compact(); // checks if there are unused memory sells and fixes them
    if(MEMORY_SIZE-takenSpace > size){
        newObjRef = createCell(size);
    } else {
        printf("Unable to successfully complete memory allocation request.\n\n");
    }

    return newObjRef;
}

void compact(){
    ulong cellNum = 0;
    ulong spaceCollected = 0;
    takenSpace = 0;
    while(top != NULL && top->refCount <= 0){
        cell* oldTop = top;
        if(top->next != NULL){
            memmove(top->cellAdress, top->next->cellAdress, MEMORY_SIZE-top->size); // segmentation fault here
            top->next->cellAdress = top->cellAdress;
            top = top->next;
            spaceCollected += oldTop->size;
            cell* i = top;
            while(i->next != NULL){
                i->next->cellAdress = (char*)i->cellAdress+i->size;
                i=i->next;
            }
        }
        free(oldTop);
    }
    takenSpace+=top->size;
    if(top != NULL){
        cell* curr = top;
        cellNum++;
        while(curr->next != NULL){
            if(curr->next->refCount > 0){
                cellNum++;
                takenSpace+=curr->next->size;
                curr = curr->next;
            } else {
                cell* oldNode = curr->next;
                if(curr->next->next != NULL){
                    memmove(curr->next->cellAdress, curr->next->next->cellAdress, MEMORY_SIZE-takenSpace);
                    curr->next->next->cellAdress = curr->next->cellAdress;
                    spaceCollected += curr->next->size;
                    curr->next = curr->next->next;
                    cell* i = curr->next;
                    while(i->next != NULL){
                        i->next->cellAdress = (char*)i->cellAdress+i->size;
                        i=i->next;
                    }
                } else {
                    break;
                }
                free(oldNode);
            }
        }
    } 
    freePtr = (char*) buffer + takenSpace;
    printf("Garbage collector statistics:\n");
    printf("Number of objects: %lu\nCurrently %lu bytes in use\nTotal space collected: %lu bytes\n\n", cellNum, takenSpace, spaceCollected);
}

Ref createCell(ulong size){

    cell* newObject = (cell*)malloc(sizeof(cell));
    newObject->ref = refCounter;
    refCounter++;
    newObject->size = size;
    newObject->refCount = 1;
    newObject->cellAdress = freePtr;
    freePtr = (char*)freePtr + size;
    newObject->next = NULL;
    takenSpace += size;

    // Adding new cell to the list of cells
    if(top == NULL){
        top = newObject;
    } else {
        cell* curr = top;
        while(curr->next != NULL){
            curr = curr->next;
        }
        curr->next = newObject;
    }

    return newObject->ref;
}
//-------------------

// returns a pointer to the object being requested given by the reference id
void* retrieveObject( Ref ref ){
    checkReference(ref);
    void* object = NULL;
    cell* curr = top;
    while(curr != NULL){
        if(curr->ref == ref){
            object = curr->cellAdress;
        }
        curr = curr->next;
    }
    return object;
}
//-------------------

// update our index to indicate that we have another reference to the given object
void addReference( Ref ref ){
    cell* curr = top;
    while(curr != NULL){
        if(curr->ref == ref){
            curr->refCount++;
        }
        curr = curr->next;
    }
}
//-------------------

// update our index to indicate that a reference is gone
void dropReference( Ref ref ){
    checkReference(ref);
    cell* curr = top;
    while(curr != NULL){
        if(curr->ref == ref){
            curr->refCount--;
        }
        curr = curr->next;
    }
}
//-------------------

// simply prints out every single taken memory cell
void dumpPool(){
    cell* curr = top;
    while(curr != NULL){
        printf("Reference ID: %lu\nStart Adress: %p\nSize: %lu bytes\n\n\n", curr->ref, curr->cellAdress, curr->size);
        curr = curr->next;
    }
}