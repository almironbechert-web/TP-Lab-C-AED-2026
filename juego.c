#include <stdio.h>
#include <math.h>
#include "juego.h"
#include "validaciones.h"

/* ===== Auxiliares internas ===== */

/* Velocidad minima desde r0 para llegar a la distancia de la Luna:
   v = sqrt(2*G*M*(1/r0 - 1/D)) */
static double velocidadMinimaLuna(double r0) {
    return sqrt(2.0 * G * MASA_TIERRA * (1.0 / r0 - 1.0 / DISTANCIA_TL));
}

static Resultado clasificar(const Nave *final, double v0, double vEscape) {
    if (final->velocidad == 0.0) {
        return RES_INSUFICIENTE;
    }
    return (v0 >= vEscape) ? RES_ESCAPE : RES_RETORNO_LIBRE;
}

static int calcularPuntaje(int exito, int intentosUsados) {
    if (!exito) {
        return 0;
    }
    return PUNTAJE_MAX - PENALIZACION * (intentosUsados - 1);
}

static const char *rangoSegunPuntaje(int puntaje) {
    if (puntaje >= PUNTAJE_MAX)                      return "Director de vuelo";
    if (puntaje >= PUNTAJE_MAX - PENALIZACION)       return "Ingeniero de trayectoria";
    if (puntaje > 0)                                 return "Cadete";
    return "Mision fallida";
}

static const char *nombreResultado(Resultado r) {
    switch (r) {
        case RES_INSUFICIENTE:  return "Insuficiente";
        case RES_RETORNO_LIBRE: return "Retorno libre";
        case RES_ESCAPE:        return "Escape";
        default:                return "(no usado)";
    }
}

static void mostrarMensajeIntento(Resultado r, const Nave *final) {
    printf("\n");
    switch (r) {
        case RES_INSUFICIENTE:
            printf("Orion se detuvo a %.1f km del centro de la Tierra, antes de llegar a la\n"
                   "Luna (%.0f km). Pista: hace falta MAS velocidad.\n",
                   final->distancia / 1000.0, DISTANCIA_TL / 1000.0);
            break;
        case RES_ESCAPE:
            printf("Orion supero la velocidad de escape: se perderia en el espacio y no\n"
                   "volveria a la Tierra. Pista: hace falta MENOS velocidad.\n");
            break;
        case RES_RETORNO_LIBRE:
            printf("EXITO: Orion llego a la distancia de la Luna y sigue ligada a la Tierra.\n"
                   "La gravedad la traera de vuelta: es un retorno libre!\n");
            break;
        default:
            break;
    }
}

/* ===== Interfaz publica ===== */

void inicializarMision(Mision *mision) {
    int i;

    for (i = 0; i < MAX_INTENTOS; i++) {
        mision->intentos[i].velocidadInyeccion = 0.0;
        mision->intentos[i].distanciaAlcanzada = 0.0;
        mision->intentos[i].resultado = RES_SIN_USAR;
    }
    mision->cantidadIntentos = 0;
    mision->exito = 0;
    mision->puntaje = 0;
    mision->alturaOrbita = ALTURA_LEO;
    mision->velocidadMinima = 0.0;
    mision->velocidadEscape = 0.0;
}

