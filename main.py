import psutil
import serial
import time
import GPUtil
ser = serial.Serial('/dev/ttyUSB0', 9600) #Instead of ‘/dev/ttyUSB0’, enter the location where you connected your Arduino. 
def sender():
    while True:
        gpus=GPUtil.getGPUs()
        for gpu in gpus:
            gpuLoad=int(gpu.load*100)
        cpu = int(psutil.cpu_percent())
        ram = int(psutil.virtual_memory().percent)
        msg = f"{cpu},{ram},{int(gpuLoad)}\n"
        ser.write(msg.encode())
        time.sleep(1)
sender()

