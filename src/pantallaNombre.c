#include <gtk/gtk.h>
#include "pantallaNombre.h"
#include "stackManager.h"

static void irPartida(GtkButton *btn, gpointer user_data) {
    GtkWidget *pantalla = crearPantallaPartida();
    agregarPantalla("partida", pantalla);
    cambiarPantalla(NULL, "partida");
    g_timeout_add(50, safe_grab_focus, pantalla);
}

char nombreJugador[100] = "";

void guardarNombreJugador(GtkButton *boton, gpointer user_data) {
    GtkEntry *entrada = GTK_ENTRY(user_data);
    const char *texto = gtk_editable_get_text(GTK_EDITABLE(entrada));
    g_strlcpy(nombreJugador, texto, sizeof(nombreJugador));
    irPartida(NULL, "partida");
}

GtkWidget* crearPantallaNombreJugador(void) {
    // Crear overlay
    GtkWidget *overlay = gtk_overlay_new();

    // Crear imagen base
    GtkWidget *imagenCaja = gtk_image_new_from_file("design/elementos/caja.svg");
    gtk_widget_set_size_request(imagenCaja, 100, 50);
    gtk_overlay_set_child(GTK_OVERLAY(overlay), imagenCaja);

    // Crear caja vertical para entrada y botón (los widgets encima)
    GtkWidget *caja = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
    // Centrar la caja en el overlay
    gtk_widget_set_halign(caja, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(caja, GTK_ALIGN_CENTER);

    // Entrada de texto - más grande
    GtkWidget *entrada = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entrada), "Ingresa tu nombre...");
    gtk_widget_set_size_request(entrada, 400, 50);  // ancho 400 px, alto 50 px
    gtk_box_append(GTK_BOX(caja), entrada);

    // Botón aceptar - más grande
    GtkWidget *botonAceptar = gtk_button_new_with_label("Aceptar");
    gtk_widget_set_size_request(botonAceptar, 200, 60);  // ancho 200 px, alto 60 px
    gtk_box_append(GTK_BOX(caja), botonAceptar);

    g_signal_connect(botonAceptar, "clicked", G_CALLBACK(guardarNombreJugador), entrada);

    // Añadir la caja al overlay como un widget superpuesto
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), caja);

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