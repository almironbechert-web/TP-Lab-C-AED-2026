Algoritmos y Estructuras de Datos - Ingeniería en Sistemas de Información

<br> 

<div align="center">
	
# TRABAJO PRÁCTICO INTEGRADOR FINAL - Lenguaje C
## Simulador de Misiones Artemis 

</div>
 
<br>

#### Integrantes del grupo:
1. Almirón Bechert, Gabriel Alejo - 30209
2. Bernardis, Ignacio Ezequiel - 28912
3. Gauna, Victor Sebastián - 30116
4. Morra Beneyto, Noel Victoria - 30841
5. Segovia, Agostina Victoria Jaqueline - 30056

#### Comisión: K1.3

#### Escenario asignado: 3

<br> 

## Descripción del escenario asignado y reglas del juego
### Escenario 3: Inyección Translunar (TLI) e Inercia en Trayectoria Libre

<div align="justify">

El proyecto consiste en el desarrollo de un simulador interactivo , basado en el tercer escenario: la Inyección Translunar (TLI) y el movimiento de la nave Orion bajo la influencia de la gravedad.

La simulación comienza cuando Orion se encuentra en una órbita terrestre baja (LEO). Para iniciar su viaje hacia la Luna, debe realizar una maniobra de propulsión que le permita alcanzar una velocidad de inyección adecuada. Una vez completada esta maniobra, los motores se apagan y la nave continúa desplazándose bajo la acción gravitatoria.

El programa utiliza como referencia los siguientes datos físicos:

<div align="center">

| Magnitud | Valor |
|---|---|
| Masa de Orion | 26.000 kg |
| Velocidad de inyección translunar | 10.900 m/s |
| Altura orbital inicial | 192 km |
| Distancia media Tierra-Luna | 384.000 km |
| Radio terrestre | 6.371 km |
| Masa terrestre | 5,972 × 10²⁴ kg |
</div>

El simulador emplea un modelo físico simplificado que considera la gravedad terrestre durante el desplazamiento de Orion, sin incorporar la influencia de la gravedad lunar sobre su trayectoria.

### Objetivo del juego

El usuario asume el rol de director de vuelo de la misión Artemis. Su objetivo consiste en seleccionar una velocidad de inyección que permita a Orion alcanzar la distancia de la Luna sin superar la velocidad de escape terrestre.

Dentro del modelo implementado, este resultado se denomina **retorno libre** y representa el éxito de la misión.

### Reglas del juego

1. **Configuración inicial:** al comenzar una partida, el usuario puede utilizar la altura orbital predeterminada de 192 km o ingresar una diferente, entre 160.000 y 2.000.000 de metros.

2. **Cantidad de intentos:** el jugador dispone de un máximo de **3 turnos** para completar la misión.

3. **Velocidad de inyección:** en cada turno, debe ingresar una velocidad comprendida entre **9.000 y 13.000 m/s**.

4. **Simulación del desplazamiento:** el programa calcula el movimiento de Orion por tramos de 20.000 km, actualizando su velocidad y aceleración gravitatoria según la distancia respecto del centro de la Tierra.

5. **Evaluación del intento:** después de cada simulación, el sistema clasifica el resultado en una de las siguientes categorías:

   - **Insuficiente:** Orion se detiene antes de alcanzar la distancia de la Luna.
   - **Retorno libre:** Orion llega a la distancia lunar sin alcanzar la velocidad de escape terrestre. Se considera una misión exitosa.
   - **Escape:** Orion alcanza o supera la velocidad de escape terrestre y deja de permanecer gravitatoriamente ligada a la Tierra.

6. **Nuevos intentos:** si el resultado no es exitoso, el programa informa lo ocurrido y proporciona una orientación para modificar la velocidad ingresada.

7. **Finalización:** la partida termina cuando se consigue el retorno libre o se agotan los tres intentos disponibles.

### Sistema de puntuación

El puntaje depende de la cantidad de intentos necesarios para completar la misión.

<div align="center">

