#include <gtk/gtk.h>
#include "stackManager.h"
#include "menu.h"
#include "opciones.h"


static void activate(GtkApplication *app, gpointer datosUsuario) {
    // Crear ventana principal
    ventanaPrincipal = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(ventanaPrincipal), "Pantalla de Prueba");
    gtk_window_set_default_size(GTK_WINDOW(ventanaPrincipal), 1200, 900);

    // Crear overlay y fondo
    GtkWidget *overlay = gtk_overlay_new();
    gtk_window_set_child(GTK_WINDOW(ventanaPrincipal), overlay);

    GtkWidget *fondo = gtk_picture_new_for_filename("design/elementos/fondo3.jpeg");
    gtk_widget_set_hexpand(fondo, TRUE);
    gtk_widget_set_vexpand(fondo, TRUE);
    gtk_picture_set_content_fit(GTK_PICTURE(fondo), GTK_CONTENT_FIT_COVER);
    gtk_overlay_set_child(GTK_OVERLAY(overlay), fondo);

    // Crear stack (pila de pantallas)
    stack = gtk_stack_new(); // <<< Aquí directamente stack
    gtk_widget_set_hexpand(stack, TRUE);
    gtk_widget_set_vexpand(stack, TRUE);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), stack);

    // Agregar pantallas al stack
    gtk_stack_add_named(GTK_STACK(stack), crearMenu(), "menu");
    gtk_stack_add_named(GTK_STACK(stack), crearPantallaOpciones(), "opciones");

    // Mostrar ventana
    gtk_window_present(GTK_WINDOW(ventanaPrincipal));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.proyecto.opciones", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int estado = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return estado;
}
