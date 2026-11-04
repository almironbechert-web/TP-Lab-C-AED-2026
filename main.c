#include <stdio.h>
#include "artemis.h"
#include "juego.h"
#include "validaciones.h"

int main(void) {
    Mision ultimaMision;
    int hayMision = 0;
    int opcion;

    inicializarMision(&ultimaMision);
    do {
        printf("\n=== ARTEMIS - INYECCION TRANSLUNAR ===\n"
               "1. Jugar mision\n"
               "2. Analisis fisico (items a, b y d)\n"
               "3. Ver reporte de la ultima mision\n"
               "4. Reglas del juego\n"
               "0. Salir\n");
        opcion = leerEntero("Opcion", 0, 4);

        switch (opcion) {
            case 1:
                jugarMision(&ultimaMision);
                hayMision = 1;
                break;
            case 2:
                ejecutarAnalisisFisico();
                break;
            case 3:
                if (hayMision) {
                    mostrarReporte(&ultimaMision);
                } else {
                    printf("\nTodavia no jugaste ninguna mision.\n");
                }
                break;
            case 4:
                mostrarReglas();
                break;
            default:
                printf("\nFin del programa. Hasta la proxima!\n");
                break;
        }
        if (opcion != 0) {
            esperarEnter("\nPresione Enter para volver al menu...");
        }
    } while (opcion != 0);

    return 0;
}