| Resultado | Puntaje | Rango |
|---|---:|---|
| Éxito en el primer intento | 1.000 puntos | Director de vuelo |
| Éxito en el segundo intento | 700 puntos | Ingeniero de trayectoria |
| Éxito en el tercer intento | 400 puntos | Cadete |
| Misión fallida | 0 puntos | Misión fallida |

</div>

El sistema aplica una penalización de 300 puntos por cada intento adicional utilizado antes de alcanzar el objetivo.

### Reporte final

Al finalizar la partida, el simulador presenta un reporte que incluye la altura orbital de partida, las velocidades ingresadas, los resultados de cada turno, las distancias alcanzadas, la ventana teórica de velocidades de retorno libre y la puntuación final.

El reporte también puede consultarse posteriormente desde el menú principal, siempre que se haya ejecutado una misión.

<br>

## Funcionamiento del programa

### Organización general

Su funcionamiento combina dos modalidades: un juego de simulación por turnos y un apartado de análisis físico relacionado con el escenario asignado. El programa utiliza estructuras condicionales y repetitivas, funciones, arreglos, estructuras de datos y punteros para organizar sus operaciones.

### Menú principal

Al iniciar la ejecución, el programa inicializa una estructura de tipo `Mision` y presenta un menú con las siguientes opciones:

1. **Jugar misión:** inicia una partida interactiva en la que el usuario intenta seleccionar la velocidad de inyección adecuada.
2. **Análisis físico:** ejecuta los cálculos correspondientes a los incisos a, b, c y d del Escenario 3.
3. **Ver reporte de la última misión:** muestra la información de la partida realizada anteriormente.
4. **Reglas del juego:** presenta el objetivo y las condiciones de la simulación.
5. **Salir:** finaliza el programa.

El menú está implementado mediante un ciclo `do...while`, que permite realizar diferentes operaciones hasta seleccionar la opción de salida. La selección de cada alternativa se realiza mediante una estructura `switch`.

### Funcionamiento del juego

Al seleccionar la opción **Jugar misión**, se ejecuta la función `jugarMision()`.

En primer lugar, se reinician los datos de la partida y se solicita al usuario que confirme si desea utilizar la altura orbital predeterminada o ingresar un valor diferente. Posteriormente, el programa calcula la velocidad mínima necesaria para alcanzar la distancia de la Luna y la velocidad de escape terrestre correspondiente a la órbita seleccionada.

En cada turno, el usuario ingresa una velocidad de inyección. Con estos datos, se inicializa el estado de la nave y se ejecuta la función `simularInercia()`, encargada de calcular su desplazamiento. La simulación utiliza un arreglo denominado `historial`, que almacena los estados sucesivos de Orion. Después de cada intento, se determina si la velocidad fue insuficiente, si se consiguió el retorno libre o si se produjo una situación de escape. El ciclo de juego continúa mientras queden intentos disponibles y la misión no haya sido completada. Finalmente, se calcula el puntaje obtenido y se genera el reporte correspondiente.

### Análisis físico

La opción **Análisis físico** ejecuta la función `ejecutarAnalisisFisico()`, que coordina los diferentes cálculos implementados en el módulo `artemis.c`.

Inicialmente, el programa permite utilizar los datos del enunciado o ingresar valores personalizados de masa, velocidad y altura orbital.

A continuación, simula el desplazamiento de Orion y muestra una tabla con la distancia, la velocidad y la aceleración gravitatoria calculadas en cada tramo.

El análisis contempla los siguientes apartados:

**- Inciso a — Primera Ley de Newton e inercia**

Explica el comportamiento de Orion después del apagado de los motores, considerando que la nave continúa desplazándose debido a su velocidad inicial mientras la gravedad terrestre modifica su movimiento.

La velocidad se calcula mediante conservación de la energía mecánica y la aceleración gravitatoria se determina en función de la distancia al centro terrestre.

**- Inciso b — Energía cinética inicial**

