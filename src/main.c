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

    // Crear contenedor centrado
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(window), box);

    // Crear botón
    GtkWidget *button1 = gtk_button_new_with_label("Iniciar partida");
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), NULL);
    gtk_widget_set_valign(button1, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(button1, GTK_ALIGN_CENTER);

    GtkWidget *button2 = gtk_button_new_with_label("Puntuaciones");
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button2_clicked), NULL);
    gtk_widget_set_valign(button2, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(button2, GTK_ALIGN_CENTER);

    // Agregar botón al contenedor
    gtk_box_append(GTK_BOX(box), button1);
    gtk_box_append(GTK_BOX(box), button2);

    // Usar gtk_window_present en lugar de gtk_widget_show
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
