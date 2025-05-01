#include <gtk/gtk.h>
#include "stackManager.h"
#include "menu.h"
#include "opciones.h"
#include "puntuaciones.h"

GtkCssProvider *cssProvider = NULL;

static void activate(GtkApplication *aplicacion, gpointer datosUsuario) {
    // Crear ventana principal
    ventanaPrincipal = gtk_application_window_new(aplicacion);
    gtk_window_set_title(GTK_WINDOW(ventanaPrincipal), "Pantalla de Prueba");
    gtk_window_set_default_size(GTK_WINDOW(ventanaPrincipal), 1200, 900);

    // Cargar css con estilo para nombres y puntuaciones
    cssProvider = gtk_css_provider_new();
    GFile *file = g_file_new_for_path("src/style.css");
    gtk_css_provider_load_from_file(cssProvider, file);
    g_object_unref(file);

    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(cssProvider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    // Crear overlay y fondo
    GtkWidget *overlay = gtk_overlay_new();
    gtk_window_set_child(GTK_WINDOW(ventanaPrincipal), overlay);

    GtkWidget *fondo = gtk_picture_new_for_filename("design/elementos/fondo3.jpeg");
    gtk_widget_set_hexpand(fondo, TRUE);
    gtk_widget_set_vexpand(fondo, TRUE);
    gtk_picture_set_content_fit(GTK_PICTURE(fondo), GTK_CONTENT_FIT_COVER);
    gtk_overlay_set_child(GTK_OVERLAY(overlay), fondo);

    // Crear stack de pantallas
    stack = gtk_stack_new();
    gtk_widget_set_hexpand(stack, TRUE);
    gtk_widget_set_vexpand(stack, TRUE);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), stack);

    // Agregar pantallas al stack
    gtk_stack_add_named(GTK_STACK(stack), crearMenu(), "menu");
    gtk_stack_add_named(GTK_STACK(stack), crearPantallaOpciones(), "opciones");
    gtk_stack_add_named(GTK_STACK(stack), crearPantallaPuntuaciones(), "puntuaciones");

    // Mostrar ventana
    gtk_window_present(GTK_WINDOW(ventanaPrincipal));
}
int main(int argc, char **argv) {
    GtkApplication *aplicacion = gtk_application_new("com.proyecto.galaga", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(aplicacion, "activate", G_CALLBACK(activate), NULL);
    int estado = g_application_run(G_APPLICATION(aplicacion), argc, argv);

    g_object_unref(aplicacion);
    return estado;
}
