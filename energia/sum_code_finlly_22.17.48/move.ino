//回程函数 收到信息后考储存在数组里的信息进行返回
#undef LED
#define LED GREEN_LED
void setupturn_rote(){
  pinMode(LED, OUTPUT);
  
}
void loopturn_rote(){



//——————巡线部分————————

  while( count_sum==0){;}
  if((mySemaphore2+mySemaphore1)>0){
    findline(mor1,mor2,x_diff);
  }


//————————转弯部分————————————
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








//————————————返回部分————————————————



  
  if(mySemaphore2=1){
    brake(mor1,mor2);
    turn_back(mor1,mor2,rote,count);                                    
      mySemaphore2=0; 
      }
}
