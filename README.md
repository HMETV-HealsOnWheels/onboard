## Upload code to Arduino
Compile:

arduino-cli compile --fqbn arduino:avr:mega /home/hmetv/test/test2/test2.ino

Upload:

arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:mega /home/hmetv/test/test2/test2.ino

Monitor:

arduino-cli monitor -p /dev/ttyACM0


IP address for Pi (may change): 192.168.1.23

