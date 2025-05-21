#include <gtk/gtk.h>
#include "nombreJugador.h"

char nombreJugador[100] = "";

void guardarNombreJugador(GtkButton *boton, gpointer user_data) {
    GtkEntry *entrada = GTK_ENTRY(user_data);
    const char *texto = gtk_editable_get_text(GTK_EDITABLE(entrada));
    g_strlcpy(nombreJugador, texto, sizeof(nombreJugador));
    g_print("Nombre guardado: %s\n", nombreJugador);
}

GtkWidget* crearPantallaNombreJugador(void) {
    GtkWidget *caja = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign(caja, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(caja, GTK_ALIGN_CENTER);

    GtkWidget *entrada = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entrada), "Ingresa tu nombre...");
    gtk_box_append(GTK_BOX(caja), entrada);

    GtkWidget *botonAceptar = gtk_button_new_with_label("Aceptar");
    gtk_box_append(GTK_BOX(caja), botonAceptar);

    g_signal_connect(botonAceptar, "clicked", G_CALLBACK(guardarNombreJugador), entrada);

    return caja;
}
