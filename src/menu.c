#include "menu.h"
#include "stackManager.h"
#include "partida.h"  

// Función para iniciar la partida
// static void irPartida(GtkButton *btn, gpointer user_data) {
//     GtkWidget *pantalla = crearPantallaPartida();
//     agregarPantalla("partida", pantalla);
//     cambiarPantalla(NULL, "partida");
//     g_timeout_add(50, safe_grab_focus, pantalla);
// }


// Función para crear botones de cambio de pantalla
static GtkWidget* crearBotonMenu(const char *rutaImagen, const char *pantallaDestino) {
    GtkWidget *boton = gtk_button_new();
    GtkWidget *imagen = gtk_picture_new_for_filename(rutaImagen);
    
    gtk_picture_set_content_fit(GTK_PICTURE(imagen), GTK_CONTENT_FIT_CONTAIN);
    gtk_widget_set_size_request(imagen, 300, 100);
    gtk_button_set_child(GTK_BUTTON(boton), imagen);
    gtk_widget_set_focusable(boton, FALSE);
    gtk_widget_add_css_class(boton, "flat");

    
    // if (strcmp(pantallaDestino, "iniciarPartida") == 0) {
    //     g_signal_connect(boton, "clicked", G_CALLBACK(irPartida), NULL);
    // } else {
    //     g_signal_connect(boton, "clicked", G_CALLBACK(cambiarPantalla), (gpointer)pantallaDestino);
    // }

    g_signal_connect(boton, "clicked", G_CALLBACK(cambiarPantalla), (gpointer)pantallaDestino);

    return boton;
}

// Función para crear el menú principal
GtkWidget* crearMenu(void) {
    GtkWidget *overlayMenu = gtk_overlay_new();
    gtk_widget_set_hexpand(overlayMenu, TRUE);
    gtk_widget_set_vexpand(overlayMenu, TRUE);

    GtkWidget *cajaMenu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_set_halign(cajaMenu, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(cajaMenu, GTK_ALIGN_CENTER);

    // Imagen del título
    GtkWidget *imagenTitulo = gtk_picture_new_for_filename("design/elementos/titulo.svg");
    gtk_widget_set_size_request(imagenTitulo, 600, 250);
    gtk_picture_set_content_fit(GTK_PICTURE(imagenTitulo), GTK_CONTENT_FIT_CONTAIN);
    gtk_widget_set_margin_top(imagenTitulo, 50);
    gtk_box_append(GTK_BOX(cajaMenu), imagenTitulo);

    // Caja de botones
    GtkWidget *cajaBotones = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(cajaBotones, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(cajaBotones, 70);

    // Otros botones
    gtk_box_append(GTK_BOX(cajaBotones), crearBotonMenu("design/elementos/iniciarPartida.svg", "pantallaNombre"));
    gtk_box_append(GTK_BOX(cajaBotones), crearBotonMenu("design/elementos/puntuaciones.svg", "puntuaciones"));
    gtk_box_append(GTK_BOX(cajaBotones), crearBotonMenu("design/elementos/opciones.svg", "opciones"));

    gtk_box_append(GTK_BOX(cajaMenu), cajaBotones);
    gtk_overlay_set_child(GTK_OVERLAY(overlayMenu), cajaMenu);

    // Botón de cerrar
    GtkWidget *botonCerrar = gtk_button_new();
    gtk_widget_set_size_request(botonCerrar, 50, 50);

    GdkPixbuf *pixbufCerrar = gdk_pixbuf_new_from_file_at_size("design/elementos/off.svg", 40, 40, NULL);
    GdkTexture *textureCerrar = gdk_texture_new_for_pixbuf(pixbufCerrar);
    GtkWidget *imagenCerrar = gtk_image_new_from_paintable(GDK_PAINTABLE(textureCerrar));
    gtk_button_set_child(GTK_BUTTON(botonCerrar), imagenCerrar);

    gtk_widget_set_halign(botonCerrar, GTK_ALIGN_END);
    gtk_widget_set_valign(botonCerrar, GTK_ALIGN_END);
    gtk_widget_set_margin_end(botonCerrar, 10);
    gtk_widget_set_margin_bottom(botonCerrar, 10);
    gtk_widget_set_focusable(botonCerrar, FALSE);
    gtk_widget_add_css_class(botonCerrar, "flat");

    g_signal_connect_swapped(botonCerrar, "clicked", G_CALLBACK(gtk_window_close), ventanaPrincipal);

    gtk_overlay_add_overlay(GTK_OVERLAY(overlayMenu), botonCerrar);

    g_object_unref(pixbufCerrar);
    g_object_unref(textureCerrar);

    return overlayMenu;
}
