#ifndef MENU_H
#define MENU_H

#include <gtk/gtk.h>
#include "partida.h"
#include "stackManager.h"

GtkWidget* crearMenu(void);
static void irPartida(GtkButton *btn, gpointer user_data);

#endif
