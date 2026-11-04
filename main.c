#include <stdio.h>
#include "artemis.h"
#include "validaciones.h"

int main(void) {
    Nave orion;
    Nave historial[MAX_TURNOS];
    int cantidad;

    mostrarIntroduccion();
    cargarNave(&orion);
    cantidad = simularInercia(&orion, historial, MAX_TURNOS);
    mostrarHistorial(historial, cantidad);
    mostrarResultadoFinal(&orion, &historial[0]);
    mostrarEnergiaCinetica(&historial[0]);   /* historial[0] = estado inicial de la nave */
    mostrarJustificacionEnergetica(&orion);

    esperarEnter("\nPresione Enter para salir...");
    return 0;
}