Calcula la energía cinética de Orion utilizando la expresión:

<div align="center">

K = ½ · m · v²
</div>

Donde `m` representa la masa de la nave y `v` su velocidad inicial de inyección.

**- Inciso c — Trabajo hasta el punto de equilibrio gravitatorio**

Calcula la posición del punto de equilibrio gravitatorio Tierra-Luna utilizando las masas de ambos cuerpos y su distancia media. Posteriormente, determina la variación de energía potencial terrestre por unidad de masa necesaria para alcanzar ese punto desde la posición inicial.

El resultado se compara con la energía cinética inicial por unidad de masa para evaluar si, según el modelo simplificado, la energía disponible resulta suficiente.

**- Inciso d — Justificación energética de ida y regreso**

Realiza una comparación entre la Tierra y la Luna a partir de sus características gravitatorias. El programa calcula las velocidades de escape, las energías asociadas y estimaciones teóricas de combustible mediante una forma simplificada de la ecuación de Tsiolkovsky.

También presenta una explicación del papel de la gravedad terrestre y del frenado atmosférico durante el regreso. Estas estimaciones representan un modelo educativo simplificado y no un cálculo completo del combustible requerido por una misión real.

### Estructuras de datos utilizadas

El programa utiliza diferentes estructuras de datos para representar los elementos de la simulación.

- **`Nave`:** almacena la masa, velocidad, distancia al centro de la Tierra y aceleración gravitatoria.
- **`Intento`:** registra la velocidad seleccionada, la distancia alcanzada y el resultado obtenido en un turno.
- **`Mision`:** almacena el historial de intentos, la cantidad utilizada, el puntaje, el estado de éxito y las velocidades de referencia.
- **`Resultado`:** tipo enumerado que representa los estados posibles de un intento.

También se utilizan arreglos de estructuras para almacenar los datos correspondientes a los distintos tramos de la simulación y a los intentos realizados.

El uso de punteros permite que las funciones reciban y modifiquen directamente los datos de las estructuras cuando resulta necesario.

### Validaciones de entrada

El programa cuenta con un módulo específico de validaciones, implementado en `validaciones.c`. Las funciones de este módulo permiten controlar los valores ingresados por el usuario y evitar errores comunes durante la ejecución.

- **`leerLinea()`:** utiliza `fgets()` para obtener los datos ingresados mediante consola.
- **`leerEntero()`:** convierte la entrada a un número entero mediante `strtol()` y verifica que pertenezca al rango permitido.
- **`leerDouble()`:** convierte la entrada mediante `strtod()` y controla el rango establecido.
- **`leerSiNo()`:** permite responder únicamente con las letras `s` o `n`, aceptando mayúsculas y minúsculas.
- **`esperarEnter()`:** permite pausar la interacción hasta que el usuario presione Enter.

Cuando se detecta una entrada inválida, las funciones de validación muestran un mensaje de error y vuelven a solicitar el dato. Estas verificaciones se utilizan en la selección de opciones del menú, el ingreso de parámetros físicos y la configuración de la simulación.

### Modularización

El proyecto se encuentra dividido en diferentes archivos, cada uno con responsabilidades específicas:

<div align="center">

| Archivo | Responsabilidad |
|---|---|
| `main.c` | Control del menú principal y flujo general del programa. |
| `artemis.c` | Implementación de los cálculos físicos y la simulación del movimiento. |
| `artemis.h` | Declaración de constantes físicas, estructuras y prototipos de funciones. |
| `juego.c` | Desarrollo de la dinámica del juego, intentos, puntuación y reportes. |
| `juego.h` | Definición de reglas, estructuras y declaraciones del módulo del juego. |
| `validaciones.c` | Implementación de las funciones de ingreso y validación de datos. |
| `validaciones.h` | Declaraciones necesarias para utilizar las funciones de validación. |

</div>

Esta organización permite separar responsabilidades, evitar la duplicación innecesaria de código y facilitar el mantenimiento del sistema.

<br>

## Instrucciones de compilación y ejecución

