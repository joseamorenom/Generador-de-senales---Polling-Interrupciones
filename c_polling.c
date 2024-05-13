/**
 * \file Signal wave generator - Polling with C
 * \Brief description of the code
 * \This file of code shows the implementation in C made with the purpose of generating four different wave signals (sin, squared, sawtooth and triangular)
 *\This implementation includes the use of a matrix keypad and an external pulse button
 * \author      José Alejandro Moreno Mesa & Valentina Restrepo Jaramillo Jarammilo (jose.morenom@udea.edu.co - valentina.restrepoj@udea.edu.co)
 * \version     0.0.1
 * \date        25/04/2024
 * \copyright   Unlicensed
 */

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

"""
@brief Definición de los pines de control del DAC
"""
#define D0_PIN 16
#define D1_PIN 17
#define D2_PIN 18
#define D3_PIN 19
#define D4_PIN 20
#define D5_PIN 21
#define D6_PIN 22
#define D7_PIN 26

/**
 * @brief Configuración de los pines para los datos del DAC
 */

void set_DAC_value(uint8_t value) {
    gpio_put(D0_PIN, (value & 0x01));
    gpio_put(D1_PIN, (value & 0x02));
    gpio_put(D2_PIN, (value & 0x04));
    gpio_put(D3_PIN, (value & 0x08));
    gpio_put(D4_PIN, (value & 0x10));
    gpio_put(D5_PIN, (value & 0x20));
    gpio_put(D6_PIN, (value & 0x40));
    gpio_put(D7_PIN, (value & 0x80));
}




//SEÑALES INDEPENDIENTES CON INDICE DE USO
// Con el indice escojo que indice usar
uint8_t indice_senal = 0;

/**
 * @brief Función seno
 *
 * Retorna los valores actualizados de la función seno al modificarse su amplitud y su offset. Además se normalizan estos valores para el máximo permitido en la implmentación.
 *
 * @param Amplitud: Valor de amplitud de la señal resultante.
 * @param Offset: Nivel DC que se entrega a la señal
 * @return Valor adaptado de la función seno.
 */

const uint8_t seno [] = { 
    128, 136, 144, 152, 160, 167, 175, 182, 189, 196, 203, 209, 215, 221, 226, 231, 236, 240,
    243, 247, 249, 251, 253, 254, 255, 255, 255, 254, 252, 250, 248, 245, 242, 238, 234, 229,
    224, 218, 213, 206, 200, 193, 186, 179, 171, 163, 156, 148, 140, 132, 123, 115, 107,  99,
    92,  84,  76,  69,  62,  55,  49,  42,  37,  31,  26,  21,  17,  13,  10,   7,   5,   3,
    1,   0,   0,   0,   1,   2,   4,   6,   8,  12,  15,  19,  24,  29,  34,  40,  46,  52,
    59,  66,  73,  80,  88,  95, 103, 111, 119, 127
};

/**
 * @brief Función cuadrada
 *
 * Retorna los valores actualizados de la función cuadrada al modificarse su amplitud y su offset. Además se normalizan estso valores para el máximo permitido en la implmentación.
 *
 * @param Amplitud: Valor de amplitud de la señal resultante.
 * @param Offset: Nivel DC que se entrega a la señal
 * @return Valor adaptado de la función cuadrada.
 */
const uint8_t cuadrada [] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};

/**
* @brief Función triangular
 *
 * Retorna los valores actualizados de la función triangular al modificarse su amplitud y su offset. Además se normalizan estso valores para el máximo permitido en la implmentación.
 *
 * @param Amplitud: Valor de amplitud de la señal resultante.
 * @param Offset: Nivel DC que se entrega a la señal
 * @return Valor adaptado de la función triangular.
 */

const uint8_t triangular [] = {
      0,   5,  10,  15,  20,  26,  31,  36,  41,  46,  51,  56,  61,  66,  71,  76,  82,
        87,  92,  97, 102, 107, 112, 117, 122, 127, 133, 138, 143, 148, 153, 158, 163, 168,
       173, 178, 184, 189, 194, 199, 204, 209, 214, 219, 224, 229, 235, 240, 245, 250, 255,
       250, 245, 240, 235, 229, 224, 219, 214, 209, 204, 199, 194, 189, 184, 178, 173, 168,
       163, 158, 153, 148, 143, 138, 133, 127, 122, 117, 112, 107, 102,  97,  92,  87,  82,
        77,  71,  66,  61,  56,  51,  46,  41,  36,  31,  25,  20,  15,  10,   5
};

/**
 * @brief Función diente de sierra
 *
 * Retorna los valores actualizados de la función diente de sierra al modificarse su amplitud y su offset. Además se normalizan estso valores para el máximo permitido en la implmentación.
 *
 * @param Amplitud: Valor de amplitud de la señal resultante.
 * @param Offset: Nivel DC que se entrega a la señal
 * @return Valor adaptado de la función diente de sierra.
 */

