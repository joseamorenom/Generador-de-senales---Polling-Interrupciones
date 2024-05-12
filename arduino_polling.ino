/**
 * \file Signal wave generator - Polling with Arduino
 * \Brief description of the code
 * \This file of code shows the implementation in Arduino made with the purpose of generating four different wave signals (sin, squared, sawtooth and triangular)
 *\This implementation includes the use of a matrix keypad and an external pulse button
 * \author      José Alejandro Moreno Mesa (jose.morenom@udea.edu.co)
 * \version     0.0.1
 * \date        20/04/2024
 * \copyright   Unlicensed
 */

#include <stdint.h>
#include <Arduino.h>

"""
@brief Definición de los pines de control del DAC
"""
const int D0_pin = 15;
const int D1_pin = 16;
const int D2_pin = 17;
const int D3_pin = 18;
const int D4_pin = 19;
const int D5_pin = 20;
const int D6_pin = 21;
const int D7_pin = 22;


/**
 * @brief Configuración de los pines para los datos del DAC
 */
void set_dac_value(uint8_t value) {
    digitalWrite(D0_pin, (value & 0x01) ? HIGH : LOW);
    digitalWrite(D1_pin, (value & 0x02) ? HIGH : LOW);
    digitalWrite(D2_pin, (value & 0x04) ? HIGH : LOW);
    digitalWrite(D3_pin, (value & 0x08) ? HIGH : LOW);
    digitalWrite(D4_pin, (value & 0x10) ? HIGH : LOW);
    digitalWrite(D5_pin, (value & 0x20) ? HIGH : LOW);
    digitalWrite(D6_pin, (value & 0x40) ? HIGH : LOW);
    digitalWrite(D7_pin, (value & 0x80) ? HIGH : LOW);
}


"""
@brief Hiperparametros usados dentro de la logica.
"""
const int MAX_AMPLITUDE = 2500;  // mV
const int MAX_OFFSET = 1250;  // mV
int i = 0;
unsigned long last_button_press = 0;
unsigned long last_keypress_time = 0;// Variable para almacenar el tiempo de la última pulsación

"""
@brief Variables para pines de columna y fila
"""
int col_pins[4];
int row_pins[4];


"""
@brief Función para inicializar los pines del teclado
"""
void asignacion() {
    for (int dato = 0; dato < 4; dato++) {
        row_pins[dato] = keypad_rows[dato];
        pinMode(row_pins[dato], OUTPUT);
        col_pins[dato] = keypad_columns[dato];
        pinMode(col_pins[dato], INPUT_PULLDOWN);
    }
}

