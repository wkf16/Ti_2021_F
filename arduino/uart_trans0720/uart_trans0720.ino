#include <Servo.h>
#include <SoftwareSerial.h>
#define servoPinBottom 5
#define servoPinTop 6
#define rxPin 7
#define txPin 8
#define data_long 20
Servo servoBottom;
Servo servoTop;

char text[4];
int count1=0;
int count2=0;
int count1_really=0;
char div_num[20];
//传回数据：字符串
//String data = "#15$12^*"//#x$y(中心点坐标差)
char data[data_long] = {0};

SoftwareSerial softUart(rxPin,txPin);
void setup()
{
  servoInitialize();
  Serial.begin(115200);
  softUart.begin(9600);
}
void loop()
{
  recFun();
  Serial.print("str1");
  delay(100);
  //servoDrive(diff_x, diff_y);
  div_char('$','&');
  //Serial.println(div_num);
  String div_num_str;
  
  div_num_str = div_num;
  
  Serial.print("str");
  delay(5);
  //Serial.println(div_num_str);
  int num = div_num_str.toInt();
  int num2 = num+1;
  Serial.println(num);
  Serial.println(num2);
  //delay(100);
}

void recFun()
{
  //Serial.println("entered");
  
  if ( softUart.available() && (char)softUart.read() == '#' )
  {
    //Serial.println("entered2");
    data[0] = '#';
    int i = 1;
    char rec = 0;
    while(rec != '^')
    {
      rec = (char)softUart.read();
      data[i] = rec;
      i++;
    }
  //Serial.println(data);
  }
  //date=data_

}
void div_char(char stand_char1,char stand_char2){
  count1=0;
  count2=0;
  count1_really=0;
  int i=0;
  while( stand_char1 !=data[count1])//找到标识符1所在位置
  {
    count1++;
  }
  while( stand_char2 !=data[count2])//找到标识符二所在位置
  {
    count2++;
  }
  count1_really=count1;  while(i!=count2-count1_really-1){
    div_num[i]=data[count1+1];
    i++;
    count1++;
  }
}
