#include <SparkFun_TB6612.h>

#define AIN1 4
#define BIN1 5
#define AIN2 11
#define BIN2 6
#define PWMA 9
#define PWMB 10
#define STBY 8
#define spe  255
#define delay_time1      100  //停止的时间
#define delay_time2      100  //停止的时间
#define delay_time3      100  //停止的时间
int turn=50  ;          //转向时候的速度
float  dev=0.0;               //偏差值
float  base=255;           //基准速度
int stop_k210=0 ;           //k210判断是否停下
int potion_k210=0;
const int offsetA = 1;
const int offsetB = -1;

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);
void setup() {
  
}

void  loop()
{ 
  control (motor1,motor2,stop_k210,potion_k210,base,dev,turn);
  }
void control (Motor motor1,Motor motor2,int stop_k210,int potion_k210,int base,float dev,int turn)//平常一直在巡线 当k210识别到数字的时候发送信息让小车停下 停下后转向 转向后巡线
{
  if(stop_k210==1 )
{
  find_line(motor1,motor2, base,dev);
  }
    else
    {
  stop_car();
 turn_car(potion_k210,turn);
 delay(500);
    }

}

  
void turn_car(bool k210,int turn){
  if(potion_k210){
 left( motor1,motor2,turn);}
  else {
    right (motor1,motor2,turn);
 }//k210输入1则左转 0则右转
}



  void find_line(Motor motor1, Motor motor2, int base,float dev)    //巡线代码
{ 
  dev= (int)dev;
    int dev_=0;
    dev_=abs(dev);
   if(dev>0)//左转
{ 
            motor1.drive(base-dev_);
            motor2.drive(base);          
           }
   else{
  
   motor1.drive(base);
   motor2.drive(base-dev_);
       }   
}//当在正常运行时可以调整车子与线的位置
void stop_car()//通过k210的值来控制运动与停止
{ delay(delay_time3) ;
   brake(motor1, motor2);
  }
  
