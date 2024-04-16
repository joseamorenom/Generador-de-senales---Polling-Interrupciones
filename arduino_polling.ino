#define PWM_PIN 3 // Pin PWM en Arduino (por ejemplo, el pin 3)

void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial
  pinMode(PWM_PIN, OUTPUT); // Configura el pin PWM como salida
}

void set_pwm_duty_cycle(int duty_cycle) {
  analogWrite(PWM_PIN, duty_cycle); // Establece el ciclo de trabajo del PWM
}

void loop() {
  // Realizar polling para verificar el estado del recurso deseado (en este caso, simplemente generar señales)
  
  Serial.println("Generando señal cuadrada...");
  for (int i = 0; i < 256; i++) {
    set_pwm_duty_cycle(i); // Incrementa gradualmente el ciclo de trabajo
    delay(10);
  }

  Serial.println("Generando señal triangular...");
  for (int i = 0; i <= 255; i++) {
    set_pwm_duty_cycle(i);
    delay(10);
  }
  for (int i = 255; i >= 0; i--) {
    set_pwm_duty_cycle(i);
    delay(10);
  }

  Serial.println("Generando señal de diente de sierra...");
  for (int i = 0; i <= 255; i++) {
    set_pwm_duty_cycle(i);
    delay(10);
  }

  Serial.println("Generando señal senoidal...");
  for (int i = 0; i < 360; i++) { // 360 grados de un ciclo senoidal
    int sine_value = 128 + 128 * sin(i * PI / 180.0); // Calcula el valor del seno
    set_pwm_duty_cycle(sine_value);
    delay(10);
  }
}
