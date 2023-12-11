int cross_bool=0;
cross_setup(){
}
cross_loop(){

 mySemaphore1.waitFor();
   cross_bool=1;
brake(mor1,mor2);
while(Wire.available()>0)
if(contrl_flag=='L')
     {rote[count]='L';
      turn_car(mor1,mor2,L_bit);
      count++;
     }
     if(contrl_flag=='L')
     {rote[count]='L';
      turn_car(mor1,mor2,R_bit);
      count++;
     }
      if(contrl_flag=='G')
      { rote[count]='L';
         count++;
       }
         cross_bool=0;
                                        
}
