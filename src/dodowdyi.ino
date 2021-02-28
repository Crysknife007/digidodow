// Inspiration from gary909 instructable:
// https://www.instructables.com/DIY-Dodow-Clone-Arduino-Sleep-Meditation-Machine

#include "Arduino.h"
#include <EEPROM.h>
#include <avr/sleep.h>

const int led = PB0;           		// LED is attached to pwm pin 5
const float pwmIntervals = 254;   // Define pwm intervals
const float minpwm = 50;			    // Minimum brightness
const float msPerMin = 60000;     // Set the ms per minute

// Fade a LED to perceived brightness rather than linearly
// Thanks to Diarmuid Mac Namara's blog for the calculation:
// https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms
// Use lookup table in EEPROM rather than RAM, don't re-write each time
void populateLookup() {
  
	const float r = (pwmIntervals * log10(2))/(log10(255));
	byte PWMval, EEPval;
  
	for (int i = 0; i <= pwmIntervals; i++) {
  
		// Calculate the required PWM value for this interval step
		PWMval = round(pow (2, (i / r))) - 1;
		EEPval = EEPROM.read(i);
    
    // Only write if changed
		if (EEPval != PWMval) EEPROM.write (i,PWMval); 
   
	}
 
}

// Ramp the brightness up and down on the LED
void ramp (float BPM, float minutesDuration) {
  
	// 40% in (up) 60% out (down) for total of 60,000/BPM milliseconds in up/down cycle
	unsigned int upmsdelay = round(0.4*msPerMin/BPM/(pwmIntervals-minpwm));
	unsigned int downmsdelay = round(0.6*msPerMin/BPM/(pwmIntervals-minpwm));
	byte reps = round(minutesDuration*BPM);
	byte brightness = 0;

	unsigned long actual;
	unsigned long start = millis();

	for (int rep = 0;rep<reps;rep++) {

		for (byte interval = (int)minpwm; interval <= (int)pwmIntervals; interval++) {
			brightness = EEPROM.read(interval);
			analogWrite(led, brightness);
			delay(upmsdelay);
		}
   
		for (byte interval = (int)pwmIntervals; interval > (int)minpwm; interval--) {
			brightness = EEPROM.read(interval);
			analogWrite(led, brightness);
			delay(downmsdelay);
		}
   
	}

	actual = millis()-start;

}

void docycle () {
      ramp (11.0,3); 	// Min 1-3 = 11 breaths per minute
      ramp (10.0,3);	// Min 4-6 = 10 breaths per minute
      ramp (9.0,3);	// Min 7-9 = 9 breaths per minute
      ramp (8.0,3);	// Min 10-12 = 8 breaths per minute
      ramp (7.0,3);	// Min 13-15 = 7 breaths per minute
      ramp (6.5,2);	// Min 16-17 = 6.5 breaths per minute
      ramp (6.0,3);	// Min 18-20 = 6 breaths per minute
      ramp (5.5,3);     // Min 20-23 = 5.5 breaths per minute
      ramp (5.0,20);    // Min 23-180 5 breaths per minute
      ramp (5.0,20);    // ^
      ramp (5.0,20);    // ^
      ramp (5.0,20);    // ^
      ramp (5.0,20);    // ^
      ramp (5.0,20);    // ^ 
      ramp (5.0,20);    // ^ 
      ramp (5.0,17);    // ^
}


// The setup routine runs once powered on or when reset
void setup() {

    // Set the led to be an output
    pinMode(led, OUTPUT);

    // Set up all the fade values
    populateLookup();

    // Run the sleep machine
    docycle();

    /* Go into low power mode: -- thanks to Nick Gammon's blog: http://www.gammon.com.au/power
    * All outputs to low, ADC off, Sleep mode to 'power down', Brown out disabled
    */
    digitalWrite (led,LOW);
    ADCSRA = 0;
    set_sleep_mode (SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_bod_disable();
    interrupts ();
    sleep_cpu ();
}

void loop() {
    // Nothing to see here
}
