#undef LED
#define LED RED_LED
void do_setup(){
  pinMode(LED, OUTPUT);
}
void do_loop(){

   if(final_flag==1){
      final_flag_real=final_flag;
      }

  if(final_flag_real==0){
    if(stop_flag ==1){
        mySemaphore1=1;
        while(mySemaphore1==0){;}              
        }
    }
  if(final_flag ==1){  
     turn_car_end(mor1,mor2);
     }
  if(final_flag_real==1){
     if(stop_flag ==1){
      mySemaphore2=1;
      while(mySemaphore2==0){;}
      }
     }
}
