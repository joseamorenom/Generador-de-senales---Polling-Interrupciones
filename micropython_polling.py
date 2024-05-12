/**
 * \file Signal wave generator - Polling with Micropython
 * \Brief description of the code
 * \This file of code shows the implementation in Micropython made with the purpose of generating four different wave signals (sin, squared, sawtooth and triangular)
 *\This implementation includes the use of a matrix keypad and an external pulse button
 * \author      José Alejandro Moreno Mesa (jose.morenom@udea.edu.co)
 * \version     0.0.1
 * \date        20/04/2024
 * \copyright   Unlicensed
 */

from machine import Pin
import utime
import math

"""
@brief Definición de los pines de control del DAC
"""

D0 = machine.Pin(20, machine.Pin.IN)
D1 = machine.Pin(19, machine.Pin.IN)
D2 = machine.Pin(17, machine.Pin.IN)
D3 = machine.Pin(18, machine.Pin.IN)
D4 = machine.Pin(21, machine.Pin.OUT)
D5 = machine.Pin(22, machine.Pin.OUT)
D6 = machine.Pin(26, machine.Pin.OUT)
D7 = machine.Pin(27, machine.Pin.OUT)

/**
 * @brief Configuración de los pines para los datos del DAC
 */

def set_dac_value(value):
    D0.value(value & 0x01)
    D1.value(value & 0x02)
    D2.value(value & 0x04)
    D3.value(value & 0x08)
    D4.value(value & 0x10)
    D5.value(value & 0x20)
    D6.value(value & 0x40)
    D7.value(value & 0x80)

/**
 * @brief Connfiguración de los pines para los datos del DAC
 * Inicialización de cada uno de los pines como las salidas de información para cada digito.
 */
D0.init(machine.Pin.IN)
D1.init(machine.Pin.IN)
D2.init(machine.Pin.IN)
D3.init(machine.Pin.IN)
D4.init(machine.Pin.OUT)
D5.init(machine.Pin.OUT)
D6.init(machine.Pin.OUT)
D7.init(machine.Pin.OUT)


 """
@brief Hiperparametros usados dentro de la logica.
"""
Amplitud_max = 2500   /**< Valor maximo de amplitud valido para procesamiento, esta en mV */
offset_max = 1250  /**< Valor maximo de offset valido para procesamiento, está en mV */
i=0 /**< Contador universal de pulsaciones */
last_button_press = 0 /**< Rastreo del ultimo botón presionado en el teclado */

teclado_fil = [2,3,4,5]    /**< Definición de las filas del teclado matricial */
keypad_columns = [6,7,8,9] /**< Definición de las columnas del teclado matricial */
button_pin = 16     /**< Pin para el botón de cambio de señal */
button = Pin(button_pin, Pin.IN, Pin.PULL_DOWN) /**< Definición del boton como input y pulldown */

col_pins = []
row_pins = []
count = 0

last_keypress_time = 0 /**< Tiempo transcurrida desde la ultima pulsacion */


Valores_teclado = [['1', '2', '3', 'A'],
               ['4', '5', '6', 'B'],
               ['7', '8', '9', 'C'],
               ['*', '0', '#', 'D']] /**< Configuración matricial para el teclado */

/**
 * @brief Función seno
 *
 * Retorna los valores actualizados de la función seno al modificarse su amplitud y su offset. Además se normalizan estos valores para el máximo permitido en la implmentación.
 *
 * @param Amplitud: Valor de amplitud de la señal resultante.
 * @param Offset: Nivel DC que se entrega a la señal
 * @return Valor adaptado de la función seno.
 */
 
seno = [ 
    128, 136, 144, 152, 160, 167, 175, 182, 189, 196, 203, 209, 215, 221, 226, 231, 236, 240,
    243, 247, 249, 251, 253, 254, 255, 255, 255, 254, 252, 250, 248, 245, 242, 238, 234, 229,
    224, 218, 213, 206, 200, 193, 186, 179, 171, 163, 156, 148, 140, 132, 123, 115, 107,  99,
    92,  84,  76,  69,  62,  55,  49,  42,  37,  31,  26,  21,  17,  13,  10,   7,   5,   3,
    1,   0,   0,   0,   1,   2,   4,   6,   8,  12,  15,  19,  24,  29,  34,  40,  46,  52,
    59,  66,  73,  80,  88,  95, 103, 111, 119, 127
]



