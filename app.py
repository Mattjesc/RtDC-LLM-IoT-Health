from flask import Flask, render_template, request, jsonify
import paho.mqtt.client as mqtt
from datetime import datetime, timedelta
import openai
from collections import deque

app = Flask(__name__)

# Initialize the OpenAI client with your API key
openai.api_key = 'YOUR_API_KEY_HERE'

# MQTT setup
mqtt_broker = "test.mosquitto.org"
mqtt_port = 1883
client = mqtt.Client()
client.connect(mqtt_broker, mqtt_port, 60)

# Temporary storage for BPM and temperature
temp_values = {'bpm': None, 'temp': None, 'last_update': datetime.now()}
saved_record = None  # This will hold the last updated record

logs = []

# Historical data storage
max_data_points = 100
bpm_history = deque(maxlen=max_data_points)
temp_history = deque(maxlen=max_data_points)

def on_connect(client, userdata, flags, rc):
    log_entry("MQTT Connection Established!")
    client.subscribe("health/bpm")
    client.subscribe("health/temperature")

def on_message(client, userdata, msg):
    global temp_values, saved_record
    message = msg.payload.decode()
    topic = msg.topic
    now = datetime.now()
    if topic == "health/bpm":
        temp_values['bpm'] = message
        temp_values['last_update'] = now
        bpm_history.append({'time': now.strftime("%H:%M:%S"), 'value': float(message)})
    elif topic == "health/temperature":
        temp_values['temp'] = f"{message}°C"
        temp_values['last_update'] = now
        temp_history.append({'time': now.strftime("%H:%M:%S"), 'value': float(message)})
    
    if temp_values['bpm'] and temp_values['temp'] and (now - temp_values['last_update'] <= timedelta(seconds=5)):
        combined_message = f"BPM: {temp_values['bpm']}, Temperature: {temp_values['temp']}"
        log_entry(combined_message)
        saved_record = combined_message
        temp_values['bpm'] = None
        temp_values['temp'] = None

def log_entry(message):
    time_stamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]
    log_message = f"{time_stamp} -> {message}"
    logs.append(log_message)
    if len(logs) > 50:
        logs.pop(0)

client.on_connect = on_connect
client.on_message = on_message
client.loop_start()

@app.route('/')
def index():
    return render_template('index.html', logs=logs)

@app.route('/logs')
def get_logs():
    return jsonify(logs)

@app.route('/send_to_llm', methods=['POST'])
def send_to_llm():
    global saved_record
    if saved_record is None:
        return jsonify({"status": "error", "response": "No health data available to analyze."})
    try:
        chat = openai.ChatCompletion.create(
            model="gpt-4o-mini",
            messages=[
                {"role": "system", "content": "Provide a plain text interpretation of the health data including BPM and environmental temperature, explaining potential conditions and effects without Markdown formatting."},
                {"role": "user", "content": f"Analyze this data: {saved_record}"}
            ],
        )
        advice = chat.choices[0].message['content']
        return jsonify({"status": "success", "response": advice})
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/bpm_data')
def get_bpm_data():
    return jsonify(list(bpm_history))

@app.route('/temp_data')
def get_temp_data():
    return jsonify(list(temp_history))

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)