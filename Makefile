output: general.o piece.o saisie.o partie.o main.o
	gcc general.o piece.o saisie.o partie.o main.o -o JDDI.exe

general.o: src/general.c src/general.h
	gcc -c -Wall src/general.c

piece.o: src/piece.c src/piece.h
	gcc -c -Wall src/piece.c

saisie.o: src/saisie.c src/saisie.h
	gcc -c -Wall src/saisie.c

partie.o: src/partie.c src/partie.h
	gcc -c -Wall src/partie.c

main.o: src/main.c
	gcc -c -Wall src/main.c

clean:
	rm -f *.o JDDI.exe
	
	
	
