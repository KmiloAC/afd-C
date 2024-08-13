#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#define MAX_ESTADOS 10
#define MAX_SIMBOLOS 10
#define NOMBRE_MAX 10

char estados[MAX_ESTADOS][NOMBRE_MAX];
char alfabeto[MAX_SIMBOLOS][NOMBRE_MAX];
char estado_inicial[NOMBRE_MAX];
char estados_finales[MAX_ESTADOS][NOMBRE_MAX];
char matriz_transicion[MAX_ESTADOS][MAX_SIMBOLOS][NOMBRE_MAX];

int num_estados = 0;
int num_simbolos = 0;
int num_estados_finales = 0;


// 

int obtenerIndice(const char *elemento, char arreglo[][NOMBRE_MAX], int num_elementos) {
    for (int i = 0; i < num_elementos; i++) {
        if (strcmp(elemento, arreglo[i]) == 0) {
            return i; // Devuelve el índice si encuentra el elemento
        }
    }
    return -1; // Devuelve -1 si no encuentra el elemento
}

int esEstadoFinal(const char *estado) {
    for (int i = 0; i < num_estados_finales; i++) {
        if (strcmp(estados_finales[i], estado) == 0) {
            return 1; // Verdadero si el estado es final    q
        }
    }
    return 0; // Falso si el estado no es final
}


void procesarCadena(const char *cadena) {
    char estado_actual[NOMBRE_MAX];
    strcpy(estado_actual, estado_inicial); // Comienza con el estado inicial

    int i = 0;
    while (cadena[i] != '\0') {
        int estadoIndex = obtenerIndice(estado_actual, estados, num_estados);
        char simbolo_actual[NOMBRE_MAX];

        if (isdigit(cadena[i])) {
            // Leer la secuencia numérica completa
            int j = 0;
            while (isdigit(cadena[i]) && j < NOMBRE_MAX - 1) {
                simbolo_actual[j++] = cadena[i++];
            }
            simbolo_actual[j] = '\0'; // Terminar la cadena
        } else {
            snprintf(simbolo_actual, NOMBRE_MAX, "%c", cadena[i]);  // Convertir el carácter actual a string
            i++;  // Avanzar al siguiente carácter
        }
        
        int simboloIndex = obtenerIndice(simbolo_actual, alfabeto, num_simbolos);

        printf("Estado actual: %s, Símbolo: %s\n", estado_actual, simbolo_actual);

        if (estadoIndex != -1 && simboloIndex != -1) {
            char siguiente_estado[NOMBRE_MAX];
            strcpy(siguiente_estado, matriz_transicion[estadoIndex][simboloIndex]);

            if (siguiente_estado[0] == '\0') {
                // No hay transición definida
                printf("Cadena rechazada (sin transición desde el estado %s con símbolo %s)\n", estado_actual, simbolo_actual);
                return;
            }

            printf("Siguiente estado: %s\n", siguiente_estado);
            strcpy(estado_actual, siguiente_estado);
        } else {
            printf("Cadena rechazada (estado o símbolo no encontrado)\n");
            return;
        }

        // Saltar comas después de procesar un símbolo o número
        while (cadena[i] == ',') {
            i++;
        }
    }

    // Verificar si el estado actual es final
    if (esEstadoFinal(estado_actual)) {
        printf("Cadena aceptada\n");
    } else {
        printf("Cadena rechazada (no se llegó a un estado final)\n");
    }
}




