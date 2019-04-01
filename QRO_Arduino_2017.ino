// ATMEHA324PU @8mhz
//#define analogReference(INTERNAL1v1);
#include <avr/wdt.h>


//Amp Type to set before compile
//#define AMP_BND_HF
#define AMP_BND_144
//#define AMP_BND_222
//#define AMP_BND_432
//#define AMP_BND_1296

int Band_value;

#define  HF 1
#define  B144 2
#define  B222 3
#define  B432 4
#define  B1296 5

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

#ifdef AMP_BND_HF  
int SWR_PROT_VAL = 150; // MAX Reverse power in ADC CONT alowable before SWR protection ,this permit to trow some power even if hight SWR    
float PWR_MIN = -41.00; // dBm value when reading 1023 on ADC  
float SWR_MIN = -41.00; // dBm value when reading 1023 on ADC   
int PWR_coupling = 60;  // sempleur coupling + board input attenuator ,set for max power  = -5dbm
int SWR_coupling = 60;  // sempleur coupling + board input attenuator ,set for max power  = -5dbm 
int BAND = HF;
#endif

#ifdef AMP_BND_144
int SWR_PROT_VAL = 150;
float PWR_MIN = -41.00;
float SWR_MIN = -41.00;    
int PWR_coupling = 60;
int SWR_coupling = 60;
int BAND = B144;
#endif

#ifdef AMP_BND_222
int SWR_PROT_VAL = 150; 
float PWR_MIN = -41.00;
float SWR_MIN = -41.00;  
int PWR_coupling = 60;
int SWR_coupling = 60;
int BAND = B222;
#endif

#ifdef AMP_BND_432
int SWR_PROT_VAL = 150; 
float PWR_MIN = -41.00;
float SWR_MIN = -41.00;  
int PWR_coupling = 60;
int SWR_coupling = 60;  
int BAND = B432;
#endif

#ifdef AMP_BND_1296
int SWR_PROT_VAL = 150;  
float PWR_MIN = -41.00;
float SWR_MIN = -41.00;  
int PWR_coupling = 60;
int SWR_coupling = 60;
int BAND = B1296;
#endif

int BAND_BCD = 0;
int RADIO_BND = 0;
float dbm = 0;
float V_SWR = 0;

int val = 0; 
boolean  IS_KEYED = false;
unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long previousMillis_tot = 0;        // will store last time LED was updated
const long interval_metering = 1000;           // interval at which to send 484 data (milliseconds)
const long interval_TOT = 300000;           // interval at which TOT (milliseconds) 5 minute = 300

unsigned long currentMillis = millis();
unsigned long currentMillis_tot = millis();


////////////////////////////////////////////////////////////

void setup()
{
 wdt_enable(WDTO_8S); 
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
  digitalWrite(LNA, HIGH);
  pinMode(XVRT_TX, OUTPUT);
  digitalWrite(XVRT_TX, LOW);
  pinMode(BIAS, OUTPUT);
  digitalWrite(BIAS, LOW);
  pinMode(not_conect, OUTPUT);
  digitalWrite(not_conect, LOW);

  pinMode(BDC_BIT_4, INPUT);
  pinMode(BDC_BIT_3, INPUT);
  pinMode(BDC_BIT_2, INPUT);
  pinMode(BDC_BIT_1, INPUT);
  pinMode(BDC_BIT_0, INPUT);
   
  pinMode(DIR_485, OUTPUT);
  digitalWrite(DIR_485, LOW);

  digitalWrite(DIR_485,HIGH); 
  Serial.println  ("V1.02 ");
  delay(500);
  digitalWrite(DIR_485,LOW); 
  
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
    IS_KEYED = false;
   }
   
//  void SWR() 
//   {
   
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
    
//   }
   
   
   
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
   
   if (digitalRead(BDC_BIT_4) == HIGH)  {bitSet(BAND_BCD, 0);}
   if (digitalRead(BDC_BIT_3) == HIGH)  {bitSet(BAND_BCD, 1);}
   if (digitalRead(BDC_BIT_2) == HIGH)  {bitSet(BAND_BCD, 2);}
   if (digitalRead(BDC_BIT_1) == HIGH)  {bitSet(BAND_BCD, 3);} 
   if (digitalRead(BDC_BIT_0) == HIGH)  {bitSet(BAND_BCD, 4);}      
       
 
