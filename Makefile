main: ObjectManager.o main.c
	clang++ -Wall ObjectManager.o main.c -o main
ObjectManager.o: ObjectManager.c ObjectManager.h
	clang++ -Wall -c ObjectManager.c -o ObjectManager.o
clean:
	rm *.o