# generated by maixhub, tested on maixpy3 v0.4.8
# copy files to TF card and plug into board and power on
import sensor, image, lcd, time
import KPU as kpu
import gc, sys
from machine import I2C

pink = [255,182,193]
input_size = (224,224)
labels = ['1', '2', '3', '4', '5', '6', '7', '8']
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing(input_size)
sensor.set_hmirror(0)
sensor.set_vflip(0)
#sensor.rotation(90)
#sensor.set_transpose(1)
sensor.skip_frames()
#lcd.init(type=1,freq=15000000,color=lcd.BLACK)
def yolo_setup():

    anchors = [1.44, 1.91, 1.28, 1.72, 1.28, 1.91, 1.44, 2.22, 2.03, 2.62]
    task = None
    task = kpu.load("/sd/m.kmodel")
    kpu.init_yolo2(task, 0.5, 0.3, 5, anchors) # threshold:[0,1], nms_value: [0, 1]
    return task
#---------yolo识别函数--------------------
def yolo_run(labels,signal,signal_start):

    if signal == 1:#判断信号，当传入信号为1，运行
        task = yolo_setup()
        count = 0
        result = ""
        count_frame = 0
        poss_result = []
        flag = 0
        if signal_start == 0:
            print("进入判断")
            while(flag <= 10 or flag == 0):
                img = sensor.snapshot()
                objects = kpu.run_yolo2(task, img)
                if objects:
                    flag = 23
                    print("识别")

                else:
                    flag = flag + 1
                    time.sleep_ms(50)
                    #print("此时应该弹出系统")
        if flag == 23:
            flag = 0
        print(flag)
        while(count_frame <= 10 and flag == 0):
            print("进入循环")
            img = sensor.snapshot()
            #lcd.display(img)
            #img.rotation_corr(z_rotation = 90)
            #t = time.ticks_ms()
            objects = kpu.run_yolo2(task, img)
            #t = time.ticks_ms() - t
            if objects:
                for obj in objects:
                    temp = obj.value()
                    count = count + 1
                    print(count)

                if count < 3:

                    for obj in objects:

                        pos = obj.rect()
                        img.draw_rectangle(pos)
                        img.draw_string(pos[0], pos[1], "%s : %.2f" %(labels[obj.classid()], obj.value()), scale=1, color=pink)
                        direction = 0
                        if obj.value() > 0.8:
                            if obj.x() - 112 < 0:
                                direction = 'L'
                            else:
                                direction = 'R'
                            result = ""

                            result = result + str(labels[obj.classid()])
                            result = result + direction
                            poss_result.append(result)


                            count_frame = count_frame + 1#运行十次识别

                count = 0
        #poss_result = ['a']
        if flag == 11:
            yolo_result = [0,"0000"]

            print('释放内存')
            kpu.deinit(task)
            del task
            gc.collect() #释放内存，注销k210的模型
            return yolo_result
        else:
            list_R = []
            list_L = []
            for j in poss_result:
                if j[1] == 'R':
                    list_R.append(j[0])
                else:
                    list_L.append(j[0])

            if list_R and list_L:
                L = max(list_L,key=list_L.count)
                R = max(list_R,key=list_R.count)
                if L == R:
                    final_result = L
                else:
                    final_result = L + 'L' + R + 'R'
            else:
                if list_R:
                    R = max(list_R,key=list_R.count)
                    final_result = R
                else:
                    L = max(list_L,key=list_L.count)
                    final_result = L
        #如果只识别到一个数字，仅返回数字，不返回左右关系(读取目标病房使用)
        #如果识别到两个数字，返回数字及对应左右关系
        #三个以上数字不识别
        #print(final_result)
            print(final_result)
            yolo_result = []
            yolo_result.append(0)
            yolo_result.append(final_result)
            kpu.deinit(task)
            del task
            gc.collect() #释放内存，注销k210的模型
            return yolo_result #返回运行结果
        #poss.append(result)
        #signal2 = 0
        #return signal2
    else:
        yolo_result = [0,"0000"]
        return yolo_result

#--------------巡线函数---------------

def line_follow(signal):
    result = []
    x_diff = 0
    final_stop = 0
    if signal == 0:
        #色块阈值
        red_line = (0, 60, 3, 68, 14, 44)
        black_line = (0, 25, -180, 180, -180, 180)
        #black_line = (0, 18, -13, 11, -13, 13)
        #roi兴趣区域
        line_roi = [0,80,224,20]
        black_roi = [72,60,80,30]
        #停止阈值
        linestop_area = 3000
        blackstop_area = 550

        pink = [255,182,193]
        #gc.enable()
        img = sensor.snapshot()
        #lcd.display(img)
        flag_stop = 0;#停止标志
        t = time.ticks_ms()
        red_blobs = img.find_blobs([red_line],roi = line_roi,merge=True)#巡线区域
        black_blobs = img.find_blobs([black_line],roi = black_roi,merge=True)#终点识别
        img.draw_line(int(224/2), 0, int(224/2), 224,color=pink,thickness=2)
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
                    #print(blob.area())
                x_diff = 160-blob.cy()

    #print( black_area )
            black_area_all = 0
            for blob in black_blobs:#获得黑色面积
                    img.draw_rectangle(blob.rect(),color=(0,255,0),thickness=2)
                    black_area_all = black_area_all + blob.area()
            if(black_area_all >= blackstop_area):#如果色块大于值并且无数字
                    final_stop = 1#停止标志

        if flag_stop == 1:
            signal = 1
        else:
            signal = 0
        result.append(signal)
        result.append(x_diff)
        result.append(final_stop)
        return result
    else:
        result = [0,0,0]
        return result

#yolo_num(img)
#--------------------i2c发送函数---------------------------
def send(data):
    i2c = I2C(I2C.I2C0, freq=100000, scl=7, sda=8)
    time.sleep_ms(50)
    i2c.writeto(0x07,data)
    i2c.deinit()
    print("sent!")



if __name__ == "__main__":

    #信号为1，运行yolo。信号为0，运行巡线
    while(1):
        signal = 1#初始信号为1
        gc.enable()
        signal_start = 1
        a = "initialized"
        send(a)
        yolo_data = yolo_run(labels,signal,signal_start)
        signal_start = yolo_data[0]
        target = '^'+yolo_data[1]
        if signal_start == 0:
            send(target)

            signal = 0
        # main(anchors = anchors, labels=labels, model_addr=0x300000, lcd_rotation=0)
        while(signal_start == 0):

            line_data = line_follow(signal)
            line_result = line_data[1]
            #巡线结果，数据类型:列表
            #列表第0位为信号，第1位为识别到的数据(x_diff),2位为终点
            signal = line_data[0]
            print("信号码：")
            print(signal)
            yolo_data = yolo_run(labels,signal,signal_start)
            #yolo识别得到的数据。数据类型:列表
            #列表第0位为信号，第1位为识别到的数据(字符串)
            yolo_result = yolo_data[1]
            #signal = yolo_data[0]
            print(str(yolo_data))

            #str_send = #数字$巡线&停止@终点
            str_send = '#'+yolo_data[1]+'$'+str(line_data[1])+'&'+str(signal)+'@'+str(line_data[2])
            #i2c.writeto(0x07,str_send)
            #i2c_send(str_send)
            send(str_send)
            str_send
            #print(str_send)
