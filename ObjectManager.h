#ifndef _OBJECT_MANAGER_H
#define _OBJECT_MANAGER_H

#ifndef MEMORY_SIZE
#define MEMORY_SIZE 1024*512
#endif

#ifndef NULL
#define NULL 0
#endif

typedef unsigned long Ref;
typedef unsigned long ulong;
typedef unsigned char uchar;

// initialize the object manager
void initPool();

// clean up the object manager (before exiting)
void destroyPool();

// inserts an object in the pool
Ref insertObject( ulong size );

// returns a pointer to the object being requested given by the reference id
void *retrieveObject( Ref ref );

// update our index to indicate that we have another reference to the given object
void addReference( Ref ref );

// update our index to indicate that a reference is gone
void dropReference( Ref ref );

void dumpPool();

#endif