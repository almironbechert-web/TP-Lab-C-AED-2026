#include <stdio.h>
#include <math.h>
#include "artemis.h"
#include "validaciones.h"

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

/* K = 1/2 * m * v^2  (resultado en Joules) */
double energiaCinetica(double masa, double velocidad) {
    return 0.5 * masa * velocidad * velocidad;
}

/* ===== Simulacion ===== */
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

/* ===== Salida por consola ===== */
void mostrarIntroduccion(void) {
    printf("\n=== 1.a LEY DE NEWTON - INERCIA ===\n"
           "Sin motores, nada empuja a Orion: por inercia sigue moviendose sola.\n"
           "Pero la gravedad terrestre tira en contra del movimiento y la frena,\n"
           "cada vez menos, porque esa fuerza disminuye con 1/r^2.\n\n");
}

void mostrarEnergiaCinetica(const Nave *nave) {
    printf("\nEnergia cinetica inicial: %.3e J\n",
           energiaCinetica(nave->masa, nave->velocidad));
}

void mostrarHistorial(const Nave historial[], int cantidad) {
    int i;

    printf("\nTurno | Distancia (km) | Velocidad (m/s) | Aceleracion (m/s^2)\n");
    for (i = 0; i < cantidad; i++) {
        printf("%5d | %14.1f | %15.2f | %19.5f\n", i, historial[i].distancia / 1000.0,
               historial[i].velocidad, historial[i].aceleracion);
    }
}

void mostrarResultadoFinal(const Nave *nave) {
    if (nave->velocidad == 0.0) {
        printf("\nLa gravedad detuvo a Orion a %.1f km del centro de la Tierra.\n",
               nave->distancia / 1000.0);
    } else {
        printf("\nOrion llegó a la Luna con %.2f m/s.\n", nave->velocidad);
    }
}
