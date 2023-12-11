from machine import I2C
import sensor, image, time, lcd , gc
import KPU as kpu
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_saturation(+2)
sensor.set_windowing((224,224))
sensor.set_hmirror(0)
sensor.set_vflip(0)
sensor.set_contrast(+2)
sensor.set_auto_gain(0,1000)
sensor.skip_frames(time = 200)
lcd.init()

#色块阈值
red_line = (0, 60, 3, 68, 14, 44)
black_line = (0, 18, -13, 11, -13, 13)
#roi兴趣区域
line_roi = [0,30,320,50]
black_roi = [110,10,100,40]
#停止阈值
linestop_area = 10000
blackstop_area = 1000

pink = [255,182,193]
#i2c = I2C(I2C.I2C0,scl=28,sda=29)
#-----------yolo初始化----------------------
task = kpu.load("/sd/m.kmodel")

labels = ['1', '2', '3', '4', '5', '6', '7', '8']
anchors = [1.44, 1.91, 1.28, 1.72, 1.28, 1.91, 1.44, 2.22, 2.03, 2.62]
kpu.init_yolo2(task, 0.5, 0.3, 5, anchors) # threshold:[0,1], nms_value: [0, 1]
#-----------识别代码------------------------
while(True):
    img = sensor.snapshot()
    objects = kpu.run_yolo2(task, img)
    if objects:
        for obj in objects:
            pos = obj.rect()
            img.draw_rectangle(pos)
            img.draw_string(pos[0], pos[1], "%s : %.2f" %(labels[obj.classid()], obj.value()), scale=2, color=(255, 0, 0))
    #img.draw_string(0, 200, "t:%dms" %(t), scale=2, color=(255, 0, 0))
#-----------巡线代码------------------------
    #gc.enable()
    flag_stop = 0;#停止标志
    t = time.ticks_ms()
    red_blobs = img.find_blobs([red_line],roi = line_roi,merge=True)#巡线区域
    black_blobs = img.find_blobs([black_line],roi = black_roi,merge=True)#终点识别
    img.draw_line(160, 0, 160, 240,color=pink,thickness=2)
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
      img.draw_line(blob.cx(), blob.cy(), 160, blob.cy(),color=pink,thickness=2)

      print(blob.area())
      x_diff = 160-blob.cy()
    black_area_all = 0
    #print( black_area )
    for blob in black_blobs:#获得黑色面积
      img.draw_rectangle(blob.rect(),color=(0,255,0),thickness=2)
      black_area_all = black_area_all + blob.area()
    #if(black_area_all >= blackstop_area)#如果色块大于值并且无数字
     # flag_stop = 1;#停止标志
