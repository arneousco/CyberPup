import tkinter as tk
from tkinter import font
from PIL import Image, ImageTk
import cv2
import urllib.request
import numpy as np
from threading import Thread
import requests

img_bytes = b''


# PROTOCOL FORMAT = 'U D L R P Q R S A B C D'

def button_pressed_thread(num):
    t = Thread(target=button_pressed, args=(num,))
    t.start()


def button_pressed(num):
    # protocol = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    # protocol[num] = 1
    # data = ''.join(map(str, protocol))
    response = requests.post("http://my-esp8266.local/post", data=str(num))
    print(response.text)




def update_video():
    global img_bytes
    url = "http://my-esp32cam.local/600x500.mjpeg"
    req = urllib.request.urlopen(url)

    # Loop until the user presses q to quit
    while True:
        # Read a chunk of data from the url
        chunk = req.read(1024)
        if not chunk:
            break
        img_bytes += chunk
        start = img_bytes.find(b'\xff\xd8')
        end = img_bytes.find(b'\xff\xd9')
        if start != -1 and end != -1:
            jpg = img_bytes[start:end + 2]
            img_bytes = img_bytes[end + 2:]
            img_array = np.frombuffer(jpg, dtype=np.uint8)
            img = cv2.imdecode(img_array, cv2.IMREAD_COLOR)
            img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
            img = Image.fromarray(img)
            img = ImageTk.PhotoImage(img)
            video_label.config(image=img)
            video_label.image = img


root = tk.Tk()
root.geometry("150x500")

# Create frames for control pads and video display
control_frame = tk.Frame(root)
control_frame.pack(side="left", fill="y")
video_frame = tk.Frame(root)
video_frame.pack(side="right", fill="both", expand=True)

# Create control pads
pad_frame = tk.Frame(control_frame)
button_font = font.Font(family="Helvetica", size=14, weight="bold")

pad_frame.pack(padx=20, pady=20)
up_button = tk.Button(pad_frame, text='U', command=lambda: button_pressed_thread('A'))
up_button.grid(row=0, column=1)
down_button = tk.Button(pad_frame, text='D', command=lambda: button_pressed_thread('B'))
down_button.grid(row=2, column=1)
left_button = tk.Button(pad_frame, text='L', command=lambda: button_pressed_thread('C'))
left_button.grid(row=1, column=0)
right_button = tk.Button(pad_frame, text='R', command=lambda: button_pressed_thread('D'))
right_button.grid(row=1, column=2)

for button in [up_button, down_button, left_button, right_button]:
    button.config(bg="#B8B9BA", fg="white", font=button_font)

pad_frame = tk.Frame(control_frame)
pad_frame.pack(padx=20, pady=20)
up_button = tk.Button(pad_frame, text='P', command=lambda: button_pressed_thread('E'))
up_button.grid(row=0, column=1)
down_button = tk.Button(pad_frame, text='Q', command=lambda: button_pressed_thread('F'))
down_button.grid(row=2, column=1)
left_button = tk.Button(pad_frame, text='R', command=lambda: button_pressed_thread('G'))
left_button.grid(row=1, column=0)
right_button = tk.Button(pad_frame, text='S', command=lambda: button_pressed_thread('H'))
right_button.grid(row=1, column=2)

for button in [up_button, down_button, left_button, right_button]:
    button.config(bg="#B8B9BA", fg="white", font=button_font)

pad_frame = tk.Frame(control_frame)
pad_frame.pack(padx=20, pady=20)
up_button = tk.Button(pad_frame, text='A', command=lambda: button_pressed_thread(8))
up_button.grid(row=0, column=1)
down_button = tk.Button(pad_frame, text='B', command=lambda: button_pressed_thread(9))
down_button.grid(row=2, column=1)
left_button = tk.Button(pad_frame, text='C', command=lambda: button_pressed_thread(10))
left_button.grid(row=1, column=0)
right_button = tk.Button(pad_frame, text='D', command=lambda: button_pressed_thread(11))
right_button.grid(row=1, column=2)

for button in [up_button, down_button, left_button, right_button]:
    button.config(bg="#B8B9BA", fg="white", font=button_font)

# video_label = tk.Label(video_frame, width=600, height=500)
# video_label.pack()
# video_thread = Thread(target=update_video)
# video_thread.start()
if __name__ == '__main__':

    root.mainloop()
