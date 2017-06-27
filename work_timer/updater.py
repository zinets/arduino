from datetime import datetime, date, time, tzinfo
import serial

def unix_time_millis(dt):
    epoch = datetime.utcfromtimestamp(0)
    return int((dt - epoch).total_seconds()) + 60 * 60

# print(unix_time_millis(datetime.now()))

ser = serial.Serial("/dev/cu.wchusbserial1a1220", 115200)
print(ser.name)

# ser.write('t')
ut = unix_time_millis(datetime.now())

toSend = "t" + str(ut) + "\r"
print("sending " + toSend)
ser.write(toSend.encode())

print ("waiting for response..")
time.sleep(3)
response = ""
while (ser.inWaiting() > 0):
	response += ser.read()

print response

ser.close()

print "exit"
