
//#define analogReference(INTERNAL1v1);

#define  LED 0
#define  BIAS 1
#define  XVRT_TX 2
#define  LNA 3
#define  PTT_IN 4
#define  not_conect 7


#define  DIR_485 10

#define  FILTER_SPARE_2 11
#define  FILTER_SPARE_1 12
#define  FILTER_6 13
#define  FILTER_10_12 14
#define  FILTER_15_17_20 15
#define  FILTER_30_40 16
#define  FILTER_75_80 17

#define  BDC_BIT_4 18
#define  BDC_BIT_3 19
#define  BDC_BIT_2 20
#define  BDC_BIT_1 21
#define  BDC_BIT_0 22

#define  ANALOG_PWR A0
#define  ANALOG_SWR A1
#define  ANALOG_PWR_IN A2
#define  ANALOG_FAN_NTC A3
#define  ANALOG_SPARE A4
#define  ANALOG_24V A5

#define  RF_RELAY 30
#define  FAN_OUT 31

int BAND_BCD = 0;
int BAND = 0;
int val = 0; 
boolean  IS_KEYED = false;
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval_metering = 1000;           // interval at which to blink (milliseconds)


////////////////////////////////////////////////////////////

void setup()
{
Serial.begin(9600);              //  setup serial
analogReference(INTERNAL1V1);
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(FILTER_SPARE_2, OUTPUT);
  digitalWrite(FILTER_SPARE_2, LOW);
  pinMode(FILTER_SPARE_1, OUTPUT);
  digitalWrite(FILTER_SPARE_1, LOW);
  pinMode(FILTER_6, OUTPUT);
  digitalWrite(FILTER_6, LOW);
  pinMode(FILTER_10_12, OUTPUT);
  digitalWrite(FILTER_10_12, LOW);
  pinMode(FILTER_15_17_20, OUTPUT);
  digitalWrite(FILTER_15_17_20, LOW);
  pinMode(FILTER_30_40, OUTPUT);
  digitalWrite(FILTER_30_40, LOW);
  pinMode(FILTER_75_80, OUTPUT);
  digitalWrite(FILTER_75_80, LOW);
  pinMode(RF_RELAY, OUTPUT); 
  digitalWrite(RF_RELAY, LOW);
  pinMode(FAN_OUT, OUTPUT); 
  digitalWrite(FAN_OUT, LOW);
  pinMode(LNA, OUTPUT); 
  digitalWrite(LNA, LOW);
  pinMode(XVRT_TX, OUTPUT);
  digitalWrite(XVRT_TX, LOW);
  pinMode(BIAS, OUTPUT);
  digitalWrite(BIAS, LOW);
  pinMode(not_conect, OUTPUT);
  digitalWrite(not_conect, LOW);
  
  pinMode(DIR_485, OUTPUT);
  digitalWrite(DIR_485, LOW);
  
}
  
  void PTT_KEY() 
   {
    digitalWrite(LNA, LOW);  // PUT OFF lna 
    delay(10);
    digitalWrite(RF_RELAY, HIGH);
    digitalWrite(FAN_OUT, HIGH);
    delay(10);
    digitalWrite(BIAS, HIGH);
    delay(4);
    digitalWrite(XVRT_TX, HIGH);
    IS_KEYED = true;
   }
  
  void PTT_UN_KEY() 
   {
    digitalWrite(XVRT_TX, LOW);
    delay(4);
    digitalWrite(BIAS, LOW);
    delay(4);
    digitalWrite(RF_RELAY, LOW);
    delay(10);
    digitalWrite(LNA, HIGH);
//    digitalWrite(FAN_OUT, LOW);  // let thermostatic FAN CTRL handel that
    IS_KEYED = false;
   }
   
  void SWR() 
   {
   
//#define adc_count 0.0048828125  // Volts per ADC count (0 to 5 volts, 0 to 1023 A/D counts)
//float V_Fwd, V_Ref = 0, V_SWR ;  // Define the variables

// V_Fwd = analogRead(0) * adc_count;    // Read the A/D converters and convert to voltages
//  V_Ref = analogRead(1) * adc_count;

     // Calculate VSWR
  //if (V_Fwd > V_Ref)
  //{
  //  V_SWR = (V_Fwd + V_Ref) / (V_Fwd - V_Ref);
 // } else {
  //  V_SWR = 0;
  
  //  glcd.printNumF(V_SWR,1,30,24);
  //glcd.print(" : 1",56,24);
    
   }
   
   
   
   void BAND_DECODER() {
/*
BCD Cable: HF + VHF Output Bit Pattern
Band Bit 4 Bit 3 Bit 2 Bit 1 Bit 0
160 0 0 0 0 1    = 1 decimal  
80 0 0 0 1 0     = 2 decimal  
60 0 0 0 0 0     = 0 decimal  (100W  max on 60m so amp off )
40 0 0 0 1 1     = 3 decimal  
30 0 0 1 0 0     = 4 decimal  
20 0 0 1 0 1     = 5 decimal  
17 0 0 1 1 0     = 6 decimal  
15 0 0 1 1 1     = 7 decimal  
12 0 1 0 0 0     = 8 decimal  
10 0 1 0 0 1     = 9 decimal  
6 1 0 0 0 1      = 17 decimal  
2 1 0 0 1 0      = 18 decimal  
222 1 0 0 1 1    = 19 decimal  
432 1 0 1 0 0    = 20 decimal  
902 1 0 1 0 1    = 21 decimal  
1296 1 0 1 1 0   = 22 decimal  
2304 1 0 1 1 1   = 23 decimal  
3456 1 1 0 0 0   = 24 decimal  
5760 1 1 0 0 1   = 25 decimal  
10368 1 1 0 1 0  = 26 decimal  
24048 1 1 0 1 1  = 27 decimal  
47088 1 1 1 0 0  = 28 decimal  
*/   
   
//   int val = 0; 
   BAND_BCD = 0; 
   
   if (digitalRead(BDC_BIT_4) == LOW)  {bitSet(BAND_BCD, 0);}
   if (digitalRead(BDC_BIT_3) == LOW)  {bitSet(BAND_BCD, 1);}
   if (digitalRead(BDC_BIT_2) == LOW)  {bitSet(BAND_BCD, 2);}
   if (digitalRead(BDC_BIT_1) == LOW)  {bitSet(BAND_BCD, 3);} 
   if (digitalRead(BDC_BIT_0) == LOW)  {bitSet(BAND_BCD, 4);}      
       
 
switch (BAND_BCD) {
  case 1:
     BAND = 160 ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
    break;
  case 2:
     BAND = 80 ;
     digitalWrite(FILTER_75_80, HIGH);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
    break;
  case 3:
     BAND = 40 ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, HIGH);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
    break;
  case 4:
     BAND = 30 ; 
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, HIGH);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
    break; 
  case 5:
     BAND = 20 ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, HIGH);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
    break;  
  case 6: 
     BAND = 17 ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, HIGH);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
    break;  
  case 7:
     BAND = 15 ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, HIGH);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
    break;  
  case 8:
     BAND = 12 ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, HIGH);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
    break;  
  case 9:
     BAND = 10 ; //disable 10m
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, HIGH);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
    break;  
  case 17:
     BAND = 6 ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, HIGH);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, HIGH);  //use 6m beam instad of G5RV  ( ANT B)
    break;  
  case 18:
     BAND = 2 ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, HIGH);  // band swichting relay
     digitalWrite(FILTER_SPARE_2, HIGH);  //use 6m beam instad of G5RV  ( ANT B) for remoce HF noise 
    break;      
