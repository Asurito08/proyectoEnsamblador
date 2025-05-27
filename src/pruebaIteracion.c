#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define rows 7
#define cols 9
#define rowlen (cols + 1)  // 9 chars + '\n'

extern int iterar_matriz(char *matrix, char tecla);

void imprimir_matriz(char *matrix) {
    for (int i = 0; i < rows; i++) {
        fwrite(matrix + i * rowlen, 1, cols, stdout);
        putchar('\n');
    }
}

int main() {
    FILE *f = fopen("saves/nivel.txt", "r");
    if (!f) {
        perror("Error al abrir nivel.txt");
        return 1;
    }

    char matrix[rows * rowlen];
    memset(matrix, 0, sizeof(matrix));

    // Leer la matriz incluyendo saltos de línea
    for (int i = 0; i < rows; i++) {
        if (!fgets(matrix + i * rowlen, rowlen + 1, f)) {
            fprintf(stderr, "Error leyendo línea %d\n", i);
            fclose(f);
            return 1;
        }
    }
    fclose(f);

    printf("Matriz inicial:\n");
    imprimir_matriz(matrix);

    char tecla;
    while (1) {
        printf("Presiona una tecla (w/a/s/d para mover, q para salir): ");
        tecla = getchar();

        // Limpiar buffer stdin para evitar problemas con fgets o getchar
        while (getchar() != '\n');

        if (tecla == 'q') {
            printf("Saliendo...\n");
            break;
        }

        int resultado = iterar_matriz(matrix, tecla);

        if (resultado == 1) {
            printf("¡Perdiste! Eliminaste una 'X'\n");
        } else if (resultado == 0) {
            printf("Movimiento válido\n");
        } else {
            printf("Movimiento inválido o sin cambio\n");
        }

        imprimir_matriz(matrix);
    }

    return 0;
}
