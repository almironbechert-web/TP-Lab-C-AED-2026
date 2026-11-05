#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "validaciones.h"
#include <math.h>

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
        if (fin == entrada || *fin != '\0'|| !isfinite(valor)) {
            printf("ERROR: ingrese solo un numero.\n");
        } else if (valor < min || valor > max) {
            printf("ERROR: valor fuera de rango.\n");
        } else {
            return valor;
        }
    }
}

int leerEntero(const char *mensaje, int min, int max) {
    char entrada[LARGO_ENTRADA], *fin;
    long valor;

    while (1) {
        printf("%s [%d - %d]: ", mensaje, min, max);
        leerLinea(entrada);
        valor = strtol(entrada, &fin, 10);
        if (fin == entrada || *fin != '\0') {
            printf("ERROR: ingrese solo un numero entero.\n");
        } else if (valor < min || valor > max) {
            printf("ERROR: valor fuera de rango.\n");
        } else {
            return (int)valor;
        }
    }
}

char leerSiNo(const char *mensaje) {
    char entrada[LARGO_ENTRADA];

    while (1) {
        printf("%s (s/n): ", mensaje);
        leerLinea(entrada);
        if (strlen(entrada) == 1 && strchr("sSnN", entrada[0]) != NULL) {
            return (char)tolower((unsigned char)entrada[0]);
        }
        printf("ERROR: responda 's' o 'n'.\n");
    }
}

void esperarEnter(const char *mensaje) {
    char descarte[LARGO_ENTRADA];

    printf("%s", mensaje);
    leerLinea(descarte);
}
