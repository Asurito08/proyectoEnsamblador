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

    // Crear overlay para fondo + contenido
    GtkWidget *overlay = gtk_overlay_new();
    gtk_window_set_child(GTK_WINDOW(window), overlay);

    // Imagen de fondo
    GtkWidget *background = gtk_picture_new_for_filename(
        "/home/fabricio/Documents/proyectoEnsamblador/design/elementos/fondo3.jpeg");
    gtk_widget_set_hexpand(background, TRUE);
    gtk_widget_set_vexpand(background, TRUE);
    gtk_picture_set_content_fit(GTK_PICTURE(background), GTK_CONTENT_FIT_COVER);
    gtk_overlay_set_child(GTK_OVERLAY(overlay), background);

    // Contenedor principal: título + botones
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_set_hexpand(main_box, TRUE);
    gtk_widget_set_vexpand(main_box, TRUE);
    gtk_widget_set_halign(main_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(main_box, GTK_ALIGN_CENTER);

    // Título
    GtkWidget *title_image = gtk_picture_new_for_filename(
        "/home/fabricio/Documents/proyectoEnsamblador/design/elementos/tituloGalaga.png");
    gtk_widget_set_size_request(title_image, 800, 250);
    gtk_picture_set_content_fit(GTK_PICTURE(title_image), GTK_CONTENT_FIT_CONTAIN);
    gtk_widget_set_hexpand(title_image, FALSE);
    gtk_widget_set_valign(title_image, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(main_box), title_image);

    // Caja de botones (vertical)
    GtkWidget *buttons_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(buttons_box, GTK_ALIGN_CENTER);
    gtk_widget_set_can_target(buttons_box, FALSE);


    // Botón 1 - Iniciar partida
    GtkWidget *button1 = gtk_button_new();
    GtkWidget *image1 = gtk_picture_new_for_filename(
        "/home/fabricio/Documents/proyectoEnsamblador/design/elementos/enemigosGalaga.png");
    gtk_picture_set_content_fit(GTK_PICTURE(image1), GTK_CONTENT_FIT_CONTAIN);
    gtk_widget_set_size_request(image1, 300, 100); // tamaño fijo
    gtk_button_set_child(GTK_BUTTON(button1), image1);

    // Evitar que el botón cambie visualmente
    gtk_widget_set_focusable(button1, FALSE);
    gtk_widget_add_css_class(button1, "flat");

    g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), NULL);
    gtk_box_append(GTK_BOX(buttons_box), button1);

    // Botón 2 - Puntuaciones
    GtkWidget *button2 = gtk_button_new();
    GtkWidget *image2 = gtk_picture_new_for_filename(
        "/home/fabricio/Documents/proyectoEnsamblador/design/elementos/corazon.png");
    gtk_picture_set_content_fit(GTK_PICTURE(image2), GTK_CONTENT_FIT_CONTAIN);
    gtk_widget_set_size_request(image2, 300, 100); // tamaño fijo
    gtk_button_set_child(GTK_BUTTON(button2), image2);

    // Igual que el botón anterior
    gtk_widget_set_focusable(button2, FALSE);
    gtk_widget_add_css_class(button2, "flat");

    g_signal_connect(button2, "clicked", G_CALLBACK(on_button2_clicked), NULL);
    gtk_box_append(GTK_BOX(buttons_box), button2);

    // Agregar caja de botones al contenedor principal
    gtk_box_append(GTK_BOX(main_box), buttons_box);

    // Agregar contenedor principal al overlay
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), main_box);

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
