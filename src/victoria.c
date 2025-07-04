#include "victoria.h"

extern GtkGrid *grid_dibujo;
#define FILAS 12
#define COLS 15
#define ROWLEN (COLS + 1)
extern GtkWidget *imagenes[FILAS][COLS];
extern char matriz[FILAS * ROWLEN];
extern GtkLabel *label_marcador;
extern GtkWidget *widget_vidas;
extern int marcador;
extern int marcador_timer_id;
extern int partida_timer_id;
extern void cambiarPantalla(const char *pantalla_actual, const char *pantalla_nueva);
extern void salirAlMenu(GtkButton *btn, gpointer user_data);

static void aceptarVictoria(GtkButton *btn, gpointer user_data) {

    grid_dibujo = NULL;
    label_marcador = NULL;
    widget_vidas = NULL;
    memset(matriz, 0, sizeof(matriz));
    memset(imagenes, 0, sizeof(imagenes));
    marcador = 0;

    // Cambia al menú
    cambiarPantalla(NULL, "menu");
}

GtkWidget* crearPantallaVictoria(void) {
    GtkWidget *overlay = gtk_overlay_new();
    gtk_widget_set_hexpand(overlay, TRUE);
    gtk_widget_set_vexpand(overlay, TRUE);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 18);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(box, 170);  // Aplica a toda la pantalla


    // Caja horizontal para trofeo + felicitaciones + trofeo
    GtkWidget *hbox_felicidades = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_halign(hbox_felicidades, GTK_ALIGN_CENTER);

    GtkWidget *img_trofeo_izq = gtk_picture_new_for_filename("design/elementos/trofeo.png");
    gtk_widget_set_size_request(img_trofeo_izq, 60, 60);
    gtk_box_append(GTK_BOX(hbox_felicidades), img_trofeo_izq);

    GtkWidget *label_felicidades = gtk_label_new("¡FELICITACIONES!");
    gtk_widget_add_css_class(label_felicidades, "felicidades");
    gtk_box_append(GTK_BOX(hbox_felicidades), label_felicidades);

    GtkWidget *img_trofeo_der = gtk_picture_new_for_filename("design/elementos/trofeo.png");
    gtk_widget_set_size_request(img_trofeo_der, 60, 60);
    gtk_box_append(GTK_BOX(hbox_felicidades), img_trofeo_der);

    gtk_box_append(GTK_BOX(box), hbox_felicidades);

    GtkWidget *label_mensaje = gtk_label_new("Has derrotado a la flota\ninvasora alienígena");
    gtk_widget_add_css_class(label_mensaje, "texto_sin_borde");
    gtk_widget_set_margin_bottom(label_mensaje, 16);
    gtk_label_set_justify(GTK_LABEL(label_mensaje), GTK_JUSTIFY_CENTER);
    gtk_label_set_xalign(GTK_LABEL(label_mensaje), 0.5f);
    gtk_box_append(GTK_BOX(box), label_mensaje);

    GtkWidget *label_puntaje = gtk_label_new("PUNTAJE FINAL");
    gtk_widget_add_css_class(label_puntaje, "texto_sin_borde");
    gtk_widget_set_margin_bottom(label_puntaje, 8);
    gtk_label_set_justify(GTK_LABEL(label_puntaje), GTK_JUSTIFY_CENTER);
    gtk_label_set_xalign(GTK_LABEL(label_puntaje), 0.5f);
    gtk_box_append(GTK_BOX(box), label_puntaje);

    char texto_marcador[64];
    snprintf(texto_marcador, sizeof(texto_marcador), "<span foreground='#e03b8a' font='32'>%d</span> pts", marcador);
    GtkWidget *label_marcador = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label_marcador), texto_marcador);
    gtk_widget_add_css_class(label_marcador, "texto_sin_borde");
    gtk_widget_set_margin_bottom(label_marcador, 18);
    gtk_label_set_justify(GTK_LABEL(label_marcador), GTK_JUSTIFY_CENTER);
    gtk_label_set_xalign(GTK_LABEL(label_marcador), 0.5f);
    gtk_box_append(GTK_BOX(box), label_marcador);

    GtkWidget *btnSalir = gtk_button_new();
    gtk_widget_set_hexpand(btnSalir, FALSE);
    gtk_widget_set_vexpand(btnSalir, FALSE);
    gtk_widget_set_halign(btnSalir, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(btnSalir, GTK_ALIGN_CENTER);

    GtkWidget *imgSalir = gtk_picture_new_for_filename("design/elementos/salir.svg");
    gtk_picture_set_content_fit(GTK_PICTURE(imgSalir), GTK_CONTENT_FIT_CONTAIN);
    gtk_widget_set_size_request(imgSalir, 300, 100);
    gtk_widget_set_hexpand(imgSalir, FALSE);
    gtk_widget_set_vexpand(imgSalir, FALSE);

    gtk_button_set_child(GTK_BUTTON(btnSalir), imgSalir);
    gtk_widget_add_css_class(btnSalir, "flat");

    g_signal_connect(btnSalir, "clicked", G_CALLBACK(salirAlMenu), NULL);
    gtk_box_append(GTK_BOX(box), btnSalir);


    // g_signal_connect(btn_menu, "clicked", G_CALLBACK(aceptarVictoria), NULL);

    gtk_overlay_set_child(GTK_OVERLAY(overlay), box);

    return overlay;
}