#undef LED
#define LED BLUE_LED
void setupinterper(){
  Serial.begin(9600);
  Serial1.begin(115200);
}
void loopinterper(){ 

recFun(data_);

if (count_sum==0){
  target_num=data_[2];
  count_sum++;
  }
else{
  div_char('#','$',count2,count1_really, div_num,data_);
  assigment(div_digital_recognition,count2,count1_really,div_num);
  
  div_char('$','&',count2,count1_really, div_num,data_);
  assigment(div_x,count2,count1_really,div_num);
  
  
  div_char('@','*',count2,count1_really, div_num,data_);
  assigment(div_cross,count2,count1_really,div_num);


  div_char('*','^',count2,count1_really, div_num,data_);
  assigment(div_end,count2,count1_really,div_num);
 
  int i=0;
  contrl_flag='G';
  while(div_digital_recognition[i]!=0){
    if(div_digital_recognition[i]==target_num)
      {contrl_flag=div_digital_recognition[i+1];
      }
    i++;
    }
    count_now= count_rec;
    count_rec++;
    if(count_rec==10){
      count_rec=0;//接受计数保证能使cross能接收到下次的信息。

    
  i=0;
  stop_flag=0;
  while(div_cross[i]!=0){
    if(div_cross[i]=='1'){
    stop_flag=1;
    }
    i++;
  }


  i=0;
  final_flag=0;
  while(div_end[i]!=0){
    if(div_end[i]!='G'){
      final_flag=1;
    }
    i++;
  }
  String div_x_str;
  div_x_str = div_x;
  int num_x = div_x_str.toInt();
  x_diff=num_x;
    }
  }
  Serial1.write(div_digital_recognition);
  Serial1.write("                     ");
  Serial1.write(div_x);
  Serial1.write("                     ");
  Serial1.write(div_cross);
  Serial1.write("                     ");
  Serial1.write(div_end);                 
}
