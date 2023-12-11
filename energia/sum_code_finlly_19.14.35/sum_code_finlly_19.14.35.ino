#include <String.h>
#define INTA1  40//正转
#define INTA2  39
#define INTB1  37 //正转
#define INTB2  38     
#define spe  255
#define delayend_time 200    //终点转向持续时间
#define delay_time_fow 100  //前进状态停止的时间
#define delay_time_stop 100     //识别到红色十字入口并决定停止的停止时间
#define delay_time_turn 100
#define turn 500            //转向时候的速度
#define base 255            //基准速度
#define R_bit 0
#define L_bit 1
#define delay_three_time_turn  100
#define three_three_turn_false  100
#define XIAOXI                  30
#define speed_car_turn          100
#define base_car                100
#define delay_turn_car_end      100
#define data_long 10



int count2=0;
int count1_really=0;
char div_num[data_long];
char div_digital_recognition[data_long];
char div_cross[data_long];
char div_end[data_long];
char div_x[data_long];
char data[data_long];
int count_div_digital_recognition=0;
int count_div_cross=0;
int count_div_end=0;
int count_div_x=0;
int count_sum=0;




char rote[10] = "";//记忆模式
char K210_rec[20] = "";
char target_num = '0';
char contrl_flag = 'G';  //L,R,G三种情况
int x_diff =0;//巡线偏差值
int stop_flag =0;//1识别红色十字
int final_flag =0;//1终点
int final_flag_real=0;//0：前路程 //1：返路程
int count =0;//十字路口计数器
struct motor {
   int IN1pin;
   int IN2pin;
};

motor mor1={INTA1,INTA2};
motor mor2={INTB1,INTB2};

int speed_motor = 0;
int position_k210 = 0;

//————————————解释器函数————————

//————————————— 运动函数————————
void motor_drive(motor mor1,int speed_motor);
void left(motor mor1, motor mor2);
void right(motor mor1, motor mor2);
void setup_mor1(motor mor1);
void setup_mor2(motor mor2);
void brake(motor mor1, motor mor2);
void turn_car(motor mor1, motor mor2,int position_k210 );
void turn_car_end(motor mor1, motor mor2);
void cross(motor mor1,motor mor2);
void findline(motor mor1, motor mor2,int x_diff);
void turn_back(motor mor1,motor mor2);
// ————————————————信息量函数————————
void recFun(char data[data_long]);
void assigment(char a[data_long],int count2,int count1_really);
void assimgent_time(char stand_char1,char stand_char2,int count2,int count1_really,int count_time);
void setup(){
    
}
void loop(){
  
}

void motor_drive(motor mor1,int speed_motor){   
    if(speed_motor>=0)
    {
              analogWrite(mor1.IN1pin,speed_motor);
              analogWrite(mor1.IN2pin, 0);
    }
       else{
              analogWrite(mor1.IN1pin,0);
              analogWrite(mor1.IN2pin, -speed_motor);
        }
  }
  

void left(motor mor1, motor mor2)
{
   motor_drive(mor1,speed_car_turn);
   motor_drive(mor2,-speed_car_turn);
}
void right(motor mor1, motor mor2)
{
   motor_drive(mor2,speed_car_turn);
   motor_drive(mor1,-speed_car_turn);
}

void setup_mor1(motor mor1){ 
    pinMode(mor1.IN1pin, OUTPUT);
    pinMode(mor1.IN2pin, OUTPUT);
}

void setup_mor2(motor mor2)
  { 
    pinMode(mor2.IN1pin, OUTPUT);
    pinMode(mor2.IN2pin, OUTPUT);
  }

void brake(motor mor1, motor mor2)
  {     
   analogWrite(mor2.IN1pin,255);
   analogWrite(mor2.IN2pin,255);
   analogWrite(mor1.IN1pin,255);
   analogWrite(mor1.IN2pin,255);
    }


void turn_car(motor mor1, motor mor2,int position_k210 ){
  if(position_k210){
     left(mor1,mor2);
     delay(delay_time_turn);
     }
  else   {
    right (mor1,mor2);
    delay(delay_time_turn);
    }//k210输入1则左转 0则右转，延迟delay_time_turn
 }
void turn_car_end(motor mor1, motor mor2){
    left(mor1,mor2);
    delay(delay_turn_car_end);
 }


 //-----------巡线代码---------------
//x_diff控制电机巡线
void findline(motor mor1,motor mor2,int x_diff){     
  int x_diff_abs = int(abs(x_diff));
  if(x_diff>0){//左转 
    motor_drive(mor1,base-x_diff_abs);
    motor_drive(mor2,base);
    delay(10);        
    }
  else{
    motor_drive(mor1,base);
    motor_drive(mor2,base-x_diff_abs);
    delay(10);
    }
}
//---------返回运动代码（不带判断）----------
void turn_back(motor mor1,motor mor2){
 if(stop_flag==1){//停止信号等于1时
   if(rote[count]!='G'){
     brake(mor1, mor2);//停车  
     if(rote[count]=='L'){
       turn_car(mor1,mor2,R_bit);//通过记忆转向,取反
       }
     if(rote[count]=='R'){
       turn_car(mor1,mor2,L_bit);//通过记忆转向
       } 
     }               
   count--;
   }
   findline(mor1,mor2,x_diff);
}






void recFun(char data[data_long])
{
  if ( Serial.available() && (char)Serial.read() == '#' )
  {
    data[0] = '#';
    int i = 1;
    char rec = 0;
    while(rec != '^')
    {
      rec = (char)Serial.read();
      data[i] = rec;
      i++;
    }
  }
}
void div_char(char stand_char1,char stand_char2,int count2,int count1_really){
  int i=0;
  int count1=0;
  count2=0;
  count1_really=0;
  while( stand_char1 !=data[count1])//找到标识符1所在位置
  {
    count1++;
  }
  while( stand_char2 !=data[count2])//找到标识符二所在位置
  {
    count2++;
  }
  count1_really=count1; 
  while(i!=count2-count1_really-1){
    div_num[i]=data[count1+1];
    i++;
    count1++;
  }
}

void assigment(char a[data_long],int count2,int count1_really){
  int i=0;
  while(i!=count2-count1_really-1){
    a[i]=div_num[i];
  } 
}
