1.pin diagram

You can see which cable goes where here.:

 dot matrix:

    DIN -> D11
    CS -> D10
    CLK -> D13
    VCC -> 5V
    GND -> GND

  lcd screen:
  
    SCL -> SCL
    SDA -> SDA
    VCC -> 5V
    GND -> GND
  remote:

    Signal -> D2
    
2.execution order:
    
    In the Python file, replace ‘/dev/ttyUSB0’ with the location where you connected your Arduino. 
    Upload the .ino file to your Arduino board, 
    then run the Python file. 
    Be careful not to run the Python file before your Arduino is running. 

3.using:

    You can use specific buttons on the remote to turn the LCD and dot matrix display on and off. 
    Additionally, you can use two animations on the dot matrix without running a Python file. 
    Since the Uno's memory is limited, I could only add two animations.
    According to my remote:
    “*” -> LCD on/off
    “#” -> Dot matrix on/off
    “1” -> Scrolling animated text (You can change this in the .ino file.)
    “2” -> Matrix rain
