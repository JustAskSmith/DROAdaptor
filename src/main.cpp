#include <Arduino.h>

// Pin definitions
#define CLOCK_PIN 1  // D1
#define DATA_PIN 2   // D2

#define MAX_BITS 21
#define MAX_CLOCK_INTERVAL 150  // Maximum expected interval between clock pulses in microseconds


// Volatile variables for interrupt handling
volatile uint32_t receivedData = 0;
volatile int bitCount = 0;
volatile bool dataReady = false;
volatile bool firstClockFound = false;  // first clock pulse correctly detected

volatile uint32_t lastClockTime = 0;  
volatile uint32_t clockInterval = 0;

// Interrupt service routine for clock signal
void onClock() {

  // Get the current time in microseconds
  uint32_t currentTime = micros();
  // Calculate the interval between clock pulses
  clockInterval = currentTime - lastClockTime;  
  lastClockTime = currentTime;  // Update last clock time
  
  if (bitCount == 0) { // Only check for the first clock pulse when we are expecting the start of a new data frame
    firstClockFound = clockInterval > MAX_CLOCK_INTERVAL;  // Check if the interval indicates the first clock pulse of a new data frame
  }
  
  if (!firstClockFound) {
    // 
    return;
  }

  // Read the data bit
  int bit = digitalRead(DATA_PIN);
  
  // Shift in the bit (MSB first, assuming)
  receivedData = (receivedData << 1) | bit;
  
  // Increment bit count
  bitCount++;
  
  // Check if we have received all 21 bits
  if (bitCount >= 21) {
    dataReady = true;
    firstClockFound = false;  // Reset for the next data frame
  }
  }


void setup() {
  // Initialize serial for output
  Serial.begin(9600);
  
  // Set pin modes
  pinMode(CLOCK_PIN, INPUT);
  pinMode(DATA_PIN, INPUT);
  
  // Attach interrupt to clock pin (falling edge)
  attachInterrupt(digitalPinToInterrupt(CLOCK_PIN), onClock, FALLING);
  
  Serial.println("DRO Adaptor initialized. Waiting for data...");
}

void loop() {
  // Check if data is ready
  if (dataReady) {
    // Disable interrupts temporarily to safely read volatile variables
    noInterrupts();
    uint32_t data = receivedData;
    int count = bitCount;
    dataReady = false;
    bitCount = 0;
    receivedData = 0;
    interrupts();
    
    // Process the received data (mask to 21 bits)
    uint32_t value = data & 0x1FFFFF;  // 21 bits mask
    
    // Output the value
    Serial.print("Received 21-bit value: ");
    Serial.println(value, BIN);  // Print in binary
    Serial.print("Decimal: ");
    Serial.println(value);
  }
  
  // Small delay to prevent busy looping
  delay(10);
}