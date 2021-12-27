# LED CUBE 5x5x5  
# Schematic  
![alt text](https://github.com/obtronyt/ledcube555/blob/master/Schematic.png?raw=true)  
  
# Youtube Link  

# How to make animations using Pattern Gen  
1. Select Top View or Front View  
2. Select Required LEDs per frame of animation  
3. Copy shown hex code into code for each register  

# Example- Sine Wave  
## Pattern Generator  
![alt text](https://github.com/obtronyt/ledcube555/blob/master/SineWaveDemo.jpg?raw=true)  
  
## Output  
Note: The HEX value generated is only for one face. So to move or add to the next face simply left shift by (face index * 5)  
Eg: To get below output load Shift Register something like this.  
  
shiftReg[reg] = value<<(0) | value<<(5) | value<<(10) | value<<(15) | value<<(20)   
  
![alt text](https://github.com/obtronyt/ledcube555/blob/master/SW.gif?raw=true)
