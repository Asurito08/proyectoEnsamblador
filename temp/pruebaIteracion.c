#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define rows 12
#define cols 15
#define rowlen (cols + 1)  // 9 chars + '\n'

extern int iterar_matriz(char *matrix, char tecla);

void imprimir_matriz(char *matrix) {
    for (int i = 0; i < rows; i++) {
        fwrite(matrix + i * rowlen, 1, cols, stdout);
        putchar('\n');
    }
}

int main() {
    FILE *f = fopen("../saves/nivel.txt", "r");
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
        printf("Presiona una tecla (w/a/s/d para mover al jugador, t/y/u/i para mover las E, z/x/c/v para mover las V, k para disparar, p para continuar o q para salir): ");
        tecla = getchar();

        // Limpiar buffer stdin para evitar problemas con fgets o getchar
        while (getchar() != '\n');

        if (tecla == 'q') {
            printf("Saliendo...\n");
            break;
        }

        int resultado = iterar_matriz(matrix, tecla);

        switch (resultado)
        {
        case 0:
            printf("Movimiento válido\n");
            break;
        case 1:
            printf("¡Perdiste una vida! Tocaste un enemigo\n");
            break;
        case 2:
            printf("Enemigo eliminado\n");
            break;
        case 3:
            printf("Ganaste, ya no quedan enemigos\n");
            break; 
        case -1:
            printf("Movimiento inválido o sin cambio\n");
            break;
        default:
            printf("numero retornado: %d", resultado);
            break;
        }
        imprimir_matriz(matrix);

        if (resultado == 3) {
            printf("Saliendo...\n");
            break;
        }
    }

    return 0;
}
