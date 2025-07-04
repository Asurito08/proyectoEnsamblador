#include "pausa.h"
#include "stackManager.h"
#include "partida.h"

extern int  marcador_timer_id;
extern int  partida_timer_id;
extern int  vidas;
extern int marcador;
extern GtkGrid *grid_dibujo;
#define FILAS 12
#define COLS 15
#define ROWLEN (COLS + 1)
extern GtkWidget *imagenes[FILAS][COLS];
extern char matriz[FILAS * ROWLEN];
extern GtkLabel *label_marcador;
extern GtkWidget *widget_vidas;

// Callback para continuar la partida
static void continuarPartida(GtkButton *btn, gpointer user_data) {
    cambiarPantalla(NULL, "partida");
}

// Callback para reiniciar la partida
static void reiniciarPartida(GtkButton *btn, gpointer user_data) {
    marcador = 0;
    vidas = 5;
    if (marcador_timer_id != 0) {
        g_source_remove(marcador_timer_id);
        marcador_timer_id = 0;
    }
    if (marcador_timer_id != 0) {
        g_source_remove(marcador_timer_id);
        marcador_timer_id = 0;
    }
    GtkWidget *pantalla = crearPantallaPartida();
    agregarPantalla("partida", pantalla);
    cambiarPantalla(NULL, "partida");
    g_timeout_add(50, safe_grab_focus, pantalla);
}

// Callback para salir al menú principal
void salirAlMenu(GtkButton *btn, gpointer user_data) {
GtkWidget *pantalla = gtk_stack_get_child_by_name(GTK_STACK(stack), "partida");    if (pantalla) {
    if (marcador_timer_id != 0) {
        g_source_remove(marcador_timer_id);
        marcador_timer_id = 0;
    }
    if (partida_timer_id != 0) {
        g_source_remove(partida_timer_id);
        partida_timer_id = 0;
    }
    if (pantalla){
    gtk_stack_remove(GTK_STACK(stack), pantalla);
    grid_dibujo = NULL;
    label_marcador = NULL;
    widget_vidas = NULL;
    memset(matriz, 0, sizeof(matriz));
    memset(imagenes, 0, sizeof(imagenes));
    }
    }
    cambiarPantalla(NULL, "menu");
}

GtkWidget* crearPantallaPausa(void) {
    GtkWidget *overlayPausa = gtk_overlay_new();
    gtk_widget_set_hexpand(overlayPausa, TRUE);
    gtk_widget_set_vexpand(overlayPausa, TRUE);

    GtkWidget *cajaPausa = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_set_halign(cajaPausa, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(cajaPausa, GTK_ALIGN_CENTER);

    // Título de pausa (opcional)
    GtkWidget *titulo = gtk_label_new("PAUSA");
    gtk_widget_add_css_class(titulo, "titulo-pausa");
    gtk_box_append(GTK_BOX(cajaPausa), titulo);

    // Caja de botones
    GtkWidget *cajaBotones = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(cajaBotones, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(cajaBotones, 40);

    // Botón Continuar
    GtkWidget *btnContinuar = gtk_button_new();
    GtkWidget *imgContinuar = gtk_picture_new_for_filename("design/elementos/continuar.svg");
    gtk_picture_set_content_fit(GTK_PICTURE(imgContinuar), GTK_CONTENT_FIT_CONTAIN);
    gtk_widget_set_size_request(imgContinuar, 300, 100);
    gtk_button_set_child(GTK_BUTTON(btnContinuar), imgContinuar);
    gtk_widget_add_css_class(btnContinuar, "flat");
    g_signal_connect(btnContinuar, "clicked", G_CALLBACK(continuarPartida), NULL);

    // Botón Reiniciar
    GtkWidget *btnReiniciar = gtk_button_new();
    GtkWidget *imgReiniciar = gtk_picture_new_for_filename("design/elementos/reiniciar.svg");
    gtk_picture_set_content_fit(GTK_PICTURE(imgReiniciar), GTK_CONTENT_FIT_CONTAIN);
    gtk_widget_set_size_request(imgReiniciar, 300, 100);
    gtk_button_set_child(GTK_BUTTON(btnReiniciar), imgReiniciar);
    gtk_widget_add_css_class(btnReiniciar, "flat");
    g_signal_connect(btnReiniciar, "clicked", G_CALLBACK(reiniciarPartida), NULL);

    // Botón Salir
    GtkWidget *btnSalir = gtk_button_new();
    GtkWidget *imgSalir = gtk_picture_new_for_filename("design/elementos/salir.svg");
    gtk_picture_set_content_fit(GTK_PICTURE(imgSalir), GTK_CONTENT_FIT_CONTAIN);
    gtk_widget_set_size_request(imgSalir, 300, 100);
    gtk_button_set_child(GTK_BUTTON(btnSalir), imgSalir);
    gtk_widget_add_css_class(btnSalir, "flat");
    g_signal_connect(btnSalir, "clicked", G_CALLBACK(salirAlMenu), NULL);

    // Añadir botones a la caja
    gtk_box_append(GTK_BOX(cajaBotones), btnContinuar);
    gtk_box_append(GTK_BOX(cajaBotones), btnReiniciar);
    gtk_box_append(GTK_BOX(cajaBotones), btnSalir);

    gtk_box_append(GTK_BOX(cajaPausa), cajaBotones);
    gtk_overlay_set_child(GTK_OVERLAY(overlayPausa), cajaPausa);

    return overlayPausa;
}