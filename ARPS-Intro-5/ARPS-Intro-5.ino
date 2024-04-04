/*
 Project: Intro-5-Analog-Input         Activity: mirobo.tech/arps-intro-5
 Date:    April 4, 2024
 
 This introductory programming activity for the mirobo.tech ARPS circuit
 demonstrates analog input and serial output for data output or to assist
 in debugging. This activity requires either the temperature sensor or the
 voltage divider resistors R20 and R21 (or both) to be installed in ARPS. 

 Additional program analysis and programming activities examine temperature
 calculations, float numeric types, and conversion between numeric types.

 See the https://mirobo.tech/arps website for more programming activities.
*/

// Define I/O pins used by ARPS human interface devices
const int SW2 = 0;      // Pushbuttons SW2 and SW3 are supported on Arduino
const int SW3 = 1;      // UNO R4 Minima and Arduino UNO R4 WiFi
const int SW4 = 2;      // Pushbuttons SW4 and SW5 work on all Arduino UNO R3
const int SW5 = 3;      // and Arduino UNO R4 circuit boards

const int LED2 = 5;     // ARPS LEDs
const int LED3 = 6;
const int LED4 = 9;
const int LED5 = 10;

const int BEEPER = 11;  // ARPS Piezo beeper LS1

const int VTMP = A2;    // ARPS temperature sensor Vout
const int VDIV = A3;    // ARPS voltage divider Vout

// Define program variables
int SW2State;
int SW3State;
int SW4State;
int SW5State;

int dec0;       // Decimal number digit 0 - ones digit (LSB)
int dec1;       // Decimal number digit 1 - tens digit
int dec2;       // Decimal number digit 2 - hundreds digit
int rawTemp;    // Raw/analog temperature conversion result 
int rawVolts;   // Raw/analog voltage divider conversion result
int tempDegC;   // Temperature in degrees C
float volts;    // Voltage divider output voltage

void setup() {
  pinMode(SW2,INPUT_PULLUP);
  pinMode(SW3,INPUT_PULLUP);
  pinMode(SW4,INPUT_PULLUP);
  pinMode(SW5,INPUT_PULLUP);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  pinMode(LED5,OUTPUT);
  pinMode(BEEPER,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);

  pinMode(VTMP,INPUT);
  pinMode(VDIV,INPUT);
  Serial.begin(9600);     // Start serial port

  tone(BEEPER,4000,100);  // Say hello!
}

void loop() {
  // Read temperature input
  rawTemp = analogRead(VTMP);
  
  // Send results to the serial monitor
  Serial.println(rawTemp);

  delay(1000);
}

// Convert an 8-bit binary number to 3 decimal digits
void binToDec(unsigned char bin)
{
  dec0 = bin;             // Store number in ones digit
  dec1 = 0;               // Clear tens digit
  dec2 = 0;               // Clear hundreds digit
  
  // Count hundreds digits in dec2 variable
  while(dec0 >= 100) {
      dec2 ++;
      dec0 = dec0 - 100;
  }
  // Count tens digits in dec1 variable, dec0 will contain remaining ones
  while(dec0 >= 10) {
      dec1 ++;
      dec0 = dec0 - 10;
  }
}

