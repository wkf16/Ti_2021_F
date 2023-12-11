#include <SoftwareSerial.h>
#define uart_831 5
#define uart_831 6 
#define rxPin 7
#define txPin 8
#define data_long 10
char data[data_long] = "";
SoftwareSerial softUart(rxPin,txPin);
void setup() {
Serial.begin(115200);
softUart.begin(9600);
}

void loop() {


}
void rec(){
  int i=0;
  while((softUart.available())&&i<data_long)
    {
      data[i]=(char)softUart.read();
      i++;
    }
}
char div_char(char stand_char1,char stand_char2){
  int count1=0;
  int count2=0;
  int count1_really=0;
  int i=0;
  while(stand_char1!=data[data_long])//找到标识符1所在位置
  {
    count1++;
  }
  while(stand_char1!=data[data_long])//找到标识符二所在位置
  {
    count2++;
  }
  count1_really=count1;
  char div_num[count2-count1-1];//两标识符中间字符
  while(i!=count2-count1_really-1){
    div_num[i]=data[count1+1];
    i++;
    count1++;
  }
  return  div_num;
}
