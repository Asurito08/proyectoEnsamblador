#include "partida.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#define FILAS 7
#define COLS 9
#define RUTA_ARCHIVO "saves/nivelModificado.txt"

extern void ejecutar_jugada(char tecla);  // Función ensamblador

static GtkWidget *imagenes[FILAS][COLS];
static GtkWidget *grid = NULL;

static gboolean actualizar_grid(gpointer user_data) {
    if (!grid || !GTK_IS_GRID(grid)) {
        g_warning("Grid no inicializado o inválido.");
        return G_SOURCE_CONTINUE;
    }

    FILE *archivo = fopen(RUTA_ARCHIVO, "r");
    if (!archivo) {
        g_warning("No se pudo abrir el archivo.");
        return G_SOURCE_CONTINUE;
    }

    char linea[COLS + 2];
    for (int i = 0; i < FILAS; i++) {
        if (!fgets(linea, sizeof(linea), archivo)) break;

        for (int j = 0; j < COLS; j++) {
            GtkWidget *nuevo = NULL;

            if (linea[j] == 'V') {
                nuevo = gtk_picture_new_for_filename("design/elementos/nave1.svg");
            } else if (linea[j] == 'X') {
                nuevo = gtk_picture_new_for_filename("design/elementos/nave2.svg");
            } else {
                nuevo = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            }

            gtk_widget_set_hexpand(nuevo, TRUE);
            gtk_widget_set_vexpand(nuevo, TRUE);
            gtk_widget_set_halign(nuevo, GTK_ALIGN_CENTER);
            gtk_widget_set_valign(nuevo, GTK_ALIGN_CENTER);

            if (imagenes[i][j]) {
                gtk_grid_remove(GTK_GRID(grid), imagenes[i][j]);
            }

            gtk_grid_attach(GTK_GRID(grid), nuevo, j, i, 1, 1);
            imagenes[i][j] = nuevo;
        }
    }

    fclose(archivo);
    gtk_widget_queue_draw(grid);
    return G_SOURCE_CONTINUE;
}

static gboolean on_key_press(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data) {
    char tecla = (char)gdk_keyval_to_unicode(keyval);

    if (tecla == 'w' || tecla == 'a' || tecla == 's' || tecla == 'd') {
        printf("→ Llamando a ejecutar_jugada con: %c\n", tecla);
        fflush(stdout); // Asegura que el mensaje se imprima antes del crash
        ejecutar_jugada(tecla);
        printf("✓ Retorno desde ejecutar_jugada\n");
        fflush(stdout);

        actualizar_grid(NULL);
    }

    return GDK_EVENT_PROPAGATE;
}


GtkWidget* crearPantallaPartida(void) {
    g_print("Creando pantalla de partida...\n");

    FILE *archivo = fopen(RUTA_ARCHIVO, "r");
    if (!archivo) {
        g_warning("No se pudo abrir el archivo: %s", RUTA_ARCHIVO);
        return NULL;
    }

    GtkWidget *contenedor = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(contenedor, TRUE);
    gtk_widget_set_vexpand(contenedor, TRUE);
    gtk_widget_set_focusable(contenedor, TRUE);  // Para recibir teclado

    grid = gtk_grid_new();
    gtk_widget_set_hexpand(grid, TRUE);
    gtk_widget_set_vexpand(grid, TRUE);
    gtk_widget_set_focusable(grid, TRUE);  // También para recibir teclado
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_box_append(GTK_BOX(contenedor), grid);

    char linea[COLS + 2];
    for (int i = 0; i < FILAS; i++) {
        if (!fgets(linea, sizeof(linea), archivo)) break;

        for (int j = 0; j < COLS; j++) {
            GtkWidget *widget = NULL;

            if (linea[j] == 'V') {
                widget = gtk_picture_new_for_filename("design/elementos/nave1.svg");
            } else if (linea[j] == 'X') {
                widget = gtk_picture_new_for_filename("design/elementos/nave2.svg");
            } else {
                widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            }

            gtk_widget_set_hexpand(widget, TRUE);
            gtk_widget_set_vexpand(widget, TRUE);
            gtk_widget_set_halign(widget, GTK_ALIGN_CENTER);
            gtk_widget_set_valign(widget, GTK_ALIGN_CENTER);

            imagenes[i][j] = widget;
            gtk_grid_attach(GTK_GRID(grid), widget, j, i, 1, 1);
        }
    }

    fclose(archivo);

    // Controlador de teclado sobre contenedor
    GtkEventController *key_controller = gtk_event_controller_key_new();
    gtk_widget_add_controller(contenedor, key_controller);
    g_signal_connect(key_controller, "key-pressed", G_CALLBACK(on_key_press), NULL);

    // Controlador de teclado sobre grid (extra robustez)
    GtkEventController *grid_keys = gtk_event_controller_key_new();
    gtk_widget_add_controller(grid, grid_keys);
    g_signal_connect(grid_keys, "key-pressed", G_CALLBACK(on_key_press), NULL);

    // Forzar foco
    gtk_widget_grab_focus(contenedor);

    // Confirmar foco
    g_print("¿Contenedor tiene foco? %s\n", gtk_widget_has_focus(contenedor) ? "Sí" : "No");

    g_timeout_add(100, actualizar_grid, NULL);

    g_idle_add_full(G_PRIORITY_DEFAULT_IDLE, (GSourceFunc)gtk_widget_grab_focus, contenedor, NULL);

    return contenedor;
}
