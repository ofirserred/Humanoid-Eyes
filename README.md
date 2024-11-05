# Humanoid Eyes

## Description
The **Humanoid Eyes** project aims to track a person in the camera view, classify their gender, and make the robotic eyes react accordingly. This project represents a significant step toward creating realistic humanoid eye robotics that provide lifelike interactions.

## Features
- Person tracking via camera
- Gender classification
- Reactive eye movements based on detected individuals

## Technologies Used
- Python
- OpenCV
- TensorFlow
- Arduino IDE
- 3D Printer
- Arduino Uno Board

## Installation Instructions

### Physical Setup
1. **Arduino Board**: Ensure you have an Arduino board for configuration and connection.
2. **Servos**: You will need six servos connected to the Arduino board. Each servo controls a different part of the eye (see `Servos_connection_info.txt` for details).
3. **3D Model**: 3D print two human eyeballs and four eyelids. Connect each one to the corresponding servos as outlined in the connection information.
4. **Power Supply**: Provide power to the servos as required.
5. **Connect Arduino**: Link your Arduino to your laptop.

### Software Setup
1. **Dependencies**: Before running the project, ensure you have the following Python packages installed:
   ```bash
   pip install opencv-python tensorflow numpy pyserial
2. **Arduino IDE**: Download and install the Arduino IDE. Open the code file Servos_Control.ino and upload it to your Arduino board.
3. **Servo Adjustment**: You may need to adjust the servo settings in the code to achieve smoother movements that meet your design expectations.

### Usage
## To use the Humanoid Eyes project:

1. Connect the Arduino board to your computer.
2. Open the Arduino IDE, select the correct board, and upload the code.
3. Run the Python script to start the camera tracking and eye movement.

### Contributing
  Contributions are welcome! If you would like to contribute to the project, please fork the repository and submit a pull request with your changes.
