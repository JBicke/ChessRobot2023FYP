from guizero import App, PushButton, Slider, Text, Box, Drawing, TextBox
import subprocess

def switch_to_layout_1():
     layout1.show()
     layout2.hide()
     layout3.hide()
     layout4.hide()
     l2_logbox.value = ""
     l3_logbox.value = ""
     l3_logbox_move.value = ""
     l3_logbox_tt.value = ""

def switch_to_layout_2():
     layout1.hide()
     layout2.show()
     layout3.hide()
     layout4.hide()

def switch_to_layout_3():
     layout1.hide()
     layout2.hide()
     layout3.show()
     layout4.hide()

def switch_to_layout_4():
     layout1.hide()
     layout2.hide()
     layout3.hide()
     layout4.show()


def button_clicked():
    print("Button Clicked")
    current_text = l2_logbox.value
    new_text = "hello"
    updated_text = new_text + "\n" + current_text
    l2_logbox.value = updated_text
    

def slider_changed(slider_value):
	print(f"Slider value cahnged to {slider_value}")

def add_a():
    current_text = l3_logbox.value
    new_text = "A"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def add_b():
    current_text = l3_logbox.value
    new_text = "B"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def add_c():
    current_text = l3_logbox.value
    new_text = "C"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def add_d():
    current_text = l3_logbox.value
    new_text = "D"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def add_e():
    current_text = l3_logbox.value
    new_text = "E"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def add_f():
    current_text = l3_logbox.value
    new_text = "F"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def add_g():
    current_text = l3_logbox.value
    new_text = "G"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def add_h():
    current_text = l3_logbox.value
    new_text = "H"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def add_1():
    current_text = l3_logbox.value
    new_text = "1"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text
      
def add_2():
    current_text = l3_logbox.value
    new_text = "2"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def add_3():
    current_text = l3_logbox.value
    new_text = "3"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def add_4():
    current_text = l3_logbox.value
    new_text = "4"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def add_5():
    current_text = l3_logbox.value
    new_text = "5"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def add_6():
    current_text = l3_logbox.value
    new_text = "6"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def add_7():
    current_text = l3_logbox.value
    new_text = "7"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def add_8():
    current_text = l3_logbox.value
    new_text = "8"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def add_q():
    current_text = l3_logbox.value
    new_text = "Q"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def add_r():
    current_text = l3_logbox.value
    new_text = "R"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def add_n():
    current_text = l3_logbox.value
    new_text = "N"
    updated_text = current_text + new_text
    l3_logbox.value = updated_text

def submit_move():
    move = l3_logbox.value
    l3_logbox.value = ""
    
    current_text = l3_logbox_move.value
    new_text = move
    updated_text = new_text + "\n" + current_text
    l3_logbox_move.value = updated_text
    
    current_text = l3_logbox_tt.value
    new_text = "hello"
    updated_text = new_text + "\n" + current_text
    l3_logbox_tt.value = updated_text

def clear_move():
    l3_logbox.value = ""


app = App("dyamic Layout", width=1024, height = 600, bg= "#bbbbbb")

#layout1
layout1 = Box(app, layout="grid")
l1_title_box = Box(layout1, width='fill', grid=[0,0], border=False)
Text(l1_title_box, text="Chester the Chess Bot", size=48, color = "#607D3B")

l1_button_box_1 = Box(layout1, grid = [0,1], width=400, height = 100, border = 2)
l1_button = PushButton(l1_button_box_1, text = "Move Pieces Yourself",  command=switch_to_layout_2, width = "fill", height = "fill")

l1_button_box_2 = Box(layout1, grid = [0,2], width=400, height = 100, border = 2)
l1_button = PushButton(l1_button_box_2, text = "Type Moves",  command=switch_to_layout_3, width = "fill", height = "fill")

l1_button_box_3 = Box(layout1, grid = [0,3], width=400, height = 100, border = 2)
l1_button = PushButton(l1_button_box_3, text = "Play Automatically",  command=switch_to_layout_4, width = "fill", height = "fill")

l1_slider = Slider(layout1, start=1350, end=3100, command = slider_changed, grid = [1,1,1,3], horizontal=False, width = 300, height = 400)


#play against bot manually
layout2 = Box(app, layout="auto")

l2_title_box = Box(layout2, align="top", border= False)
Text(l2_title_box, text="Chester            ", size=48, color = "#607D3B", align="left")

l2_reset = PushButton(l2_title_box, align = "right", height = 5, width = 10, text = "Restart", command = switch_to_layout_1)


l2_log_box = Box(layout2, align="left", width=500, height = 400, border = 2)
l2_logbox = TextBox(l2_log_box, width = 'fill', height = 'fill', multiline = True)

l2_button_box = Box(layout2, align="right", width=200, height = 400, border = 2)
l2_button = PushButton(l2_button_box, text = "Make Move",  command=button_clicked, width = "fill", height = "fill")
layout2.hide()

#play against bot
layout3 = Box(app, layout="grid")
l3_logbox = TextBox(layout3, width=100, grid = [0,0, 9, 1])



