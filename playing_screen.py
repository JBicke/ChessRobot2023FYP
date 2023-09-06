from guizero import App, PushButton, Slider, Text, Box, Drawing, TextBox

def button_clicked():
	print("Button Clicked")
	logbox.value ="hello"

app = App(layout="auto", width=1024, height=600, bg = "#bbbbbb")

title_box = Box(app, align="top", border= False)
Text(title_box, text="Chester            ", size=48, color = "#607D3B", align="left")


drawing = Drawing(title_box)
drawing.rectangle(10,10,100,100, color = 'blue')

log_box = Box(app, align="left", width=500, height = 400, border = 2)
logbox = TextBox(log_box, width = 'fill', height = 'fill')

button_box = Box(app, align="right", width=200, height = 400, border = 2)
button = PushButton(button_box, text = "Make Move",  command=button_clicked, width = "fill", height = "fill")

app.display()
