#define ENCODER_DO_NOT_USE_INTERRUPTS
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

Encoder readEnc1(5, 6);

int encoder_scale;
int encoder_up_count = 1;  // a positive number such as 1 0 for both of these does not divide encoder count
int encoder_down_count = -1;  // a negative number such as -1

int readEncoder_1;
int value;  
int speed;

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("begin setup");
  encoder_scale = 3;   // 1 for cheap driver  2 for gecko? 3 testing for smaller pulley vee machine
  pinMode(3, OUTPUT); // step
  pinMode(4, OUTPUT); // direction 
  pinMode(5, INPUT_PULLUP); // encoder, no pullup
  pinMode(6, INPUT_PULLUP); // encoder, no pullup    
  pinMode(13, OUTPUT);   
  digitalWriteFast(LED_BUILTIN, HIGH); 
}

void loop() {
   interrupts();
   read_encoders();  
}
void read_encoders(){ 
  encoder_scale = 3; 
    readEncoder_1 = readEnc1.read();
    if (readEncoder_1 < encoder_down_count) {  // forward or clockwise
    for (int i=0; i < encoder_up_count; i++){
      cwpulse_test();
    }    
      readEnc1.write(0);
    }   
    if (readEncoder_1 > encoder_up_count){ // backwards or counter clockwise
    for (int i=0; i < encoder_up_count; i++){
      ccwpulse_test();
    }       
      readEnc1.write(0);
    }      
} 
void cwpulse_test(void){
    digitalWriteFast(4, LOW); // pin 4 low for CW 
    delayMicroseconds(1);  //dir must preceed step by 200 ns
    for (int i=0; i < encoder_scale; i++){
      digitalWriteFast(3, HIGH); // step occurs on positive transition
      delayMicroseconds(2);      // needs to be 2, 1 is not enough for Chinese stepper might be enough for Gecko
      digitalWriteFast(3, LOW); // pin 3 remains low until next step pulse
      delayMicroseconds(1);   // needed for time to read the analog input 
    }
} 
void ccwpulse_test(void){
    digitalWriteFast(4, HIGH); // pin 4 high for CCW 
    delayMicroseconds(1);  //dir must preceed step by 200 ns
    for (int i=0; i < encoder_scale; i++){ 
      digitalWriteFast(3, HIGH); // pin 3 high
      delayMicroseconds(2);  
      digitalWriteFast(3, LOW); // pin 3 low
      delayMicroseconds(1);   // needed for time to read the analog input 
    }    
}
