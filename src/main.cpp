/*
  Arduino 3-Relay Light Pattern Controller
  For Arduino Mini with relays on pins 9, 10, and 11
  20 different light patterns
  Manual pattern change button on pin 12
*/
#include <Arduino.h>
// Define the pins for the relays
const int relay1 = 9;
const int relay2 = 10;
const int relay3 = 11;
const int buttonPin = 12; // Button for changing patterns

// Pattern durations (in milliseconds)
const int patternDuration = 20000; // Each pattern runs for 20 seconds
unsigned long patternStartTime;
int currentPattern = 0;

// Button state variables
int buttonState = LOW;       // Current button state (LOW when not pressed with pull-down)
int lastButtonState = LOW;   // Previous button state
unsigned long lastDebounceTime = 0;  // Last time button was pressed
unsigned long debounceDelay = 50;    // Debounce time in milliseconds

void setup() {
  // Initialize relay pins as outputs
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  
  // Initialize button pin as input with pull-up resistor
  pinMode(buttonPin, INPUT);
  
  // Initially turn off all relays (HIGH = OFF for most relay modules)
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  
  // Start the first pattern
  patternStartTime = millis();
  
  // Initialize serial communication for debugging (optional)
  Serial.begin(9600);
  Serial.println("Relay Light Pattern System Started");
}

void loop() {
  // Check button state for manual pattern change
  checkButtonForPatternChange();
  
  // Check if it's time to change to the next pattern automatically
  if (millis() - patternStartTime >= patternDuration) {
    // Move to next pattern
    currentPattern = (currentPattern + 1) % 20;
    patternStartTime = millis();
    
    // Print the current pattern number (optional)
    Serial.print("Switching to pattern: ");
    Serial.println(currentPattern + 1);
  }
  
  // Run the current pattern
  switch (currentPattern) {
    case 0:
      // Pattern 1: All on
      allOn();
      break;
    case 1:
      // Pattern 2: All off
      allOff();
      break;
    case 2:
      // Pattern 3: Sequential on/off
      sequentialOnOff(500);
      break;
    case 3:
      // Pattern 4: Alternating 1+2 / 3
      alternating12_3(800);
      break;
    case 4:
      // Pattern 5: Slow chase
      chase(1000);
      break;
    case 5:
      // Pattern 6: Fast chase
      chase(250);
      break;
    case 6:
      // Pattern 7: Random blinking
      randomBlink(300);
      break;
    case 7:
      // Pattern 8: Heartbeat - double pulse
      heartbeat(60);
      break;
    case 8:
      // Pattern 9: Binary counter
      binaryCount(500);
      break;
    case 9:
      // Pattern 10: Fade in/out simulation
      fadeSimulation(100);
      break;
    case 10:
      // Pattern 11: Alternating 1/2+3
      alternating1_23(700);
      break;
    case 11:
      // Pattern 12: Wave pattern
      wavePattern(300);
      break;
    case 12:
      // Pattern 13: SOS morse code
      morseCodeSOS();
      break;
    case 13:
      // Pattern 14: Slow strobe
      strobe(400);
      break;
    case 14:
      // Pattern 15: Fast strobe
      strobe(100);
      break;
    case 15:
      // Pattern 16: Cascade
      cascade(300);
      break;
    case 16:
      // Pattern 17: Firefly effect
      fireflyEffect();
      break;
    case 17:
      // Pattern 18: Alternating pairs
      alternatingPairs(600);
      break;
    case 18:
      // Pattern 19: Thunder effect
      thunderEffect();
      break;
    case 19:
      // Pattern 20: Sequential pairs
      sequentialPairs(500);
      break;
  }
}

// Function to turn all relays on
void allOn() {
  digitalWrite(relay1, LOW); // LOW activates most relay modules
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  delay(50); // Small delay to prevent too frequent updates
}

// Function to turn all relays off
void allOff() {
  digitalWrite(relay1, HIGH); // HIGH deactivates most relay modules
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  delay(50);
}

