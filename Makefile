C = gcc
CFLAGS = -g -Wall -std=c99 -pedantic

all: mytar

mytar: createArchive.o createTable.o extractArchive.o extractDir.o extractFile.o extractHeader.o extractSymLink.o fillZeros.o makeDirHier.o makeHeader.o mytar.o writeDir.o writeFile.o writeSym.o
	$(CC) $(CFLAGS) -o mytar createArchive.o createTable.o extractArchive.o extractDir.o extractFile.o extractHeader.o extractSymLink.o fillZeros.o makeDirHier.o makeHeader.o mytar.o writeDir.o writeFile.o writeSym.o

mytar.o: mytar.c mytar.h
	$(CC) -c -o mytar.o mytar.c $(CFLAGS)

createArchive.o: createArchive.c mytar.h
	$(CC) -c -o createArchive.o createArchive.c $(CFLAGS)

createTable.o: createTable.c mytar.h
	$(CC) -c -o createTable.o createTable.c $(CFLAGS)

extractArchive.o: extractArchive.c mytar.h
	$(CC) -c -o extractArchive.o extractArchive.c $(CFLAGS)

extractDir.o: extractDir.c mytar.h
	$(CC) -c -o extractDir.o extractDir.c $(CFLAGS)

extractFile.o: extractFile.c mytar.h
	$(CC) -c -o extractFile.o extractFile.c $(CFLAGS)

extractHeader.o: extractHeader.c mytar.h
	$(CC) -c -o extractHeader.o extractHeader.c $(CFLAGS)

extractSymLink.o: extractSymLink.c mytar.h
	$(CC) -c -o extractSymLink.o extractSymLink.c $(CFLAGS)

fillZeros.o: fillZeros.c mytar.h
	$(CC) -c -o fillZeros.o fillZeros.c $(CFLAGS)

makeDirHier.o: makeDirHier.c mytar.h
	$(CC) -c -o makeDirHier.o makeDirHier.c $(CFLAGS)

makeHeader.o: makeHeader.c mytar.h
	$(CC) -c -o makeHeader.o makeHeader.c $(CFLAGS)

writeDir.o: writeDir.c mytar.h
	$(CC) -c -o writeDir.o writeDir.c $(CFLAGS)

writeFile.o: writeFile.c mytar.h
	$(CC) -c -o writeFile.o writeFile.c $(CFLAGS)

writeSym.o: writeSym.c mytar.h
	$(CC) -c -o writeSym.o writeSym.c $(CFLAGS)

clean:
	rm *.o core* mytar
