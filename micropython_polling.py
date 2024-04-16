import machine 
import math
import time

PWM_PIN = 16  # Pin PWM en Raspberry Pi Pico (por ejemplo, GP16)

# Configuración del PWM
PWM_FREQ = 1000  # Frecuencia PWM en Hz

# Inicialización del PWM
pwm = machine.PWM(machine.Pin(PWM_PIN))
pwm.freq(PWM_FREQ)

def set_pwm_duty_cycle(duty_cycle):
    pwm.duty_u16(duty_cycle)  # Establece el ciclo de trabajo del PWM

while True:
    # Realizar polling para verificar el estado del recurso deseado (en este caso, simplemente generar señales)
    
    print("Generando señal cuadrada...")
    for i in range(1024):
        set_pwm_duty_cycle(i)  # Incrementa gradualmente el ciclo de trabajo
        time.sleep_ms(10)

    print("Generando señal triangular...")
    for i in range(1024):
        set_pwm_duty_cycle(i)
        time.sleep_ms(10)
    for i in range(1023, -1, -1):
        set_pwm_duty_cycle(i)
        time.sleep_ms(10)

    print("Generando señal de diente de sierra...")
    for i in range(1024):
        set_pwm_duty_cycle(i)
        time.sleep_ms(10)

    print("Generando señal senoidal...")
    for i in range(360):  # 360 grados de un ciclo senoidal
        sine_value = int(32767 + 32767 * math.sin(math.radians(i)))  # Calcula el valor del seno
        set_pwm_duty_cycle(sine_value)
        time.sleep_ms(10)
