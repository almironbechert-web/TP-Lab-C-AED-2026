#ifndef ARTEMIS_H
#define ARTEMIS_H

/* ===== Constantes ===== */
#define G              6.674e-11
#define MASA_TIERRA    5.972e24
#define RADIO_TIERRA   6371000.0
#define ALTURA_LEO     192000.0
#define DISTANCIA_TL   384000000.0
#define V_TLI          10900.0
#define MASA_ORION     26000.0
#define PASO_DISTANCIA 20000000.0
#define MAX_TURNOS     25

/* ===== Estructuras ===== */
typedef struct {
    double masa;         /* kg */
    double velocidad;    /* m/s */
    double distancia;    /* m, medida desde el centro de la Tierra */
    double aceleracion;  /* m/s^2, debida a la gravedad terrestre */
} Nave;

/* ===== Fisica ===== */
double aceleracionGravitatoria(double r);
double velocidadEnDistancia(double v0, double r0, double r);
double distanciaMaxima(double v0, double r0);

/* ===== Simulacion (Escenario 3, item a) ===== */
void cargarNave(Nave *nave);
int  simularInercia(Nave *nave, Nave historial[], int maxTurnos);

/* ===== Salida por consola ===== */
void mostrarIntroduccion(void);
void mostrarHistorial(const Nave historial[], int cantidad);
void mostrarResultadoFinal(const Nave *nave);

#endif
