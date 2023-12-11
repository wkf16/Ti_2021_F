String rec_V831_str = "";
#include "basicMovement.h"
//char *p_rec_V831 = rec_V831;
String recFun();
int num_flag = 0;
int x_diff  = 0;
int cross_flag = 0; //返回是不是路口
int cross_count =0; //返回第几个路口
int final_flag = 0;
int start_flag = 0;
char control_flag = 0; //控制指令
char target_num = 0;

String explain_str(char flag_start, char flag_end, String input_str);
char get_num(String rec_V831_str);
char get_contrl_flag(String rec_V831_str);
Motor motor;

void setupRec() {
  

  Serial.begin(115200);
  Serial1.begin(115200);
}
void loopRec() {
  //Serial1.println("233333");
  rec_V831_str = recFun(); //接收传感器数据
  if(rec_V831_str != ""){ //接收目标数字
    Serial1.println(start_flag);
    Serial1.print("target_num:");
    Serial1.println(target_num);
    if (start_flag==0){
      target_num = get_num(rec_V831_str);
      if(target_num != 0)start_flag++;
      }//当start_flag == 0时，解释收到的目标数字
    else if(start_flag == 1 && rec_V831_str != ""){
       control_flag = get_contrl_flag(rec_V831_str); //执行指令(左or右or前进)
       x_diff = explain_str( '$','&',rec_V831_str ).toInt(); //巡线用
       cross_flag = explain_str( '&','@',rec_V831_str ).toInt(); //是否十字路口   //stop_flag?
       final_flag = explain_str( '@','*',rec_V831_str ).toInt(); //是否终点
       //需要：更新识别到的数字，方向，一旦数字可行出发，记下第几个路口
       /*if(control_flag != 0){
        Serial1.print("contrl_flag:");
        Serial1.println(control_flag);
        cross_flag = 1;
       }*/
       Serial1.print("contrl_flag:");
       Serial1.print(control_flag);
       Serial1.print(",");
       Serial1.print("x_diff:");
       Serial1.print(x_diff);
       Serial1.print(",");
       Serial1.print("cross_flag:");
       Serial1.print(cross_flag);
       Serial1.print(",");
       Serial1.print("final_flag:");
       Serial1.print(final_flag);
       Serial1.print(",");
       Serial1.print("cross_count:");
       Serial1.println(cross_count);
       }
     }
}

int contrl_once = 0;



