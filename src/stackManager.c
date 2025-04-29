#include "stackManager.h"

GtkWidget *stack;
GtkWidget *ventanaPrincipal;

// Función para cambiar de pantalla
void cambiarPantalla(GtkButton *button, gpointer datosUsuario) {
    const char *nombrePantalla = (const char *)datosUsuario;
    gtk_stack_set_visible_child_name(GTK_STACK(stack), nombrePantalla);
}