### Requisitos previos

Para compilar y ejecutar el simulador Artemis es necesario contar con:

- Un compilador de lenguaje C, como **GCC**.
- La herramienta **GNU Make**, para utilizar el archivo `Makefile` incluido en el proyecto.
- Una terminal o consola de comandos.

El proyecto utiliza el estándar C99 y la biblioteca matemática de C.

### Compilación mediante Makefile

1. Obtener los archivos del repositorio y ubicarlos en el mismo directorio.
2. Abrir una terminal en la carpeta principal del proyecto.
3. Ejecutar el comando:

   `make`

El archivo `Makefile` compila los módulos `main.c`, `artemis.c`, `juego.c` y `validaciones.c`, generando sus correspondientes archivos objeto. Posteriormente, enlaza los módulos y genera el ejecutable.

La compilación utiliza las opciones `-Wall`, `-Wextra` y `-std=c99`, junto con `-lm` para enlazar la biblioteca matemática. En algunas instalaciones de Windows con MinGW, la herramienta de compilación puede denominarse `mingw32-make`. En ese caso, se debe utilizar: `mingw32-make`

### Ejecución

Una vez compilado el programa, ejecutar el archivo generado según el sistema operativo.

**Windows (PowerShell):**

`.\artemis.exe`

**Linux:**

`./artemis`

Al iniciarse, el programa muestra el menú principal y permite comenzar una misión, realizar análisis físicos, consultar reportes y visualizar las reglas del juego.

### Limpieza de archivos compilados

Para eliminar los archivos objeto y el ejecutable generado, utilizar:

`make clean`

Este comando permite limpiar los archivos de compilación sin eliminar el código fuente.

### Compilación manual alternativa

Si no se dispone de GNU Make, el proyecto puede compilarse directamente mediante GCC.

**Windows:**

`gcc -Wall -Wextra -std=c99 main.c artemis.c juego.c validaciones.c -o artemis.exe -lm`

**Linux:**

`gcc -Wall -Wextra -std=c99 main.c artemis.c juego.c validaciones.c -o artemis -lm`

Una vez compilado, ejecutar el archivo generado utilizando los comandos indicados anteriormente.

<br>

##  Registro detallado del uso de Inteligencia Artificial.
#### IAs utilizadas: ChatGPT y Claude.
 
#### Identificación de posibles errores y mejoras

Se realizaron consultas para detectar aspectos del código que pudieran requerir correcciones, optimizaciones o aclaraciones.

Entre las observaciones obtenidas se encuentran:

1. La omisión del inciso C en el texto del menú principal, aunque su función ya estaba implementada.

2. La necesidad de verificar posibles caracteres adicionales que pueden generar errores a la hora de la ejecución.

3. La posibilidad de mejorar la subacción `leerDouble()` para rechazar valores no finitos, como NaN e infinito, mediante `isfinite()`.

4. La importancia de aclarar las simplificaciones físicas utilizadas en los cálculos de energía y trayectoria.

<br>


#### Elaboración del archivo README.md

Se utilizó como apoyo para mejorar la redaccón y organizar los apartados correspondientes a la descripción del escenario, las reglas del juego, el funcionamiento del programa y las instrucciones de compilación y ejecución.

La documentación se elaboró a partir del código fuente compartido, procurando que las explicaciones reflejaran las funcionalidades implementadas.

También se realizaron consultas sobre Markdown para mejorar la presentación del documento en GitHub.

<br>

#### Verificación y adaptación de las respuestas

Las respuestas proporcionadas por la herramienta se contrastaron con los requisitos establecidos en la consigna y con el contenido de los archivos del proyecto.

Las sugerencias se analizaron considerando la estructura modular existente, con el objetivo de evitar modificaciones innecesarias y mantener la coherencia entre los distintos componentes del programa.

Se diferenciaron las funcionalidades efectivamente implementadas de las propuestas de mejora, así como las simplificaciones utilizadas en el modelo físico.
</div>



