#include<String.h>
String K210_rec = "^564651231213";
char target_num = '2';

char contrl_flag;  //L,R,G三种情况
float x_diff = 0;//巡线偏差值
int stop_flag = 0;//1停止
int final_flag = 0;//1终点

void setup() {
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  get_target();
  //rec_explain();
  Serial.print(target_num);
  Serial.print(',');
  Serial.print(x_diff);
  Serial.print(',');
  Serial.print(stop_flag);
  Serial.print(',');
  Serial.println(final_flag);
}

void get_target(){
  //—————————获得目标数字—————————————————
  char str[20] = "";
  K210_rec.toCharArray(str, 6);
  if(str[0] == '^'){
    target_num = str[1];
    }
}

void rec_explain(){
 //-------------------解释器--------------------------------
  char str[20] = "";
  K210_rec.toCharArray(str, 6);
  char x_diff_char[5];
  int i = 1;
//——————————————————————————————————  
  contrl_flag = 'G';
  int num_flag = 0;
   while(str[i] != '$'){//遍历数字部分输出contrl_flag
     if(str[i] == target_num){contrl_flag = str[i+1];num_flag = 1;}
     else if(num_flag == 0) contrl_flag = 'G';
     i++;
     }
//——————————————————————————————————    
  int diff_i = 0;
  i++;
  while(str[i] != '&'){//遍历x_diff部分输出x_diff
    x_diff_char[diff_i] = str[i];
    diff_i++;
    i++;
    }
  String x_diff_str;
  x_diff_str = x_diff_char;
  x_diff = x_diff_str.toFloat();//转浮点数
//——————————————————————————————————
  i++; 
  while(str[i]!='@'){//遍历stop_flag部分输出stop_flag
    stop_flag = str[i]-'0';
    i++;
    }    
//——————————————————————————————————
//遍历final_flag部分输出final_flag
    i++;
    final_flag = str[i]-'0';  
}
