import _thread
from time import sleep_ms
import network
import socket
from machine import Pin, I2C
from os import listdir, chdir
from MPU6050 import MPU6050



SSID = "Felipe's Hotspot"
PASSWORD  = "10201234"

# UDP Server (Receiver) IP & Port
UDP_IP = "192.168.137.1"  # Change to the receiver's IP (e.g., a PC)
UDP_PORT = 10044         # Choose any open port

global led_status
led_status = 0
led = Pin(2, Pin.OUT)  # GPIO2 is often the built-in LED
mpu = MPU6050()
wifi = network.WLAN(network.STA_IF)
wifi.active(True)
wifi.connect(SSID, PASSWORD)
print("Connecting to Wi-Fi...")
while not wifi.isconnected():
    led.value(1)  # Turn LED ON
    pass
print("Connected!", wifi.ifconfig())

# Create UDP Socket
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # Create UDP socket

def core_1():
    global led_status
    while True:
        if led_status:
            led.value(1)
            sleep_ms(100)
            led.value(0)
            sleep_ms(100)
        else:
            led.value(1)
            sleep_ms(1000)
            led.value(0)
            sleep_ms(1000)

_thread.start_new_thread(core_1,())
while True:
    try:
        accel = mpu.read_accel_data() # read the accelerometer [ms^-2]
        aX = accel["x"]
        aY = accel["y"]
        aZ = accel["z"]
        data = f"{aX:.2f}, {aY:.2f}, {aZ:.2f}"  # Replace with sensor data
        s.sendto(data.encode(), (UDP_IP, UDP_PORT))
        led_status = True
    except:
        print("Impossible to send data")  
        led_status = False
    pass



# ampy --port COM7 put main.py