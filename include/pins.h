#ifndef PINS_H
#define PINS_H

// --- BUILT-IN RELAYS (Outputs) ---
// The Waveshare board uses these specific GPIOs for its 8 relays
#define RELAY_MOTOR     2      // Using Relay 1 for Induction Motor
#define VALVE_1         4      // Using Relay 2
#define VALVE_2         5      // Using Relay 3
#define VALVE_3         6      // Using Relay 4
#define ACTUATOR_UP     7      // Using Relay 5 (Linear Actuator Extend)
#define ACTUATOR_DOWN   15     // Using Relay 6 (Linear Actuator Retract)
// Relays 7 and 8 are spare (GPIO 16, 17)

// --- DIGITAL INPUTS (Optocouplers) ---
// These are the 8 input terminals on your board
#define INPUT_1         1      
#define INPUT_2         41     
#define INPUT_3         42     

// --- SENSORS (Expansion Header) ---
// You will need to use the expansion pins for these
#define TEMP_BUS_PIN    14     // OneWire Bus for 3x DS18B20
#define LOADCELL_DOUT   39     // HX711 Data
#define LOADCELL_SCK    40     // HX711 Clock

// --- POWER MONITORING (UART) ---
// The PZEM-004T needs Serial communication
#define PZEM_RX_PIN     48     
#define PZEM_TX_PIN     47     

#endif