/* Learn More -- Program Analysis Activities
 * 
 * 1.   Let's test the ARPS temperature sensor. Instead of trying to display
 *      the temperature on ARPS's four LEDs, Arduino provides an alternate way
 *      of sharing data using its serial port. Serial data sent from the
 *      Arduino can be displayed using the Serial Monitor in the connected
 *      Arduino IDE. This program's setup() function includes the following
 *      program statement required to start using the Serial port:
 
  Serial.begin(9600);     // Start serial port

 *      The main loop() of the program is composed of the statements:

  // Read temperature input
  rawTemp = analogRead(VTMP);

  // Send results to the serial monitor
  Serial.println(rawTemp);

  delay(1000);

 *      Every time the 'Serial.println(rawTemp);' statement is executed, it
 *      will print a new line containing the rawTemp variable converted from
 *      the temperature sensor by the ADC (Analog-to-Digital Converter).
 *      Build and upload the program into your circuit, and then click on the
 *      Serial Monitor icon at the top right of the IDE screen. The Serial
 *      monitor should open in a tab beside the output tab at the bottom of
 *      the screen, and a new number should appear every second representing
 *      the latest conversion result. What are your results? Grab and hold the
 *      temperature sensor with your fingers. Do your results change?

 * 2.   The ADC in the Arduino UNO quantizes input voltages between 0 and 5V
 *      into 10-bit (binary) numbers representing the input voltage. How many
 *      different states or numbers can be represented using 10 binary bits?
 *
 * 3.   Calculate the amount of input voltage change necessary to cause a 1-bit
 *      (or single binary digit) change in the converted output result when
 *      using 10 bits to represent a 5V input range.
 *
 * 4.   The analog temperature sensor used in ARPS has a temperature coefficient
 *      of 10mV/°C, meaning that it produces an output voltage that increases by
 *      10mV for every 1°C rise in temperature. The sensor has a built-in offset
 *      voltage to allow its minimum temperature to be below 0°C, as well as to
 *      help improve its output linearity. What this means is that we will need
 *      to develop an algorithm to convert the analog voltage input from the
 *      temperature sensor into a useful temperature that we can use or display.
 *
 *      Based on the information above, calculating the actual temperature
 *      should be as simple as removing the offset, and then scaling the 
 *      sensor's temperature coefficient to the ADC's quantization voltage
 *      calculated in step 2, above. The scale factor can be given by:
 *
 *      10mV/°C / mV/bit (ADC mV/bit from step 2)
 *
 *      The result you should get when you divide your ADC sensitivity into the
 *      sensor's temperature will be close to 2, meaning that a 1°C change in
 *      temperature will create a 2-digit change in ADC output values. If
 *      necessary, that could allow us to display temperatures with an accuracy
 *      of 0.5°C, but we will define an int(eger) variable for temperature and
 *      won't use that extra accuracy in this example.
 *
 *      To remove the offset, it's best to know the actual temperature and then
 *      to calibrate the offset amount from that value. For the MCP9700A 
 *      temperature sensor recommended in the ARPS schematics, try an offset
 *      of 100 to start if using the Arduino UNO R3, or an offset of 127 if
 *      using Arduino UNO R4. For the TMP36 temperature sensor in Tinkercad,
 *      the offset is easy to find by adjusting the temperature to 0°C and
 *      using the raw number from the conversion at this temperature as the
 *      offset. Let's add this temperature conversion calculation after the
 *      analogRead() function in the program:
 
  tempDegC = (rawTemp - 100) / 2;   // Calculation for MCP9700A temperature sensor in ARPS

 *      Next, replace the single Serial.println() function with these lines:

  Serial.print("rawTemp: ");
  Serial.print(rawTemp);
  Serial.print("\t");
  Serial.print("Temp: ");
  Serial.print(tempDegC);
  Serial.println("C");

 *      Rather than just outputting a number, the Serial.print() functions will
 *      now build a line of text, starting with the string 'rawTemp: ', adding
 *      the value of the rawTemp variable, inserting a tab ('\t') character,
 *      printing the string 'Temp: ', adding the converted temperature, and
 *      finishing with the letter 'C'. Notice that the last statement uses the
 *      Serial.println() function, which finishes the current line of text
 *      output and starts a new line for the next Serial.print() function.
 *
 * 5.   The ARPS voltage divider resistors were chosen to produce a voltage
 *      coefficient scaled for an 8-bit ADC. What is the maximum value that
 *      an 8-bit ADC can produce? In programming analysis activity 2, above,
 *      you calculated the maximum value capable of being represented by the
 *      10-bit ADC in the Arduino. What scale factor will be needed to
 *      correlate the voltage divider output to the ADC input of Arduino?
 *
 * 6.   The battery voltage can be calculated from the voltage divider using
 *      the following set of instructions. You might be wondering why the
 *      scale factor is different from the one calcualted in step 5, above.
 *      To get 0.1V resolution in an 8-bit ADC, the voltage divider values
 *      were chosen to provide a 10x larger numeric output than the voltage.

  // Read voltage divider and convert to volts
  rawVolts = analogRead(VDIV);
  volts = float(rawVolts) / 40;

 *      Add this section code to your program, along with instructions to
 *      print the rawVolts and volts variables to the serial monitor. To
 *      get an accurate reading, your ARPS and Arduino will have to be
 *      powered through an external power supply connected to the DC input
 *      ARPS screw strip instead of using USB power.
 *
 *      Whether you are connected to an extenal power supply or not, there
 *      is an important type conversion used in this example. Comment out
 *      the rawVolts assignment and replace it with the following statement:

  rawVolts = 238;

 *      Run the program. The volts output should be 5.95.
 *      
 *      Next, remove the float conversion function from the volts assignment
 *      so that it reads like this:

  volts = rawVolts / 40;

 *      Run the program again. What is the volts output now? Can you explain
 *      what might be happening to make the volts output different? (Hint:
 *      think about the types of the variables that are used.)
 *
 * 7.   This program contains the binToDec() function which demonstrates a
 *      simple method of converting an 8-bit binary value to three decimal
 *      digits. A function like this might be useful if a value needs to be
 *      displayed on individual digits in a 7-segment display, for example.
 *      Can you figure out how the function works? Explain the function, or
 *      create a flow chart to describe its operation.
 * 
 * Programming Activities
 * 
 * 1.   Create a program that uses the binToDec() function to convert the raw
 *      temperature into three decimal digits, and then transmit the digits to
 *      the IDE using a combination of Serial.print() and Serial.println()
 *      functions to display the values as a number in the serial monitor.
 * 
 * 2.   Does your ARPS circuit have the voltage divider resistors (R20 and R21)
 *      and the screw terminal strip installed? If it does, an external power
 *      supply can be connected to power both the ARPS and Arduino, and the use
 *      of this external power supply enables the voltage divider circuit on
 *      ARPS to measure the supply voltage by reading the voltage divider.
 *
 *      Create a function that returns the power supply voltage to the calling
 *      program code.
 *
 * 3.   ARPS can sense its optical floor sensors using either digital or
 *      analog inputs. If you plan on using ARPS to make a floor or line 
 *      sensing robot, using analog input provides some advantages over
 *      digital input. Create a function that reads the left and right floor
 *      or line sensors in analog mode and returns digital values representing
 *      whether or not each floor sensor's light level has crossed a user-
 *      determined threshold.
 *      
 *      After building the robot and installing the sensors, the serial
 *      monitor can be used to set appropriate light threshold values for the
 *      floor sensors based on the distance they are mounted away from the
 *      floor and the reflectivity of the floor surface.
 *
 * 4.   Analog inputs can be used to control analog outputs through servo
 *      pulses, PWM, or by using the analogWrite() function. Create a program
 *      for a line-following robot that uses analog line sensor inputs to
 *      determine analog motor outputs, or a program that rotates a servo 
 *      based on the amount of the amount of light or temperature input.
 */
