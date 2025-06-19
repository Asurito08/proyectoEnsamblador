#include "stackManager.h"

GtkWidget *stack;
GtkWidget *ventanaPrincipal;

void cambiarPantalla(GtkButton *button, gpointer datosUsuario) {
    const char *nombrePantalla = (const char *)datosUsuario;
    gtk_stack_set_visible_child_name(GTK_STACK(stack), nombrePantalla);
}

void agregarPantalla(const char *id, GtkWidget *pantalla) {
    GtkWidget *anterior = gtk_stack_get_child_by_name(GTK_STACK(stack), id);
    if (anterior) {
        gtk_stack_remove(GTK_STACK(stack), anterior);
    }
    gtk_stack_add_named(GTK_STACK(stack), pantalla, id);
}
