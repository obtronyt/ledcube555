from tkinter import *
from tkinter.ttk import *


def toggle(id):
        global off
        global ledOff
        global ledOn
        id=int(id)
        if state[id] == 0:
            button[id]['image']=ledOn
            state[id]=1
            pixelAdd(id)
        else:
            button[id]['image']=ledOff
            state[id]=0
            pixelDel(id)
        display()
def pixelAdd(id):
    global regVal
    global v
    side=v.get()
    if(side=='1'):#TOP VIEW
        regVal[0] = regVal[0] | 1<<id
        print(hex(regVal[0]))
    else: #Front View
        if(id<5):
            regVal[0] = regVal[0] | 1<<id
        elif (id<10):
            regVal[1] = regVal[1] | 1<<(id-5)
        elif (id<15):
            regVal[2] = regVal[2] | 1<<(id-10)
        elif (id<20):
            regVal[3] = regVal[3] | 1<<(id-15)
        elif (id<25):
            regVal[4] = regVal[4] | 1<<(id-20)
def pixelDel(id):
    global regVal
    global v
    side=v.get()
    if(side=='1'):
        regVal[0] = regVal[0] & ((1<<id)^(0x1ffffff))
        print(hex(regVal[0]))
    else: #Front View
        if(id<5):
            regVal[0] = regVal[0] & ((1<<id)^(0x1ffffff))
        elif (id<10):
            regVal[1] = regVal[1] & ((1<<(id-5))^(0x1ffffff))
        elif (id<15):
            regVal[2] = regVal[2] & ((1<<(id-10))^(0x1ffffff))
        elif (id<20):
            regVal[3] = regVal[3] & ((1<<(id-15))^(0x1ffffff))
        elif (id<25):
            regVal[4] = regVal[4] & ((1<<(id-20))^(0x1ffffff))

def display():
    global regVal;
    out=""
    for i,j in enumerate(regVal):
        out=out+str(i)+': '+hex(j)+'\n'
    disp.delete(1.0,END)
    disp.insert(END,out)
regVal=[0,0,0,0,0]
leds=list()
state=list()

for led in range (0,25):
    leds.append(str(led))
    state.append(0)
button=list()
win = Tk()
f1= LabelFrame(win)
f1.grid(row=0,column=0,padx=10,pady=10)
f2= LabelFrame(win)
f2.grid(row=1,column=0,padx=10,pady=10)
f3= LabelFrame(win)
f3.grid(row=0,column=1,padx=10,pady=10)
disp=Text(f3,height=10,width=20,bg="light cyan")
ledOff = PhotoImage(file='GreyButton.png')
ledOn = PhotoImage(file='RedButton.png')
win.geometry('500x400')
for i in range (5):
    for j in range(5):
        pos=j+(i*5)	
        button.append(Button(f1, text=leds[pos],image=ledOff, command=lambda c=pos: toggle(button[c].cget("text"))))
        button[pos].grid(row=i,column=j)
v = StringVar(f2, "1")
values = {"Top View" : "1","Front View" : "2"}
 
# Loop is used to create multiple Radiobuttons
# rather than creating each button separately
for (text, value) in values.items():
    Radiobutton(f2, text = text, variable = v,value = value).pack()
disp.pack()
win.mainloop()