l3_buttonA = PushButton(layout3, command=add_a, text = "A",  width = 10, height = 10, padx=10, pady=10, grid = [0, 1])
l3_buttonB = PushButton(layout3, text = "B", command=add_b, width = 10, height = 10, padx=10, pady=10, grid = [1, 1])
l3_buttonC = PushButton(layout3, text = "C", command=add_c, width = 10, height = 10, padx=10, pady=10, grid = [2, 1])
l3_buttonD = PushButton(layout3, text = "D", command=add_d, width = 10, height = 10, padx=10, pady=10, grid = [3, 1])
l3_buttonE = PushButton(layout3, text = "E", command=add_e, width = 10, height = 10, padx=10, pady=10, grid = [4, 1])
l3_buttonF = PushButton(layout3, text = "F", command=add_f, width = 10, height = 10, padx=10, pady=10, grid = [5, 1])
l3_buttonG = PushButton(layout3, text = "G", command=add_g, width = 10, height = 10, padx=10, pady=10, grid = [6, 1])
l3_buttonH = PushButton(layout3, text = "H", command=add_h, width = 10, height = 10, padx=10, pady=10, grid = [7, 1])
l3_buttonQ = PushButton(layout3, text = "Promotion: Q", command=add_q, width = 10, height = 10, padx=10, pady=10, grid = [8, 1])
l3_buttonK = PushButton(layout3, text = "Promotion: K", command=add_n, width = 10, height = 10, padx=10, pady=10, grid = [9, 1])

l3_button1 = PushButton(layout3, text = "1", command=add_1, width = 10, height = 10, padx=10, pady=10, grid = [0, 2])
l3_button2 = PushButton(layout3, text = "2", command=add_2, width = 10, height = 10, padx=10, pady=10, grid = [1, 2])
l3_button3 = PushButton(layout3, text = "3", command=add_3, width = 10, height = 10, padx=10, pady=10, grid = [2, 2])
l3_button4 = PushButton(layout3, text = "4", command=add_4, width = 10, height = 10, padx=10, pady=10, grid = [3, 2])
l3_button5 = PushButton(layout3, text = "5", command=add_5, width = 10, height = 10, padx=10, pady=10, grid = [4, 2])
l3_button6 = PushButton(layout3, text = "6", command=add_6, width = 10, height = 10, padx=10, pady=10, grid = [5, 2])
l3_button7 = PushButton(layout3, text = "7", command=add_7, width = 10, height = 10, padx=10, pady=10, grid = [6, 2])
l3_button8 = PushButton(layout3, text = "8", command=add_8, width = 10, height = 10, padx=10, pady=10, grid = [7, 2])
l3_buttonR = PushButton(layout3, text = "Promotion: R", command=add_r, width = 10, height = 10, padx=10, pady=10, grid = [8, 2])
l3_buttonb = PushButton(layout3, text = "Promotion: B", command=add_b, width = 10, height = 10, padx=10, pady=10, grid = [9, 2])

l3_button_sub = PushButton(layout3, text="Submit Move", command = submit_move, width = 60, height = 10, grid = [0,3,6,1])
l3_logbox_move = TextBox(layout3, width = 10, height = 10, multiline = True, grid = [6,3,1,5])
l3_logbox_tt = TextBox(layout3, width = 10, height = 10, multiline = True, grid = [7,3,1,5])
l3_button_reset = PushButton(layout3, width = 10, height = 3, text = "Restart", command = switch_to_layout_1, grid = [9, 3, 1, 5])
l3_button_clear = PushButton(layout3, width = 10, height = 3, text = "Clear Move", command = clear_move, grid = [8,3,1,5])

layout3.hide()


#automatically
layout4 = Box(app, layout = "grid")

l4_logbox = TextBox(layout4, width=20, height = 20, multiline = True, grid = [0,0,1 , 4])



l3_buttonA = PushButton(layout4, command=add_a, text = " ",  width = 10, height = 10, padx=10, pady=10, grid = [1, 0])
l3_buttonB = PushButton(layout4, text = "", command=add_b, width = 10, height = 10, padx=10, pady=10, grid = [2, 0])
l3_buttonC = PushButton(layout4, text = "", command=add_c, width = 10, height = 10, padx=10, pady=10, grid = [3, 0])
l3_buttonD = PushButton(layout4, text = "", command=add_d, width = 10, height = 10, padx=10, pady=10, grid = [4, 0])
l3_buttonE = PushButton(layout4, text = "", command=add_e, width = 10, height = 10, padx=10, pady=10, grid = [5, 0])
l3_buttonF = PushButton(layout4, text = "", command=add_f, width = 10, height = 10, padx=10, pady=10, grid = [1, 1])
l3_buttonG = PushButton(layout4, text = "", command=add_g, width = 10, height = 10, padx=10, pady=10, grid = [2, 1])
l3_buttonH = PushButton(layout4, text = "", command=add_h, width = 10, height = 10, padx=10, pady=10, grid = [3, 1])
l3_buttonQ = PushButton(layout4, text = "", command=add_q, width = 10, height = 10, padx=10, pady=10, grid = [4, 1])
l3_buttonK = PushButton(layout4, text = "", command=add_n, width = 10, height = 10, padx=10, pady=10, grid = [5, 1])

layout4.hide()



app.display()