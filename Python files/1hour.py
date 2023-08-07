import glob
import time
from datetime import datetime
from time import gmtime, localtime, strftime
import sqlite3

base_dir = '/sys/bus/w1/devices/'
sn1 = '28-012032e3d6b5'
device_display = glob.glob(base_dir + sn1)[0] + '/w1_slave'

def read_temp_raw():
    f = open(device_display, 'r')
    lines = f.readlines()
    f.close()
    return lines
 
def read_temp():
    lines = read_temp_raw()
    while lines[0].strip()[-3:] != 'YES':
        time.sleep(0.2)
        lines = read_temp_raw()
    equals_pos = lines[1].find('t=')
    if equals_pos != -1:
        temp_string = lines[1][equals_pos+2:]
        temp_c = float(temp_string) / 1000.0
        return temp_c

conn = sqlite3.connect('Display_temp.db')
c = conn.cursor()
c.execute("""CREATE TABLE IF NOT EXISTS aquarium (
    date text,
    time text,
    display real
    )""")
conn.commit()
conn.close()

wk = strftime("%W", localtime())
dt = strftime("%d %B %Y", localtime())
hr = strftime("%H:%M:%S", localtime())
dis = read_temp()

conn = sqlite3.connect('Display_temp.db')
c = conn.cursor()

c.execute("INSERT INTO aquarium (date, time, display) VALUES (?, ?, ?)",
          (dt, hr, dis))

conn.commit()
conn.close()
#print ("1 hour  temp sucessfull")
#print (read_temp())