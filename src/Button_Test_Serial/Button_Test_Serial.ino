/*
  Button_Test_Serial
  
  Reads the state of every button on each loop and prints it to the Serial Monitor.

  Trent Dye 2018
  
 */

#define BUTTON_A 2
#define BUTTON_B 3
#define BUTTON_R 7
#define BUTTON_D 5
#define BUTTON_U 6
#define BUTTON_L 4

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_R, INPUT_PULLUP);
  pinMode(BUTTON_D, INPUT_PULLUP);
  pinMode(BUTTON_U, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(digitalRead(BUTTON_A));
  Serial.print('\t');
  Serial.print(digitalRead(BUTTON_B));
  Serial.print('\t');
  Serial.print(digitalRead(BUTTON_R));
  Serial.print('\t');
  Serial.print(digitalRead(BUTTON_D));
  Serial.print('\t');
  Serial.print(digitalRead(BUTTON_U));
  Serial.print('\t');
  Serial.print(digitalRead(7));
  Serial.println();
}