//  case 0:
  default:
     BAND = 0 ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);  
    break;
    }
    
//   digitalWrite(DIR_485,HIGH); 
//   Serial.print(BAND_BCD);
 //   Serial.print(" band ");
//    Serial.println(BAND);
//    delay(500);
    
   }

  
void loop()
{ 
  if (IS_KEYED == false ) { // check band decoder only if amp is RX !!!!!
     BAND_DECODER() ;}      

  
  if (BAND == 2)  // VHF whit 28Mhz transverter
//  if (BAND >= 6  )  // HF amp + 6M  6 to 160m 
   {
     digitalWrite(DIR_485,HIGH); 
     unsigned long currentMillis = millis();
     if (currentMillis - previousMillis >= interval_metering)  {    
        previousMillis = currentMillis;      
//      int sensorValue = analogRead(ANALOG_FAN_NTC);  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
//      float voltage = sensorValue * (1.1 / 1023.0);  // print out the value you read:
        Serial.print(" TEMP ");
        Serial.print(analogRead(ANALOG_FAN_NTC));
//      sensorValue = analogRead(ANALOG_PWR);  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
//      voltage = sensorValue * (1.1 / 1023.0);  // print out the value you read:
        Serial.print(" PWR ");
        Serial.print(analogRead(ANALOG_PWR)* 2); //2M
//        Serial.print(analogRead(ANALOG_PWR)); //HF
//      sensorValue = analogRead(ANALOG_SWR);  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
//      voltage = sensorValue * (1.1 / 1023.0);  // print out the value you read:
        Serial.print("W SWR ");
        Serial.print(analogRead(ANALOG_SWR));  
      
        int sensorValue = analogRead(ANALOG_24V);  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
        float voltage = sensorValue * (1.1 / 1023.0);  // print out the value you read:
        voltage = voltage * 47.00;
        Serial.print(" B+ ");
        Serial.print(voltage); 
        
        Serial.print(" BAND= ");
        Serial.println(BAND); 
        delay(20);          
       }

    if (digitalRead(PTT_IN) == HIGH  && IS_KEYED == false){  // 2m amplifier so only key on transveter whit 10m IF          
       PTT_KEY();
       digitalWrite(LED, HIGH);
      }
       
   }
  else 
   {
     digitalWrite(DIR_485,LOW);  // if band decoder not match stop TX on 485!
   }
  
 
  
  if (digitalRead(PTT_IN) == LOW  && IS_KEYED == true  ){ //  ptt is invert logic !!!  not forgot that      
     PTT_UN_KEY();
     digitalWrite(LED, LOW);
    }   

  if (analogRead(ANALOG_FAN_NTC )  < 500  ) {//~65   110 @ 5v     
     digitalWrite(FAN_OUT, HIGH);
     if (analogRead(ANALOG_FAN_NTC )  < 91 ){  //~90deg  100MV = 90 DEG   20 @ 5v
       PTT_UN_KEY();
       Serial.print ("overheat");
       while(true){  // die here LDMOS cooked !! , pass go and paid 200$ for new BLF188       
        BAND = 1;
       }
      }  
     }
    if (analogRead(ANALOG_FAN_NTC )  > 544 && IS_KEYED == false) {  // turn off fan only in RX and when under 40 deg   120 @ 5v    
      digitalWrite(FAN_OUT, LOW);
     } 

     if (analogRead(ANALOG_SWR )  > 700 ){  // 700 for 2m   
       while(digitalRead(PTT_IN) == HIGH) {       
        digitalWrite(BIAS, LOW);
        Serial.println ("SWR Prot   !!!");
        delay(250);
       }        
     }
    
}

