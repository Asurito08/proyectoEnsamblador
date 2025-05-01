#include "puntuaciones.h"
#include "stackManager.h"
#include <stdio.h>

//se crea un struct Columnas para un manejo mas facil
typedef struct {
    GtkWidget *nombresBox;
    GtkWidget *puntosBox;
} Columnas;

//funcion para insertar las cajas donde van los nombres y los puntajes
static GtkWidget* crearColumnaPuntuacion(GtkWidget **contenedorInterno) {
    GtkWidget *overlay = gtk_overlay_new();
    gtk_widget_set_hexpand(overlay, TRUE);
    gtk_widget_set_vexpand(overlay, TRUE);
    gtk_widget_set_size_request(overlay, 500, 600);

    GtkWidget *fondo = gtk_picture_new_for_filename("design/elementos/caja.svg");
    gtk_picture_set_content_fit(GTK_PICTURE(fondo), GTK_CONTENT_FIT_CONTAIN);
    gtk_widget_set_hexpand(fondo, TRUE);
    gtk_widget_set_vexpand(fondo, TRUE);
    gtk_overlay_set_child(GTK_OVERLAY(overlay), fondo);

    *contenedorInterno = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_set_halign(*contenedorInterno, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(*contenedorInterno, GTK_ALIGN_CENTER);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), *contenedorInterno);

    return overlay;
}

//funcion para cargar nombres y puntuaciones
static void cargarPuntuaciones(GtkWidget *nombresBox, GtkWidget *puntosBox) {
    FILE *archivo = fopen("src/puntuaciones.txt", "r");
    if (!archivo) return;

    char nombre[100];
    int puntaje;

    while (fscanf(archivo, "%s %d", nombre, &puntaje) == 2) {
        GtkWidget *labelNombre = gtk_label_new(nombre);
        gtk_widget_add_css_class(labelNombre, "rojo");
        gtk_widget_set_hexpand(labelNombre, TRUE);
        gtk_widget_set_halign(labelNombre, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(labelNombre, GTK_ALIGN_CENTER);
        gtk_box_append(GTK_BOX(nombresBox), labelNombre);

        char textoPuntaje[20];
        sprintf(textoPuntaje, "%d", puntaje);
        GtkWidget *labelPuntaje = gtk_label_new(textoPuntaje);
        gtk_widget_add_css_class(labelPuntaje, "rojo");
        gtk_widget_set_hexpand(labelPuntaje, TRUE);
        gtk_widget_set_halign(labelPuntaje, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(labelPuntaje, GTK_ALIGN_CENTER);
        gtk_box_append(GTK_BOX(puntosBox), labelPuntaje);
    }

    fclose(archivo);
}

//funcion para limpiar el .txt con las puntuaciones y la pantalla, se llama al presionar el boton reset
static void limpiarPuntuaciones(GtkButton *boton, gpointer datosUsuario) {
    Columnas *c = (Columnas *)datosUsuario;

    // Limpiar archivo
    FILE *archivo = fopen("src/puntuaciones.txt", "w");
    if (archivo) fclose(archivo);

    // Eliminar widgets (labels)
    GtkWidget *child;
    while ((child = gtk_widget_get_first_child(c->nombresBox)) != NULL)
        gtk_widget_unparent(child);

    while ((child = gtk_widget_get_first_child(c->puntosBox)) != NULL)
        gtk_widget_unparent(child);

}

//crea la pantalla de puntuaciones con su contenido
GtkWidget* crearPantallaPuntuaciones(void) {
    GtkWidget *overlay = gtk_overlay_new();
    gtk_widget_set_hexpand(overlay, TRUE);
    gtk_widget_set_vexpand(overlay, TRUE);

    GtkWidget *contenedorCentral = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(contenedorCentral, 1000, -1);
    gtk_widget_set_halign(contenedorCentral, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(contenedorCentral, GTK_ALIGN_CENTER);

    GtkWidget *cajas = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_hexpand(cajas, TRUE);
    gtk_widget_set_vexpand(cajas, TRUE);

    GtkWidget *nombresBox;
    GtkWidget *puntosBox;

    GtkWidget *columnaIzquierda = crearColumnaPuntuacion(&nombresBox);
    GtkWidget *columnaDerecha   = crearColumnaPuntuacion(&puntosBox);

    gtk_box_append(GTK_BOX(cajas), columnaIzquierda);
    gtk_box_append(GTK_BOX(cajas), columnaDerecha);
    gtk_box_append(GTK_BOX(contenedorCentral), cajas);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), contenedorCentral);

    // Llenar columnas
    cargarPuntuaciones(nombresBox, puntosBox);

    // Crear estructura para pasar al botón reset
    Columnas *columnas = g_malloc(sizeof(Columnas));
    columnas->nombresBox = nombresBox;
    columnas->puntosBox = puntosBox;

    //se crea el boton de reset para borrar las puntuaciones
    GtkWidget *botonReset = gtk_button_new();
    gtk_widget_set_size_request(botonReset, 140, 140);

    GdkPixbuf *pixbufReset = gdk_pixbuf_new_from_file_at_size("design/elementos/reset.svg", 80, 80, NULL);
    GdkTexture *textureReset = gdk_texture_new_for_pixbuf(pixbufReset);
    GtkWidget *imagenReset = gtk_image_new_from_paintable(GDK_PAINTABLE(textureReset));
    gtk_button_set_child(GTK_BUTTON(botonReset), imagenReset);

    gtk_widget_set_halign(botonReset, GTK_ALIGN_START);
    gtk_widget_set_valign(botonReset, GTK_ALIGN_END);
    gtk_widget_set_margin_start(botonReset, 20);
    gtk_widget_set_margin_bottom(botonReset, 20);
    gtk_widget_add_css_class(botonReset, "flat");

    g_signal_connect(botonReset, "clicked", G_CALLBACK(limpiarPuntuaciones), columnas);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), botonReset);

    g_object_unref(pixbufReset);
    g_object_unref(textureReset);

    // Se crea el boton volver a la pantalla principal
    GtkWidget *botonVolver = gtk_button_new();
    gtk_widget_set_size_request(botonVolver, 60, 60);

    GdkPixbuf *pixbufVolver = gdk_pixbuf_new_from_file_at_size("design/elementos/back.svg", 40, 40, NULL);
    GdkTexture *textureVolver = gdk_texture_new_for_pixbuf(pixbufVolver);
    GtkWidget *imagenVolver = gtk_image_new_from_paintable(GDK_PAINTABLE(textureVolver));
    gtk_button_set_child(GTK_BUTTON(botonVolver), imagenVolver);

    gtk_widget_set_halign(botonVolver, GTK_ALIGN_END);
    gtk_widget_set_valign(botonVolver, GTK_ALIGN_END);
    gtk_widget_set_margin_end(botonVolver, 20);
    gtk_widget_set_margin_bottom(botonVolver, 20);
    gtk_widget_add_css_class(botonVolver, "flat");

    g_signal_connect(botonVolver, "clicked", G_CALLBACK(cambiarPantalla), (gpointer)"menu");
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), botonVolver);

    g_object_unref(pixbufVolver);
    g_object_unref(textureVolver);

    return overlay;
}
