int turn_rote_bool =0;
void turn_rote_setup(){  
}
void turn_rote_loop(){
  mySemaphore2.waitFor();
  turn_rote_bool =1;
  brake(mor1,mor2);
  while(Wire.available())
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
       turn_rote_bool =0                       
                    }
