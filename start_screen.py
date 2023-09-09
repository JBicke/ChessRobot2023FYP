from guizero import App, PushButton, Slider, Text, Box
import subprocess

def button_clicked():
    try:
        subprocess.run(["jupyter", "nbconvert", "--to", "notebook", "--execute", r"C:\Users\Jackson\playing_screen.ipynb"], check=True)
        app.destroy()
    except subprocess.CalledProcessError as e:
        print(f"Error running the script: {e}")
    else:
        print("Script executed successfully")
    
    

def slider_changed(slider_value):
	print(f"Slider value cahnged to {slider_value}")

app = App(layout="auto", width=1024, height=600, bg = "#bbbbbb")
title_box = Box(app, width='fill', align='top', border=False)
Text(title_box, text="Chester the Chess Bot", size=48, color = "#607D3B")

button_box = Box(app, align="left", width=400, height = 400, border = 2)
button = PushButton(button_box, text = "Start Game",  command=button_clicked, width = "fill", height = "fill")


slider = Slider(app, start=1, end=3500, command = slider_changed, horizontal = False, align="right", width = 400, height = 400)

app.display()