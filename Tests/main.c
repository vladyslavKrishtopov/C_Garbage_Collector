#include "ObjectManager.h"
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#define LENGTH 10000

int testsFailed = 0;
int totalNumberOfTests = 0;

void test();

int main(int argc, char const *argv[])
{
    test();


    return 0;
}

void test(){
    printf("Testing how it handles overflowing the buffer.\n");
    initPool();
    
    printf("Testing insert Function\n");
    Ref objects[LENGTH];
    for(int i = 0; i < LENGTH; i++){
        objects[i] = insertObject(sizeof(int));
    }

    printf("Testing references recieved by the insert function\n");
    
    for(int i = 0; i < LENGTH; i++){
        totalNumberOfTests++;
        if((i+1)*sizeof(int) < MEMORY_SIZE){
            if(objects[i] == 0){
                printf("Wrong reference at slot #%d", i);
                testsFailed++;
            }
        } else {
            if(objects[i] != 0){
                printf("Wrong reference at slot #%d", i);
                testsFailed++;
            }
        }
    }

    if(testsFailed == 0){
        printf("All the objects were updated succesfully\n");
    } else {
        printf("Insert failed.\n");
    }

    int insertsFailed = 0;
    printf("Inserting numbers into memory slots\n");
    for(int i = 0; i < LENGTH; i++){
        if(objects[i] != 0)
            ((int*)retrieveObject(objects[i]))[0] = i;
    }

    for(int i = 0; i < LENGTH; i++){
        totalNumberOfTests++;
        if(objects[i] != 0){
            if(((int*)retrieveObject(objects[i]))[0] != i){
                testsFailed++;
                insertsFailed++;
            }
        }
    }
    
    if(insertsFailed == 0){
        printf("Retirieve function gives right adress");
    } else {
        printf("Something is wrong with retrieveObject()");
    }

    printf("Testing Add/Drop reference\n");

    srand(time(NULL));
    int index = rand()%(MEMORY_SIZE/sizeof(int)); // generating a random index within existing range
    int prevNumber = ((int*)retrieveObject(objects[index]))[0];
    printf("%d\n", prevNumber);
    dropReference(index);
    insertObject(sizeof(int)); // inserting object to run compression
    totalNumberOfTests++;
    printf("%d\n", ((int*)retrieveObject(objects[index]))[0]);
    if(prevNumber != ((int*)retrieveObject(objects[index]))[0]){
        printf("Test passed\n");
    } else{
        printf("Test failed\n");
        testsFailed++;
    }
    index = rand()%(MEMORY_SIZE/sizeof(int));
    prevNumber = ((int*)retrieveObject(objects[index]))[0];
    addReference(objects[index]);
    dropReference(objects[index]);
    insertObject(sizeof(int)); // should fail
    if(prevNumber != ((int*)retrieveObject(objects[index]))[0]){
        printf("Test failed\n");
        testsFailed++;
    } else {
        printf("Test passed\n");
    }

    destroyPool();
}