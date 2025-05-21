#include "puntuaciones.h"
#include "stackManager.h"
#include <stdio.h>

typedef struct {
    GtkWidget *nombresBox;
    GtkWidget *puntosBox;
    GtkWidget *overlay;
    GtkWidget *botonReset;
} WidgetsPuntuaciones;

static void mostrarMensajeSinPuntajes(GtkWidget *overlay, GtkWidget *botonReset) {
    GtkWidget *mensaje = gtk_label_new("No hay puntuaciones registradas");
    gtk_widget_add_css_class(mensaje, "formato");
    gtk_widget_set_halign(mensaje, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(mensaje, GTK_ALIGN_CENTER);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), mensaje);
    gtk_widget_set_visible(botonReset, FALSE);
}

static void cargarPuntuaciones(GtkWidget *nombresBox, GtkWidget *puntosBox, GtkWidget *overlay, GtkWidget *botonReset) {
    FILE *archivo = fopen("saves/puntuaciones.txt", "r");
    if (!archivo) return;

    char nombre[100];
    int puntaje;
    int posicion = 1;
    int hayPuntajes = 0;

    while (fscanf(archivo, "%s %d", nombre, &puntaje) == 2) {
        hayPuntajes = 1;

        char nombreFormateado[150];
        snprintf(nombreFormateado, sizeof(nombreFormateado), "%2d. %s", posicion, nombre);

        GtkWidget *labelNombre = gtk_label_new(nombreFormateado);
        gtk_widget_add_css_class(labelNombre, "formato");
        gtk_widget_set_halign(labelNombre, GTK_ALIGN_CENTER);
        gtk_box_append(GTK_BOX(nombresBox), labelNombre);

        char textoPuntaje[20];
        sprintf(textoPuntaje, "%d", puntaje);
        GtkWidget *labelPuntaje = gtk_label_new(textoPuntaje);
        gtk_widget_add_css_class(labelPuntaje, "formato");
        gtk_widget_set_halign(labelPuntaje, GTK_ALIGN_CENTER);
        gtk_box_append(GTK_BOX(puntosBox), labelPuntaje);

        posicion++;
    }

    fclose(archivo);

    if (!hayPuntajes) {
        mostrarMensajeSinPuntajes(overlay, botonReset);
    }
}

static void limpiarPuntuaciones(GtkButton *boton, gpointer datosUsuario) {
    WidgetsPuntuaciones *w = (WidgetsPuntuaciones *)datosUsuario;

    FILE *archivo = fopen("saves/puntuaciones.txt", "w");
    if (archivo) fclose(archivo);

    GtkWidget *child;
    while ((child = gtk_widget_get_first_child(w->nombresBox)) != NULL)
        gtk_widget_unparent(child);
    while ((child = gtk_widget_get_first_child(w->puntosBox)) != NULL)
        gtk_widget_unparent(child);
    
    mostrarMensajeSinPuntajes(w->overlay, w->botonReset);
}

GtkWidget* crearPantallaPuntuaciones(void) {
    GtkWidget *overlay = gtk_overlay_new();
    gtk_widget_set_hexpand(overlay, TRUE);
    gtk_widget_set_vexpand(overlay, TRUE);

    // Fondo de caja visual
    GtkWidget *caja = gtk_picture_new_for_filename("design/elementos/caja.svg");
    gtk_widget_set_size_request(caja, 800, 600);
    gtk_widget_set_halign(caja, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(caja, GTK_ALIGN_CENTER);
    gtk_picture_set_content_fit(GTK_PICTURE(caja), GTK_CONTENT_FIT_CONTAIN);
    gtk_overlay_set_child(GTK_OVERLAY(overlay), caja);

    // Contenedor principal vertical (título + columnas)
    GtkWidget *contenedorVertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(contenedorVertical, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(contenedorVertical, GTK_ALIGN_CENTER);

    // Título centrado
    GtkWidget *titulo = gtk_label_new("MEJORES PUNTUACIONES");
    gtk_widget_set_margin_bottom(titulo, 40);  
    gtk_widget_add_css_class(titulo, "formato"); 
    gtk_widget_set_halign(titulo, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(contenedorVertical), titulo);

    // Contenedor horizontal para columnas
    GtkWidget *contenedor = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 200);
    gtk_widget_set_halign(contenedor, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(contenedor, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(contenedor, -1, 400);

    GtkWidget *nombresBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    GtkWidget *puntosBox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);

    gtk_box_append(GTK_BOX(contenedor), nombresBox);
    gtk_box_append(GTK_BOX(contenedor), puntosBox);
    gtk_box_append(GTK_BOX(contenedorVertical), contenedor);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), contenedorVertical);

    // Botón Reset
    GtkWidget *botonReset = gtk_button_new();
    gtk_widget_set_size_request(botonReset, 140, 140);
    GdkPixbuf *pixbufReset = gdk_pixbuf_new_from_file_at_size("design/elementos/reset.svg", 80, 80, NULL);
    GdkTexture *textureReset = gdk_texture_new_for_pixbuf(pixbufReset);
    GtkWidget *imagenReset = gtk_image_new_from_paintable(GDK_PAINTABLE(textureReset));
    gtk_button_set_child(GTK_BUTTON(botonReset), imagenReset);
    gtk_widget_add_css_class(botonReset, "flat");
    gtk_widget_set_halign(botonReset, GTK_ALIGN_START);
    gtk_widget_set_valign(botonReset, GTK_ALIGN_END);
    gtk_widget_set_margin_start(botonReset, 20);
    gtk_widget_set_margin_bottom(botonReset, 20);
    WidgetsPuntuaciones *widgets = g_malloc(sizeof(WidgetsPuntuaciones));
    widgets->nombresBox = nombresBox;
    widgets->puntosBox = puntosBox;
    widgets->overlay = overlay;
    widgets->botonReset = botonReset;
    g_signal_connect(botonReset, "clicked", G_CALLBACK(limpiarPuntuaciones), widgets);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), botonReset);
    g_object_unref(pixbufReset);
    g_object_unref(textureReset);

    cargarPuntuaciones(nombresBox, puntosBox, overlay, botonReset);

    // Botón Volver
    GtkWidget *botonVolver = gtk_button_new();
    gtk_widget_set_size_request(botonVolver, 60, 60);
    GdkPixbuf *pixbufVolver = gdk_pixbuf_new_from_file_at_size("design/elementos/back.svg", 40, 40, NULL);
    GdkTexture *textureVolver = gdk_texture_new_for_pixbuf(pixbufVolver);
    GtkWidget *imagenVolver = gtk_image_new_from_paintable(GDK_PAINTABLE(textureVolver));
    gtk_button_set_child(GTK_BUTTON(botonVolver), imagenVolver);
    gtk_widget_add_css_class(botonVolver, "flat");
    gtk_widget_set_halign(botonVolver, GTK_ALIGN_END);
    gtk_widget_set_valign(botonVolver, GTK_ALIGN_END);
    gtk_widget_set_margin_end(botonVolver, 20);
    gtk_widget_set_margin_bottom(botonVolver, 20);
    g_signal_connect(botonVolver, "clicked", G_CALLBACK(cambiarPantalla), (gpointer)"menu");
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), botonVolver);
    g_object_unref(pixbufVolver);
    g_object_unref(textureVolver);

    return overlay;
}