void setupMove() {
    motor.setPins(37,38,40,39);
    motor.speed_car_turn = 255;
    motor.line_base_speed = 200;
    motor.kp = 4; 
    pinMode(RED_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
}
/*
具体执行器
*/
int stop_count = 0;
int count_start = 1;
void loopMove() {

 if(start_flag == 1){
  //count_start = 1;
   if (count_start)
  {
    delay(1000);
    count_start = 0;
   }
  
  if(cross_flag == 0){
   // delay(1000);
    digitalWrite(RED_LED, HIGH);
    motor.line_following(x_diff);
    }
  else if(cross_flag == 1)
  {
    if(num_flag == 0 && stop_count == 0){
      motor.forward(-200);
      delay(100);
      motor.brake();
      stop_count++;
      }
    if(control_flag == 0){;}
    digitalWrite(RED_LED, LOW);
    //识别到数字结果，进入执行
    if(control_flag == 'L')
    {
      digitalWrite(GREEN_LED, HIGH);
      motor.forward(100);
      delay(1000);
      motor.left();
      delay(1000);
      motor.brake();
      num_flag = 1;
      stop_count = 0;
    }
    if(control_flag == 'R')
    {
      digitalWrite(BLUE_LED, HIGH);
      motor.forward(100);
      delay(1000);
      motor.right();
      delay(1000);
      motor.brake();
      num_flag = 1;
      stop_count = 0;
      
    }
    if(control_flag == 'G')
    {
      digitalWrite(RED_LED, HIGH);
      motor.line_following(x_diff);
      cross_flag = 0;
      num_flag = 1;
      stop_count = 0;
      
    }
  }
 }
}

/*
判断路口的循环，决定执行器策略
*/
/*void setupcross()
{
  
}
int nums = 0;
//由于识别到数字时会返回放行信号，stop_flag置0，故放弃使用stop_flag作为进入本循环标志。考虑到路口一定有数字，将是否识别到两个数字作为本循环进入标志
void loopcross()
{

  if(start_flag==1 && rec_V831_str != "")
  {   
    if(control_flag != 0)
    {
        num_flag = 0;
    }
  }
    /*String num_frame = explain_str( '#','$',rec_V831_str );//获得当前传感器数据帧的数字位结果
    if(num_frame.indexOf('L') != -1 && num_frame.indexOf('R') != -1) //判断是否识别到有效数字
    {
      Serial1.println("LR found");
      control_flag = get_contrl_flag(rec_V831_str);
      //cross_flag = 1;
      num_flag = 0;
      //String num_frame = explain_str( '#','$',rec_V831_str );
      //Serial1.print("nums:");
      //Serial1.println(num_frame);
      //判断识别到的数字是否和上一次识别一样
      char nums_arr[5] = {0};
      num_frame.toCharArray(nums_arr,num_frame.length());

      int nums2 = (nums_arr[0] - '0') * ( nums_arr[2] - '0');//通过两数相乘判断是否与上一次识别一致
      Serial1.print("nums:");
      Serial1.println(nums2);
      if(nums != nums2)
      {
        
        Serial1.println("control flag updated");
        
        cross_count++; //让路口计数++
        nums = nums2;
      }
    }
  }*/
//}
/*
 * 函数功能:获取当前传感器传入的数据帧
 * 函数输入:无
 * 函数返回:数据帧字符串
 */
String recFun(){
  //Serial.println("entered");
  String rec_V831_str = "";
  static char data[20] = {0};
  if(Serial.available() && (char)Serial.read() == '#')
  {
    rec_V831_str = '#' + Serial.readStringUntil('^') + '^';
  }
  return rec_V831_str;
}

char get_num(String rec_V831_str){
if (rec_V831_str.indexOf('S')!= -1){
  String target_num_str;
  char target_num_ar[2] = "";
  target_num_str = explain_str( 'S','^',rec_V831_str );
  int len = target_num_str.length() + 1;
  target_num_str.toCharArray(target_num_ar,len);
  Serial1.println(target_num_ar);
  return target_num_ar[0];
  }
else
  return 0;
}
/*
 * 函数功能:基于识别到的数据获得control_flag
 * 函数输入:当前数据帧
 * 函数返回:指令(L,R,G)
 */
char get_contrl_flag(String rec_V831_str){
  char flag = 'G';
  if (control_flag != 0 && num_flag == 0)
  {
    flag = control_flag;
    return flag;
  }
  
  String nums_str = explain_str( '#','$',rec_V831_str );//取得#和$中间的值
  char nums_ar[5] = ""; //转字符串数组
  int len = nums_str.length() + 1;
  
  nums_str.toCharArray(nums_ar,len);
  if (nums_str != "" && nums_str != "0000"){
    for(int i=0;i<4;i++){
      if(nums_ar[i] == target_num){
        flag = nums_ar[i+1];
        num_flag = 0;
        break;
        }
      }
    }
  else{

      flag = 0;
    
    }
  return flag;
}
/*
 * 函数功能:抽出一个字符串两个特定标志位中间的数据
 * 函数输入:起始标志位，终止标志位,当前数据帧
 * 函数返回:目标字符串
 */
String explain_str(char flag_start, char flag_end, String input_str)
{
  int index_start = 0;
  int index_end = 0;
  String final_data = "";
  char str_array[20] = {0};
  int len = input_str.length() + 1;
  input_str.toCharArray(str_array,len);
  index_start = input_str.indexOf(flag_start);
  index_end = input_str.indexOf(flag_end);
  //Serial1.print(str_array[index_start]);
  //Serial1.print(',');
  //Serial1.println(str_array[index_end]);
  for ( int i = index_start+1; i < index_end; i++ ){
    final_data += str_array[i];
  }
  return final_data;
}
