#include <SparkFun_TB6612.h>

int motor_c_ENA=6;   //控制板与驱动板的引脚连接
int motor_c_IN1=8;
int motor_c_IN2=7;
#define AIN1 4
#define BIN1 5
#define AIN2 11
#define BIN2 6
#define PWMA 9
#define PWMB 10
#define STBY 8
#define ENCODER_A_PIN  2     //编码器A相接控制板2号引脚，对应0号外部中断
#define ENCODER_B_PIN  12     //编码器B相接控制板3号引脚，
#define spe  255
#define turn  50            //转向时候的速度
#define delay_time1      100  //停止的时间
#define delay_time2      100  //停止的时间
#define delay_time3      100  //停止的时间
const int offsetA = 1;
const int offsetB = -1;
float  dev=0.0;               //偏差值
float  base=255;           //基准速度
int stop_k210=0 ;           //k210判断是否停下
int potion_k210=0;
int potion_k210_key=0;


Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

void setup() {
}

void loop() {
  motor1.drive(turn);
  motor2.drive(-turn);
  delay(1000);
  brake(motor1, motor2);
  
  motor1.drive(-turn);
  motor2.drive(turn);
  delay(1000);
  brake(motor1, motor2);
}
void control ()//平常一直在巡线 当k210识别到数字的时候发送信息让小车停下 停下后转向 转向后巡线
{  if(stop_k210=1 )
{find_line(motor1,motor2, base,dev);}
if (stop_k210=1)
{stop_car();}
if(potion_k210_key=1)
{
 turn_car(potion_k210);
}
   
  
  


  


  

   
  }
void turn_car(bool k210){
  if(potion_k210){
 left_drive ( motor1,motor2);}
  else {
    left_drive (motor1,motor2);
 }//k210输入1则左转 0则右转
}
void left_drive (Motor motor1, Motor motor2){
  motor1.drive(turn);
  motor2.drive(-turn);
   brake(motor1, motor2);
  
  }

void right_drive (Motor motor1, Motor motor2){
  motor1.drive(-turn);
  motor2.drive(turn);
   brake(motor1, motor2);
  
  }
  
  void find_line(Motor motor1, Motor motor2, int base,float dev)    //巡线代码
{   dev= (int)dev;
    int dev_=0;
    dev_=abs(dev);
   if(dev>0)//左转
           {
  motor1.drive(base);
  motor2.drive(base-dev_);
           }
   else{
   motor1.drive(base-dev);
   motor2.drive(base);
            }   
}//当在正常运行时可以调整车子与线的位置
void stop_car()//通过k210的值来控制运动与停止
{ int delay_time=100;
   brake(motor1, motor2);
  delay(delay_time3) ;
  }
  
