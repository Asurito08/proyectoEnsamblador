#ifndef PUNTUACIONES_H
#define PUNTUACIONES_H

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *nombresBox;
    GtkWidget *puntosBox;
    GtkWidget *overlay;
    GtkWidget *botonReset;
} WidgetsPuntuaciones;

typedef struct {
    char nombre[100];
    int puntaje;
} Puntuacion;

extern WidgetsPuntuaciones *widgetsGlobal;


GtkWidget* crearPantallaPuntuaciones(void);
void cargarPuntuaciones(GtkWidget *nombresBox, GtkWidget *puntosBox, GtkWidget *overlay, GtkWidget *botonReset);

#endif // PUNTUACIONES_H