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

int marcador = 0;
GtkLabel *label_marcador = NULL;

void cargar_matriz(const char *filename, char *matriz) {
    memset(matriz, 0, FILAS * ROWLEN);
    FILE *archivo = fopen(filename, "r");
    if (!archivo) {
        perror("Error al abrir el archivo");
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
        g_warning("grid_dibujo no está inicializado");
        return;
    }

    g_print("Matriz pintada:\n");
    for (int k = 0; k < FILAS; k++) {
        fwrite(matriz + k * ROWLEN, 1, COLS, stdout);
        putchar('\n');
    }

    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLS; j++) {
            char c = matriz[i * ROWLEN + j];
            const char *ruta = NULL;

            switch (c) {
                case 'V': ruta = "design/elementos/nave1.svg"; break;
                case 'X': ruta = "design/elementos/nave2.svg"; break;
                case '^': ruta = "design/elementos/bala.svg"; break;
                case '0': ruta = "design/elementos/vacio.svg"; break;
                default:
                    g_warning("Carácter inválido: '%c' (ASCII %d) en [%d][%d]", c, c, i, j);
                    ruta = NULL;
                    break;
            }

            if (imagenes[i][j] == NULL) {
                g_critical("ERROR: imagenes[%d][%d] ES NULL", i, j);
                continue;
            }

            if (!GTK_IS_PICTURE(imagenes[i][j])) {
                g_critical("ERROR: imagenes[%d][%d] NO es GtkPicture", i, j);
                continue;
            }

            gtk_picture_set_filename(GTK_PICTURE(imagenes[i][j]), ruta);
        }
    }

    gtk_widget_queue_draw(GTK_WIDGET(grid_dibujo));
}

gboolean capturar_tecla(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data) {
    gunichar unicode = gdk_keyval_to_unicode(keyval);

    if (unicode != 0 && unicode <= 127) {
        char tecla = (char)unicode;
        g_print("Tecla presionada: %c\n", tecla);
        tecla_buffer = tecla;
    }

    return TRUE;
}

gboolean loop_juego(gpointer user_data) {
    if (tecla_buffer != 0) {
        char tecla = (char)tecla_buffer;
        tecla_buffer = 0;

        if (tecla == 'x') {
            g_print("Juego finalizado por tecla 'x'\n");
            gtk_window_close(GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(grid_dibujo))));
            return FALSE;
        }

        if (strchr("wasdpk", tecla)) {
            int r = iterar_matriz(matriz, tecla);
            if (r == -1 ) {
                g_print("Juego finalizado por iterar_matriz == -1\n");
                gtk_window_close(GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(grid_dibujo))));
                return FALSE;
            }

            pintar_matriz();
        }
    }

    return TRUE;
}

gboolean actualizar_marcador(gpointer user_data) {
    marcador += 5;
    char texto[32];
    snprintf(texto, sizeof(texto), "Marcador: %d", marcador);
    gtk_label_set_text(label_marcador, texto);
    return TRUE;
}

GtkWidget* crearPantallaPartida(void) {
    cargar_matriz(RUTA_ARCHIVO, matriz);
    memset(imagenes, 0, sizeof(imagenes));

    // Crear grid de juego
    GtkWidget *grid_principal = gtk_grid_new();
    grid_dibujo = GTK_GRID(grid_principal);

    gtk_widget_set_hexpand(grid_principal, TRUE);
    gtk_widget_set_vexpand(grid_principal, TRUE);
    gtk_grid_set_row_homogeneous(grid_dibujo, TRUE);
    gtk_grid_set_column_homogeneous(grid_dibujo, TRUE);

    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLS; j++) {
            GtkWidget *widget = gtk_picture_new();
            gtk_widget_set_hexpand(widget, TRUE);
            gtk_widget_set_vexpand(widget, TRUE);
            gtk_grid_attach(GTK_GRID(grid_principal), widget, j, i, 1, 1);
            imagenes[i][j] = widget;
        }
    }

    // Crear el marcador
    label_marcador = GTK_LABEL(gtk_label_new("Marcador: 0"));
    gtk_widget_add_css_class(GTK_WIDGET(label_marcador), "marcador");

    // Alinear abajo a la izquierda
    gtk_widget_set_valign(GTK_WIDGET(label_marcador), GTK_ALIGN_END);
    gtk_widget_set_halign(GTK_WIDGET(label_marcador), GTK_ALIGN_START);
    gtk_widget_set_margin_start(GTK_WIDGET(label_marcador), 10);
    gtk_widget_set_margin_bottom(GTK_WIDGET(label_marcador), 10);

    // Crear overlay para superponer marcador sobre grid
    GtkWidget *overlay = gtk_overlay_new();
    gtk_overlay_set_child(GTK_OVERLAY(overlay), grid_principal);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), GTK_WIDGET(label_marcador));

    // Controlador de teclado
    GtkEventController *teclado = gtk_event_controller_key_new();
    gtk_event_controller_set_propagation_phase(teclado, GTK_PHASE_CAPTURE);
    gtk_widget_add_controller(overlay, teclado);
    g_signal_connect(teclado, "key-pressed", G_CALLBACK(capturar_tecla), NULL);

    // Asegurarse de que el overlay reciba el foco
    gtk_widget_set_focusable(overlay, TRUE);
    gtk_widget_grab_focus(overlay);
    // Timers
    g_timeout_add(50, loop_juego, NULL);
    g_timeout_add_seconds(3, actualizar_marcador, NULL);

    pintar_matriz();

    return overlay;
}