// Function for sequential on/off pattern
void sequentialOnOff(int speed) {
  static unsigned long lastChange = 0;
  static int sequence = 0;
  
  if (millis() - lastChange >= speed) {
    lastChange = millis();
    
    // Turn all relays off
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    
    // Turn on just one relay based on sequence
    switch (sequence) {
      case 0:
        digitalWrite(relay1, LOW);
        break;
      case 1:
        digitalWrite(relay2, LOW);
        break;
      case 2:
        digitalWrite(relay3, LOW);
        break;
    }
    
    // Update sequence for next time
    sequence = (sequence + 1) % 3;
  }
}

// Function for alternating pattern - relays 1 & 2 vs relay 3
void alternating12_3(int speed) {
  static unsigned long lastChange = 0;
  static bool state = false;
  
  if (millis() - lastChange >= speed) {
    lastChange = millis();
    state = !state;
    
    if (state) {
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, HIGH);
    } else {
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, LOW);
    }
  }
}

// Function for chasing pattern
void chase(int speed) {
  static unsigned long lastChange = 0;
  static int position = 0;
  
  if (millis() - lastChange >= speed) {
    lastChange = millis();
    
    // Turn all relays off
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    
    // Turn on just one relay based on position
    switch (position) {
      case 0:
        digitalWrite(relay1, LOW);
        break;
      case 1:
        digitalWrite(relay2, LOW);
        break;
      case 2:
        digitalWrite(relay3, LOW);
        break;
    }
    
    // Update position for next time
    position = (position + 1) % 3;
  }
}

// Function for random blinking
void randomBlink(int speed) {
  static unsigned long lastChange = 0;
  
  if (millis() - lastChange >= speed) {
    lastChange = millis();
    
    // Randomly set each relay
    digitalWrite(relay1, random(2) ? LOW : HIGH);
    digitalWrite(relay2, random(2) ? LOW : HIGH);
    digitalWrite(relay3, random(2) ? LOW : HIGH);
  }
}

// Function for heartbeat pattern
void heartbeat(int baseSpeed) {
  static unsigned long lastChange = 0;
  static int beatStep = 0;
  static int currentDelay = baseSpeed;
  
  if (millis() - lastChange >= currentDelay) {
    lastChange = millis();
    
    switch (beatStep) {
      case 0: // First beat
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, LOW);
        digitalWrite(relay3, LOW);
        currentDelay = baseSpeed;
        break;
      case 1:
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, HIGH);
        digitalWrite(relay3, HIGH);
        currentDelay = baseSpeed / 2; // Short delay between beats
        break;
      case 2: // Second beat
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, LOW);
        digitalWrite(relay3, LOW);
        currentDelay = baseSpeed;
        break;
      case 3:
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, HIGH);
        digitalWrite(relay3, HIGH);
        currentDelay = baseSpeed * 4; // Long pause after second beat
        break;
    }
    
    beatStep = (beatStep + 1) % 4;
  }
}

// Function for binary counter pattern
void binaryCount(int speed) {
  static unsigned long lastChange = 0;
  static int counter = 0;
  
  if (millis() - lastChange >= speed) {
    lastChange = millis();
    
    // Display binary counter on relays (0-7)
    digitalWrite(relay1, (counter & 0x01) ? LOW : HIGH);
    digitalWrite(relay2, (counter & 0x02) ? LOW : HIGH);
    digitalWrite(relay3, (counter & 0x04) ? LOW : HIGH);
    
    // Increment counter
    counter = (counter + 1) % 8;
  }
}

