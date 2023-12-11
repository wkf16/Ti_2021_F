void setup_interper(){
  Serial.begin(115200);
}
void loop_interper(){ 
 recFun(data[data_long]);
 target_num=data[2];
  count_sum++;
if(count_sum!=1){
  recFun(data[data_long]);
  div_char('$','&',count2,count1_really);
  assigment(div_digital_recognition,count2,count1_really);
  if(count_div_digital_recognition==10){
     count_div_digital_recognition=0;
    }
  count_div_digital_recognition++;
  div_char('&','^',count2,count1_really);
  assigment(div_x,count2,count1_really);
  if(count_div_x==10){
     count_div_x=0;
    }
    count_div_x++;
  div_char('^','#',count2,count1_really);
  assigment(div_cross,count2,count1_really);
  if(count_div_cross==10){
     count_div_cross=0;
    }
    count_div_cross++;
  div_char('^','#',count2,count1_really);
  assigment(div_end,count2,count1_really);
  if(count_div_end==10){
     count_div_end=0;
    }
    count_div_end++;

  int i=0;
  contrl_flag='G';
  while(div_digital_recognition[i]!=0){
    if(div_digital_recognition[i]==target_num)
      {contrl_flag=div_digital_recognition[i+1];
      }
    i++;
    }

    
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
  

   if(final_flag==1){
      final_flag_real=final_flag;
      }

  if(final_flag_real==0){
    if(stop_flag ==1){
        mySemaphore1=1;
        count_now=count_div_digital_recognition;
        while(mySemaphore1==0){;}              
        }
    }
  if(final_flag ==1){  
     turn_car_end(mor1,mor2);
     }
  if(final_flag_real==1){
     if(stop_flag ==1){
      mySemaphore2=2;
      while(mySemaphore2==0){;}
      }
     }  
}                     
}
