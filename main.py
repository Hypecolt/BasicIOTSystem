#!/usr/bin/python3

import cgi
import cgitb
import paho.mqtt.publish as mqttpublish
cgitb.enable()

mqtt_broker_ip = #the IP of the device that hosts the MQTT server
mqtt_topic = "main"
mqtt_username = "main1"
mqtt_password = "main1"
mqtt_clientid = "web"
mqtt_port = 1833
mqtt_auth = {'username' : mqtt_username, 'password' : mqtt_password}

print ('Content-type: text/html\n\n')
print ('<h1>IoT by Vlad</h1>')
print ("""
        <style>
            .da{width: 200px;
                   height: 100px;}
        </style>
      """)

print ("""
        </br><h3>Lumina Camera</h3></br>
        <a href="?topic=ESP1&msg=on_LGHT" type=\"button\"><button class=\"da\">Light On</button></a>&ensp;
        <a href="?topic=ESP1&msg=off_LGHT" type=\"button\"><button class=\"da\">Light Off</button></a></br></br>
        
        </br</br><h3>Aer Conditionat</h3></br>
        <a href="?topic=ESP1&msg=on_AC" type=\"button\"><button class=\"da\">Air Conditioner On</button></a>
        <a href="?topic=ESP1&msg=off_AC" type=\"button\"><button class=\"da\">Air Conditioner Off</button></a></br></br>
      """)

def send(topic, msg):
	mqttpublish.single(topic, msg, hostname=mqtt_broker_ip, auth=mqtt_auth)

arguments = cgi.parse()

try:
    topic = arguments['topic'][0]
    msg = arguments['msg'][0]
    send(topic,msg)
except:
    pass