#include <Servo.h>

int leftMotorPin1 = 2;
int leftMotorPin2 = 3;
int rightMotorPin1 = 4;
int rightMotorPin2 = 5;
int left_ENA_pin = 6;
int right_ENB_pin = 11;
int s1_pin = 9;
int s2_pin = 10;

int direction_;
int speed_;
bool promptComm;
String command;

int pos1;
int pos2;

Servo s1; //(left servo)
Servo s2; //(right servo)

void setup() {
  Serial.begin(9600);
  while (!Serial);

  //Setting Up Pins
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(left_ENA_pin, OUTPUT);
  pinMode(right_ENB_pin, OUTPUT);
  Serial.println("All pins initialized.");

  //Initial Speed and Direction
  setSpeed(0);
  direction_ = 0;
  Serial.println("Speed and Direction initialized to OFF.\n");

  promptComm = true;

  pos1 = 90;
  pos2 = 90;
  s1.attach(s1_pin);
  s2.attach(s2_pin);
  s1.write(pos1);
  s2.write(pos2);
}

void loop() {
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');

    if (command.equals("direction")) {
      Serial.print("Current direction: ");
      Serial.println(direction_);
      changeDirection();
    }
    else if (command.equals("speed")) {
      Serial.print("Current speed: ");
      Serial.println(speed_);
      changeSpeed();
    }
    else if (command.equals("u")) {
      Serial.print("Current speed: ");
      Serial.println(speed_);
      incrementSpeed();
    }
    else if (command.equals("d")) {
      Serial.print("Current speed: ");
      Serial.println(speed_);
      decrementSpeed();
    }
    else if (command.equals("on")) {
      enableCar();
    }
    else if (command.equals("off")) {
      disableCar();
    }
    else if (command.equals("t")) {
      Serial.println("Transforming");
      transform();
    }
    else {
      for (int i = 0; i < 10; i++) {
        Serial.println("");
      }
      Serial.println("Invalid Command");
      delay(2000);
      promptComm = true;
    }
  }
  
  if (promptComm) {
    promptCommand();
    promptComm = false;
    for (int i = 0; i < 10; i++) {
      Serial.println("");
    }
  }
}

void enableCar() {
  //Turns on the car to default values
  //Default direction: FORWARD
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);

  //Default speed: 255
  speed_ = 250;
  setSpeed(speed_);

  Serial.println("Car is now on!\n");
  Serial.print("Speed: ");
  Serial.println(speed_);
  Serial.print("Direction: ");
  Serial.println(direction_);
  promptComm = true;
}

void disableCar() {
  //Turns off the car to default values
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, LOW);

  //Default speed: 0
  speed_ = 0;
  setSpeed(speed_);

  Serial.println("Car is now off!\n");
  Serial.print("Speed: ");
  Serial.println(speed_);
  Serial.print("Direction: ");
  Serial.println(direction_);
  promptComm = true;
}

void changeDirection() {
  if (direction_ == 0) { //Direction is FORWARD
    //Change to REVERSE
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
    direction_ = 1;
  }
  else {//Direction is REVERSE
    //Change to FORWARD
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
    direction_ = 0;
  }
  
  Serial.print("New Direction: ");
  Serial.println(direction_);
  Serial.println("Direction changed!\n");
  promptComm = true;
}

void setSpeed(int speed_) {
  analogWrite(left_ENA_pin, speed_);
  analogWrite(right_ENB_pin, speed_);
}

void changeSpeed() {
  Serial.println("\nPlease enter new number for speed (0-255):");
  bool loop = true;
  int new_speed;
  do {
    //Serial.println("Waiting...");
    if (Serial.available()) {
      new_speed = Serial.parseInt();
      loop = false;
    }
  }
  while (loop);

  Serial.print("New Speed:");
  Serial.println(new_speed);
  setSpeed(new_speed);
  
  promptComm = true;
}

void incrementSpeed() {
  Serial.print("THE SPEED: ");
  Serial.println(speed_);
 
  speed_ += 10;
  if (speed_ > 245) {
    Serial.println("SPEED KINDA WORKS");
    speed_ = 255;
  }

  Serial.print("THE NEXT SPEED: ");
  Serial.println(speed_);

  
  setSpeed(speed_);
  
  Serial.print("New Speed: ");
  Serial.println(speed_);
  Serial.println("Speed incremented!\n");
  promptComm = true;
}

void decrementSpeed() {
  Serial.print("THE SPEED: ");
  Serial.println(speed_);
 
  speed_ -= 10;
  if (speed_ < 0) {
    Serial.println("SPEED KINDA WORKS");
    speed_ = 0;
  }

  Serial.print("THE NEXT SPEED: ");
  Serial.println(speed_);

  
  setSpeed(speed_);
  
  Serial.print("New Speed: ");
  Serial.println(speed_);
  Serial.println("Speed decremented!\n");
  promptComm = true;
}

void transform() {
  Serial.print("Pos1: ");
  Serial.println(pos1);
  
  if (pos1 == 90) {
    for (pos1 = 90; pos1 >= 0; pos1--) {
      s1.write(pos1);
    }
    for (pos2 = 90; pos2 <= 180; pos2++)  {
      s2.write(pos2);
    }
    pos1 = 0;
    pos2 = 180;
  }
  else {
    for (pos1 = 0; pos1 <= 90; pos1++) {
      s1.write(pos1);
      //delay(15);
    }
    for (pos2 = 180; pos2 >= 90; pos2--) {
      s2.write(pos2);
      //delay(15);
    }
    pos1 = 90;
    pos2 = 90;
  }

  Serial.println("TRANSFORMED!");
  promptComm = true;
}
void promptCommand() {
  for (int i = 0; i < 10; i++) {
    Serial.println("");
  }
  Serial.println("Please enter one of the following commands:");
  Serial.println("\"direction\" - Change Direction between Forward/Reverse");
  Serial.println("\"speed\" - Change Speed to Discrete Value");
  Serial.println("\"u\" - Increment Speed by 10");
  Serial.println("\"d\" - Decrement Speed by 10");
  Serial.println("\"on\" - Turn on the car (uses previous speed and direction)");
  Serial.println("\"off\" - Turn off the car");
  Serial.println("\"t\" - Transform");
  //promptComm = false;
}