void jugarMision(Mision *mision) {
    Nave nave, historial[MAX_TRAMOS];
    double r0, v0;
    int turno, cantidad;
    Resultado res;

    inicializarMision(mision);
    printf("\n=== MISION: INYECCION TRANSLUNAR ===\n"
           "Objetivo: elegir la velocidad de inyeccion para que Orion llegue a la Luna\n"
           "y siga ligada a la Tierra (retorno libre). Tenes %d turnos.\n\n", MAX_INTENTOS);

    if (leerSiNo("Usar la orbita del enunciado (192 km de altura)?") == 'n') {
        mision->alturaOrbita = leerDouble("Altura de la orbita baja (m)", 160000, 2000000);
    }
    r0 = RADIO_TIERRA + mision->alturaOrbita;
    mision->velocidadMinima = velocidadMinimaLuna(r0);
    mision->velocidadEscape = velocidadEscape(MASA_TIERRA, r0);

    for (turno = 0; turno < MAX_INTENTOS && !mision->exito; turno++) {
        printf("\n=== TURNO %d de %d ===\n", turno + 1, MAX_INTENTOS);
        printf("Referencia: el enunciado propone %.0f m/s.\n", V_TLI);
        v0 = leerDouble("Velocidad de inyeccion (m/s)", VEL_MIN_JUEGO, VEL_MAX_JUEGO);

        nave.masa = MASA_ORION;
        nave.velocidad = v0;
        nave.distancia = r0;
        nave.aceleracion = aceleracionGravitatoria(r0);
        cantidad = simularInercia(&nave, historial, MAX_TRAMOS);
        mostrarHistorial(historial, cantidad);

        res = clasificar(&nave, v0, mision->velocidadEscape);
        mision->intentos[turno].velocidadInyeccion = v0;
        mision->intentos[turno].distanciaAlcanzada = nave.distancia;
        mision->intentos[turno].resultado = res;
        mision->cantidadIntentos = turno + 1;
        mision->exito = (res == RES_RETORNO_LIBRE);
        mostrarMensajeIntento(res, &nave);
    }

    mision->puntaje = calcularPuntaje(mision->exito, mision->cantidadIntentos);
    mostrarReporte(mision);
}

void mostrarReporte(const Mision *mision) {
    int i;
    const Intento *it;

    printf("\n=== REPORTE DE MISION ===\n");
    printf("Orbita de partida: %.0f km de altura\n\n", mision->alturaOrbita / 1000.0);
    printf("Turno | Vel. inyeccion (m/s) | Resultado     | Dist. alcanzada (km)\n");
    for (i = 0; i < MAX_INTENTOS; i++) {
        it = &mision->intentos[i];
        if (it->resultado == RES_SIN_USAR) {
            printf("%5d | %20s | %-13s | %20s\n", i + 1, "-", nombreResultado(it->resultado), "-");
        } else {
            printf("%5d | %20.1f | %-13s | %20.1f\n", i + 1, it->velocidadInyeccion,
                   nombreResultado(it->resultado), it->distanciaAlcanzada / 1000.0);
        }
    }
    printf("\nVentana teorica de retorno libre: %.1f - %.1f m/s (margen: %.1f m/s)\n",
           mision->velocidadMinima, mision->velocidadEscape,
           mision->velocidadEscape - mision->velocidadMinima);
    printf("Estado : %s\n", mision->exito ? "MISION CUMPLIDA" : "MISION FALLIDA");
    printf("Puntaje: %d / %d   Rango: %s\n", mision->puntaje, PUNTAJE_MAX,
           rangoSegunPuntaje(mision->puntaje));
}

void mostrarReglas(void) {
    printf("\n=== REGLAS DEL JUEGO ===\n"
           "Sos el director de vuelo. Orion esta en orbita baja y debe partir hacia la Luna.\n"
           "En cada turno elegis la velocidad de inyeccion y se simula el vuelo tramo a tramo\n"
           "(un tramo = %.0f km). Tenes %d turnos para lograr el retorno libre.\n\n"
           "Resultados posibles:\n"
           "  Insuficiente  : la gravedad detiene a Orion antes de llegar a la Luna.\n"
           "  Retorno libre : llega a la distancia de la Luna sin superar la velocidad de\n"
           "                  escape, asi que sigue ligada a la Tierra y vuelve sola. EXITO.\n"
           "  Escape        : supera la velocidad de escape y no vuelve.\n\n"
           "Puntaje: %d si lo logras en el turno 1, y se restan %d por cada turno extra.\n"
           "Si no lo logras, 0 puntos. Al final se muestra la ventana de velocidades correcta.\n\n"
           "PD: Modelo simplificado: solo actua la gravedad de la Tierra (sin la de la Luna).\n",
           PASO_DISTANCIA / 1000.0, MAX_INTENTOS, PUNTAJE_MAX, PENALIZACION);
}
