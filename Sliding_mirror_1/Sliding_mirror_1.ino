#include <Servo.h>
//~~~~~~~~~~~~~~~~~~~~~~~~~ can edit ~~~~~~~~~~~~~~~~~~~~~~~~~//

int servoPin    = 3;       //pin to which servo has been attached
int switchPin   = 4;       //pin to which stitch has been attached
int servo_pos_0 = 0;       //'off' position
int servo_pos_1 = 116;     //'on' position, calibrate to give slight tension on the spring to ensure repeatable position
int step_time   = 50;      //milliseconds spent on each step, determines servo move speed

//~~~~~~~~~~~~~~~~~~~~~~~~~ don't edit ~~~~~~~~~~~~~~~~~~~~~~~~~//

Servo servo;
void setup() {
  servo.attach(servoPin); 
  pinMode(switchPin,INPUT_PULLUP);
  Serial.begin(115200);
}


int d_servo_position = servo_pos_1;
int c_servo_position = 80;
long unsigned int prev_time = 0;
bool stable = false;

void update_servo_position(){
  long unsigned int current_time = millis();
  if(current_time > prev_time + 50){
    prev_time = current_time;
    if(d_servo_position > c_servo_position) {
      c_servo_position = c_servo_position + 1;
      servo.write(c_servo_position);
      stable = false;
      }
    if(d_servo_position < c_servo_position) { 
      c_servo_position = c_servo_position - 1; 
      servo.write(c_servo_position);
      stable = false;
      }
    if(d_servo_position == c_servo_position && not stable){
      stable = true;
      Serial.println("/stable;");
    }
  }
}

// serial commands
String device = "";
String command1 = "";
int serial_part = 0;

void check_serial(){   
  char rc;
  while (Serial.available()) {
   rc = Serial.read();
   if      (rc == 47)         {serial_part = 1; device = "";command1 = "";}   // '/' char
   else if (rc == 46)         {serial_part += 1;}                             // '.' char
   else if (rc == 59)         {respond(device,command1); serial_part = 0;}    // ';' char
   else if (serial_part == 1) {device   += rc;}
   else if (serial_part == 2) {command1 += rc;}
 }
}

void respond(String device,String command1) {
    if(device == "A")         {d_servo_position = servo_pos_1;}
    if(device == "B")         {d_servo_position = servo_pos_0;}
  }

int prev_switch_position  = 0;

void check_switch(){
  if(digitalRead(switchPin) != prev_switch_position) { // the switch has been moved
    prev_switch_position = digitalRead(switchPin);
    if      (d_servo_position == servo_pos_0)  {d_servo_position = servo_pos_1;}  //toggle position
    else if (d_servo_position == servo_pos_1)  {d_servo_position = servo_pos_0;}  //toggle position
  }
}

void loop() {
  update_servo_position();
  check_switch();
  check_serial();
  
}

