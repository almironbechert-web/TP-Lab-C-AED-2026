#ifndef JUEGO_H
#define JUEGO_H

#include "artemis.h"

/* ===== Reglas del juego ===== */
#define MAX_INTENTOS    3        /* turnos de la partida */
#define VEL_MIN_JUEGO   9000.0   /* rango permitido de velocidad de inyeccion, m/s */
#define VEL_MAX_JUEGO   13000.0
#define PUNTAJE_MAX     1000
#define PENALIZACION    300      /* puntos que se pierden por cada turno extra usado */

typedef enum {
    RES_SIN_USAR,
    RES_INSUFICIENTE,    /* se detuvo antes de llegar a la Luna */
    RES_RETORNO_LIBRE,   /* llego a la Luna y sigue ligada a la Tierra */
    RES_ESCAPE           /* supero la velocidad de escape */
} Resultado;

typedef struct {
    double    velocidadInyeccion;   /* m/s */
    double    distanciaAlcanzada;   /* m, desde el centro de la Tierra */
    Resultado resultado;
} Intento;

typedef struct {
    Intento intentos[MAX_INTENTOS];
    int     cantidadIntentos;
    int     exito;               /* 1 si logro el retorno libre */
    int     puntaje;
    double  alturaOrbita;        /* m */
    double  velocidadMinima;     /* m/s: minima para llegar a la Luna */
    double  velocidadEscape;     /* m/s: maxima para seguir ligada a la Tierra */
} Mision;

void inicializarMision(Mision *mision);
void jugarMision(Mision *mision);
void mostrarReporte(const Mision *mision);
void mostrarReglas(void);

#endif
