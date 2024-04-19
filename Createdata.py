import sqlite3
import paho.mqtt.client as mqtt


mqtt_broker = 'broker.emqx.io'
mqtt_port = 1883
mqtt_topic = 'esp32/barcode'
mqtt_topictype='esp32/type'
mqtt_user = 'emqx'
mqtt_password = 'public'

database = 'mqttrecycle.db'
table = 'barcodedata'


conn = sqlite3.connect(database)
cursor = conn.cursor()


def check_barcode(barcode):
    barcode = barcode.strip()
    if (barcode=='0'):
        print("scdv")
    print(barcode)
    cursor.execute('SELECT type FROM {} WHERE barcode = ?'.format(table), (barcode,))
    result = cursor.fetchone()
    
    if result is not None:
        type_value = result[0]
        mqtt_client.publish(mqtt_topictype, str(type_value))
    else:
        mqtt_client.publish(mqtt_topictype, '2')


def on_connect(client, userdata, flags, rc):
    print('connect to code' + str(rc))
    client.subscribe(mqtt_topic)


def on_message(client, userdata, msg):
    barcode = msg.payload.decode('utf-8')
    print('recieved barcode: ' + barcode)
    check_barcode(barcode)


mqtt_client = mqtt.Client()


mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message

mqtt_client.connect(mqtt_broker, mqtt_port, 60)


mqtt_client.loop_forever()