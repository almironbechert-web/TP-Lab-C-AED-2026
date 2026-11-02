#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#define LARGO_ENTRADA 100

/* Lee una linea de stdin sin el salto de linea. Termina el programa si falla. */
void   leerLinea(char *entrada);

/* Pide un numero real hasta que sea valido y este dentro de [min, max]. */
double leerDouble(const char *mensaje, double min, double max);

/* Pide 's' o 'n' (mayuscula o minuscula). Devuelve siempre 's' o 'n'. */
char   leerSiNo(const char *mensaje);

/* Muestra un mensaje y espera a que el usuario presione Enter. */
void   esperarEnter(const char *mensaje);

#endif
