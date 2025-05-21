#ifndef NOMBRE_JUGADOR_H
#define NOMBRE_JUGADOR_H

#include <gtk/gtk.h>

extern char nombreJugador[100];

GtkWidget* crearPantallaNombreJugador(void);
void guardarNombreJugador(GtkButton *boton, gpointer user_data);

#endif // NOMBRE_JUGADOR_H
