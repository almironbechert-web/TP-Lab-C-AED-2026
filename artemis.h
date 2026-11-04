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
#define MAX_TRAMOS     25
#define MASA_LUNA      7.342e22
#define RADIO_LUNA     1737400.0
#define VE_COMBUSTIBLE 4500.0   /* velocidad de salida de los gases (H2-O2), m/s */
#define NOMBRE_COMBUSTIBLE "hidrogeno y oxigeno liquidos"

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

/* ===== Energia (Escenario 3, item b) ===== */
double energiaCinetica(double masa, double velocidad);

/* ===== Justificacion energetica (Escenario 3, item d) ===== */
double pozoPotencial(double masaCuerpo, double radio);
double velocidadEscape(double masaCuerpo, double radio);
double energiaEscape(double masaNave, double masaCuerpo, double radio);
double combustibleNecesario(double masaNave, double deltaV);

/* ===== Simulacion (Escenario 3, item a) ===== */
void cargarNave(Nave *nave);
int  simularInercia(Nave *nave, Nave historial[], int maxTramos);

/* ===== Salida por consola ===== */
void mostrarIntroduccion(void);
void mostrarEnergiaCinetica(const Nave *nave);
void mostrarHistorial(const Nave historial[], int cantidad);
void mostrarJustificacionEnergetica(const Nave *nave);

/* ===== Modo analisis fisico (items a, b, d; falta el c) ===== */
void ejecutarAnalisisFisico(void);
void mostrarResultadoFinal(const Nave *final, const Nave *inicial);

#endif
