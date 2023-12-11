//收到十字路口信号后停下 等待下次收到数字识别发来的信息进行相应转向
void setupcross_setup(){
   
}
void loop_cross(){
if(mySemaphore1==1){
  brake(mor1,mor2);
  while(count_now==count_rec){;}//等待至下一次信号发送
  if(contrl_flag=='L')
     {rote[count]='L';
      turn_car(mor1,mor2,L_bit);
      count++;
     }
     if(contrl_flag=='R')
     {rote[count]='R';
      turn_car(mor1,mor2,R_bit);
      count++;
     }
      if(contrl_flag=='G')
      { rote[count]='G';
         count++;
       }
  mySemaphore1=0;
  
  }              
}
