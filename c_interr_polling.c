#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "math.h"

#define PWM_PIN 2 // Pin PWM

// Configuración del temporizador
#define TIMER_INDEX 0
#define TIMER_FREQ 1000 // Frecuencia del temporizador en Hz

volatile uint16_t duty_cycle = 0;
volatile bool signal_update_needed = false;

void pwm_init() {
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
    pwm_set_wrap(pwm_gpio_to_slice_num(PWM_PIN), 1023); // Rango de 0 a 1023
    pwm_set_clkdiv(pwm_gpio_to_slice_num(PWM_PIN), 16.0f); // Divide la frecuencia base del sistema por 16
    pwm_set_enabled(pwm_gpio_to_slice_num(PWM_PIN), true); // Habilita el PWM
}

void set_pwm_duty_cycle(uint16_t duty) {
    duty_cycle = duty;
}

void timer_callback() {
    // Incrementar el ciclo de trabajo
    duty_cycle++;
    if (duty_cycle > 1023) {
        duty_cycle = 0;
    }
    signal_update_needed = true; // Indicar que se necesita actualizar la señal
}

int main() {
    stdio_init_all();
    sleep_ms(2000); // Espera para establecer una conexión serial

    pwm_init();

    // Configurar e iniciar el temporizador
    uint32_t interval_us = 1000000 / TIMER_FREQ;
    uint32_t timer_period = timer_add_hw_callback(TIMER_INDEX, timer_callback, NULL);
    timer_set_hw_irq(TIMER_INDEX, timer_period, true);
    timer_set_hw_irq_en(TIMER_INDEX, true);
    timer_hw_set_wrap(TIMER_INDEX, interval_us);
    timer_hw_clear_interrupt(TIMER_INDEX);

    // Configurar interrupciones
    irq_set_exclusive_handler(TIMER_IRQ_0, timer_callback);
    irq_set_enabled(TIMER_IRQ_0, true);
    __enable_irq();

    while (1) {
        // Verificar si se necesita actualizar la señal
        if (signal_update_needed) {
            // Generar señal cuadrada
            for (int i = 0; i < 1024; i++) {
                set_pwm_duty_cycle(i);
                sleep_ms(10);
            }

            // Generar señal triangular
            for (int i = 0; i <= 1023; i++) {
                set_pwm_duty_cycle(i);
                sleep_ms(10);
            }
            for (int i = 1023; i >= 0; i--) {
                set_pwm_duty_cycle(i);
                sleep_ms(10);
            }

            // Generar señal de diente de sierra
            for (int i = 0; i <= 1023; i++) {
                set_pwm_duty_cycle(i);
                sleep_ms(10);
            }

            // Generar señal senoidal
            for (int i = 0; i < 360; i++) { // 360 grados de un ciclo senoidal
                int sine_value = 512 + 512 * sin(i * 3.14159 / 180); // Calcula el valor del seno
                set_pwm_duty_cycle(sine_value);
                sleep_ms(10);
            }

            // Restablecer la bandera
            signal_update_needed = false;
        }
    }

    return 0;
}
