#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <sys/wait.h>

struct termios oldt;

// Restaurar la terminal, usar también como handler de señales
void restore_terminal(int sig) {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n"); // Por si el cursor quedó en la misma línea
    exit(0);
}

int main() {
    struct termios newt;

    // Obtener configuración actual
    if (tcgetattr(STDIN_FILENO, &oldt) == -1) {
        perror("tcgetattr");
        return 1;
    }

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);

    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1) {
        perror("tcsetattr");
        return 1;
    }

    // Registrar manejadores de señal
    signal(SIGINT, restore_terminal);
    signal(SIGTERM, restore_terminal);

    // Crear proceso hijo
    pid_t pid = fork();

    if (pid == 0) {
        // Proceso hijo: ejecuta ensamblador
        execl("./bin/matrizElementos", "matrizElementos", (char *)NULL);
        perror("execl"); // Si falla
        exit(1);
    } else if (pid > 0) {
        // Proceso padre: espera al hijo
        int status;
        waitpid(pid, &status, 0);

        // Restaurar terminal al terminar
        restore_terminal(0);
    } else {
        perror("fork");
        restore_terminal(0);
        return 1;
    }

    return 0;
}
