#include <SparkFun_TB6612.h>
#include<String.h>
#include <Wire.h>
char target_num = '0';
#define XIAOXI 70 //巡线偏差值
#define AIN1 4
#define BIN1 5
#define AIN2 11
#define BIN2 6
#define PWMA 9
#define PWMB 10
#define STBY 8
#define spe  255
#define delayend_time 200    //终点转向持续时间
#define delay_time_fow 100  //前进状态停止的时间
#define delay_time_stop 100     //识别到红色十字入口并决定停止的停止时间
#define delay_time_turn 100
#define turn 500            //转向时候的速度
#define base 255            //基准速度
#define R_bit 0
#define L_bit 1
const int offsetA =1;
const int offsetB =-1;
char rote[4];
Motor motor1=Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2=Motor(BIN1, BIN2, PWMB, offsetB, STBY);
char contrl_flag;  //L,R,G三种情况
float x_diff =0;//巡线偏差值
int stop_flag =0;//1识别红色十字
int final_flag =0;//1终点
int final_flag_real=0;//0：前路程 //1：返路程
int count =0;//十字路口计数器
int I2Crec_flag;
char K210_rec[20]="";
void setup() {
  Serial.begin(115200);
  Wire.begin(0x07);
  Wire.onReceive(getnums);
}

void loop() {
  //delay(50);
  while(target_num == '0'){
    get_target();
    Serial.println(target_num);
    }
  while(K210_rec[0] != '#'){
    Serial.println("别急");
    }
  if(K210_rec[0] == '#')rec_explain();
  Serial.print(target_num);
  Serial.print(',');
  Serial.print(x_diff);
  Serial.print(',');
  Serial.print(stop_flag);
  Serial.print(',');
  Serial.print(final_flag);
  Serial.print(',');
  Serial.println(contrl_flag);
  
  if(final_flag ==1){//终点判断
    final_flag_real=1;
    }
    control();
}


/**************************************
 ****** 接受k210的数据并更新数组**********
 **********                   **********
 **************************************/
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

//----------转向控制-----------------------
void turn_car(int potion_k210 ){
  if(potion_k210){
    left( motor1,motor2,turn);
    delay(delay_time_turn);
    }
  else {
    right (motor1,motor2,turn);
    delay(delay_time_turn);
    }//k210输入1则左转 0则右转，延迟delay_time_turn
}

//---------返回运动代码（不带判断）----------
void turn_back(){
 if(stop_flag==1){//停止信号等于1时
  if(rote[count]!='G'){
    stop_car();//停车  
    if(rote[count]=='L'){
       turn_car(R_bit);//通过记忆转向,取反
       }
    if(rote[count]=='R'){
       turn_car(L_bit);//通过记忆转向
       } 
    }               
   count--;
 }
   findline();
}
//---------前路程运动模型-------------
//小车未到达终点时的处理代码
void start_car(){
 if(stop_flag == 0)findline();//不停止就执行findline
 else{
   cross();//入口检测并动作
   if (final_flag==1);//final不执行
   the_end();
   }
}
//-----------全程控制模型-------------
void control(){
   if(final_flag_real=0){
    start_car();
    }
     else {   
         turn_back();
         }
}
//------------停车代码------------------
   void stop_car(){ 
   delay(delay_time_stop);
   brake(motor1, motor2);
}
//---------前程路口控制（不判断）-----------------
//开始路线时遇到十字路口处理的代码并收集路线状况
void cross(){
    stop_car();
    switch(contrl_flag){
      case'L':
      turn_car(L_bit);
      rote[count]='L';
      count++;
      break;
      
      case'R':
      turn_car(R_bit);
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
//-----------巡线代码---------------
//x_diff控制电机巡线
void findline(){     
  int x_diff_abs = int(abs(x_diff));
  if(x_diff>0){//左转 
    motor1.drive(base-x_diff_abs);
    motor2.drive(base);  
    delay(50);        
      }
  else{
    motor1.drive(base);
    motor2.drive(base-x_diff_abs);
    delay(50);
    }
}
//---------终点转向控制代码-----------
void the_end(){
  turn_car(R_bit);
  delay(delayend_time);//转向延迟
}
 void get_target(){
//—————————获得目标数字—————————————————
  if(K210_rec[0] == '^'){
    target_num = K210_rec[1];
    }

}

void rec_explain(){
 //-------------------解释器--------------------------------
  char x_diff_char[5];
  int i = 1;
//——————————————————————————————————  
  contrl_flag = 'G';
  int num_flag = 0;
   while(K210_rec[i] != '$'){//遍历数字部分输出contrl_flag
     if(K210_rec[i] == target_num){contrl_flag = K210_rec[i+1];num_flag = 1;}
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
  String x_diff_str;
  x_diff_str = x_diff_char;
  x_diff = x_diff_str.toFloat()- XIAOXI;//转浮点数
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

                                                                                                                                                                            
 
                                                                                                                                                                          






 
 
