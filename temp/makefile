target: ejecutable

ejecutable: matrizElementos.o pruebaIteracion.o
	gcc matrizElementos.o pruebaIteracion.o -o ejecutable

pruebaIteracion.o: pruebaIteracion.c
	gcc -c pruebaIteracion.c -o pruebaIteracion.o

matrizElementos.o: matrizElementos.asm
	nasm -f elf64 matrizElementos.asm -o matrizElementos.o

run: ejecutable
	./ejecutable

clean:
	rm -f ejecutable matrizElementos.o pruebaIteracion.o