#include "partida.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FILAS 12
#define COLS 15
#define ROWLEN (COLS + 1)
#define RUTA_ARCHIVO "saves/nivel.txt"

int vidas = 5;
int marcador_timer_id = 0;
int partida_timer_id = 0;
GtkGrid *grid_dibujo = NULL;
GtkWidget *imagenes[FILAS][COLS];
char matriz[FILAS * ROWLEN];
gint tecla_buffer = 0;
int marcador = 0;
GtkLabel *label_marcador = NULL;
GtkWidget *widget_vidas = NULL;

extern int iterar_matriz(char *matriz, char tecla);

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
                case 'E': ruta = "design/elementos/nave3.svg"; break;
                case '^': ruta = "design/elementos/bala.svg"; break;
                case '0': ruta = "design/elementos/vacio.svg"; break;
                default:
                    printf("Carácter inválido: '%c' (ASCII %d) en [%d][%d]", c, c, i, j);
                    ruta = NULL;
                    break;
            }

            if (imagenes[i][j] == NULL || !GTK_IS_PICTURE(imagenes[i][j])) {
                g_critical("ERROR: imagenes[%d][%d] no es GtkPicture válido", i, j);
                continue;
            }
            printf("Pintando [%d][%d] con ruta: %s\n", i, j, ruta ? ruta : "NULL");
            if (ruta != NULL) {
                gtk_picture_set_filename(GTK_PICTURE(imagenes[i][j]), ruta);
            } else {
                g_warning("ruta nula en [%d][%d] con char '%c'", i, j, c);
            }

        }
    }
    printf("Pintando matriz en grid_dibujo\n");
     printf("Verificando punteros de imagenes[][] antes del queue_draw...\n");
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLS; j++) {
            GtkWidget *widget = imagenes[i][j];
            if (!GTK_IS_PICTURE(widget)) {
                printf("⚠️  Puntero dañado en imagenes[%d][%d]: %p\n", i, j, (void *)widget);
            }
        }
    }

    printf("Verificando puntero de grid_dibujo...\n");
    if (!GTK_IS_GRID(grid_dibujo)) {
        printf("❌ grid_dibujo está dañado: %p\n", (void *)grid_dibujo);
    }

    gtk_widget_queue_draw(GTK_WIDGET(grid_dibujo));
    printf("Matriz pintada correctamente\n");
}

void mostrar_marcador() {
    if (label_marcador) {
        char texto[32];
        snprintf(texto, sizeof(texto), "Marcador: %d", marcador);
        gtk_label_set_text(label_marcador, texto);
    }
}

GtkWidget* crear_widget_vidas(int vidas) {
    GtkWidget *caja_vidas = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    for (int i = 0; i < vidas; i++) {
        GtkWidget *corazon = gtk_picture_new_for_filename("design/elementos/corazon.png");
        gtk_widget_set_size_request(corazon, 32, 32);
        gtk_box_append(GTK_BOX(caja_vidas), corazon);
    }

    return caja_vidas;
}

void actualizar_widget_vidas(int vidas) {
    if (!widget_vidas) return;

    GtkWidget *child = gtk_widget_get_first_child(widget_vidas);
    while (child) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_widget_unparent(child);
        child = next;
    }

    for (int i = 0; i < vidas; i++) {
        GtkWidget *corazon = gtk_picture_new_for_filename("design/elementos/corazon.png");
        gtk_widget_set_size_request(corazon, 32, 32);
        gtk_box_append(GTK_BOX(widget_vidas), corazon);
    }
}