switch (BAND_BCD) {
  case 1:// 160m
     RADIO_BND = HF ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
     Band_value = 160;
     break;
  case 2: //80m
     RADIO_BND = HF ;
     digitalWrite(FILTER_75_80, HIGH);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
     Band_value = 80;
    break;
  case 3://40m
     RADIO_BND = HF ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, HIGH);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
     Band_value = 40;
    break;
  case 4://30m
     RADIO_BND = HF ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, HIGH);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
     Band_value = 30;
    break; 
  case 5://20m
     RADIO_BND = HF ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, HIGH);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
     Band_value = 20;
    break;  
  case 6://17m 
     RADIO_BND = HF ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, HIGH);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
     Band_value = 17;
    break;  
  case 7://15m
     RADIO_BND = HF ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, HIGH);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
     Band_value = 15;
    break;  
  case 8://12m
     RADIO_BND = HF ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, HIGH);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
     Band_value = 12;
    break;  
  case 9://10m
     RADIO_BND = HF ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, HIGH);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW);
     Band_value = 10;
    break;  
  case 17://6m
    RADIO_BND = HF ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, HIGH);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, HIGH);  //use 6m beam instad of G5RV  ( ANT B)
     Band_value = 6;
    break;  
  case 18://144mhz
     RADIO_BND = B144 ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, HIGH);  // band swichting relay
     digitalWrite(FILTER_SPARE_2, HIGH);  //use 6m beam instad of G5RV  ( ANT B) for remoce HF noise 
     Band_value = 144;
     break;
   case 19://222mhz 
     RADIO_BND = B222 ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, HIGH);  // band swichting relay
     digitalWrite(FILTER_SPARE_2, LOW); 
     Band_value = 222;
    break;      
   case 20://432mhz 
     RADIO_BND = B432 ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, HIGH);  // band swichting relay
     digitalWrite(FILTER_SPARE_2, LOW);
     Band_value = 432; 
    break; 
   case 22://1296mhz 
     RADIO_BND = B1296 ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, HIGH);  // band swichting relay
     digitalWrite(FILTER_SPARE_2, LOW);
     Band_value = 1296; 
    break;              
//  case 0:
  default:
     RADIO_BND = 1 ;
     digitalWrite(FILTER_75_80, LOW);
     digitalWrite(FILTER_30_40, LOW);
     digitalWrite(FILTER_15_17_20, LOW);
     digitalWrite(FILTER_10_12, LOW);
     digitalWrite(FILTER_6, LOW);
     digitalWrite(FILTER_SPARE_1, LOW);
     digitalWrite(FILTER_SPARE_2, LOW); 
     {String Band_value[] = "bad";} 
    break;
    }

//   digitalWrite(DIR_485,HIGH); 
//   Serial.print(" BAND= ");
//   Serial.println(Band_value); 
//   delay(500);
    
   }
   
