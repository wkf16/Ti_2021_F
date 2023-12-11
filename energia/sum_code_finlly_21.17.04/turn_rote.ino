//回程函数 收到信息后考储存在数组里的信息进行返回
void setupturn_rote(){
  
}
void loopturn_rote(){
  if(mySemaphore2=1){
    brake(mor1,mor2);
    if(rote[count]=='R')
      {
        turn_car(mor1,mor2,L_bit);
        count--;
      }
    if(rote[count]=='L')
     {
      turn_car(mor1,mor2,R_bit);
      count--;
     }
     if(rote[count]=='G')
      { 
        count--;
      }                                      
      mySemaphore2=0; 
      }
}
