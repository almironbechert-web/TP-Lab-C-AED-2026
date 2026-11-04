#include <stdio.h>
#include <math.h>
#include "artemis.h"
#include "validaciones.h"

/* ===== Fisica ===== */
double aceleracionGravitatoria(double r) {
    return G * MASA_TIERRA / (r * r);
}

/* ===== Trabajo hasta el punto de equilibrio (item c) ===== */
/* r_eq = D / (1 + sqrt(M_L/M_T))  (m desde el centro de la Tierra) */
double puntoEquilibrio(double distanciaTierraLuna) {
    return distanciaTierraLuna / (1.0 + sqrt(MASA_LUNA / MASA_TIERRA));
}

/* W/m = G*M_T*(1/r0 - 1/r_eq)  (J/kg) */
double trabajoPorKg(double r0, double rEq) {
    return G * MASA_TIERRA * (1.0 / r0 - 1.0 / rEq);
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

/* ===== Justificacion energetica (item d) ===== */

/* U = G*M/R  (J/kg): energia por kg que hay que aportar para salir del cuerpo */
double pozoPotencial(double masaCuerpo, double radio) {
    return G * masaCuerpo / radio;
}

/* v_esc = sqrt(2*G*M/R)  (m/s) */
double velocidadEscape(double masaCuerpo, double radio) {
    return sqrt(2.0 * pozoPotencial(masaCuerpo, radio));
}

/* E = m * G*M/R  (J) */
double energiaEscape(double masaNave, double masaCuerpo, double radio) {
    return masaNave * pozoPotencial(masaCuerpo, radio);
}

/* Ecuacion del cohete (Tsiolkovsky): m_comb = m * (e^(dV/ve) - 1)  (kg) */
double combustibleNecesario(double masaNave, double deltaV) {
    return masaNave * (exp(deltaV / VE_COMBUSTIBLE) - 1.0);
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

int simularInercia(Nave *nave, Nave historial[], int maxTramos) {
    double v0 = nave->velocidad, r0 = nave->distancia, v;
    int n = 0;

    historial[n++] = *nave;
    while (n < maxTramos && nave->velocidad > 0.0 && nave->distancia < DISTANCIA_TL) {
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
           "cada vez menos, porque esa fuerza disminuye con 1/r^2.\n"
           "(r = distancia desde el centro de la Tierra hasta Orion; es la columna\n"
           "'Distancia' de la tabla. Al inicio vale: radio terrestre + altura de la orbita.)\n\n");
}

void mostrarEnergiaCinetica(const Nave *nave) {
    printf("\n=== 1.b ENERGIA CINETICA INICIAL ===\n"
           "Formula utilizada (m = masa de Orion, v = velocidad inicial de inyeccion):\n"
           "  Energia cinetica : K = 1/2*m*v^2\n\n"
           "Energia cinetica inicial: %.3e J\n",
           energiaCinetica(nave->masa, nave->velocidad));
}

void mostrarTrabajoEquilibrio(const Nave *inicial) {
    double rEq    = puntoEquilibrio(DISTANCIA_TL);
    double w      = trabajoPorKg(inicial->distancia, rEq);
    double kPorKg = 0.5 * inicial->velocidad * inicial->velocidad;   /* K/m = v^2/2 */

    printf("\n=== 1.c TRABAJO HASTA EL PUNTO DE EQUILIBRIO TIERRA-LUNA ===\n"
           "Formulas utilizadas (D = distancia Tierra-Luna, r0 = distancia inicial,\n"
           "M_T = masa de la Tierra, M_L = masa de la Luna, v = velocidad de inyeccion):\n"
           "  Punto de equilibrio      : r_eq = D / (1 + sqrt(M_L/M_T))\n"
           "  Trabajo minimo por kg    : W/m = G*M_T*(1/r0 - 1/r_eq)\n"
           "  Energia cinetica por kg  : K/m = v^2/2\n\n");

    printf("Distancia inicial (r0)        : %.1f km\n", inicial->distancia / 1000.0);
    printf("Punto de equilibrio (r_eq)    : %.1f km del centro de la Tierra\n", rEq / 1000.0);
    printf("Trabajo minimo total          : %.3e J/kg\n", w);
    printf("Energia cinetica de la TLI    : %.3e J/kg\n", kPorKg);

    if (w > kPorKg) {
        printf("Trabajo adicional que falta   : %.3e J/kg\n", w - kPorKg);
        printf("(Con solo la inyeccion, Orion no llega al punto de equilibrio.)\n");
    } else {
        printf("La TLI alcanza: sobran %.3e J/kg para llegar al punto de equilibrio.\n",
               kPorKg - w);
    }
    printf("PD: Modelo simplificado, solo se considera el potencial terrestre.\n");
}

void mostrarHistorial(const Nave historial[], int cantidad) {
    int i;

    printf("\nTramo | Distancia (km) | Velocidad (m/s) | Aceleracion (m/s^2)\n");
    for (i = 0; i < cantidad; i++) {
        printf("%5d | %14.1f | %15.2f | %19.5f\n", i, historial[i].distancia / 1000.0,
               historial[i].velocidad, historial[i].aceleracion);
    }
}

void mostrarResultadoFinal(const Nave *final, const Nave *inicial) {
    double vEsc = velocidadEscape(MASA_TIERRA, inicial->distancia);

    if (final->velocidad == 0.0) {
        printf("\nLa gravedad detuvo a Orion a %.1f km del centro de la Tierra.\n",
               final->distancia / 1000.0);
        printf("Motivo: partio de %.1f km con %.2f m/s, pero la velocidad de escape a esa\n"
               "distancia es %.2f m/s (le faltaron %.2f m/s).\n",
               inicial->distancia / 1000.0, inicial->velocidad, vEsc, vEsc - inicial->velocidad);
        printf("(v_esc = sqrt(2*G*M/r), con M = masa de la Tierra y r = distancia inicial:\n"
               "no depende de la masa de Orion.)\n");
    } else {
        printf("\nOrion alcanzo la distancia de la Luna con %.2f m/s.\n", final->velocidad);
    }
}

void mostrarJustificacionEnergetica(const Nave *nave) {
    double masas[2]  = {MASA_TIERRA, MASA_LUNA};
    double radios[2] = {RADIO_TIERRA, RADIO_LUNA};
    double pozo[2], vEsc[2], energia[2], comb[2];
    int i;

    for (i = 0; i < 2; i++) {
        pozo[i]    = pozoPotencial(masas[i], radios[i]);
        vEsc[i]    = velocidadEscape(masas[i], radios[i]);
        energia[i] = energiaEscape(nave->masa, masas[i], radios[i]);
        comb[i]    = combustibleNecesario(nave->masa, vEsc[i]);
    }

    printf("\n=== 1.d JUSTIFICACION ENERGETICA: IDA Y REGRESO ===\n"
           "Formulas utilizadas (G = constante de gravitacion universal, R = radio,\n"
           "M = masa del cuerpo, m = masa de Orion, ve = velocidad de salida de los gases):\n"
           "  Pozo de potencial     : U = G*M/R\n"
           "  Velocidad de escape   : v_esc = sqrt(2*G*M/R)\n"
           "  Energia para escapar  : E = m*G*M/R\n"
           "  Combustible (cohete)  : m_comb = m*(e^(v_esc/ve) - 1)\n\n");

    printf("Masa de Orion usada: %.0f kg\n", nave->masa);
    printf("%-28s | %12s | %12s\n", "Tema a analizar", "Tierra", "Luna");
    printf("%-28s | %12.1f | %12.1f\n", "Radio (km)", radios[0] / 1000.0, radios[1] / 1000.0);
    printf("%-28s | %12.3e | %12.3e\n", "Pozo de potencial (J/kg)", pozo[0], pozo[1]);
    printf("%-28s | %12.1f | %12.1f\n", "Velocidad de escape (m/s)", vEsc[0], vEsc[1]);
    printf("%-28s | %12.3e | %12.3e\n", "Energia para escapar (J)", energia[0], energia[1]);

    printf("\nCombustible estimado (ve = %.0f m/s):\n", VE_COMBUSTIBLE);
    printf("  Ida (escapar de la Tierra):  %9.0f kg de %s\n", comb[0], NOMBRE_COMBUSTIBLE);
    printf("  Vuelta (escapar de la Luna): %9.0f kg de %s\n", comb[1], NOMBRE_COMBUSTIBLE);
    printf("  La ida requiere %.1f veces mas combustible que la vuelta.\n", comb[0] / comb[1]);

    printf("\nAyudas al regreso:\n"
           "  - La gravedad terrestre acelera a Orion durante la caida hacia la Tierra.\n"
           "  - La atmosfera frena la capsula sin gastar combustible (escenario 4).\n"
           "PD: Modelo simplificado sin perdidas gravitatorias ni maniobras reales.\n");
}

void ejecutarAnalisisFisico(void) {
    Nave orion;
    Nave historial[MAX_TRAMOS];
    int cantidad;

    mostrarIntroduccion();
    cargarNave(&orion);
    cantidad = simularInercia(&orion, historial, MAX_TRAMOS);
    mostrarHistorial(historial, cantidad);
    mostrarResultadoFinal(&orion, &historial[0]);
    mostrarEnergiaCinetica(&historial[0]);   /* historial[0] = estado inicial */
    mostrarTrabajoEquilibrio(&historial[0]); /* item c: usa el estado inicial */
    mostrarJustificacionEnergetica(&orion);
}
