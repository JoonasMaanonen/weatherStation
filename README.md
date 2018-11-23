# weatherStation 
This repository has instructions on how to create a basic home automation system. The system can measure the temperature and humidity of any place you want.  

## Why I did this project
I attended a course(Prediction and Time Series Analysis) in Aalto University, where a very inspiring lecturer (Pauliina Ilmonen) mentioned temperature as an example about a time series. Therefore,  I figured I'd make a system where I could get some real time series data and then analyze it using the techniques learned on the course.

I happened to have an old Linux laptop and Arduino Yun lying around, So I hacked around for a weekend and came up with this simple system.

## Hardware
- Arduino Yun
- DHT 22  (AM2302), TempHum Pro 
- Linux Server

## Instructions 
1. Configure Arduino Yun WiFi according to these instructions:
    - https://www.arduino.cc/en/Guide/ArduinoYun
2. Git clone this repo to your Linux server.
3. Move the cloned code from [arduino folder](arduino) to the arduino Yun using some method e.g scp. 
4. Ssh in to the Arduino Yun.
5. Move this [python script](arduino/sendMeasurements.py) to [Arduino's folder](/usr/lib/python2.7/bridge).
6. Create a config.json file in the same directory(/usr/lib/python2.7/bridge) that contains the following fields:
'''json
    {
        "sensor_id" : "Some ID for your sensor measurements e.g location etc."
        "ip" : "YOUR LINUX SERVER IP ADDRESS"
        "port": "YOUR LINUX SERVER PORT"
    }
'''
7. Make sure you have [the server code](udpServer) in somewhere in your Linux server where you can run it safely.
8. Compile the server code and create measurements.csv file for the measurements in the [server directory](udpServer/server)
9. Run the UDP server by  '''./server hostname port'''
10. Check if the daemon started correctly by checking the syslog file (/usr/log/syslog). You should find log print [WeatherStation]: daemon started.
11. Now we have our UDP server running, where we store our measurements.
12. Upload the [arduino code](arduino/WeatherStation/WeatherStation.ino) to the Arduino Yun.
13. You need to put the DHT sensor to the correct PIN that is defined in the [code](arduino/WeatherStation/WeeatherStation.ino)

## Improvements 
- Database service where to store the measurements data.
- Website to showcase the data online.
- Rest API to query the measurements 
