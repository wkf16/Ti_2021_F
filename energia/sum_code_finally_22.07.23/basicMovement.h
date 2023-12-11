/****************************************************
 * 基础运动模型，需输入：左右电机对应引脚
 * 可选输入：基准速度（巡线用），基准转向速度（转弯用）
 * 直接操控电机
*****************************************************/
#define basicMovement_h
#include <Energia.h>
class Motor
{
  public:
    int kp = 2;
    int speed_car_turn = 100;
    int line_base_speed = 150;
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