void mostrar_pausa(GtkWindow *parent) {
    GtkWidget *pantalla_pausa = crearPantallaPausa();
    agregarPantalla("pausa", pantalla_pausa);
    cambiarPantalla(NULL, "pausa");
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

gboolean safe_grab_focus(gpointer widget_ptr) {
    GtkWidget *widget = GTK_WIDGET(widget_ptr);
    if (GTK_IS_WIDGET(widget) && gtk_widget_get_root(widget)) {
        gtk_widget_grab_focus(widget);
    }
    return G_SOURCE_REMOVE;
}

gboolean loop_juego(gpointer user_data) {
    if (tecla_buffer != 0) {
        char tecla = (char)tecla_buffer;
        tecla_buffer = 0;

        if (tecla == 'q') {
            GtkWindow *parent = GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(grid_dibujo)));
            mostrar_pausa(parent);
            return TRUE;
        }

            int resultado;
        if (strchr("wasdpk", tecla)) {
            
            resultado = iterar_matriz(matriz, tecla);
            g_print("Resultado ASM: %d\n", resultado);

            switch (resultado) {
                case 0:
                    break;
                case 1:
                    vidas -= 1;
                    actualizar_widget_vidas(vidas);
                    break;
                case 2:
                    marcador += 3;
                    mostrar_marcador();
                    break;
                case 3:
                    g_print("Ganaste, ya no quedan enemigos\n");
                    //gtk_window_close(GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(grid_dibujo))));
                    return FALSE;
                case -1:
                    g_print("Movimiento inválido o sin cambio\n");
                    break;
                default:
                    break;
            }
            printf("hola");
            pintar_matriz();
            printf("adios");
        }
    }
    return TRUE;
}

gboolean actualizar_marcador(gpointer user_data) {
    marcador += 5;
    mostrar_marcador();
    return TRUE;
}

GtkWidget* crearPantallaPartida(void) {
    cargar_matriz(RUTA_ARCHIVO, matriz);
    memset(imagenes, 0, sizeof(imagenes));

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

    // Crear marcador
    label_marcador = GTK_LABEL(gtk_label_new("Marcador: 0"));
    gtk_widget_add_css_class(GTK_WIDGET(label_marcador), "texto_sin_borde");

    // Crear widget de vidas
    widget_vidas = crear_widget_vidas(vidas);

    // Crear caja horizontal (marcador a la izquierda, vidas a la derecha)
    GtkWidget *hud_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_widget_set_halign(hud_box, GTK_ALIGN_FILL);
    gtk_widget_set_valign(hud_box, GTK_ALIGN_END);
    gtk_widget_set_margin_bottom(hud_box, 10);
    gtk_widget_set_margin_start(hud_box, 10);
    gtk_widget_set_margin_end(hud_box, 10);
    gtk_widget_set_hexpand(hud_box, TRUE);

    gtk_box_append(GTK_BOX(hud_box), GTK_WIDGET(label_marcador));
    gtk_widget_set_hexpand(GTK_WIDGET(label_marcador), TRUE);
    gtk_widget_set_halign(GTK_WIDGET(label_marcador), GTK_ALIGN_START);

    gtk_box_append(GTK_BOX(hud_box), widget_vidas);
    gtk_widget_set_hexpand(widget_vidas, TRUE);
    gtk_widget_set_halign(widget_vidas, GTK_ALIGN_END);

    // Crear overlay y agregar grid + HUD
    GtkWidget *overlay = gtk_overlay_new();
    gtk_overlay_set_child(GTK_OVERLAY(overlay), grid_principal);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), hud_box);

    // Teclado
    GtkEventController *teclado = gtk_event_controller_key_new();
    gtk_event_controller_set_propagation_phase(teclado, GTK_PHASE_CAPTURE);
    gtk_widget_add_controller(overlay, teclado);
    g_signal_connect(teclado, "key-pressed", G_CALLBACK(capturar_tecla), NULL);

    gtk_widget_set_focusable(overlay, TRUE);

    if (partida_timer_id != 0) g_source_remove(partida_timer_id);
    if (marcador_timer_id != 0) g_source_remove(marcador_timer_id);
    partida_timer_id = g_timeout_add(50, loop_juego, NULL);
    marcador_timer_id = g_timeout_add_seconds(3, actualizar_marcador, NULL);

    pintar_matriz();

    return overlay;
}