/**
 * @brief Función cuadrada
 *
 * Retorna los valores actualizados de la función cuadrada al modificarse su amplitud y su offset. Además se normalizan estso valores para el máximo permitido en la implmentación.
 *
 * @param Amplitud: Valor de amplitud de la señal resultante.
 * @param Offset: Nivel DC que se entrega a la señal
 * @return Valor adaptado de la función cuadrada.
 */
 
cuadrada = [
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
]
/**
* @brief Función triangular
 *
 * Retorna los valores actualizados de la función triangular al modificarse su amplitud y su offset. Además se normalizan estso valores para el máximo permitido en la implmentación.
 *
 * @param Amplitud: Valor de amplitud de la señal resultante.
 * @param Offset: Nivel DC que se entrega a la señal
 * @return Valor adaptado de la función triangular.
 */
 
triangular = [
      0,   5,  10,  15,  20,  26,  31,  36,  41,  46,  51,  56,  61,  66,  71,  76,  82,
        87,  92,  97, 102, 107, 112, 117, 122, 127, 133, 138, 143, 148, 153, 158, 163, 168,
       173, 178, 184, 189, 194, 199, 204, 209, 214, 219, 224, 229, 235, 240, 245, 250, 255,
       250, 245, 240, 235, 229, 224, 219, 214, 209, 204, 199, 194, 189, 184, 178, 173, 168,
       163, 158, 153, 148, 143, 138, 133, 127, 122, 117, 112, 107, 102,  97,  92,  87,  82,
        77,  71,  66,  61,  56,  51,  46,  41,  36,  31,  25,  20,  15,  10,   5
]

/**
 * @brief Función diente de sierra
 *
 * Retorna los valores actualizados de la función diente de sierra al modificarse su amplitud y su offset. Además se normalizan estso valores para el máximo permitido en la implmentación.
 *
 * @param Amplitud: Valor de amplitud de la señal resultante.
 * @param Offset: Nivel DC que se entrega a la señal
 * @return Valor adaptado de la función diente de sierra.
 */
 
sierra = [
    129, 131, 134, 137, 139, 142, 144, 147, 149, 152, 155, 157, 160, 162, 165, 167,
        170, 173, 175, 178, 180, 183, 185, 188, 191, 193, 196, 198, 201, 203, 206, 209,
        211, 214, 216, 219, 222, 224, 227, 229, 232, 234, 237, 240, 242, 245, 247, 250,
        252, 255,   0,   3,   5,   8,  10,  13,  15,  18,  21,  23,  26,  28,  31,  33,
         36,  39,  41,  44,  46,  49,  52,  54,  57,  59,  62,  64,  67,  70,  72,  75,
         77,  80,  82,  85,  88,  90,  93,  95,  98, 100, 103, 106, 108, 111, 113, 116,
        118, 121, 124, 126
]

/**
 * @brief Asignación teclado
 *
 * Asignación correspondiente de los datos para las columnas y filas del teclado.
 */

def asignacion():   
    for tecla in range(len(teclado_fil)):
        row_pins.append(Pin(teclado_fil[tecla], Pin.OUT))
        col_pins.append(Pin(keypad_columns[tecla], Pin.IN, Pin.PULL_DOWN))
        
/**
 * @brief Asignación de los parametros a cada una de las señales
 *
 * Personalización de la señal luego de tener una entrada para alguno de los parámetros (Amplitud, frecuencia u offset)
 */