"""
@brief Mapeo del teclado
"""
char matrix_keys[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

int keypad_rows[] = {6,7,8,9};
int keypad_columns[] = {2, 3, 4, 5};
int button_pin = 14;

/**
 * @brief Función seno
 *
 * Retorna los valores actualizados de la función seno al modificarse su amplitud y su offset. Además se normalizan estos valores para el máximo permitido en la implmentación.
 *
 * @param Amplitud: Valor de amplitud de la señal resultante.
 * @param Offset: Nivel DC que se entrega a la señal
 * @return Valor adaptado de la función seno.
 */
int generate_sine_point(int amplitude, int offsete) {
    static int i = 0;
    int amplitude_normalized = map(amplitude, 0, MAX_AMPLITUDE, 0, 255);
    int offset_normalized = map(offsete, 0, MAX_OFFSET, 0, 255);
    int value = amplitude_normalized * sin(radians(i * 7.2)) + offset_normalized;
    i = (i + 1) % 50;
    return value;
}


/**
 * @brief Función cuadrada
 *
 * Retorna los valores actualizados de la función cuadrada al modificarse su amplitud y su offset. Además se normalizan estso valores para el máximo permitido en la implmentación.
 *
 * @param Amplitud: Valor de amplitud de la señal resultante.
 * @param Offset: Nivel DC que se entrega a la señal
 * @return Valor adaptado de la función cuadrada.
 */
int generate_square_point(int amplitude, int offsete) {
    static int i = 0;
    int amplitude_normalized = map(amplitude, 0, MAX_AMPLITUDE, 0, 255);
    int offset_normalized = map(offsete, 0, MAX_OFFSET, 0, 255);
    int value;
    if (i < 25) {
        value = offset_normalized;
    } else {
        value = amplitude_normalized + offset_normalized;
    }
    i = (i + 1) % 50;
    return value;
}

/**
* @brief Función triangular
 *
 * Retorna los valores actualizados de la función triangular al modificarse su amplitud y su offset. Además se normalizan estso valores para el máximo permitido en la implmentación.
 *
 * @param Amplitud: Valor de amplitud de la señal resultante.
 * @param Offset: Nivel DC que se entrega a la señal
 * @return Valor adaptado de la función triangular.
 */

int generate_triangular_point(int amplitude, int offsete) {
    static int i = 0;
    int amplitude_normalized = map(amplitude, 0, MAX_AMPLITUDE, 0, 255);
    int offset_normalized = map(offsete, 0, MAX_OFFSET, 0, 255);
    int value;
    if (i < 25) {
        value = (i * (amplitude_normalized * 2 / 50)) + offset_normalized;
    } else {
        value = ((50 - i) * (amplitude_normalized * 2 / 50)) + offset_normalized;
    }
    i = (i + 1) % 50;
    return value;
}

/**
 * @brief Función diente de sierra
 *
 * Retorna los valores actualizados de la función diente de sierra al modificarse su amplitud y su offset. Además se normalizan estso valores para el máximo permitido en la implmentación.
 *
 * @param Amplitud: Valor de amplitud de la señal resultante.
 * @param Offset: Nivel DC que se entrega a la señal
 * @return Valor adaptado de la función diente de sierra.
 */
int generate_sawtooth_point(int amplitude, int offsete) {
    static int i = 0;
    int amplitude_normalized = map(amplitude, 0, MAX_AMPLITUDE, 0, 255); // Normaliza la amplitud al rango de 0 a 255
    int offset_normalized = map(offsete, 0, MAX_OFFSET, 0, 255); // Normaliza el offset al rango de 0 a 255
    int value = (i * amplitude_normalized / 50) + offset_normalized; // Calcula el valor de salida
    i = (i + 1) % 50; // Incrementa el índice y asegúrate de que permanezca en el rango de 0 a 49
    return value;
}

/**
 * @brief Inicialización de los pines
  */
void setup() {
    asignacion();
    Serial.begin(9600);
    pinMode(D0_pin, OUTPUT);
    pinMode(D1_pin, OUTPUT);
    pinMode(D2_pin, OUTPUT);
    pinMode(D3_pin, OUTPUT);
    pinMode(D4_pin, OUTPUT);
    pinMode(D5_pin, OUTPUT);
    pinMode(D6_pin, OUTPUT);
    pinMode(D7_pin, OUTPUT);
}

/**
 * @brief Bucle principal
 *
 * Generación de la señal final, recibiendo las modificaciones por teclado
 */

void loop() {
    static int contador = 0;
    static unsigned long ultimo_pulsador_presionado = 0;
    unsigned int amplitud = 1000 / 4;
    unsigned int desplazamiento = 500 / 5;
    unsigned int frecuencia = 1 * 50;
    unsigned int siguiente_tiempo_ejecucion = millis();  // Tiempo para la próxima ejecución del ciclo

    char texto_entrada[20] = {0};
    unsigned long ultimo_tiempo_pulsacion = 0;
    while (true) {
        for (int fila = 0; fila < 4; fila++) {
            digitalWrite(pines_fila[fila], HIGH);
            for (int columna = 0; columna < 4; columna++) {
                if (digitalRead(pines_columna[columna]) == HIGH) {
                    unsigned long tiempo_actual = micros();
                    if (tiempo_actual - ultimo_tiempo_pulsacion > 500000) { // 500ms en microsegundos
                        char tecla_pulsada = matriz_teclas[fila][columna];
                        if (tecla_pulsada == 'D') {  // Si se presiona '#', finalizar la entrada
                            Serial.print("Texto ingresado: ");
                            Serial.println(texto_entrada);
                            if (texto_entrada[0] == 'A') {
                                    amplitud = atoi(&texto_entrada[1]);
                                if (amplitud >= 100 && amplitud <= 2500) {
                                    Serial.print("Configuración ingresada: Amplitud -> ");
                                    Serial.println(amplitud);
                                } else {
                                    Serial.println("Configuración de amplitud inválida");
                                }
                            } else if (texto_entrada[0] == 'B') {
                                    desplazamiento = atoi(&texto_entrada[1]);
                                if (desplazamiento >= 50 && desplazamiento <= 1250) {
                                    Serial.print("Configuración ingresada: Offset -> ");
                                    Serial.println(desplazamiento);
                                } else {
                                    Serial.println("Configuración de offset inválida");
                                }
                            } else if (texto_entrada[0] == 'C') {
                                frecuencia = atoi(&texto_entrada[1]);
                                Serial.print("Configuración ingresada: Frecuencia -> ");
                                Serial.println(frecuencia);
                            }
                            texto_entrada[0] = '\0';  // Reiniciar el texto ingresado
                        } else {
                            strncat(texto_entrada, &tecla_pulsada, 1);
                            if (strlen(texto_entrada) >= 10) {  // Por ejemplo, se puede establecer un límite de 20 caracteres
                                Serial.println("Texto demasiado largo. Presione 'D' para finalizar.");
                                texto_entrada[0] = '\0';  // Reiniciar el texto ingresado
                            }
                        }
                        ultimo_tiempo_pulsacion = tiempo_actual;
                    }
                }
            }
            digitalWrite(pines_fila[fila], LOW);



      if (digitalRead(pin_boton) == HIGH) {
          unsigned long tiempo_actual = millis();
          if (tiempo_actual - ultimo_pulsador_presionado > 300) {
              contador = (contador + 1) % 4;  // Incrementar el contador al presionar el botón
              ultimo_pulsador_presionado = tiempo_actual;
          }
      }

      if (millis() >= siguiente_tiempo_ejecucion) {
          siguiente_tiempo_ejecucion += 1000 / frecuencia;
          int valor_señal = 0;
          switch (contador) {
              case 0:
                  //Serial.println("Generando Seno");
                  valor_señal = generar_punto_seno(amplitud, desplazamiento);
                  establecer_valor_dac(valor_señal);
                  //Serial.println(valor_señal);
                  break;
              case 1:
                  //Serial.println("Generando cuadrada");
                  valor_señal = generar_punto_cuadrada(amplitud, desplazamiento);
                  establecer_valor_dac(valor_señal);
                  //Serial.println(valor_señal);
                  break;
              case 2:
                  Serial.println("Generando triangular");
                  valor_señal = generar_punto_triangular(amplitud, desplazamiento);
                  establecer_valor_dac(valor_señal);
                  Serial.println(valor_señal);
                  break;
              case 3:
                  Serial.println("Generando diente sierra");
                  valor_señal = generar_punto_diente_sierra(amplitud, desplazamiento);
                  establecer_valor_dac(valor_señal);
                  Serial.println(valor_señal);
                  break;
                  }
              }
          }
      }

      }