// Function to simulate fading with on/off patterns
void fadeSimulation(int speed) {
  static unsigned long lastChange = 0;
  static int fadeStep = 0;
  static bool fadeDirection = true; // true = fading in, false = fading out
  
  if (millis() - lastChange >= speed) {
    lastChange = millis();
    
    // Simple fade simulation with digital relays
    switch (fadeStep) {
      case 0:
        digitalWrite(relay1, fadeDirection ? LOW : HIGH);
        digitalWrite(relay2, HIGH);
        digitalWrite(relay3, HIGH);
        break;
      case 1:
        digitalWrite(relay1, fadeDirection ? LOW : HIGH);
        digitalWrite(relay2, fadeDirection ? LOW : HIGH);
        digitalWrite(relay3, HIGH);
        break;
      case 2:
        digitalWrite(relay1, fadeDirection ? LOW : HIGH);
        digitalWrite(relay2, fadeDirection ? LOW : HIGH);
        digitalWrite(relay3, fadeDirection ? LOW : HIGH);
        break;
    }
    
    // Update fade step
    if (fadeDirection) {
      fadeStep++;
      if (fadeStep >= 3) {
        fadeDirection = false;
        fadeStep = 2;
      }
    } else {
      fadeStep--;
      if (fadeStep < 0) {
        fadeDirection = true;
        fadeStep = 0;
      }
    }
  }
}

// Function for alternating pattern - relay 1 vs relays 2 & 3
void alternating1_23(int speed) {
  static unsigned long lastChange = 0;
  static bool state = false;
  
  if (millis() - lastChange >= speed) {
    lastChange = millis();
    state = !state;
    
    if (state) {
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, HIGH);
    } else {
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
    }
  }
}

// Function for wave pattern
void wavePattern(int speed) {
  static unsigned long lastChange = 0;
  static int waveStep = 0;
  
  if (millis() - lastChange >= speed) {
    lastChange = millis();
    
    switch (waveStep) {
      case 0:
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, HIGH);
        digitalWrite(relay3, HIGH);
        break;
      case 1:
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, LOW);
        digitalWrite(relay3, HIGH);
        break;
      case 2:
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, LOW);
        digitalWrite(relay3, HIGH);
        break;
      case 3:
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, LOW);
        digitalWrite(relay3, LOW);
        break;
      case 4:
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, HIGH);
        digitalWrite(relay3, LOW);
        break;
      case 5:
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, HIGH);
        digitalWrite(relay3, LOW);
        break;
    }
    
    waveStep = (waveStep + 1) % 6;
  }
}

// Function for SOS morse code pattern
void morseCodeSOS() {
  static unsigned long lastChange = 0;
  static int sosStep = 0;
  static int dotDuration = 200; // Duration of a dot
  static int dashDuration = dotDuration * 3; // Duration of a dash
  static int elementGap = dotDuration; // Gap between elements (dots/dashes)
  static int letterGap = dotDuration * 3; // Gap between letters
  static int wordGap = dotDuration * 7; // Gap between words
  
  static int durations[] = {
    dotDuration, elementGap, dotDuration, elementGap, dotDuration, letterGap, // S (...)
    dashDuration, elementGap, dashDuration, elementGap, dashDuration, letterGap, // O (---)
    dotDuration, elementGap, dotDuration, elementGap, dotDuration, wordGap // S (...)
  };
  
  static bool states[] = {
    true, false, true, false, true, false, // S (...)
    true, false, true, false, true, false, // O (---)
    true, false, true, false, true, false  // S (...)
  };
  
  if (millis() - lastChange >= durations[sosStep]) {
    lastChange = millis();
    
    if (states[sosStep]) {
      allOn(); // Turn on all relays for visibility
    } else {
      allOff(); // Turn off all relays
    }
    
    sosStep = (sosStep + 1) % 18;
  }
}

// Function for strobe effect
void strobe(int speed) {
  static unsigned long lastChange = 0;
  static bool state = false;
  
  if (millis() - lastChange >= speed) {
    lastChange = millis();
    state = !state;
    
    if (state) {
      allOn();
    } else {
      allOff();
    }
  }
}

// Function for cascade effect
void cascade(int speed) {
  static unsigned long lastChange = 0;
  static int step = 0;
  
  if (millis() - lastChange >= speed) {
    lastChange = millis();
    
    switch (step) {
      case 0:
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, HIGH);
        digitalWrite(relay3, HIGH);
        break;
      case 1:
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, LOW);
        digitalWrite(relay3, HIGH);
        break;
      case 2:
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, LOW);
        digitalWrite(relay3, LOW);
        break;
      case 3:
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, LOW);
        digitalWrite(relay3, LOW);
        break;
      case 4:
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, HIGH);
        digitalWrite(relay3, LOW);
        break;
      case 5:
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, HIGH);
        digitalWrite(relay3, HIGH);
        break;
    }
    
    step = (step + 1) % 6;
  }
}

