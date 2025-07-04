#include <gtk/gtk.h>
#include <gst/gst.h>
#include <glib.h>
#include "stackManager.h"
#include "menu.h"
#include "opciones.h"
#include "puntuaciones.h"
#include "partida.h"

GtkCssProvider *cssProvider = NULL;
GstElement *pipeline;

static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data) {
    switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_EOS:
            // Reiniciar al inicio para hacer loop
            gst_element_seek_simple(pipeline, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH, 0);
            gst_element_set_state(pipeline, GST_STATE_PLAYING);
            break;
        case GST_MESSAGE_ERROR: {
            GError *err;
            gchar *debug;
            gst_message_parse_error(msg, &err, &debug);
            g_printerr("Error: %s\n", err->message);
            g_error_free(err);
            g_free(debug);
            break;
        }
        default:
            break;
    }
    return TRUE;
}

static void activate(GtkApplication *aplicacion, gpointer datosUsuario) {
    // Crear ventana principal
    ventanaPrincipal = gtk_application_window_new(aplicacion);
    gtk_window_set_title(GTK_WINDOW(ventanaPrincipal), "Galaga");
    gtk_window_set_default_size(GTK_WINDOW(ventanaPrincipal), 1200, 900);

    // Cargar CSS
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

    GtkWidget *fondo = gtk_picture_new_for_filename("design/elementos/fondo.jpeg");
    gtk_widget_set_hexpand(fondo, TRUE);
    gtk_widget_set_vexpand(fondo, TRUE);
    gtk_picture_set_content_fit(GTK_PICTURE(fondo), GTK_CONTENT_FIT_COVER);
    gtk_overlay_set_child(GTK_OVERLAY(overlay), fondo);

    // Crear stack
    stack = gtk_stack_new();
    gtk_widget_set_hexpand(stack, TRUE);
    gtk_widget_set_vexpand(stack, TRUE);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), stack);

    // Agregar pantallas al stack
    agregarPantalla("menu", crearMenu());
    agregarPantalla("opciones", crearPantallaOpciones());
    agregarPantalla("puntuaciones", crearPantallaPuntuaciones());

    if (pipeline) {
        g_object_set(pipeline, "volume", 0.3, NULL);
        gst_element_set_state(pipeline, GST_STATE_PLAYING);
    } else {
        g_printerr("Pipeline no está inicializado\n");
    }

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // Mostrar ventana (inicia en el menú por defecto)
    gtk_window_present(GTK_WINDOW(ventanaPrincipal));
}

int main(int argc, char **argv) {

    gst_init(&argc, &argv);

    // Obtener directorio actual
    gchar *current_dir = g_get_current_dir();

    // Ruta relativa a la música
    const gchar *relative_path = "design/elementos/musica.mp3";

    // Crear URI absoluta con file://
    gchar *uri = g_strdup_printf("file://%s/%s", current_dir, relative_path);

    g_free(current_dir);

    gchar *pipeline_desc = g_strdup_printf("playbin uri=%s", uri);
    pipeline = gst_parse_launch(pipeline_desc, NULL);
    g_free(pipeline_desc);


    g_free(uri);

    if (!pipeline) {
        g_printerr("Error creando pipeline\n");
        return 1;
    }

    // Conectar el bus para detectar fin de reproducción
    GstBus *bus = gst_element_get_bus(pipeline);
    gst_bus_add_watch(bus, bus_call, NULL);
    gst_object_unref(bus);

    GtkApplication *aplicacion = gtk_application_new("com.proyecto.galaga", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(aplicacion, "activate", G_CALLBACK(activate), NULL);
    int estado = g_application_run(G_APPLICATION(aplicacion), argc, argv);

    g_object_unref(aplicacion);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    return estado;
}