void monitoring ()
{
      digitalWrite(DIR_485,HIGH); 
      Serial.print(" TEMP ");
      Serial.print((1024 - analogRead(ANALOG_FAN_NTC) ) * 0.1 + 8 );
      
      Serial.print(" PWR "); 
      dbm = PWR_MIN - (analogRead(ANALOG_PWR)-1024) * 0.050;  // -41.44 = calib of 1023 adc cont 
      dbm = dbm + PWR_coupling;  // assume 63db coupling sensor (coupling+ ATT )
      double ADC_W = pow( 10.0, (dbm - 30) / 10.0); //pwr in W
      Serial.print(ADC_W); 
     
      Serial.print("W SWR ");
      dbm = SWR_MIN - (analogRead(ANALOG_SWR)-1024) * 0.050;  // -41.44 = calib of 1023 adc cont 
      dbm = dbm + SWR_coupling;  // assume 63db coupling sensor (coupling+ ATT )
      double ADC_SWR = pow( 10.0, (dbm - 30) / 10.0); //pwr in W

      // Calculate VSWR
      if (ADC_W > ADC_SWR)
       {float  V_SWR = (ADC_W + ADC_SWR) / (ADC_W - ADC_SWR); } 
      else 
       {V_SWR = 0;}
      Serial.print(ADC_SWR); 
      Serial.print(V_SWR); 
       
      int sensorValue = analogRead(ANALOG_24V);  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
      float voltage = sensorValue * (1.1 / 1023.0);  // print out the value you read:
      voltage = voltage * 47.00;
      Serial.print(" B+ ");
      Serial.print(voltage); 
        
      Serial.print(" BAND= ");
      Serial.print(Band_value); 
      if (IS_KEYED == true ){Serial.println(" TX! ");}
       else {Serial.println("");}
}  
void loop()
{  
  wdt_reset();       
  
  if (IS_KEYED == false ) { // check band decoder only if amp is RX !!!!!
     BAND_DECODER() ;} 

 //     RADIO_BND = B432 ;
 
  if (BAND == RADIO_BND  )  // HF amp  work on 6m to 160m   

   {
     currentMillis = millis();
     currentMillis_tot = millis();
     if (currentMillis - previousMillis >= interval_metering)  {  // report operation parameter every segond  
        previousMillis = currentMillis;   
        monitoring(); 
        }

////////////////////////////////// PTT activate  ////////////////////////////////////   

    if (digitalRead(PTT_IN) == HIGH  && IS_KEYED == false){  // hif hardware PTT pin hight and first time go into loop        
       PTT_KEY();                                            // put AMP in transmit mode  
       digitalWrite(LED, HIGH);                              // let it knot by light led
       }
       
   }
  else 
   {
      digitalWrite(DIR_485,LOW);  // if band decoder not match stop TX on 485!
   }


    
     if ((currentMillis_tot - previousMillis_tot >= interval_TOT) &&  IS_KEYED == true ) {  // if AMO is on traamit mode for more that 120 segond  
        previousMillis_tot = currentMillis_tot; 
        PTT_UN_KEY();                                                                       // put back in receive mode   
        Serial.print ("TOT");                                                               // report in by monitor link
        while(digitalRead(PTT_IN) == HIGH){  // time out timer trigged                      // blink led until hardware pin PTT go low again  
        digitalWrite(LED, LOW);                                                             // this also ensure it not retun in TX mode   
        delay(100);
        digitalWrite(LED, HIGH); 
        delay(100); 
        wdt_reset();  
        }
     } 

       if ( IS_KEYED == false ) { previousMillis_tot = currentMillis_tot; }  // kill TOT timer if in RX 
  
 ////////////////////////////////// PTT Relase ////////////////////////////////////   
  
  if (digitalRead(PTT_IN) == LOW  && IS_KEYED == true  ){ //  if PTT line got low put back in receive mode     
     PTT_UN_KEY();
     digitalWrite(LED, LOW);                              // and close led 
     }   

////////////////////////////////// TEMP protection + Termostatic colling ////////////////////////////////////     

  if (analogRead(ANALOG_FAN_NTC )  < 500  ) {// turn couling FAN on if temp raise above set point   
     digitalWrite(FAN_OUT, HIGH);
     if (analogRead(ANALOG_FAN_NTC )  < 91 ){  //if raise above ~90 deg go in Receive mode 
       PTT_UN_KEY();
       Serial.print ("overheat");
       while(true){  // die here LDMOS cooked !! , pass go and paid 200$ for new BLF188       
       {char BAND[] = "" ; 
        wdt_reset();
       }
      }  
     }
    } 
    if (analogRead(ANALOG_FAN_NTC )  > 544 && IS_KEYED == false) {  // turn off fan only in RX and when under ~50 deg   120 @ 5v    
      digitalWrite(FAN_OUT, LOW);
     } 

     
//////////////////////////////////  SWR  protection code ////////////////////////////////////   

     if (analogRead(ANALOG_SWR)  > SWR_PROT_VAL ){  // if reflected power is over limit  
       while(digitalRead(PTT_IN) == HIGH) {       
        digitalWrite(BIAS, LOW);                     // put Bias off (no more ouput power on amp )
        Serial.println ("SWR Prot   !!!");
        delay(250);
        wdt_reset();
       }        
     }        
}
