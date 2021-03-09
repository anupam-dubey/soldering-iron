#include <FastPID.h>              //Dwonload it here: https://www.electronoobs.com/eng_arduino_fastPID.php
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


/*PID variables. Fine tune this values. Start with P=1 and D and I = 0. Start increasing till you get good results*/
float Kp=7.98, Ki=0.055, Kd=0.86, Hz=10;          /*My values: Kp=7.98, Ki=0.055, Kd=0.86, Hz=10;*/
int output_bits = 8;                              //With 8 bits, maximum PID output is 255, and that's waht we need for analogWrite
bool output_signed = false;
FastPID myPID(Kp, Ki, Kd, Hz, output_bits, output_signed);

// adc variable
double CurrentTemp;
#define ADC_MULTISAMPLING 5
#define ADC_MULTISAMPLING_SAMPLES (1 << ADC_MULTISAMPLING)
// ao for tempread and d11 for mosfet heater

unsigned long currentMillis = 0;
unsigned long previousMillis = 0; 
//Inputs and outputs
int MOSFET_pin = 11;     //Pin that goes to the BJT that drives the n-MSOFET gate
/*int increase_pin = 7;   //Top button of the board (temperature increase)
int decrease_pin = 4;   //Bottomop button of the board (temperature decrease)
int vibrate = A1;       //Input from the vibrations sensor*/
int temp_sense = A0;    //Input from OPAMP (temperature read from thermocouple) My OPAMP gain is 201 (R1 = 1k, R2 = 200k)
int tr=0;
int tp=0;
float temperature_read,pot_read;
int   setpoint = 650;                            //Temperature setpoint initial value
float Delay=300; 
int td=0;
double read_pot()
{
   // Filter the ADC by multisampling with the values defined at the beginning
  int adc = 0;
  for (int i = 0; i < ADC_MULTISAMPLING_SAMPLES; ++i)
    adc += analogRead(A1);
    adc = adc >> ADC_MULTISAMPLING; 
  //Get the linear regression equations from excel data (Graph adc/temperature) (You need to perform real measures with externam thermometer) 
 double Input = map(adc, 0, 1023, 50, 500);
  // Additional filtering
//  CurrentTemp += (temp-CurrentTemp)*0.05;
//  return(CurrentTemp);
 return(Input);
}
float read_temperature()
{
   // Filter the ADC by multisampling with the values defined at the beginning
  int adc = 0;
  for (int i = 0; i < ADC_MULTISAMPLING_SAMPLES; ++i)
    adc += analogRead(temp_sense);
    adc = adc >> ADC_MULTISAMPLING; 
  //Get the linear regression equations from excel data (Graph adc/temperature) (You need to perform real measures with externam thermometer) 
 //double temp =  0.946*adc + 35;
   double temp = map(adc, 0, 700, 25, 350);
  // Additional filtering
  CurrentTemp += (temp-CurrentTemp)*0.02;
  return(CurrentTemp);
// return(adc);
}

void setup()
{ Serial.begin(115200);
  pinMode(MOSFET_pin,OUTPUT);
  
  digitalWrite(MOSFET_pin,LOW);
   lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("*Solder v5.0*");
 
  delay(1000);
  analogWrite(MOSFET_pin, 0);

}



void loop()
{
 int ms = 3;
 currentMillis = millis(); //First, we save the current elapsed time in milliseconds
 if (currentMillis - previousMillis >= Delay) 
 {
  previousMillis += Delay;
 temperature_read=read_temperature();
 tr=temperature_read;
 pot_read=read_pot();
 tp=pot_read;
 setpoint=tp;
 Serial.println("Temperature=");
 Serial.print(tr);
 /*PID part*/
    uint32_t before, after;
    before = micros();
    uint8_t output = myPID.step(setpoint, temperature_read);  
    after = micros();
    /*END of PID part*/
    if(output>150)
    output=150;
 
    analogWrite(MOSFET_pin, output);       
  /*Finally we write the PWM signal to the n-MOSFET
 
    Since we have a BJT driver at the n-MOSFET gate, we invert the signal (255 - output) where
    output is the PWM value from the PID control*/
 /*td++;
 if(td>180)
 {
  td=0;
  analogWrite(MOSFET_pin, 0);
  Serial.println("ok");
 }*/
 lcd.setCursor(1,1);
 lcd.write('(');
 lcd.print(tr);
  lcd.write(')');
 lcd.print(" SET ");
 lcd.print(setpoint);
 lcd.print("  ");
 }
 
 }
 
