#ifndef STACK_MANAGER_H
#define STACK_MANAGER_H

#include <gtk/gtk.h>

 //extern se utiliza para definir variables globales, se definen pero no se crean
extern GtkWidget *stack;
extern GtkWidget *ventanaPrincipal;

void cambiarPantalla(GtkButton *button, gpointer datosUsuario);

#endif
