#ifndef STACK_MANAGER_H
#define STACK_MANAGER_H

#include <gtk/gtk.h>
#include "partida.h"

extern GtkWidget *stack;
extern GtkWidget *ventanaPrincipal;

void cambiarPantalla(GtkButton *button, gpointer datosUsuario);
void agregarPantalla(const char *id, GtkWidget *pantalla);

#endif
