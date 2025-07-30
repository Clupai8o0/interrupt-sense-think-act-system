// Constants
const int MOISTURE_THRESHOLD = 400;
const int DISTANCE_THRESHOLD = 100;

// Pin Assignments
const int buttonPin = 8;	// Button Input (PCI)
const int ledPin = 13;		// Main Condtional LED
const int blinkingPin = 12;	// Blinking LED (for timer logic)
const int trigPin = 6; 		// ultrasonic trigger
const int echoPin = 9; 		// ultrasonic echo (PCI)
const int moistPin = A0; 	// moisture sensor

// Volatile Variables for ISR
volatile int buttonPressed = false;		// Button state
volatile unsigned long echoStart = 0;	// Timestamp for ultrasonic pulse start
volatile unsigned long echoEnd = 0;		// Timestamp for ultrasonic pulse end
volatile bool echoDone = false;			// Flag for ultrasonic measurement complete
volatile bool timerFlag = false;		// Flag for timer interrupt
 
void setup() {
  // Setting pins
  pinMode(buttonPin, INPUT); 	// Button input
  pinMode(ledPin, OUTPUT);		// Condition LED output
  pinMode(blinkingPin, OUTPUT);	// Blinking LED output
  pinMode(trigPin, OUTPUT);		// Ultrasonic trigger output
  pinMode(echoPin, INPUT);		// Ultrasonic echo input
  pinMode(moistPin, INPUT);		// Moisture sensor input
  
  Serial.begin(9600); // Initializing serial monitor
  
  // PCI Setup
  PCICR |= 0b00000001; 	// Enable PCI on group B
  PCMSK0 |= 0b00000011; // Enable PCI for D8 and D9
  
  // Timer1 Setup
  noInterrupts();
  TCCR1A = 0;							// Clearing registers
  TCCR1B = 0;
  TCNT1 = 1;
  OCR1A = 15625; 						// every 1 second toggle
  TCCR1B |= (1 << CS12) | (1 << CS10);	// Seting prescalar to 1024
  TIMSK1 |= (1 << OCIE1A);				// Enabling Timer1
  TCCR1B |= (1 << WGM12);				// Setting CTC mode
  interrupts();
}

// Timer1 ISR, triggering every 1 second to set flag for LED blink
ISR(TIMER1_COMPA_vect) {
  timerFlag = true;
}

// PCI ISR
ISR(PCINT0_vect) {
  buttonPressed = digitalRead(buttonPin); // Capturing button state
  
  // Ultrasonic echo detection
  if (digitalRead(echoPin) == HIGH) 
  	echoStart = micros(); 			// Recording pulse start time
  else {
  	echoEnd = micros();				// Recording pulse end time
    echoDone = true;				// Flag measurement complete
  }
}

// Sensor functions
// Reading moisture sensor
int senseMoisture() {
  return analogRead(moistPin);
}

// Sending trigger pulse to ultrasonic sensor
void triggerUltrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

// Calculating distance (cm) from ultrasonic echo timings
long calculateDistance(unsigned long echoStart, unsigned long echoEnd) {
  unsigned long duration;
  long distance;
  
  duration = echoEnd - echoStart; 	// Pulse width in microseconds
  distance = (duration / 2) / 29.1;	// Converting to distance (speed of sound)
  echoDone = false;					// reset flag for next measurement
  
  return distance;
}

// Helper functions
// Determine if condition LED should turn on or off based on condition
bool shouldTurnOnLED(int moisture, long distance, bool button) {
	return (button && distance < DISTANCE_THRESHOLD && moisture < MOISTURE_THRESHOLD);
}
// Update LED state to on or off
void updateConditionLED(bool state) {
	digitalWrite(ledPin, state ? HIGH : LOW);
}

// Logging current sensor and button data
void logSensorData(int moisture, long distance, bool button) {
	Serial.print("Moisture: ");
    Serial.print(moisture);
    Serial.print(" | Distance: ");
    Serial.print(distance);
    Serial.print(" | Button: ");
    Serial.println(button);
}

void loop() {
  triggerUltrasonic(); // Firing ultrasonic pulse
  
  int moisture = senseMoisture(); // Reading moisture sensor data
  
  // Once ultrasonic measurement is complete
  if (echoDone) {
    long distance = calculateDistance(echoStart, echoEnd); // computing distance
    bool ledState = shouldTurnOnLED(moisture, distance, buttonPressed); // evaluating LED condition
    updateConditionLED(ledState); // updating LED state based on result
    logSensorData(moisture, distance, buttonPressed); // Printing debug info
  }
  
  // Handling periodic timer event
  if (timerFlag) {
    timerFlag = false;										// reseting timer flag
    digitalWrite(blinkingPin, !digitalRead(blinkingPin));	// toggle blinking LED
    Serial.println("TIMER: Blink LED");						// Log timer event
  }
}
