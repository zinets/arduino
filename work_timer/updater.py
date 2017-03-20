import serial
connected = False

ser = serial.Serial("/dev/cu.wchusbserial1d1320", 115200)
print(ser.name)

ser.write('t1490017611')

if (ser.inWaiting() > 0):
    myData = ser.readline()
    print myData

ser.close()

print "exit"
