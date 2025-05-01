#include "opciones.h"
#include "stackManager.h"

//Funcion momentanea para mostar que los botones de las naves si funcioan
static void seleccionarNave(GtkButton *boton, gpointer datosUsuario) {
    const char *nombreNave = (const char *)datosUsuario;
    g_print("%s ha sido escogida\n", nombreNave);
}

//Funcion para crear la pantalla de opciones y agregarle su contenido
GtkWidget* crearPantallaOpciones(void) {
    GtkWidget *overlayOpciones = gtk_overlay_new();
    gtk_widget_set_hexpand(overlayOpciones, TRUE);
    gtk_widget_set_vexpand(overlayOpciones, TRUE);

    GtkWidget *cajaOpciones = gtk_box_new(GTK_ORIENTATION_VERTICAL, 40);
    gtk_widget_set_halign(cajaOpciones, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(cajaOpciones, GTK_ALIGN_CENTER);

    //Se agrega la caja de sonido

    GtkWidget *seccionSonido = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign(seccionSonido, GTK_ALIGN_CENTER);

    
    GtkWidget *overlaySonido = gtk_overlay_new();
    gtk_widget_set_size_request(overlaySonido, 600, 250);

    GtkWidget *fondoSonido = gtk_picture_new_for_filename("design/elementos/sonido.svg");
    gtk_picture_set_content_fit(GTK_PICTURE(fondoSonido), GTK_CONTENT_FIT_CONTAIN);
    gtk_widget_set_hexpand(fondoSonido, TRUE);
    gtk_widget_set_vexpand(fondoSonido, TRUE);
    gtk_overlay_set_child(GTK_OVERLAY(overlaySonido), fondoSonido);

    GtkWidget *controlSonido = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 40);
    gtk_widget_set_margin_top(controlSonido, 30);
    gtk_widget_set_halign(controlSonido, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(controlSonido, GTK_ALIGN_CENTER);

    //Se pone el slider para regular el sonido
    GtkWidget *sliderVolumen = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
    gtk_widget_set_size_request(sliderVolumen, 300, -1);
    gtk_range_set_value(GTK_RANGE(sliderVolumen), 60);
    gtk_box_append(GTK_BOX(controlSonido), sliderVolumen);

    gtk_overlay_add_overlay(GTK_OVERLAY(overlaySonido), controlSonido);
    gtk_box_append(GTK_BOX(seccionSonido), overlaySonido);
    gtk_box_append(GTK_BOX(cajaOpciones), seccionSonido);

    //Se crea la seccion de naves
    GtkWidget *seccionNaves = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign(seccionNaves, GTK_ALIGN_CENTER);

    GtkWidget *overlayNaves = gtk_overlay_new();
    gtk_widget_set_size_request(overlayNaves, 600, 250);

    GtkWidget *fondoNave = gtk_picture_new_for_filename("design/elementos/estiloNave.svg");
    gtk_picture_set_content_fit(GTK_PICTURE(fondoNave), GTK_CONTENT_FIT_CONTAIN);
    gtk_widget_set_hexpand(fondoNave, TRUE);
    gtk_widget_set_vexpand(fondoNave, TRUE);
    gtk_overlay_set_child(GTK_OVERLAY(overlayNaves), fondoNave);

    GtkWidget *filaNaves = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 40);
    gtk_widget_set_margin_top(filaNaves, 30); 
    gtk_widget_set_halign(filaNaves, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(filaNaves, GTK_ALIGN_CENTER);

    //se crean e introducen los botones de las naves
    for (int i = 1; i <= 4; i++) {
        char ruta[100];
        char nombre[20];
        sprintf(ruta, "design/elementos/nave%d.svg", i);
        sprintf(nombre, "nave%d", i);

        GtkWidget *botonNave = gtk_button_new();
        gtk_widget_set_size_request(botonNave, 70, 70);

        GtkWidget *imagen = gtk_picture_new_for_filename(ruta);
        gtk_picture_set_content_fit(GTK_PICTURE(imagen), GTK_CONTENT_FIT_CONTAIN);
        gtk_widget_set_size_request(imagen, 60, 60);

        gtk_button_set_child(GTK_BUTTON(botonNave), imagen);
        gtk_widget_add_css_class(botonNave, "flat");

        g_signal_connect(botonNave, "clicked", G_CALLBACK(seleccionarNave), g_strdup(nombre));

        gtk_box_append(GTK_BOX(filaNaves), botonNave);
    }

    gtk_overlay_add_overlay(GTK_OVERLAY(overlayNaves), filaNaves);
    gtk_box_append(GTK_BOX(seccionNaves), overlayNaves);
    gtk_box_append(GTK_BOX(cajaOpciones), seccionNaves);

    //Se crea la flecha para volver a la pantalla principal
    GtkWidget *botonVolver = gtk_button_new();
    gtk_widget_set_size_request(botonVolver, 60, 60);

    GdkPixbuf *pixbufVolver = gdk_pixbuf_new_from_file_at_size("design/elementos/back.svg", 40, 40, NULL);
    GdkTexture *textureVolver = gdk_texture_new_for_pixbuf(pixbufVolver);
    GtkWidget *imagenVolver = gtk_image_new_from_paintable(GDK_PAINTABLE(textureVolver));
    gtk_button_set_child(GTK_BUTTON(botonVolver), imagenVolver);

    gtk_widget_set_halign(botonVolver, GTK_ALIGN_END);
    gtk_widget_set_valign(botonVolver, GTK_ALIGN_END);
    gtk_widget_set_margin_end(botonVolver, 10);
    gtk_widget_set_margin_bottom(botonVolver, 10);
    gtk_widget_add_css_class(botonVolver, "flat");

    g_signal_connect(botonVolver, "clicked", G_CALLBACK(cambiarPantalla), (gpointer)"menu");

    gtk_overlay_set_child(GTK_OVERLAY(overlayOpciones), cajaOpciones);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlayOpciones), botonVolver);

    g_object_unref(pixbufVolver);
    g_object_unref(textureVolver);

    return overlayOpciones;
}