/// Forma de onda de sierra
const uint8_t sierra [] = {
    129, 131, 134, 137, 139, 142, 144, 147, 149, 152, 155, 157, 160, 162, 165, 167,
        170, 173, 175, 178, 180, 183, 185, 188, 191, 193, 196, 198, 201, 203, 206, 209,
        211, 214, 216, 219, 222, 224, 227, 229, 232, 234, 237, 240, 242, 245, 247, 250,
        252, 255,   0,   3,   5,   8,  10,  13,  15,  18,  21,  23,  26,  28,  31,  33,
         36,  39,  41,  44,  46,  49,  52,  54,  57,  59,  62,  64,  67,  70,  72,  75,
         77,  80,  82,  85,  88,  90,  93,  95,  98, 100, 103, 106, 108, 111, 113, 116,
        118, 121, 124, 126
};




/**
 * @brief Asignación de los parametros a cada una de las señales
 *
 * Personalización de la señal luego de tener una entrada para alguno de los parámetros (Amplitud, frecuencia u offset)
 */

void generador_senal(uint8_t tipo, uint32_t Amplitud, uint32_t DC){
    Amplitud /= 2;
    uint16_t valor_senal;

    switch (tipo)
    {
    case 0:
        valor_senal = seno[indice_senal];
        break;
    case 1: 
        valor_senal = triangular[indice_senal];
        break;
    case 2:
        valor_senal = sierra[indice_senal];
        break;
    case 3:
        valor_senal = cuadrada[indice_senal];
        break;
    
    default:
        break;
    }

    //ojo, Se NORMALIZAN los datos aquí
    uint16_t normalizado_DC = 255 - ((DC * 255) / 1250); //Offset norm
    uint16_t normalizado_Amplitud = 2500 / Amplitud; //Amplitud norm
    valor_senal = (valor_senal / normalizado_Amplitud) - normalizado_DC; //Manda la senal personalziada al DAC
    set_DAC_value(valor_senal);
    indice_senal += 1; //cambio de indice
    indice_senal %= 100;
}

/**
 * @brief Parámetros de uso del teclado
 *
 *
 * @param Filas: Primeras conexiones del teclado.
 * @param Columnas: Segundas conexiones del teclado.
 * @param Boton: Boton para cambio de indice.
 */

#define FILAS_TECLADO 4
#define COLUMNAS_TECLADO 4
#define Button_PIN 1
char teclas_matriz[FILAS_TECLADO][COLUMNAS_TECLADO] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

int pines_filas[FILAS_TECLADO] = {2, 3, 4, 5}; //Filas
int pines_columnas[COLUMNAS_TECLADO] = {6, 7, 8, 9}; //Columnas
int contador = 0; //Para asignar al indice
int ultima_pulsacion_boton = 0; //Temporozación de pulsaciones del botón
int ultima_pulsacion_tecla = 0; //Temporozación de pulsaciones del teclado

/**
 * @brief Connfiguración de los pines para los datos del DAC
 * Inicialización de cada uno de los pines como las salidas de información para cada digito.
 */
void configurar() {
    gpio_init(D0_PIN);
    gpio_set_dir(D0_PIN, GPIO_OUT);
    gpio_init(D1_PIN);
    gpio_set_dir(D1_PIN, GPIO_OUT);
    gpio_init(D2_PIN);
    gpio_set_dir(D2_PIN, GPIO_OUT);
    gpio_init(D3_PIN);
    gpio_set_dir(D3_PIN, GPIO_OUT);
    gpio_init(D4_PIN);
    gpio_set_dir(D4_PIN, GPIO_OUT);
    gpio_init(D5_PIN);
    gpio_set_dir(D5_PIN, GPIO_OUT);
    gpio_init(D6_PIN);
    gpio_set_dir(D6_PIN, GPIO_OUT);
    gpio_init(D7_PIN);
    gpio_set_dir(D7_PIN, GPIO_OUT);
    gpio_init(Boton_pin);
    gpio_set_dir(Boton_pin, GPIO_IN);
}

/**
 * @brief Pines para las filas/columnas del teclado matricial de arriba
 */
void asignar_pines() {
    for (int i = 0; i < FILAS_TECLADO; i++) {
        pines_fila[i] = pines_filas[i];
        gpio_init(pines_fila[i]);
        gpio_set_dir(pines_fila[i], GPIO_OUT);
    }
    for (int i = 0; i < COLUMNAS_TECLADO; i++) {
        pines_columna[i] = pines_columnas[i];
        gpio_init(pines_columna[i]);
        gpio_set_dir(pines_columna[i], GPIO_IN);
        gpio_pull_down(pines_columna[i]);
    }
}

/**
 * @brief Ciclo principal
 */
