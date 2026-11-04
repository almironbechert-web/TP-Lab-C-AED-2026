#include <stdio.h>
#include "artemis.h"
#include "validaciones.h"

int main(void) {
    Nave orion;
    Nave historial[MAX_TURNOS];
    int cantidad;

    mostrarIntroduccion();
    cargarNave(&orion);
    mostrarEnergiaCinetica(&orion);   /* antes de simular: la simulacion modifica orion */
    cantidad = simularInercia(&orion, historial, MAX_TURNOS);
    mostrarHistorial(historial, cantidad);
    mostrarResultadoFinal(&orion);
    mostrarJustificacionEnergetica(&orion);

    esperarEnter("\nPresione Enter para salir...");
    return 0;
}
