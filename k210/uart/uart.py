from fpioa_manager import fm
from machine import UART
import time



fm.register(7, fm.fpioa.UART2_TX, force=True)
fm.register(8, fm.fpioa.UART2_RX, force=True)
str_send = "#0000$6&1@"
uart_ard=UART(UART.UART2, 9600, 8, 0, 0, timeout=10000, read_buf_len=2*8)
while(1):
     uart_ard.write(str_send)
     print("sent!")
     '''read_data = uart_ard.read()
     while（read_data=read_data）'''
     time.sleep_ms(50)
