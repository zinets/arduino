from datetime import datetime, date, time, tzinfo
import serial

def unix_time_millis(dt):
    epoch = datetime.utcfromtimestamp(0)
    return int((dt - epoch).total_seconds()) #+ 60 * 60

# print(unix_time_millis(datetime.now()))

ser = serial.Serial("/dev/cu.wchusbserial1a1220", 115200)
print(ser.name)

# ser.write('t')
ut = unix_time_millis(datetime.now())

toSend = 't' + str(ut)
print(toSend)
ser.write(toSend)

if (ser.inWaiting() > 0):
    myData = ser.readline()
    print myData

ser.close()

print "exit"
