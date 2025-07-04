#ifndef PARTIDA_H
#define PARTIDA_H

#include <gtk/gtk.h>
#include "stackManager.h"
#include "pausa.h"
#include "victoria.h"
#include "derrota.h"

GtkWidget* crearPantallaPartida(void);
gboolean loop_juego(gpointer data);
gboolean safe_grab_focus(gpointer widget_ptr);

#endif