def senal_gen(enumerador, Amp, DC):
    global signal_index
    Amp //= 2

    if enumerador == 0:
        signal = seno[signal_index]
    elif enumerador == 1:
        signal = triangular[signal_index]
    elif enumerador == 2:
        signal = sierra[signal_index]
    elif enumerador == 3:
        signal = cuadrada[signal_index]
    else:
        return

    DC_fin = 255 - ((DC * 255) // 1250)
    AMP_fin = 2500 // Amp
    sig_final = (signal // AMP_fin) - DC_fin
    set_dac_value(sig_final)
    signal_index += 1
    signal_index %= 100
    


/**
 * @brief Ciclo main
 *
 * Bajo el modelo de Polling, se tiene un ciclo infinito donde se están revisando constantemente la siguientes situaciones:
     1. Se presionó la tecla A para cambiar la amplitud
     2. Se preisonó la tecla B para cambiar el offset
     3. Se presionó la tecla C para cambiar la frecuencia
     4. Se preisonó el botón para cambiar el tipo de señal.
 */

def main():  
    asignacion()
    text_input = ""
     """
@brief Variables globales para el ciclo
"""
    global count
    global last_keypress_time  
    global last_button_press
    
    amplitude = 1000/4
    offsete = 500/5
    frequency = 1*50
    
    next_execution_time = utime.ticks_ms()  
    while True:
        # Se verifica secuencialmente el ingres de nuevas teclas
        for row in range(len(row_pins)):
            for col in range(len(col_pins)):
                row_pins[row].on()
                if col_pins[col].value() == 1:
                    current_time = utime.ticks_ms()
                    if current_time - last_keypress_time > 500:
                        key_pressed = Valores_teclado[row][col]
                        if key_pressed == 'D':  #Ya termina la entrada
                            if text_input.startswith('A'):
                                amplitud = int(text_input[1:])
                                if 100 <= amplitud <= 2500:
                                    print("Nueva amplitud:", amplitud)
                                    # Se altera la amplitud y se entrega una nueva señal modificada
                                    amplitude = amplitud/4
                                else:
                                    print("Por favor revise, la amplitud es inválida")
                            if text_input.startswith('B'):
                                offset = int(text_input[1:])
                                if 50 <= offset <= 1250:
                                    print("Nuevo offset: ", offset)
                                    # Se altera el offset y se entrega una nueva señal modificada
                                    offsete = offset
                                else:
                                    print("Por favor revise, el offset es inválido")
                            if text_input.startswith('C'):
                                frecuencia = int(text_input[1:])
                                if 1 <= frecuencia <= 12000000:
                                    print("Nueva frecuencia: ", frecuencia)
                                    # Se altera la frecuencia y se entrega una nueva señal modificada
                                    frequency = frecuencia*50
                                else:
                                    print("Por favor revise, la frecuencia es inválida")
                            print("Entrada de texto: ", text_input)
                            text_input = "" #Se borra la entrada del sistema
                        else:
                            text_input += key_pressed
                            if len(text_input) >= 10:
                                print("Limite de texto. Reinicie oprimiendo D")
                                text_input = ""  #Se borra la entrada del sistema
                        last_keypress_time = current_time
                row_pins[row].off()
        
        if button.value() == 1:
            current_time = utime.ticks_ms()
            if current_time - last_button_press > 300:
                count = (count + 1) % 4  # Incrementar el contador al presionar el botón
                last_button_press= current_time
        if utime.ticks_ms() >= next_execution_time:
            next_execution_time += 1000 // frequency
            if count==0:
                value = funcion_seno(amplitude, offsete)
                set_dac_value(value)
                print("Señal Seno")
                print(value)
            elif count==1:
                value = funcion_cuadrada(amplitude, offsete)
                set_dac_value(value)
                print("Señal cuadrada")
                print(value)
            elif count==2:
                value = funcion_triangular(amplitude, offsete)
                set_dac_value(value)
                print("Señal triangular")
                print(value)   
            elif count==3:
                value = funcion_diente(amplitude, offsete)
                set_dac_value(value)
                print("Señal diente sierra")
                print(value)                
  
if __name__ == '__main__':
    main()
    

