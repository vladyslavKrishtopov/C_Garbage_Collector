#include "ObjectManager.h"
#include <stdio.h>

#define LENGTH 10000

int main(int argc, char const *argv[])
{
    initPool();
    Ref objects[LENGTH];
    for(int i = 0; i < LENGTH; i++){
        objects[i] = insertObject(sizeof(int));
    }

    for(int i = 0; i < LENGTH; i++){
        if(objects[i] != 0)
            ((int*)retrieveObject(objects[i]))[0] = i;
    }

    for(int i = 0; i < LENGTH; i++){
        if(objects[i] != 0)
            printf("The number at slot#%d is %d\n\n", i, ((int*)retrieveObject(objects[i]))[0]);
    }
    destroyPool();


    return 0;
}