// Esta función es la principal del programa
void programa_principal() {
    // Inicialización de la entrada y salida estándar
    stdio_init_all();
    // Configuración de los dispositivos
    configurar();
    // Asignación de los pines del teclado
    asignar_pines();
    // Inicialización de variables
    char texto_ingresado[20] = "";  // Inicializa la variable para almacenar el texto ingresado.
    uint32_t amplitud = 1000; // Valor predeterminado para la amplitud de la señal.
    uint32_t offset = 100; // Valor predeterminado para el offset de la señal.
    uint32_t frecuencia = 10; // Valor predeterminado para la frecuencia de la señal.
    uint32_t proxima_ejecucion = time_us_32() / 1000;  // Tiempo para la próxima ejecución del ciclo.
    uint32_t puntos = 100;  // Número de puntos de la señal.
    uint32_t frec_muestreo = (uint32_t)((1000000.0)*(1.0/puntos)*(1.0/frecuencia)); // Frecuencia de muestreo de la señal.
    uint32_t tiempo_muestreo = time_us_32(); // Tiempo de inicio del muestreo.
    char tipo_senal[11] = " ";  // Tipo de señal generada.

    // Bucle principal del programa
    while (true) {
        // Verificar si se han ingresado nuevas teclas
        for (int fila = 0; fila < FILAS_TECLADO; fila++) {
            for (int columna = 0; columna < COLUMNAS_TECLADO; columna++) {
                gpio_put(pines_fila[fila], 1);
                if (gpio_get(pines_columna[columna]) == 1) {
                    int tiempo_actual = time_us_32() / 1000;
                    if (tiempo_actual - last_keypress_time > 500) {
                        char tecla_presionada = matrix_keys[fila][columna];
                        if (tecla_presionada == 'D') { 
                            if (texto_ingresado[0] == 'A') {
                                uint32_t nueva_amplitud = atoi(&texto_ingresado[1]);
                                if (100 <= nueva_amplitud && nueva_amplitud <= 2500) {
                                    printf("Configuracion ingresada : Amplitud-> %d\n", nueva_amplitud);
                                    amplitud = nueva_amplitud; // Generar señal con nueva amplitud
                                } else {
                                    printf("Configuracion de amplitud invalida\n");
                                }
                            } else if (texto_ingresado[0] == 'B') {
                                uint32_t nuevo_offset = atoi(&texto_ingresado[1]);
                                if (50 <= nuevo_offset && nuevo_offset <= 1250) {
                                    printf("Configuracion ingresada : Offset-> %d\n", nuevo_offset);
                                    offset = nuevo_offset; // Generar señal con nuevo offset
                                } else {
                                    printf("Configuracion de offset invalida\n");
                                }
                            } else if (texto_ingresado[0] == 'C') {
                                uint32_t nueva_frecuencia = atoi(&texto_ingresado[1]);
                                if (1 <= nueva_frecuencia && nueva_frecuencia <= 12000000) {
                                    printf("Configuracion ingresada : Frecuencia-> %d\n", nueva_frecuencia);
                                    frecuencia = nueva_frecuencia; // Generar señal con nueva frecuencia
                                    frec_muestreo = (uint32_t)((1000000.0)*(1.0/puntos)*(1.0/frecuencia)); // Se calcula la nueva frecuencia de muestreo
                                } else {
                                    printf("Configuracion de frecuencia invalida\n");
                                }
                            }
                            printf("Texto ingresado: %s\n", texto_ingresado);
                            texto_ingresado[0] = '\0';  
                        } else {
                            strncat(texto_ingresado, &tecla_presionada, 1);
                            if (strlen(texto_ingresado) >= 10) { 
                                printf("Texto demasiado largo. Presione 'D' para finalizar.\n");
                                texto_ingresado[0] = '\0';  
                            }
                        }
                        last_keypress_time = tiempo_actual;
                    }
                }
                gpio_put(pines_fila[fila], 0);
            }
        }

        // Lógica para procesar el botón 
        if (gpio_get(Boton_pin) == 1) {
            int tiempo_actual = time_us_32() / 1000;
            if (tiempo_actual - last_button_press > 300) {
                count = (count + 1) % 4; 
                last_button_press = tiempo_actual;
            }
        }

        // Lógica para generar la señal
        if ((time_us_32() - tiempo_muestreo) > frec_muestreo) {
            generador(count, amplitud, offset);
            tiempo_muestreo = time_us_32();
        }

        //Logica para imprimir por serial el estado de la señal
        uint32_t tiempo_actual = time_us_32()/1000;
        if (tiempo_actual - proxima_ejecucion >= 1000) {
            if (count==0) {
                strcpy(tipo_senal, "Seno");
            } else if (count==1){
                strcpy(tipo_senal, "Triangular");
            } else if (count==2){
               strcpy(tipo_senal, "Sierra");
            } else if (count==3){
                strcpy(tipo_senal, "Cuadrada");
            }
             printf("Señal: Tipo -> %s, Amplitud -> %d mV, Offset -> %d mV, Frecuencia -> %d Hz\n",
                tipo_senal, amplitud, offset, frecuencia);
            proxima_e

    }
}