void readConfig(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("No se pudo abrir el archivo de configuración");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    char *token;
    int line = 0;
    int num_lineas = 0;

    // Contar líneas no vacías
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0; // Eliminar el salto de línea
        if (strlen(buffer) != 0) {
            num_lineas++;
        }
    }

    // Volver al inicio del archivo para lectura
    rewind(file);

    // Verificar que haya al menos 5 líneas no vacías
    if (num_lineas < 5) {
        printf("El archivo debe tener al menos 5 líneas.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0; // Eliminar el salto de línea
        if (strlen(buffer) == 0) {
            continue; // Omitir líneas vacías
        }

        if (line == 0) {
            // Leer estados
            token = strtok(buffer, ",");
            while (token != NULL) {
                strcpy(estados[num_estados++], token);
                token = strtok(NULL, ",");
            }
        } else if (line == 1) {
            // Leer alfabeto
            token = strtok(buffer, ",");
            while (token) {
                strcpy(alfabeto[num_simbolos++], token);
                token = strtok(NULL, ",");
            }
        } else if (line == 2) {
            // Leer estado inicial
            strcpy(estado_inicial, buffer);
            printf("El estado inicial es: %s\n", estado_inicial);
            for (int i = 0; i < num_estados; i++) {
                // Print each string in the row
                printf("%s\n", estados[i]);
            }
            while (obtenerIndice(estado_inicial, estados, num_estados) == -1) {
                printf("Obtenido: \n");
                printf("%s   ", estado_inicial);
                printf("no está en la lista de estados.\n");
                printf("\n Ingrese un estado inicial válido: ");
                scanf("%s", estado_inicial);
                estado_inicial[strcspn(estado_inicial, "\n")] = 0;
                printf("%d\t ", obtenerIndice(estado_inicial, estados, num_estados));
            }
        } else if (line == 3) {
            // Leer estados finales
            token = strtok(buffer, ",");
            while (token) {
                if (obtenerIndice(token, estados, num_estados) != -1) {
                    strcpy(estados_finales[num_estados_finales++], token);
                } else {
                    printf("\n Advertencia: Estado final '%s' no está en la lista de estados y será eliminado.\n", token);
                }
                token = strtok(NULL, ",");
            }
        } else {
            // Leer transiciones
            char state1[NOMBRE_MAX], symbol[NOMBRE_MAX], state2[NOMBRE_MAX];
            int fields = sscanf(buffer, "%[^,],%[^,],%s", state1, symbol, state2);

            while (fields != 3 || obtenerIndice(state1, estados, num_estados) == -1 ||
                   obtenerIndice(symbol, alfabeto, num_simbolos) == -1 ||
                   obtenerIndice(state2, estados, num_estados) == -1) {
                printf("\n Error: La línea de transición '%s' no está bien definida y será ignorada.\n", buffer);
                break;
            }

            if (fields == 3) {
                int stateIndex = obtenerIndice(state1, estados, num_estados);
                int symbolIndex = obtenerIndice(symbol, alfabeto, num_simbolos);
                int state2Index = obtenerIndice(state2, estados, num_estados);

                if (stateIndex != -1 && symbolIndex != -1 && state2Index != -1) {
                    strcpy(matriz_transicion[stateIndex][symbolIndex], state2);
                } else {
                    printf("\n Advertencia: La transición '%s' tiene elementos no válidos y será ignorada.\n", buffer);
                }
            }
        }
        line++;
    }
    fclose(file);
}


void imprimirMatrizTransicion() {
    int ancho_celda = 8;  // Ancho fijo de la celda

    // Imprimir encabezado de la matriz
    printf("|");
    for (int j = 0; j < num_simbolos; j++) {
        for (int k = 0; k < ancho_celda; k++) printf("_");
        printf("|");
    }
    printf("\n");

    for (int i = 0; i < num_estados; i++) {
        for (int j = 0; j < num_simbolos; j++) {
            if (j == 0) {
                printf("|");
            }

            if (matriz_transicion[i][j][0] == '\0') {
                printf(" %-7s|", "NULL");  // 7 espacios para alinear "NULL"
            } else {
                printf(" %-7s|", matriz_transicion[i][j]);  // 7 espacios para el contenido
            }
        }
        printf("\n");

        // Imprimir separador entre filas
        printf("|");
        for (int j = 0; j < num_simbolos; j++) {
            for (int k = 0; k < ancho_celda; k++) printf("_");
            printf("|");
        }
        printf("\n");
    }
}


int main() {
    char filename[100];
    char cadena[256];

    // Leer archivo de configuración
    printf("Ingrese el nombre del archivo de configuración: ");
    scanf("%s", filename);
    readConfig(filename);

    // Imprimir la matriz de transiciones
    printf("Matriz de transiciones:\n");
    imprimirMatrizTransicion();

    // Leer la cadena a procesar
    printf("Ingrese la cadena a procesar (Si el alfabeto son numeros, separelos por comas): ");
    scanf("%s", cadena);

    // Procesar la cadena
    procesarCadena(cadena);

    return 0;
}
