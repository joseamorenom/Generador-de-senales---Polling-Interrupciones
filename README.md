# Signal Generator – Polling & Interrupts

This repository contains the source code for a signal generator project implemented on a Raspberry Pi Pico using C. Two programming approaches are provided: one based on **Polling** and another based on **Interrupts**. The system generates four distinct waveforms (sine, square, sawtooth, and triangular) and allows the user to adjust key signal parameters (amplitude, offset, and frequency) via a matrix keypad and an external push button.

## Description

The project is developed in C with a modular structure. Two control methods are implemented:

- **Polling:** The microcontroller continuously checks (polls) the state of peripherals (keypad and button) in an infinite loop to update the DAC output.
- **Interrupts:** Interrupts are configured to react immediately to external events (such as key presses or timer pulses) without continuously polling. This results in improved efficiency and response time.

Using interrupts, the code is divided into functions that handle specific events (e.g., button press, keypad input, timer expiration). This modular approach improves maintainability and debugging, and it optimizes resource usage by allowing the microcontroller to perform other tasks when no event is occurring.

## Features

- **Waveforms Generated:** Sine, Square, Sawtooth, and Triangular.
- **Real-Time Adjustments:** Users can modify amplitude, offset, and frequency via a matrix keypad.
- **User Interface:** A 4x4 matrix keypad for parameter input and an external push button to toggle between waveforms.
- **Modular Implementation:** Separate modules for pin configuration, signal generation, input handling, and interrupt/timer management.

## Hardware Requirements

- **Raspberry Pi Pico:** Microcontroller board based on the RP2040.
- **DAC0808 (or equivalent):** Digital-to-Analog Converter for output signal visualization.
- **4x4 Matrix Keypad:** For user input of signal parameters.
- **Push Button:** For toggling the waveform type.
- **Cables & Breadboard:** For connecting all components.

## Software Requirements

- **Raspberry Pi Pico SDK in C/C++:** To compile and program the project.
- **Visual Studio Code (or another IDE):** Configured for Pico development.
- **CMake & Make:** Build tools for the C version of the project.

## Project Structure

The source code is organized into several modules and functions:

- **Pin and Device Configuration:** Functions to initialize DAC, keypad, and button pins.
- **Signal Generation:** Functions that generate data vectors for each waveform, adjusted according to the specified amplitude, offset, and frequency.
- **Input Handling:** Functions to process user input from the matrix keypad and update signal parameters.
- **Interrupts and Timers:** Configuration of interrupts for keypad, button, and timers to control signal updates periodically.

## Interrupt-Driven Flow in C

The interrupt-based approach is characterized by:

1. **Input Interrupt Configuration:**  
   Interrupts are set on the keypad and button pins so that when an event occurs (e.g., a key press), an interrupt handler is immediately invoked. This eliminates the need for constant polling.

2. **Timer Utilization:**  
   Timers generate periodic interrupts to update the DAC output at fixed intervals, ensuring the generated waveform remains stable and precise.

3. **Modular Code Structure:**  
   Each interrupt handler is dedicated to a specific task (e.g., updating the signal output, processing user input, or managing waveform sequences), thereby enhancing code clarity and ease of maintenance.

4. **Debouncing and Synchronization:**  
   Debounce mechanisms are implemented to prevent erroneous multiple triggers from mechanical switches, and interrupt priorities are managed to ensure critical events are handled promptly.

## Instructions for Use

1. **Hardware Setup:**  
   - Connect the DAC and button pins as defined in the code.
   - Wire the matrix keypad according to the provided pin configuration.

2. **Environment Setup:**  
   - Install the Raspberry Pi Pico SDK and set up your development environment (e.g., Visual Studio Code) to compile the project.
   - Configure CMake and Make to build the C code.

3. **Compilation and Flashing:**  
   - Compile the project using appropriate commands (e.g., `cmake` and `make`).
   - Flash the compiled firmware to the Raspberry Pi Pico.

4. **Testing and Verification:**  
   - Use an oscilloscope or multimeter to verify the DAC output signal.
   - Adjust signal parameters using the matrix keypad and switch waveforms using the push button.

## Results and Conclusion

Testing has shown that the interrupt-driven implementation offers superior efficiency and responsiveness compared to the polling approach. The modular task separation and optimized resource usage have resulted in stable signal generation at optimal frequencies.

## Contribution

Contributions are welcome! Please open an issue or submit a pull request with your suggestions and improvements.

## License

This project is distributed under the **MIT** license.


