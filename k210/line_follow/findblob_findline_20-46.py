from machine import I2C
import sensor, image, time, lcd
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_saturation(+2)
sensor.set_contrast(+2)
sensor.set_auto_gain(0,1000)
sensor.skip_frames(time = 200)
lcd.init()
#色块阈值
red_line = (0, 60, 3, 68, 14, 44)
black_line = (0, 18, -13, 11, -13, 13)
#roi兴趣区域
line_roi = [70,0,60,240]
black_roi = [60,80,30,80]
#停止阈值
linestop_area = 10000
blackstop_area = 1000

pink = [255,182,193]
#i2c = I2C(I2C.I2C0,scl=28,sda=29)
while(True):
    flag_stop = 0;#停止标志
    img = sensor.snapshot()
    red_blobs = img.find_blobs([red_line],roi = line_roi,merge=True)#巡线区域
    black_blobs = img.find_blobs([black_line],roi = black_roi,merge=True)#终点识别
    img.draw_rectangle(line_roi,color=pink,thickness=2)
    img.draw_rectangle(black_roi,color=(0,255,0),thickness=2)
    maxblob_area=0
    for blob in red_blobs:#获得最大色块
      maxblob_area=max(maxblob_area,blob.area())
    for blob in red_blobs:#输出最大色块 输出差值 判断是否停止
     if(blob.area()>=maxblob_area):
      if(blob.area()>=linestop_area):#当识别到入口时
       flag_stop = 1;#输出停止
      img.draw_rectangle(blob.rect(),color=pink,thickness=2)
      img.draw_cross(blob.cx(), blob.cy(),color=pink,thickness=2)
      img.draw_line(blob.cx(), blob.cy(), blob.cx(), 120,color=pink,thickness=2)
      img.draw_line(0, 120, 320, 120,color=pink,thickness=2)
      print(blob.area())
      x_diff = 120-blob.cx()
    black_area_all = 0
    #print( black_area )
    for blob in black_blobs:#获得黑色面积
      img.draw_rectangle(blob.rect(),color=(0,255,0),thickness=2)
      black_area_all = black_area_all + blob.area()
    if(black_area_all >= blackstop_area):#如果色块大于值并且无数字
      flag_stop = 1 #停止标志


