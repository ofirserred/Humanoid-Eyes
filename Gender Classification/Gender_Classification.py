import cv2
import tensorflow as tf
import numpy as np
import serial
import time

# Open the serial port (update COM port if needed)
ser = serial.Serial('COM6', 9600)
time.sleep(2)  # Give time for the connection to establish

# Load the pre-trained model
model = tf.keras.models.load_model('pre_trained_model.h5')

# Start the camera
cap = cv2.VideoCapture(0)

# Load the Haar Cascade for face detection
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# Function to adjust brightness and contrast
def adjust_brightness_contrast(image, brightness=0, contrast=0):
    beta = brightness
    alpha = 1.0 + contrast / 100.0
    return cv2.convertScaleAbs(image, alpha=alpha, beta=beta)

# Function for gamma correction
def adjust_gamma(image, gamma=1.0):
    invGamma = 1.0 / gamma
    table = np.array([((i / 255.0) ** invGamma) * 255 for i in np.arange(0, 256)]).astype("uint8")
    return cv2.LUT(image, table)

while True:
    # Capture frame-by-frame
    ret, frame = cap.read()
    frame = cv2.flip(frame, 1)  # Flip the frame to match the mirror effect

    # Adjust lighting (gamma correction, brightness, contrast)
    frame = adjust_brightness_contrast(frame, brightness=20, contrast=30)
    frame = adjust_gamma(frame, gamma=1.2)

    # Convert to grayscale for face detection
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Apply CLAHE to improve contrast in low-light conditions
    clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8,8))
    gray = clahe.apply(gray)

    # Detect faces
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)

    # If multiple faces are detected, choose the largest one
    if len(faces) > 0:
        # Sort faces by size (area) and choose the largest
        largest_face = max(faces, key=lambda face: face[2] * face[3])

        # Get the x, y, width, and height of the largest face
        x, y, w, h = largest_face

        # Extract the face region
        face = frame[y:y+h, x:x+w]
        face_resized = cv2.resize(face, (150, 150))
        face_resized = face_resized / 255.0
        face_resized = np.expand_dims(face_resized, axis=0)

        # Predict the gender using the pre-trained model
        prediction = model.predict(face_resized)

        # Decide the label and rectangle color
        if prediction[0][0] > 0.1:
            label = "Woman"
            color = (147, 20, 255)  # Pink color (BGR)
        else:
            label = "Man"
            color = (255, 0, 0)  # Blue color (BGR)

        data = f"{label},{x},{y}\n"  # Sending label and face position
        ser.write(data.encode())  # Send data via serial to Arduino

        # Draw a rectangle around the largest face with the corresponding color
        cv2.rectangle(frame, (x, y), (x+w, y+h), color, 2)
        cv2.putText(frame, label, (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, color, 2)

    # Display the resulting frame
    cv2.imshow('Gender Classification', frame)

    # Exit on pressing 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the camera and close all windows
cap.release()
cv2.destroyAllWindows()
ser.close()  # Close the serial connection
