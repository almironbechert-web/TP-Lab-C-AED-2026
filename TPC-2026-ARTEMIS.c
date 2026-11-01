#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

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
#define LARGO_ENTRADA  100

/* ===== Estructuras ===== */
typedef struct {
    double masa;
    double velocidad;
    double distancia;
    double aceleracion;
} Nave;

/* ===== Prototipos ===== */
void   leerLinea(char *entrada);
double leerDouble(const char *mensaje, double min, double max);
char   leerSiNo(const char *mensaje);
double aceleracionGravitatoria(double r);
double velocidadEnDistancia(double v0, double r0, double r);
double distanciaMaxima(double v0, double r0);
void   cargarNave(Nave *nave);
int    simularInercia(Nave *nave, Nave historial[], int maxTurnos);
void   mostrarHistorial(Nave historial[], int cantidad);

/* ===== Programa principal ===== */
int main(void) {
    Nave orion;
    Nave historial[MAX_TURNOS];
    int cantidad;

    printf("\n=== 1.a LEY DE NEWTON - INERCIA ===\n"
           "Sin motores, nada empuja a Orion: por inercia sigue moviendose sola.\n"
           "Pero la gravedad terrestre tira en contra del movimiento y la frena,\n"
           "cada vez menos, porque esa fuerza disminuye con 1/r^2.\n\n");

    cargarNave(&orion);
    cantidad = simularInercia(&orion, historial, MAX_TURNOS);
    mostrarHistorial(historial, cantidad);

    if (orion.velocidad == 0.0) {
        printf("\nLa gravedad detuvo a Orion a %.1f km del centro de la Tierra.\n", orion.distancia / 1000.0);
    } else {
        printf("\nOrion alcanzo la distancia de la Luna con %.2f m/s.\n", orion.velocidad);
    }
    return 0;
}

/* ===== Validaciones ===== */
void leerLinea(char *entrada) {
    if (fgets(entrada, LARGO_ENTRADA, stdin) == NULL) {
        printf("\nNo se pudo leer la entrada. Fin del programa.\n");
        exit(1);
    }
    entrada[strcspn(entrada, "\n")] = '\0';
}

double leerDouble(const char *mensaje, double min, double max) {
    char entrada[LARGO_ENTRADA], *fin;
    double valor;

    while (1) {
        printf("%s [%.0f - %.0f]: ", mensaje, min, max);
        leerLinea(entrada);
        valor = strtod(entrada, &fin);
        if (fin == entrada || *fin != '\0') {
            printf("ERROR: ingrese solo un numero.\n");
        } else if (valor < min || valor > max) {
            printf("ERROR: valor fuera de rango.\n");
        } else {
            return valor;
        }
    }
}

char leerSiNo(const char *mensaje) {
    char entrada[LARGO_ENTRADA];

    while (1) {
        printf("%s (s/n): ", mensaje);
        leerLinea(entrada);
        if (strlen(entrada) == 1 && strchr("sSnN", entrada[0]) != NULL) {
            return tolower(entrada[0]);
        }
        printf("ERROR: responda 's' o 'n'.\n");
    }
}

/* ===== Fisica ===== */
double aceleracionGravitatoria(double r) {
    return G * MASA_TIERRA / (r * r);
}

double velocidadEnDistancia(double v0, double r0, double r) {
    double v2 = v0 * v0 - 2.0 * G * MASA_TIERRA * (1.0 / r0 - 1.0 / r);
    return (v2 < 0.0) ? -1.0 : sqrt(v2);
}

double distanciaMaxima(double v0, double r0) {
    return 1.0 / (1.0 / r0 - v0 * v0 / (2.0 * G * MASA_TIERRA));
}

/* ===== Item a ===== */
void cargarNave(Nave *nave) {
    double altura = ALTURA_LEO;

    nave->velocidad = V_TLI;
    nave->masa = MASA_ORION;
    if (leerSiNo("Usar los datos del enunciado?") == 'n') {
        nave->velocidad = leerDouble("Velocidad de inyeccion (m/s)", 1, 20000);
        nave->masa = leerDouble("Masa de la nave (kg)", 1000, 100000);
        altura = leerDouble("Altura de la orbita baja (m)", 160000, 2000000);
    }
    nave->distancia = RADIO_TIERRA + altura;
    nave->aceleracion = aceleracionGravitatoria(nave->distancia);
}

int simularInercia(Nave *nave, Nave historial[], int maxTurnos) {
    double v0 = nave->velocidad, r0 = nave->distancia, v;
    int n = 0;

    historial[n++] = *nave;
    while (n < maxTurnos && nave->velocidad > 0.0 && nave->distancia < DISTANCIA_TL) {
        nave->distancia = fmin(nave->distancia + PASO_DISTANCIA, DISTANCIA_TL);
        v = velocidadEnDistancia(v0, r0, nave->distancia);
        if (v < 0.0) {
            nave->distancia = distanciaMaxima(v0, r0);
            v = 0.0;
        }
        nave->velocidad = v;
        nave->aceleracion = aceleracionGravitatoria(nave->distancia);
        historial[n++] = *nave;
    }
    return n;
}

void mostrarHistorial(Nave historial[], int cantidad) {
    int i;

    printf("\nTurno | Distancia (km) | Velocidad (m/s) | Aceleracion (m/s^2)\n");
    for (i = 0; i < cantidad; i++) {
        printf("%5d | %14.1f | %15.2f | %19.5f\n", i, historial[i].distancia / 1000.0,
               historial[i].velocidad, historial[i].aceleracion);
    }
}