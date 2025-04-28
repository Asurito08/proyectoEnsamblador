#include <gtk/gtk.h>

GtkWidget *stack; // Stack global, se utiliza para manejar las diferenste pantallas que tiene la interfaz
GtkWidget *main_window; // Ventana principal global, permite cerrar la ventana principal desde cualquier funcion

// Funciones para cambiar de pantalla
static void on_button1_clicked(GtkButton *button, gpointer user_data) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "iniciar"); //al presionar el buton 1 (Iniciar Partida), muestra la pantalla de iniciar partida
}

static void on_button2_clicked(GtkButton *button, gpointer user_data) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "puntuaciones");// hace lo mismo con la pantalla puntuaciones
}

static void on_button3_clicked(GtkButton *button, gpointer user_data) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "configuracion");//hace lo mismo con la pantalla configuracion
}

static void on_back_clicked(GtkButton *button, gpointer user_data) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "menu");//muestra la pantalla principal
}

// Función para cerrar la ventana
static void on_close_clicked(GtkButton *button, gpointer user_data) {
    gtk_window_close(GTK_WINDOW(main_window));//cierra el programa
}

static void activate(GtkApplication *app, gpointer user_data) { // se ejecuta al abrir el programa
    // Crear ventana principal, mostrando la imagen Galaga
    main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(main_window), "Galaga");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 1200, 900);//tamano de la ventana

    // Crear overlay, me permite dividir la ventana en capas
    GtkWidget *overlay = gtk_overlay_new();
    gtk_window_set_child(GTK_WINDOW(main_window), overlay);

    // Fondo, crea un puntero que contiene la direccion de memoria de la foto y la fija como fondo de la ventana
    GtkWidget *background = gtk_picture_new_for_filename(
        "/home/fabricio/Documents/proyectoEnsamblador/design/elementos/fondo3.jpeg");
    gtk_widget_set_hexpand(background, TRUE);
    gtk_widget_set_vexpand(background, TRUE);
    gtk_picture_set_content_fit(GTK_PICTURE(background), GTK_CONTENT_FIT_COVER);
    gtk_overlay_set_child(GTK_OVERLAY(overlay), background);

    // Crear stack, crea un nuevo stack, hace que ocupe toda la pantalla posible y lo pone sobre el fondo
    stack = gtk_stack_new();
    gtk_widget_set_hexpand(stack, TRUE);
    gtk_widget_set_vexpand(stack, TRUE);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), stack);

    // -------- Pantalla del Menú principal --------
    GtkWidget *menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_set_halign(menu_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(menu_box, GTK_ALIGN_CENTER);

    // Título, toma la imagen del titulo, le da un tama;o y la pone en la caja menu_box
    GtkWidget *title_image = gtk_picture_new_for_filename(
        "/home/fabricio/Documents/proyectoEnsamblador/design/elementos/tituloGalaga.png");
    gtk_widget_set_size_request(title_image, 800, 250);
    gtk_picture_set_content_fit(GTK_PICTURE(title_image), GTK_CONTENT_FIT_CONTAIN);
    gtk_box_append(GTK_BOX(menu_box), title_image);

    // Caja de botones
    GtkWidget *buttons_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(buttons_box, GTK_ALIGN_CENTER);

    // Botón 1 - Iniciar partida
    GtkWidget *button1 = gtk_button_new();
    GtkWidget *image1 = gtk_picture_new_for_filename(
        "/home/fabricio/Documents/proyectoEnsamblador/design/elementos/enemigosGalaga.png");
    gtk_picture_set_content_fit(GTK_PICTURE(image1), GTK_CONTENT_FIT_CONTAIN);
    gtk_widget_set_size_request(image1, 300, 100);
    gtk_button_set_child(GTK_BUTTON(button1), image1);
    gtk_widget_set_focusable(button1, FALSE);
    gtk_widget_add_css_class(button1, "flat");
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), NULL);
    gtk_box_append(GTK_BOX(buttons_box), button1);

    // Botón 2 - Puntuaciones
    GtkWidget *button2 = gtk_button_new();
    GtkWidget *image2 = gtk_picture_new_for_filename(
        "/home/fabricio/Documents/proyectoEnsamblador/design/elementos/corazon.png");
    gtk_picture_set_content_fit(GTK_PICTURE(image2), GTK_CONTENT_FIT_CONTAIN);
    gtk_widget_set_size_request(image2, 300, 100);
    gtk_button_set_child(GTK_BUTTON(button2), image2);
    gtk_widget_set_focusable(button2, FALSE);
    gtk_widget_add_css_class(button2, "flat");
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button2_clicked), NULL);
    gtk_box_append(GTK_BOX(buttons_box), button2);

    // Botón 3 - Configuración (otro corazón)
    GtkWidget *button3 = gtk_button_new();
    GtkWidget *image3 = gtk_picture_new_for_filename(
        "/home/fabricio/Documents/proyectoEnsamblador/design/elementos/corazon.png");
    gtk_picture_set_content_fit(GTK_PICTURE(image3), GTK_CONTENT_FIT_CONTAIN);
    gtk_widget_set_size_request(image3, 300, 100);
    gtk_button_set_child(GTK_BUTTON(button3), image3);
    gtk_widget_set_focusable(button3, FALSE);
    gtk_widget_add_css_class(button3, "flat");
    g_signal_connect(button3, "clicked", G_CALLBACK(on_button3_clicked), NULL);
    gtk_box_append(GTK_BOX(buttons_box), button3);

    gtk_box_append(GTK_BOX(menu_box), buttons_box);

    gtk_stack_add_named(GTK_STACK(stack), menu_box, "menu");

    // -------- Pantalla Iniciar Partida --------
    GtkWidget *iniciar_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_set_halign(iniciar_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(iniciar_box, GTK_ALIGN_CENTER);

    GtkWidget *label_iniciar = gtk_label_new("Pantalla de Iniciar Partida");
    gtk_box_append(GTK_BOX(iniciar_box), label_iniciar);

    GtkWidget *back1 = gtk_button_new_with_label("Volver");
    gtk_widget_set_size_request(back1, 200, 50);
    g_signal_connect(back1, "clicked", G_CALLBACK(on_back_clicked), NULL);
    gtk_box_append(GTK_BOX(iniciar_box), back1);

    gtk_stack_add_named(GTK_STACK(stack), iniciar_box, "iniciar");

    // -------- Pantalla Puntuaciones --------
    GtkWidget *puntuaciones_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_set_halign(puntuaciones_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(puntuaciones_box, GTK_ALIGN_CENTER);

    GtkWidget *label_puntuaciones = gtk_label_new("Pantalla de Puntuaciones");
    gtk_box_append(GTK_BOX(puntuaciones_box), label_puntuaciones);

    GtkWidget *back2 = gtk_button_new_with_label("Volver");
    gtk_widget_set_size_request(back2, 200, 50);
    g_signal_connect(back2, "clicked", G_CALLBACK(on_back_clicked), NULL);
    gtk_box_append(GTK_BOX(puntuaciones_box), back2);

    gtk_stack_add_named(GTK_STACK(stack), puntuaciones_box, "puntuaciones");

    // -------- Pantalla Configuración --------
    GtkWidget *configuracion_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_set_halign(configuracion_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(configuracion_box, GTK_ALIGN_CENTER);

    GtkWidget *label_configuracion = gtk_label_new("Pantalla de Configuración");
    gtk_box_append(GTK_BOX(configuracion_box), label_configuracion);

    GtkWidget *back3 = gtk_button_new_with_label("Volver");
    gtk_widget_set_size_request(back3, 200, 50);
    g_signal_connect(back3, "clicked", G_CALLBACK(on_back_clicked), NULL);
    gtk_box_append(GTK_BOX(configuracion_box), back3);

    gtk_stack_add_named(GTK_STACK(stack), configuracion_box, "configuracion");

    // --- Botón de Cerrar (corazon pequeño en esquina inferior derecha) ---
    GtkWidget *close_button = gtk_button_new();
    gtk_widget_set_size_request(close_button, 50, 50); // Tamaño pequeño

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_size(
        "/home/fabricio/Documents/proyectoEnsamblador/design/elementos/corazon.png",
        40, 40, NULL); // Imagen 40x40

    GtkWidget *close_image = gtk_image_new_from_pixbuf(pixbuf);
    gtk_button_set_child(GTK_BUTTON(close_button), close_image);

    gtk_widget_set_halign(close_button, GTK_ALIGN_END);
    gtk_widget_set_valign(close_button, GTK_ALIGN_END);
    gtk_widget_set_margin_end(close_button, 10);
    gtk_widget_set_margin_bottom(close_button, 10);

    gtk_widget_set_focusable(close_button, FALSE);
    gtk_widget_add_css_class(close_button, "flat");

    g_signal_connect(close_button, "clicked", G_CALLBACK(on_close_clicked), NULL);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), close_button);

    g_object_unref(pixbuf); // Liberar pixbuf

    // Mostrar ventana
    gtk_window_present(GTK_WINDOW(main_window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.Galaga.gtk4", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
