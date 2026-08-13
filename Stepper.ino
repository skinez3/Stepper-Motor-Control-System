// Motor control variables
// Delay between steps in microseconds (lower value = higher physical speed)
int speedMotor = 80000;
int countNull = 0;   // Step counter
int countStep = 512; // Number of steps for one full revolution (360 degrees)

// Half-step sequence array for 28BYJ-48 stepper motor
int myArray[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};

// Hardware pin definitions for inputs
const int buttonIncrease = 3; // Button to increase motor speed
const int buttonDecrease = 2; // Button to decrease motor speed

// Hardware pin definitions for LED speed indicators
const int led1 = 7; // Represents 25% speed
const int led2 = 6; // Represents 50% speed
const int led3 = 5; // Represents 75% speed
const int led4 = 4; // Represents 100% speed

// Logical speed tracking
int speed = 1000; // Current logical speed
const int maxSpeed = 80000;
const int minSpeed = 1000;

void setup() {
  // Initialize button pins with internal pull-up resistors
  pinMode(buttonIncrease, INPUT_PULLUP);
  pinMode(buttonDecrease, INPUT_PULLUP);
  
  // Initialize LED pins as outputs
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  
  // Initialize motor driver pins as outputs (ULN2003 IN1-IN4)
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  
  // Set initial LED states to OFF
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
}

void loop() {
  // Handle speed increase request
  if (digitalRead(buttonIncrease) == HIGH) {
    if (speed < maxSpeed) {
      speed += 500;
      speedMotor -= 500; // Decrease delay to increase physical rotation speed
      
      // Clamp to maximum speed limits
      if (speed > maxSpeed) speed = maxSpeed; 
      
      updateLEDs();
      delay(100); // Debounce and UI update delay
    }
  }
  
  // Handle speed decrease request
  if (digitalRead(buttonDecrease) == HIGH) {
    if (speed > minSpeed) {
      speed -= 500;
      speedMotor += 500; // Increase delay to decrease physical rotation speed
      
      // Clamp to minimum speed limits
      if (speed < minSpeed) speed = minSpeed; 
      
      updateLEDs();
      delay(100); // Debounce and UI update delay
    }
  }
  
  // Execute motor rotation if the speed is above the minimum threshold
  if (speed > 1000){
    forward_rotate();
  }
}

// Triggers the next sequence of steps for forward rotation
void forward_rotate(){
  for (int i = 7; i >= 0; i--) {
    progressRotate(i);
    delayMicroseconds(speedMotor);
  }
}

// Writes the specific bit pattern to the motor driver pins based on the array index
void progressRotate(int outScore) {
  digitalWrite(11, bitRead(myArray[outScore], 0));
  digitalWrite(10, bitRead(myArray[outScore], 1));
  digitalWrite(9, bitRead(myArray[outScore], 2));
  digitalWrite(8, bitRead(myArray[outScore], 3));
}

// Updates the LED array based on the current speed percentage
void updateLEDs() {
  digitalWrite(led1, speed >= maxSpeed * 0.25 ? HIGH : LOW);
  digitalWrite(led2, speed >= maxSpeed * 0.50 ? HIGH : LOW);
  digitalWrite(led3, speed >= maxSpeed * 0.75 ? HIGH : LOW);
  digitalWrite(led4, speed >= maxSpeed ? HIGH : LOW);
}
