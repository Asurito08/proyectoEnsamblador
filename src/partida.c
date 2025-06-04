#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FILAS 7
#define COLS 9
#define ROWLEN (COLS + 1)
#define RUTA_ARCHIVO "saves/nivel.txt"

extern int iterar_matriz(char *matriz, char tecla);

GtkGrid *grid_dibujo = NULL;
GtkWidget *imagenes[FILAS][COLS];
char matriz[FILAS * ROWLEN];
gint tecla_buffer = 0;
int frame_count = 0;

void cargar_matriz(const char *filename, char *matriz) {
    memset(matriz, 0, FILAS * ROWLEN);
    FILE *archivo = fopen(filename, "r");
    if (!archivo) {
        perror("Error al abrir nivel");
        return;
    }

    for (int i = 0; i < FILAS; i++) {
        char *linea = matriz + i * ROWLEN;
        if (fgets(linea, ROWLEN + 1, archivo)) {
            linea[strcspn(linea, "\n")] = '\0'; 
        }
    }

    fclose(archivo);
}

void pintar_matriz(void) {
    if (!GTK_IS_GRID(grid_dibujo)) {
        printf("Error: grid_dibujo no está inicializado.\n");
        return;
    }

    // Eliminar widgets existentes del grid
    GtkWidget *child = gtk_widget_get_first_child(GTK_WIDGET(grid_dibujo));
    while (child != NULL) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_grid_remove(GTK_GRID(grid_dibujo), child);
        child = next;
    }

    // Limpiar referencias previas
    memset(imagenes, 0, sizeof(imagenes));

    // Dibujar la matriz
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLS; j++) {
            GtkWidget *widget = NULL;
            char c = matriz[i * ROWLEN + j];

            printf("Carácter en [%d][%d]: '%c'\n", i, j, c);

            switch (c) {
                case 'V':
                    widget = gtk_picture_new_for_filename("design/elementos/nave1.svg");
                    break;
                case 'X':
                    widget = gtk_picture_new_for_filename("design/elementos/nave2.svg");
                    break;
                case '^':
                    widget = gtk_picture_new_for_filename("design/elementos/bala.svg");
                    break;
                case '0':
                    widget = gtk_picture_new();
                    break;
                default:
                printf("Carácter desconocido: '%c' [%d][%d]\n", c, i, j);
                    widget = gtk_picture_new();
                    break;
            }

            
            if (GTK_IS_WIDGET(widget)) {
                gtk_grid_attach(GTK_GRID(grid_dibujo), widget, j, i, 1, 1);
                imagenes[i][j] = widget;
            }
        }
    }
    gtk_widget_queue_draw(GTK_WIDGET(grid_dibujo));
}

void loop_juego(void) {
    if (tecla_buffer != 0) {
        char tecla = (char)tecla_buffer;
        tecla_buffer = 0;
        printf("tecla seleccionada: %c\n", tecla);

        iterar_matriz(matriz, tecla);
    }

    pintar_matriz();
}

gboolean capturar_tecla(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data) {
    gunichar unicode = gdk_keyval_to_unicode(keyval);

    if (unicode != 0 && unicode <= 127) {
        char tecla = (char)unicode;

        g_print("Tecla presionada: %c\n", tecla);

        if (strchr("wasdpk", tecla)) {
            tecla_buffer = tecla;
            loop_juego();
        }
    }

    return TRUE;
}

GtkWidget* crearPantallaPartida(void) {
    cargar_matriz(RUTA_ARCHIVO, matriz);
    memset(imagenes, 0, sizeof(imagenes));

    GtkWidget *contenedor = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(contenedor, TRUE);
    gtk_widget_set_vexpand(contenedor, TRUE);
    gtk_widget_set_focusable(contenedor, TRUE);
    gtk_widget_grab_focus(contenedor);

    GtkWidget *grid_widget = gtk_grid_new();
    grid_dibujo = GTK_GRID(grid_widget);
    gtk_widget_set_hexpand(grid_widget, TRUE);
    gtk_widget_set_vexpand(grid_widget, TRUE);
    gtk_grid_set_row_homogeneous(grid_dibujo, TRUE);
    gtk_grid_set_column_homogeneous(grid_dibujo, TRUE);
    gtk_box_append(GTK_BOX(contenedor), grid_widget);

    GtkEventController *teclado = gtk_event_controller_key_new();
    gtk_event_controller_set_propagation_phase(teclado, GTK_PHASE_CAPTURE);
    gtk_widget_add_controller(contenedor, teclado);
    g_signal_connect(teclado, "key-pressed", G_CALLBACK(capturar_tecla), NULL);

    g_timeout_add(50, (GSourceFunc)gtk_widget_grab_focus, contenedor);

    loop_juego();

    return contenedor;
}
