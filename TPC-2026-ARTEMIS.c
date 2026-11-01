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

/* ===== Estructuras ===== */
typedef struct {
    double masa;
    double velocidad;
    double distancia;
    double aceleracion;
} Nave;

typedef struct {
    int    turno;
    double distancia;
    double velocidad;
    double aceleracion;
} RegistroTurno;

/* ===== Variables de main ===== */
Nave orion;
RegistroTurno historial[MAX_TURNOS];
int cantidad;

/* ===== Variables de simularInercia ===== */
double v0 = nave->velocidad;
double r0 = nave->distancia;
int turno = 0;
int detenida = 0;
double nuevaDistancia;
double nuevaVelocidad;