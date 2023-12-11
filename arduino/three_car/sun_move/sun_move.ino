#include <SparkFun_TB6612.h>
#define AIN1 4
#define BIN1 5
#define AIN2 11
#define BIN2 6
#define PWMA 9
#define PWMB 10
#define STBY 8
#define spe  255
#define delay_three_time_turn  100
#define three_three_turn_false  100
int delayend_time=200  ;  //终点转向持续时间
int  delay_time_fow=100;  //前进状态停止的时间
int  delay_time_stop =100;     //识别到红色十字入口并决定停止的停止时间
int turn=50  ;          //转向时候的速度
float  base=255;           //基准速度
const int offsetA = 1;
const int offsetB = -1;
char rote[10];
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);
char contrl_flag;  //L,R,G三种情况
float x_diff = 0;//巡线偏差值
int stop_flag = 0;//1识别红色十字
int final_flag = 0;//1终点
int final_flag_real=0;//判断是否已经经历终点
int count =0;//十字路口计数器
void setup() {
}

void loop() {
  if(final_flag ==1){
 final_flag_real=1;
    }
    control();
 
  

}
void turn_car(int potion_k210 ){
  if(potion_k210){
 left( motor1,motor2,turn);}
  else {
    right (motor1,motor2,turn);
 }//k210输入1则左转 0则右转
}
void turn_back()//消除个返回的时候的处理代码
 {
  if(stop_flag==1)
  if(rote[count]!='G'){
    stop_car() ;  
 if(rote[count]=='L'){
  turn_car(0);
  }
  else 
  {turn_car(1);
  }
 
                     }
                   
   count--;
    findline();
 }
 void start_car()//小车未到达终点时的处理代码
{

 if(stop_flag ==0){
 findline();
                 }
else   {
       if(rote[1]=='G'||rote[2]=='G'||count==3){
        three_turn;
        }
       else{
        cross();
       if (final_flag==1);
       the_end();
       }
       }
}
void control(){
   if(final_flag_real=0){
    start_car();
    }
     else {   
         turn_back();
         }
  }
   void stop_car()//停车代码
{ delay(delay_time_stop) ;
   brake(motor1, motor2);
}


void cross()//开始路线时遇到十字路口处理的代码病收集路线状况
{
  
   stop_car();
    switch(contrl_flag){
      case'L':
      turn_car(1);
      rote[count]='L';
      count++;
      break;
        case'R':
      turn_car(0);
      rote[count]='R';
      count++;
      break;
      case'G':
      forward(motor1, motor2, 255);
      delay(delay_time_fow);
      rote[count]='G';
      count++;
      break;
                    }
         }
 void findline  ()    //巡线代码
{ float x_diff_=0.0;
  x_diff_= x_diff;
    x_diff_=abs(x_diff_);
   if(x_diff>0)//左转
{ 
            motor1.drive(base-x_diff_);
            motor2.drive(base);          
 }
  else{
  
   motor1.drive(base);
   motor2.drive(base-x_diff_);
       } 
}

void the_end()//终点转向代码
{
  turn_car(0);
delay(delayend_time);
 }
  void three_turn (){

      left(motor1,motor2,three_turn);
      delay(delay_three_time_turn);
      stop_car();
      if(contrl_flag!='G')
      {rote[count]='L';
      count++;  
        }
      
      else{
        turn_car(0);
        delay(three_three_turn_false);
        rote[count]='R';
        count++;
        }
  }

                                                                                                                                                                            
 
                                                                                                                                                                          






 
 
