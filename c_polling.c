#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"
#include "math.h"
#include "pico/time.h"

#define PWM_PIN 2 // Pin PWM

// Configuración del PWM
#define PWM_FREQ 1000 // Frecuencia PWM en Hz

void pwm_init() {
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
    pwm_set_wrap(pwm_gpio_to_slice_num(PWM_PIN), 1023); // Rango de 0 a 1023
    pwm_set_clkdiv(pwm_gpio_to_slice_num(PWM_PIN), 16.0f); // Divide la frecuencia base del sistema por 16
    pwm_set_enabled(pwm_gpio_to_slice_num(PWM_PIN), true); // Habilita el PWM
}

void set_pwm_duty_cycle(uint16_t duty_cycle) {
    pwm_set_chan_level(pwm_gpio_to_slice_num(PWM_PIN), pwm_gpio_to_channel(PWM_PIN), duty_cycle);
}

int main() {
    stdio_init_all();
    sleep_ms(2000); // Espera para establecer una conexión serial

    pwm_init();

    while (1) {
        // Realizar polling para verificar el estado del recurso deseado (en este caso, simplemente generar señales)
        printf("Generando señal cuadrada...\n");
        for (int i = 0; i < 1024; i++) {
            set_pwm_duty_cycle(i); // Incrementar gradualmente el ciclo de trabajo
            sleep_ms(10);
        }

        printf("Generando señal triangular...\n");
        for (int i = 0; i <= 1023; i++) {
            set_pwm_duty_cycle(i);
            sleep_ms(10);
        }
        for (int i = 1023; i >= 0; i--) {
            set_pwm_duty_cycle(i);
            sleep_ms(10);
        }

        printf("Generando señal de diente de sierra...\n");
        for (int i = 0; i <= 1023; i++) {
            set_pwm_duty_cycle(i);
            sleep_ms(10);
        }

        printf("Generando señal senoidal...\n");
        for (int i = 0; i < 360; i++) { // 360 grados de un ciclo senoidal
            uint16_t sine_value = (uint16_t)(512 + 512 * sin(i * 3.14159 / 180)); // Calcula el valor del seno
            set_pwm_duty_cycle(sine_value);
            sleep_ms(10);
        }
    }

    return 0;
}
