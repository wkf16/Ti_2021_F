/*
  模块用途：基础运动函数库，指令集，与收发
*/
#include <Energia.h>
using namespace std;
class Motor
{
  public:
    int speed_car_turn = 100;
    int line_base_speed = 255;
    void setPins( int leftPIN1, int leftPIN2, int rightIN1, int rightIN2 ); //设置两个电机对应针脚
    void drive( char motorTyoe, int val ); //对单个电机进行驱动
    void set_speed_turn(int car_turn_val); //设置转向速度（默认100）
    void left();
    void right();
    void brake();
    void line_following(int x_diff);
    void forward(int val);
  private:
    int L_IN1;
    int L_IN2;
    int R_IN1;
    int R_IN2;
};

void Motor::setPins( int A_PIN1, int A_PIN2, int B_PIN1, int B_PIN2 )
{
  L_IN1 = A_PIN1;
  L_IN2 = A_PIN2;
  R_IN1 = B_PIN1;
  R_IN2 = B_PIN2;
  pinMode(L_IN1, OUTPUT);
  pinMode(L_IN2, OUTPUT);
  pinMode(R_IN1, OUTPUT);
  pinMode(R_IN2, OUTPUT);
}

void Motor::set_speed_turn( int car_turn_val )
{
  speed_car_turn = car_turn_val;
}

void Motor::drive( char motorType, int val ) //输入电机类型(A/B)，实现对电机单独操控
{
  if (val < 0)
  {
    val = abs(val);
    if (motorType == 'R')
    {
      digitalWrite(R_IN1, LOW);
      analogWrite(R_IN2, val);
    }
    else
    {
      digitalWrite(L_IN1, LOW);
      analogWrite(L_IN2, val);    
    }
 }
 else
 {
    if (motorType == 'R')
  {
    analogWrite(R_IN1, val);
    digitalWrite(R_IN2, LOW);
  }
  else
  {
    analogWrite(L_IN1, val);
    digitalWrite(L_IN2, LOW);    
  }
 }

}

void Motor::left()
{
  drive('L', speed_car_turn);
  drive('R', -speed_car_turn);
}

void Motor::right()
{
  drive('R', speed_car_turn);
  drive('L', -speed_car_turn);
}

void Motor::brake()
{     
  analogWrite(L_IN1,255);
  analogWrite(L_IN2,255);
  analogWrite(R_IN1,255);
  analogWrite(R_IN2,255);
}

void Motor::line_following(int x_diff)
{
  int x_diff_abs = (int)abs(x_diff);
  if(x_diff>0)
  {
    drive('L',line_base_speed-x_diff_abs);
    drive('R',line_base_speed);
    delay(10);        
  }
  else
  {
    drive('R',line_base_speed-x_diff_abs);
    drive('L',line_base_speed);
    delay(10);   
  }
}
void Motor::forward(int val)
{
  drive('R', val);
  drive('L', val);
}
