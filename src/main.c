#include <gtk/gtk.h>

// Función que se ejecuta al presionar el botón
static void on_button1_clicked(GtkButton *button, gpointer user_data) {
    g_print("Iniciar Partida\n");
}

static void on_button2_clicked(GtkButton *button, gpointer user_data) {
    g_print("Puntuaciones\n");
}

// Función que arma la interfaz
static void activate(GtkApplication *app, gpointer user_data) {
    // Crear ventana principal
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Galaga");
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 900);

    // Crear overlay
    GtkWidget *overlay = gtk_overlay_new();
    gtk_window_set_child(GTK_WINDOW(window), overlay);

    // Crear imagen de fondo
    GtkWidget *background = gtk_picture_new_for_filename("/home/fabricio/Documents/proyectoEnsamblador/design/elementos/fondo3.jpeg");
    gtk_widget_set_halign(background, GTK_ALIGN_FILL);
    gtk_widget_set_valign(background, GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(background, TRUE);
    gtk_widget_set_vexpand(background, TRUE);
    gtk_overlay_set_child(GTK_OVERLAY(overlay), background);

    // Crear caja para los botones
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

    // Crear botones
    GtkWidget *button1 = gtk_button_new_with_label("Iniciar partida");
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), NULL);

    GtkWidget *button2 = gtk_button_new_with_label("Puntuaciones");
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button2_clicked), NULL);

    // Agregar botones a la caja
    gtk_box_append(GTK_BOX(box), button1);
    gtk_box_append(GTK_BOX(box), button2);

    // Agregar la caja encima del fondo
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), box);

    // Crear imagen para el título (parte superior)
    GtkWidget *title_image = gtk_picture_new_for_filename("/home/fabricio/Documents/proyectoEnsamblador/design/elementos/tituloGalaga.png");
    gtk_widget_set_halign(title_image, GTK_ALIGN_CENTER);  // Centrado horizontalmente
    gtk_widget_set_valign(title_image, GTK_ALIGN_START);   // Arriba
    gtk_widget_set_margin_top(title_image, 80); 
    gtk_picture_set_content_fit(GTK_PICTURE(title_image), GTK_CONTENT_FIT_CONTAIN);
    gtk_widget_set_size_request(title_image, 500, 150);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), title_image);

    // Mostrar ventana
    gtk_window_present(GTK_WINDOW(window));
}

// Función principal
int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.Galaga.gtk4", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
