//在没有收到信号时执行巡线代码

void setufind_line(){

}
void loopfind_line_(){
  if((mySemaphore2+mySemaphore1)>0){
    findline(mor1,mor2,x_diff);
  }
}
