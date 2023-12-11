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
