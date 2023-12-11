#include "basicMovement.h"
Motor motor;
void setup() 
{
  motor.setPins(37,38,40,39);
  motor.set_speed_turn(255);

}

void loop() {
 motor.right();
  
}
