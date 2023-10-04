from guizero import App, PushButton, Slider, Text, Box, Drawing, TextBox
import subprocess

def switch_to_layout_1():
     layout1.show()
     layout2.hide()

def switch_to_layout_2():
     layout1.hide()
     layout2.show()




def button_clicked():
    print("Button Clicked")
    current_text = l2_logbox.value
    new_text = "hello"
    updated_text = new_text + "\n" + current_text
    l2_logbox.value = updated_text
    

def slider_changed(slider_value):
	print(f"Slider value cahnged to {slider_value}")

def add_character(char):
      l3_text_box.value += char
      move_input += char

app = App("dyamic Layout", width=1024, height = 600, bg= "#bbbbbb")

#layout1
layout1 = Box(app, layout="grid")
l1_title_box = Box(layout1, width='fill', grid=[0,0], border=False)
Text(l1_title_box, text="Chester the Chess Bot", size=48, color = "#607D3B")

l1_button_box_1 = Box(layout1, grid = [0,1], width=400, height = 100, border = 2)
l1_button = PushButton(l1_button_box_1, text = "Move Pieces Yourself",  command=switch_to_layout_2, width = "fill", height = "fill")

l1_button_box_2 = Box(layout1, grid = [0,2], width=400, height = 100, border = 2)
l1_button = PushButton(l1_button_box_2, text = "Type Moves",  command=switch_to_layout_2, width = "fill", height = "fill")

l1_button_box_3 = Box(layout1, grid = [0,3], width=400, height = 100, border = 2)
l1_button = PushButton(l1_button_box_3, text = "Play Automatically",  command=switch_to_layout_2, width = "fill", height = "fill")

l1_slider = Slider(layout1, start=1, end=3500, command = slider_changed, grid = [1,1,1,3], horizontal=False, width = 300, height = 400)


#play against bot manually
layout2 = Box(app, layout="auto")

l2_title_box = Box(layout2, align="top", border= False)
Text(l2_title_box, text="Chester            ", size=48, color = "#607D3B", align="left")

l2_drawing = Drawing(l2_title_box)
l2_drawing.rectangle(10,10,100,100, color = 'blue')

l2_log_box = Box(layout2, align="left", width=500, height = 400, border = 2)
l2_logbox = TextBox(l2_log_box, width = 'fill', height = 'fill', multiline = True)

l2_button_box = Box(layout2, align="right", width=200, height = 400, border = 2)
l2_button = PushButton(l2_button_box, text = "Make Move",  command=button_clicked, width = "fill", height = "fill")
layout2.hide()

#play against bot
layout3 = Box(app, layout="grid")
l3_text_box = TextBox(layout3, width=30, height=3)

characters = "ABCDE12345678"
for char in characters:
      l3_button = PushButton(layout3, txt=char, padx=10, pady=10)
      l3_button.when_clicked = add_character(char)

#automatically

app.display()