// Function for firefly effect - random, subtle blinking
void fireflyEffect() {
  static unsigned long lastChange[3] = {0, 0, 0};
  static unsigned long durations[3] = {0, 0, 0};
  static bool states[3] = {false, false, false};
  
  // Check each relay independently
  for (int i = 0; i < 3; i++) {
    if (millis() - lastChange[i] >= durations[i]) {
      lastChange[i] = millis();
      states[i] = !states[i];
      
      // Set new random duration
      if (states[i]) {
        // Light on duration: shorter (100-300ms)
        durations[i] = random(100, 300);
      } else {
        // Light off duration: longer (1000-3000ms)
        durations[i] = random(1000, 3000);
      }
    }
  }
  
  // Update relay states
  digitalWrite(relay1, states[0] ? LOW : HIGH);
  digitalWrite(relay2, states[1] ? LOW : HIGH);
  digitalWrite(relay3, states[2] ? LOW : HIGH);
}

// Function for alternating pairs pattern
void alternatingPairs(int speed) {
  static unsigned long lastChange = 0;
  static int pairStep = 0;
  
  if (millis() - lastChange >= speed) {
    lastChange = millis();
    
    switch (pairStep) {
      case 0:
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, LOW);
        digitalWrite(relay3, HIGH);
        break;
      case 1:
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, HIGH);
        digitalWrite(relay3, LOW);
        break;
      case 2:
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, LOW);
        digitalWrite(relay3, LOW);
        break;
    }
    
    pairStep = (pairStep + 1) % 3;
  }
}

// Function for thunder effect
void thunderEffect() {
  static unsigned long lastChange = 0;
  static int thunderStep = 0;
  static int thunderSequence[] = {1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0};
  static int thunderTiming[] = {50, 40, 100, 40, 300, 100, 50, 30, 70, 40, 200, 1000, 2000};
  
  if (millis() - lastChange >= thunderTiming[thunderStep]) {
    lastChange = millis();
    
    if (thunderSequence[thunderStep] == 1) {
      allOn();
    } else {
      allOff();
    }
    
    thunderStep = (thunderStep + 1) % 13;
  }
}

// Function for sequential pairs pattern
void sequentialPairs(int speed) {
  static unsigned long lastChange = 0;
  static int seqStep = 0;
  
  if (millis() - lastChange >= speed) {
    lastChange = millis();
    
    switch (seqStep) {
      case 0:
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, LOW);
        digitalWrite(relay3, HIGH);
        break;
      case 1:
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, LOW);
        digitalWrite(relay3, LOW);
        break;
      case 2:
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, HIGH);
        digitalWrite(relay3, LOW);
        break;
    }
    
    seqStep = (seqStep + 1) % 3;
  }
}

// Function to check button state and change pattern if pressed
void checkButtonForPatternChange() {
  // Read the button state (with pull-up resistor, LOW means pressed)
  int reading = digitalRead(buttonPin);
  
  // Check if the button state has changed
  if (reading != lastButtonState) {
    // Reset the debounce timer
    lastDebounceTime = millis();
  }
  
  // If the button state has been stable for longer than the debounce delay
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the button state has changed
    if (reading != buttonState) {
      buttonState = reading;
      
      // If the button is pressed (LOW with pull-up resistor)
      if (buttonState == LOW) {
        // Change to the next pattern
        currentPattern = (currentPattern + 1) % 20;
        patternStartTime = millis();
        
        // Print the current pattern number (optional)
        Serial.print("Button pressed! Switching to pattern: ");
        Serial.println(currentPattern + 1);
        
        // Small delay to prevent multiple triggers
        delay(200);
      }
    }
  }
  
  // Save the current reading for the next loop
  lastButtonState = reading;
}