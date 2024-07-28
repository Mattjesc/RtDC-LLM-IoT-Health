```markdown
# RtDC-LLM-IoT-Health

## Real-time Data Capture for Integration with Large Language Models to Enhance User Experience in IoT-based Personal Healthcare: Monitoring using Pulse and Temperature Sensors

**Note: This is a Final Year Project for De Montfort University and Asia Pacific University of Technology and Innovation Malaysia**

### Overview

This project leverages real-time data capture from heart pulse and temperature sensors to integrate with Large Language Models (LLMs) to enhance the user experience in IoT-based personal healthcare. The system monitors environmental temperature and heart pulse, processes the data, and provides insightful feedback using LLMs.

### Features

- Real-time data capture from IoT sensors
- Integration with OpenAI's Large Language Models
- User-friendly interface using Soft UI Dashboard
- Continuous monitoring and logging of health data
- Insights and advice generation based on health data

### Requirements

#### Software Requirements

- Python 3.8+
- Flask and additional Flask-related libraries
- Paho-MQTT
- OpenAI API
- Bootstrap 5.1.3 (for spinner)
- Arduino IDE for ESP32 setup

#### Hardware Requirements

- ESP32 Development Board
- Breadboard (for quick testing and non-soldered setups)
- LM35 Temperature Sensor
- Pulse Sensor Heart Rate (generic or labelled as XD-58C)
- Connecting wires

### Project Structure

```
RtDC-LLM-IoT-Health/
│
├── static/
│   ├── assets/
│   │   ├── css/
│   │   │   ├── soft-ui-dashboard.min.css
│   │   │   ├── nucleo-icons.css
│   │   │   ├── nucleo-svg.css
│   │   │   └── style.css
│   │   ├── js/
│   │   │   ├── core/
│   │   │   │   ├── popper.min.js
│   │   │   │   └── bootstrap.min.js
│   │   │   ├── plugins/
│   │   │   │   ├── perfect-scrollbar.min.js
│   │   │   │   └── smooth-scrollbar.min.js
│   │   │   └── soft-ui-dashboard.min.js
│   ├── main.js
│
├── templates/
│   ├── base.html
│   ├── index.html
│
├── main/
│   └── main.ino
│
├── app.py
└── requirements.txt
```

### Setup Instructions

#### Python and Flask Setup

1. **Clone the repository:**

   ```bash
   git clone https://github.com/yourusername/RtDC-LLM-IoT-Health.git
   cd RtDC-LLM-IoT-Health
   ```

2. **Create and activate a virtual environment:**

   ```bash
   python -m venv venv
   source venv/bin/activate   # On Windows: venv\Scripts\activate
   ```

3. **Install dependencies:**

   ```bash
   pip install -r requirements.txt
   ```

4. **Set up OpenAI API:**

   Replace `'YOUR_API_KEY_HERE'` in `app.py` with your actual OpenAI API key.

   ```python
   openai.api_key = 'YOUR_API_KEY_HERE'
   ```

   We are using GPT-4o mini in this case to save costs for testing, but you are free to use any LLM AI models, providers, or host locally. This FYP uses GPT-4o mini to prove the concept works.

5. **Run the application:**

   ```bash
   python app.py
   ```

6. **Access the dashboard:**

   Open your web browser and go to `http://127.0.0.1:5000/`.

#### Arduino Setup

1. **Install the ESP32 board in Arduino IDE:**

   - Open Arduino IDE and go to `File` > `Preferences`.
   - In the `Additional Boards Manager URLs` field, add the following URL:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Go to `Tools` > `Board` > `Boards Manager`.
   - Search for `ESP32` and install the `esp32` package by Espressif Systems.

2. **Select the correct board:**

   - Go to `Tools` > `Board` and select `ESP32 Dev Module`.

3. **Install required libraries in Arduino IDE:**

   - **PubSubClient**: For MQTT communication.
   - **PulseSensor Playground**: For handling the pulse sensor.
   - **WiFi.h Library**: For Wi-Fi connectivity.

   You can install these libraries via the Arduino Library Manager.

4. **Open the `main.ino` file in Arduino IDE:**

   ```bash
   cd RtDC-LLM-IoT-Health/main
   open main.ino
   ```

5. **Upload the code to your ESP32:**

   Ensure your ESP32 is connected to your computer, select the correct board and port, then click on the upload button in the Arduino IDE.

### Dependencies

- Flask
- Paho-MQTT
- OpenAI
- Other Flask-related libraries

**`requirements.txt`:**

```
flask==2.2.5
Werkzeug==2.3.7
jinja2==3.1.2
flask-login==0.6.2
flask_migrate==4.0.4
WTForms==3.0.1
flask_wtf==1.2.1
flask-sqlalchemy==3.0.5
sqlalchemy==2.0.21
email_validator==2.0.0
python-dotenv==0.19.2
blinker==1.4
WTForms-Alchemy==0.18.0
flask-restx==1.0.3
gunicorn==20.1.0
Flask-Minify==0.37
Flask-Dance==7.0.0
PyJWT==2.4.0
Flask==2.2.5
paho-mqtt==1.6.1
openai==0.10.2
```

### Usage

- **Real-time Data Capture:** The system captures real-time data from heart pulse and temperature sensors using MQTT protocol.
- **Data Logging:** Logs the data and displays it on the dashboard.
- **LLM Integration:** Sends the captured data to OpenAI's LLMs to get insightful advice.
- **User Interface:** Uses Soft UI Dashboard for an intuitive and visually appealing interface.

### Soft UI Dashboard

The user interface is built using Soft UI Dashboard, which includes:

- `soft-ui-dashboard.min.css`: Core CSS file for Soft UI Dashboard.
- `nucleo-icons.css` and `nucleo-svg.css`: Icon libraries.
- `soft-ui-dashboard.min.js`: Core JavaScript file for Soft UI Dashboard.

For more details, visit the [Soft UI Dashboard repository](https://github.com/app-generator/flask-soft-ui-dashboard).

### Disclaimer

This project is limited by financial constraints, and the results may vary with higher quality sensors and equipment. In this implementation, we are using the GPT-4o mini model to save costs for testing purposes.

### License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

### Contributing

Contributions are welcome! Please open an issue or submit a pull request for any improvements or bug fixes.

### Acknowledgements

- [OpenAI](https://www.openai.com/) for their powerful LLMs.
- [Soft UI Dashboard](https://github.com/app-generator/flask-soft-ui-dashboard) for the beautiful dashboard design.
```
