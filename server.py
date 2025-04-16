import serial
import socket
import serial.tools.list_ports
import time
import sys

# Command bytes (for other robots, not used with Arduino serial)
FORWARD = int(1).to_bytes(1, byteorder='big')
BACKWARD = int(2).to_bytes(1, byteorder='big')
LEFT = int(3).to_bytes(1, byteorder='big')
RIGHT = int(4).to_bytes(1, byteorder='big')
DONE = int(5).to_bytes(1, byteorder='big')
STOP = int(6).to_bytes(1, byteorder='big')
WAIT = int(7).to_bytes(1, byteorder='big')

possible_commands = {
    "forward": FORWARD,
    "backward": BACKWARD,
    "left": LEFT,
    "right": RIGHT,
    "done": DONE,
    "stop": STOP,
    "wait": WAIT
}

# Command mappings for Arduino (char commands)
COMMAND_MAP = {
    "forward": 'f',
    "backward": 'b',
    "left": 'l',
    "right": 'r',
    "emergency-stop": 's',
    "quit": 'd'
}

print_mode = False
Scanning = True
path = []
obstacle_detected = False
MILISECOND = 1.0 / 1000.0
arduino = None

# Get IP address for the socket server
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.connect(("8.8.8.8", 80))
TCP_IP = s.getsockname()[0]
s.close()

TCP_PORT = 5001
BAUD_RATE = 9600
SERIAL_PORT = '/dev/ttyACM0'  # Adjust this if needed

# Setup Arduino serial connection
try:
    arduino = serial.Serial(SERIAL_PORT, BAUD_RATE)
    print(f"[✓] Arduino connected on {SERIAL_PORT}")
except Exception as e:
    print(f"[X] ERROR connecting to Arduino: {e}")
    Scanning = False

# Create the server socket
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((TCP_IP, TCP_PORT))
server.listen(1)

# Unified function to send command to Arduino
def send_to_arduino(cmd_char, duration=None):
    message = cmd_char
    if duration is not None:
        message += str(duration)
    arduino.write((message + '\n').encode('utf-8'))

# Path runner
def run():
    global path
    if len(path) < 1:
        print("NO PATH LOADED")
        return

    for command, duration in path:
        if print_mode:
            print(f"Running: {command}, {duration}")
        else:
            send_to_arduino(COMMAND_MAP[command], duration)

        # Let Arduino handle the cruise time
        if command != "emergency-stop" and command != "stop":
            time.sleep((duration + 500) / 1000.0)

    print("Path complete!")
    path.clear()


# Server loop
print(f"[✓] Server listening on {TCP_IP}:{TCP_PORT}...")
while Scanning:
    try:
        conn, addr = server.accept()
        print(f"[✓] Connection from {addr}")
        data = conn.recv(1024).decode('utf-8').strip()
        print(f"[→] Received: {data}")

        for t_command in data.split('\n'):
            try:
                cmd, duration = t_command.split(',')
                cmd = cmd.strip().lower()
                duration = int(duration)

                if cmd == "emergency-stop":
                    if not print_mode:
                        send_to_arduino(COMMAND_MAP[cmd])
                    else:
                        print(cmd)
                    path.clear()
                    break

                if cmd in COMMAND_MAP:
                    path.append([cmd, duration])
                else:
                    print(f"[!] Unknown command: {cmd}")

            except ValueError:
                print(f"[!] Invalid format: {t_command}")

        run()
        conn.close()

    except Exception as e:
        print(f"[X] Server error: {e}")
