# digidojow
**A digispark ( ATTiny85 sleep aid )**  

Parts list:  
* Digispark ( Clone ) 
* Red LED  
* Appropriate Resistor for LED Chosen
* Heatshrink for entombing the assembly

The LED and resistor assembly are placed in between ground and P0. And then the whole thing can be wrapped in heat shrink tubing.

The faint glow on the ceiling fading in and out and gradually slowing down is quite soothing in my experience. Breathe in as the red circle increases, breathe out slowly as it fades.

This code has been modified from dshiffmans version to run for three hours by default. The lights pulsation gradually slows to 5 cycles per minute.

There's no on/off switch because at the end of the cycle the ATTiny85 is put to sleep and only draws a few micro-amps. 
