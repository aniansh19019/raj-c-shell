ls := bin/ls
date := bin/date
mkdir := bin/mkdir
rm := bin/rm
cat := bin/cat

binaries := $(ls) $(date) $(mkdir) $(rm) $(cat)



all: raj_shell $(binaries)
shell: raj_shell

# binaries

$(ls): ls/ls.c
	cd ls && make

$(date): date/date.c
	cd date && make

$(mkdir): mkdir/mkdir.c
	cd mkdir && make

$(rm): rm/rm.c
	cd rm && make

$(cat): cat/cat.c
	cd cat && make

# binaries done

raj_shell: raj_shell.o
	gcc -o raj_shell raj_shell.o

raj_shell.o: raj_shell.c
	gcc -c -o raj_shell.o raj_shell.c

clean:
	rm -f raj_shell raj_shell.o
	cd ls && make clean
	cd date && make clean
	cd mkdir && make clean
	cd rm && make clean
	cd cat && make clean