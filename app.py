from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)  # Enable CORS for all routes

# Dictionary to store the latest data
data_store = {
    "button_state": 0,
    "motion_detected": 0,
    "temperature": 0.0,
    "humidity": 0.0,
    "mq2_value": 0,
    "wall_name": "",
    "mac_address": "",
    "rssi": 0
}

@app.route("/data", methods=["POST"])
def receive_data():
    try:
        data = request.json
        if data["type"] == "sensor":
            data_store["button_state"] = int(data["button_state"])
            data_store["motion_detected"] = int(data["motion_detected"])
            data_store["temperature"] = data["temperature"]
            data_store["humidity"] = data["humidity"]
            data_store["mq2_value"] = data["mq2_value"]
        elif data["type"] == "BLE":
            wall_name_parts = data['wall_name'].split(',')

            # Assign to variables
            print(wall_name_parts[0])
            data_store['wall_name'] = wall_name_parts[0]
            data_store['mac_address'] = wall_name_parts[1]
            data_store['rssi'] = wall_name_parts[2]
            print(data)
        print(data_store)
        return jsonify({"status": "success", "message": "Data updated"}), 200
    except Exception as e:
        print(e)
        return jsonify({"status": "error", "message": str(e)}), 400

@app.route("/data", methods=["GET"])
def get_data():
    return jsonify(data_store)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
