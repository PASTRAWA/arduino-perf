import psutil
import serial
import time
import GPUtil
ser = serial.Serial('/dev/ttyUSB0', 9600)
def sender():
    while True:
        gpus=GPUtil.getGPUs()
        for gpu in gpus:
            #print(gpu.id,gpu.name)
            gpuLoad=int(gpu.load*100)
            #print(f"Boş Bellek: {gpu.memoryFree}MB")
            #print(f"Kullanılan Bellek: {gpu.memoryUsed}MB")
            #print(f"Toplam Bellek: {gpu.memoryTotal}MB")
            #print(f"Sıcaklık: {gpu.temperature} °C")
            #print("-" * 30)
        cpu = int(psutil.cpu_percent())
        ram = int(psutil.virtual_memory().percent)
        msg = f"{cpu},{ram},{int(gpuLoad)}\n"
        ser.write(msg.encode())
        time.sleep(1)

def gpuKullanimi(gpu):
    for gpu in gpus:
        print(gpu.id,gpu.name)
        print(f"GPU Yükü: {gpu.load*100}%")
        print(f"Boş Bellek: {gpu.memoryFree}MB")
        print(f"Kullanılan Bellek: {gpu.memoryUsed}MB")
        print(f"Toplam Bellek: {gpu.memoryTotal}MB")
        print(f"Sıcaklık: {gpu.temperature} °C")
        print("-" * 30)


sender()

