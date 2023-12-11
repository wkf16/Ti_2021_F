#include <Energia.h>





#include <Wire.h>
#include<String.h>
#define INTA1  39//正转
#define INTA2  40
#define INTB1  38 //正转
#define INTB2  37     
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
char rote[10];//记忆模式
char K210_rec[20]="";
char target_num = '0';
char contrl_flag;  //L,R,G三种情况
float x_diff =0;//巡线偏差值
int stop_flag =0;//1识别红色十字
int final_flag =0;//1终点
int final_flag_real=0;//0：前路程 //1：返路程
int count =0;//十字路口计数器
int I2Crec_flag;
struct mortor {
   int IN1pin;
   int IN2pin;
};
mortor mor1={INTA1,INTA2};
mortor mor2={INTB1,INTB2};
//————————————解释器函数————————
void rec_explain();
void getnums();
 void get_target();
//————————————— 运动函数————————
void mortor_drive(mortor mor1,int speed_mortor);
void left(mortor mor1, mortor mor2);
void right(mortor mor1, mortor mor2);
void setup_mor1(mortor mor1);
void setup_mor2(mortor mor2);
void brake(mortor mor1, mortor mor2);
void findline(mortor mor1, mortor mor2);
void turn_car(mortor mor1, mortor mor2,int potion_k210 );
void turn_car_end(mortor mor1, mortor mor2);
void turn_back(mortor mor1,mortor mor2);
 void cross(mortor mor1,mortor mor2);


 // ————————————————信息量函数————————
mySemaphore1.begin(0);
mySemaphore2.begin(0);



 void interper_setup(){
}
void interper_loop(){ 
}





void rec_explain(){
 //---------------解释器---------------------------
  char x_diff_char[4] = "";
  int i = 1;
//——————————————————————————————————  
  contrl_flag = '0';
  int num_flag = 0;
   while(K210_rec[i] != '$'){//遍历数字部分输出contrl_flag
     if(K210_rec[i] == target_num){contrl_flag = K210_rec[i+1];num_flag = 1;}//如果识别到目标数字，contrl_flag更新
     else if(num_flag == 0) contrl_flag = 'G';
     
     i++;
     }
//——————————————————————————————————    
  int diff_i = 0;
  i++;
  while(K210_rec[i] != '&'){//遍历x_diff部分输出x_diff
    x_diff_char[diff_i] = K210_rec[i];
    diff_i++;
    i++;
    }
  String x_diff_str = "";
  x_diff_str = x_diff_char;
  Serial.println(x_diff_str);
  x_diff = x_diff_str.toInt()- XIAOXI;//转浮点数
//——————————————————————————————————
  i++; 
  while(K210_rec[i]!='@'){//遍历stop_flag部分输出stop_flag
    stop_flag = K210_rec[i]-'0';
    i++;
    }    
//——————————————————————————————————
//遍历final_flag部分输出final_flag
    i++;
    final_flag = K210_rec[i]-'0';  
}
void getnums(){
      // 循环读取数据(除了最后一个字符)
    int i = 0;
    while (Wire.available())
    {
      // 接收字节数据并赋值给变量c(char)
        K210_rec[i]= Wire.read();
        i++;
    }
     //nums_str=nums;
}
 void get_target(){
//—————————获得目标数字—————————————————
  if(K210_rec[0] == '^'){
    target_num = K210_rec[1];
    }
}

void mortor_drive(mortor mor1,int speed_mortor){   
    if(speed_mortor>=0)
    {
              analogWrite(mor1.IN1pin,speed_mortor);
              analogWrite(mor1.IN2pin, 0);
    }
       else{
              analogWrite(mor1.IN1pin,0);
              analogWrite(mor1.IN2pin, speed_mortor);
        }
  }
  
 //-----------巡线代码---------------
//x_diff控制电机巡线
void findline(motor mor1,motor mor2){     
  int x_diff_abs = int(abs(x_diff));
  if(x_diff>0){//左转 
    motor1.drive(base-x_diff_abs);
    motor2.drive(base);  
    delay(10);        
      }
  else{
    mor1.drive(base);
    mor2.drive(base-x_diff_abs);
    delay(10);
    }
}
void left(mortor mor1, mortor mor2)
{
   mortor_drive(mor1,-speed_car_turn);
   mortor_drive(mor2,speed_car_turn);
}
void right(mortor mor1, mortor mor2)
{
   mortor_drive(mor2,-speed_car_turn);
   mortor_drive(mor1,speed_car_turn);
}

void setup_mor1(mortor mor1){ 
    pinMode(mor1.IN1pin, OUTPUT);
    pinMode(mor1.IN2pin, OUTPUT);
}

void setup_mor2(mortor mor2)
  { 
    pinMode(mor2.IN1pin, OUTPUT);
    pinMode(mor2.IN2pin, OUTPUT);
  }
void brake(mortor mor1, mortor mor2)
  {     
   analogWrite(mor2.IN1pin,255);
   analogWrite(mor2.IN2pin,255);
   analogWrite(mor1.IN1pin,255);
   analogWrite(mor1.IN2pin,255);
    }


void turn_car(mortor mor1, mortor mor2,int potion_k210 ){
  if(potion_k210){
     left(mor1,mor2);
     delay(delay_time_turn);
     }
  else   {
    right (mor1,mor2);
    delay(delay_time_turn);
    }//k210输入1则左转 0则右转，延迟delay_time_turn
 }
void turn_car_end(mortor mor1, mortor mor2){
    left(mor1,mor2);
    delay(delay_turn_car_end);
 }

//---------返回运动代码（不带判断）----------
void turn_back(mortor mor1,mortor mor2){
 if(stop_flag==1){//停止信号等于1时
  if(rote[count]!='G'){
    stop_car();//停车  
    if(rote[count]=='L'){
       turn_car(mor1,mor2,R_bit);//通过记忆转向,取反
       }
    if(rote[count]=='R'){
       turn_car(mor1,mor2,L_bit);//通过记忆转向
       } 
    }               
   count--;
 }
   findline(mor1,mor2);
}
