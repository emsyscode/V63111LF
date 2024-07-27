/****************************************************/
/* This is only one example of code structure       */
/* OFFCOURSE this code can be optimized, but        */
/* the idea is let it so simple to be easy catch    */
/* where we can do changes and look to the results  */
/****************************************************/


#define VFD_dataIn 8// If 0 write LCD, if 1 read of LCD
#define VFD_clk 9 // if 0 is a command, if 1 is a data0
#define VFD_stb 10 // Must be pulsed to LCD fetch data of bus

uint8_t myByte= 0x01;   // this variable is only related with swapLed1.
unsigned char secs;
unsigned char number;
bool vfdOn = true;

unsigned int segments[] ={
  //This not respect the normal table for 7segm like "abcdefgh"  //
  // ggfb/\|a         d\/ec             //The central segment have 2, by this reason use 2 g! 
   0b00110001,   0b00010011,  0b00000000, //0  // 
   0b00010000,   0b00000001,  0b00000000, //1  // 
   0b11010001,   0b00010010,  0b00000000, //2  // 
   0b11010001,   0b01010001,  0b00000000, //3  // 
   0b11110000,   0b00000001,  0b00000000, //4  // 
   0b11100001,   0b01010001,  0b00000000, //5  // 
   0b11100001,   0b01010011,  0b00000000, //6  // 
   0b00010001,   0b00000001,  0b00000000, //7  // 
   0b11110001,   0b01010011,  0b00000000, //8  // 
   0b11110001,   0b01010011,  0b00000000, //9  // 
   0b00000000,   0b00000000,  0b00000000  //10 // empty display
  };
unsigned int segmAlpha[] ={
  //This not respect the normal table for 7segm like "abcdefgh"  // 
  // ggfb/\|a         d\/ec             //The central segment have 2, by this reason use 2 g!
  0b11100001,   0b00010001,   0b00000000, //Digit 7
  0b10101000,   0b00001010,   0b00000000, //Digit 6
  0b00100000,   0b00010010,   0b00000000, //Digit 5
  0b00110001,   0b00010011,   0b00000000, //Digit 4
  0b11100001,   0b01000010,   0b00000000, //Digit 3
  0b00000000,   0b00000000,   0b00000000, //Digit 2
  0b00000011,   0b01010000,   0b00000000, //Digit 1
  0b11110000,   0b00000011,   0b00000000, //Digit 0
  0b00000000,   0b00000000,   0b00000000  //   // empty display
  };
/************************** Initialize of driver V63111***************************************/
void V63111_init(void){
    delayMicroseconds(250); //power_up delay
    // Note: Allways the first byte in the input data after the STB go to LOW is interpret as command!!!

    // Configure VFD display (grids)
    cmd_with_stb(0b00000000);//  (0b01000000)    cmd1 8 grids 20 segm in 63111 // (0b00001001) ten digits!
    delayMicroseconds(1);
    // turn vfd on, stop key scannig
    cmd_with_stb(0b10001000);//(BIN(01100110)); 
    delayMicroseconds(1);
    // Write to memory display, increment address, normal operation
    cmd_with_stb(0b01000000);//(BIN(01000000));
    delayMicroseconds(1);
    // Address 00H - 15H ( total of 11*2Bytes=176 Bits)
    cmd_with_stb(0b11000000);//(BIN(01100110)); 
    delayMicroseconds(1);
    // set DIMM/PWM to value
    cmd_with_stb((0b10001000) | 7);//0 min - 7 max  )(0b01010000)
    delayMicroseconds(1);
}
/********************** Send a command with the strobe/chip select only to 4 bits*******************************************/
void cmd_4bitsWithout_stb(unsigned char a){
  // send without stb
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  
  data=a;
  //This don't send the strobe signal, to be used in burst data send
   for (mask = 0b00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_dataIn, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_dataIn, LOW);
     }
    delayMicroseconds(5);
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(5);
   }
   //digitalWrite(VFD_clk, LOW);
}
/*********************** Send command without strobe/chip select******************************************/
void cmd_without_stb(unsigned char a){
  // send without stb
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  
  data=a;
  //This don't send the strobe signal, to be used in burst data send
   for (mask = 0b00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_dataIn, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_dataIn, LOW);
     }
    delayMicroseconds(5);
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(5);
   }
   //digitalWrite(VFD_clk, LOW);
}
/************************ Send a command with the strobe/chip select only to 4 bits *****************************/
void cmd_4bitsWith_stb(unsigned char a){
  // send with stb
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  
  data=a;
  
  //This send the strobe signal
  //Note: The first byte input at in after the STB go LOW is interpreted as a command!!!
  digitalWrite(VFD_stb, LOW);
  delayMicroseconds(1);
   for (mask = 0b00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     delayMicroseconds(1);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_dataIn, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_dataIn, LOW);
     }
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(1);
   }
   digitalWrite(VFD_stb, HIGH);
   delayMicroseconds(1);
}
/********************** Send a command with the strobe/chip select *******************************************/
void cmd_with_stb(unsigned char a){
  // send with stb
  unsigned char data = 0x00; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  
  data=a;
  
  //This send the strobe signal
  //Note: The first byte input at in after the STB go LOW is interpreted as a command!!!
  digitalWrite(VFD_stb, LOW);
  delayMicroseconds(1);
   for (mask = 0b00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     delayMicroseconds(1);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_dataIn, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_dataIn, LOW);
     }
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(1);
   }
   digitalWrite(VFD_stb, HIGH);
   delayMicroseconds(1);
}
/*********************** Run a test to VFD, let it all bright******************************************/
void test_VFD(void){
    clear_VFD();   
      digitalWrite(VFD_stb, LOW);
      delayMicroseconds(1);
      cmd_with_stb(0b00000000); // cmd 1 // 8 Grids & 20 Segments
      cmd_with_stb(0b01000000); // cmd 2 //Write VFD, Normal operation; Set pulse as 1/16, Auto increment
      cmd_with_stb(0b10001000 | 0x07); // cmd 2 //set on, dimmer to max
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(1);
        cmd_without_stb((0b11000000)); //cmd 3 wich define the start address (00H to 15H)
        // Only here must change the bit to test, first 2 bytes and 1/2 byte of third.
        // for (int i = 0; i < 8 ; i++){ // test base to 16 segm and 5 grids
          // Zone of test, if write 1 on any position of 3 bytes below position, will bright segment corresponding it.
         cmd_without_stb(0b11111111); // Data to fill table 5*16 = 80 bits
         cmd_without_stb(0b11111111); // Data to fill table 5*16 = 80 bits
         cmd_without_stb(0b00000000); // Data to fill table 5*16 = 80 bits
         //}
      //cmd_without_stb(0b00000001); // cmd1 Here I define the 5 grids and 16 Segments
      //cmd_with_stb((0b10001000) | 7); //cmd 4
      digitalWrite(VFD_stb, HIGH);
      delay(3); 
}
void msgHiFolks(void){
    clear_VFD();   
      digitalWrite(VFD_stb, LOW);
      delayMicroseconds(1);
      cmd_with_stb(0b00000000); // cmd 1 // 8 Grids & 20 Segments
      cmd_with_stb(0b01000000); // cmd 2 //Write VFD, Normal operation; Set pulse as 1/16, Auto increment
      cmd_with_stb(0b10001000 | 0x07); // cmd 2 //set on, dimmer to max
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(1);
        cmd_without_stb((0b11000000)); //cmd 3 wich define the start address (00H to 15H)
        // Only here must change the bit to test, first 2 bytes and 1/2 byte of third.
          // Zone of test, if write 1 on any position of 3 bytes below position, will bright segment corresponding it.
          cmd_without_stb(0b11100001); cmd_without_stb(0b00010001); cmd_without_stb(0b00000000); //Digit 7
          cmd_without_stb(0b10101000); cmd_without_stb(0b00001010); cmd_without_stb(0b00000000); //Digit 6
          cmd_without_stb(0b00100000); cmd_without_stb(0b00010010); cmd_without_stb(0b00000000); //Digit 5
          cmd_without_stb(0b00110001); cmd_without_stb(0b00010011); cmd_without_stb(0b00000000); //Digit 4
          cmd_without_stb(0b11100001); cmd_without_stb(0b01000010); cmd_without_stb(0b00000000); //Digit 3
          cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); //Digit 2
          cmd_without_stb(0b00000011); cmd_without_stb(0b01010000); cmd_without_stb(0b00000000); //Digit 1
          cmd_without_stb(0b11110000); cmd_without_stb(0b00000011); cmd_without_stb(0b00000000); //Digit 0
      //cmd_without_stb(0b00000001); // cmd1 Here I define the 5 grids and 16 Segments
      //cmd_with_stb((0b10001000) | 7); //cmd 4
      digitalWrite(VFD_stb, HIGH);
      delay(3); 
}
/*********************** Clear of VFD display ******************************************/
void clear_VFD(void){
  /*
  Here I clean all registers 
  Could be done only on the number of grid
  to be more fast. The 12 * 3 bytes = 36 registers
  */
      for (int n=0; n < 24; n++){ //  // Note: The char is constituted by 2 & 1/2 bytes, by this reason count 3 bytes by char!!!
        cmd_with_stb(0b00000000); //       cmd 1 // 10 Grids & 18 Segments
        cmd_with_stb(0b01000000); //       cmd 2 //Normal operation; Set pulse as 1/16
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(1);
            cmd_without_stb((0b11000000) | n); // cmd 3 //wich define the start address (00H to 15H)
            cmd_without_stb(0b00000000); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_without_stb(0b00000000); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_without_stb(0b00000000); // only half byte of third byte.
            //
            //cmd_with_stb((0b10001000) | 7); //cmd 4
            digitalWrite(VFD_stb, HIGH);
            delayMicroseconds(1);
     }
}
void fillAll_VFD(void){
  /*
  Here I clean all registers 
  Could be done only on the number of grid
  to be more fast. The 12 * 3 bytes = 36 registers
  */
      for (int n=0; n < 24; n++){ //  // Note: The char is constituted by 2&1/2 bytes, by this reason count 3 bytes by char!!!
        cmd_with_stb(0b00000000); //       cmd 1 // 10 Grids & 18 Segments
        cmd_with_stb(0b01000000); //       cmd 2 //Normal operation; Set pulse as 1/16
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(1);
            cmd_without_stb((0b11000000) | n); // cmd 3 //wich define the start address (00H to 15H)
            cmd_without_stb(0b11111111); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_without_stb(0b11111111); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_without_stb(0b11111111); // only half byte of third byte.
            //
            cmd_with_stb((0b10001000) | 7); //cmd 4
            digitalWrite(VFD_stb, HIGH);
            delayMicroseconds(1);
     }
}
/********************** To adapt the 7 seg if use VFD with different 7 seg digits *****************************/
void DigitTo7SegEncoder( unsigned char digit){
  switch(digit){
    case 0:   number=0;     break;  // if remove the LongX, need put here the segments[x]
    case 1:   number=1;     break;
    case 2:   number=2;     break;
    case 3:   number=3;     break;
    case 4:   number=4;     break;
    case 5:   number=5;     break;
    case 6:   number=6;     break;
    case 7:   number=7;     break;
    case 8:   number=8;     break;
    case 9:   number=9;     break;
  }
} 
void send14segmNum(unsigned int num){
      cmd_with_stb(0b00001001); // cmd 1 // 10 Grids & 18 Segments
      cmd_with_stb(0b01000000); // cmd 2 //Normal operation; Set pulse as 1/16
        //
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(1);
        cmd_without_stb((0b11000000) | (num)); //cmd 3 wich define the start address (00H)
        //
          cmd_without_stb(segments[num]); // 
          cmd_without_stb(segments[num+1]); // minuts units
          cmd_4bitsWithout_stb(segments[num+2]); // minuts dozens
      digitalWrite(VFD_stb, HIGH);
      cmd_with_stb((0b10001000) | 7); //cmd 4
      delay(1);
      delay(50);  
}
void send14segmAlpha(unsigned int num){
      cmd_with_stb(0b00001001); // cmd 1 // 10 Grids & 18 Segments
      cmd_with_stb(0b01000000); // cmd 2 //Normal operation; Set pulse as 1/16
        //
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(1);
        cmd_without_stb((0b11000000) | (num)); //cmd 3 wich define the start address (00H)
        //
          cmd_without_stb(segmAlpha[num]); // 
          cmd_without_stb(segmAlpha[num+1]); // minuts units
          cmd_4bitsWithout_stb(segmAlpha[num+2]); // minuts dozens
      digitalWrite(VFD_stb, HIGH);
      cmd_with_stb((0b10001000) | 7); //cmd 4
      delay(1);
      delay(50);  
}
/************************* Swap the led Function ***************************************/
void swapLeds(){
    digitalWrite(VFD_stb, LOW);
    delay(2);
    cmd_without_stb(0B01000000); // This is to define write to LED's
    delay(2);
    myByte ^=(0b00011111);  //Here invert bit of led 0,3 & 4, repeat this function if you want use other of 5 remaining leds. //3 led meio
    //myByte=(0B00000000);
    cmd_without_stb(myByte);
    delay(3);
    digitalWrite(VFD_stb, HIGH);
    delay(2);
}
void onOff(bool on){
    digitalWrite(VFD_stb, LOW);
    delayMicroseconds(20);
    if (!on){
    cmd_without_stb(0b10000000); // This swithc Off display!!!
    delayMicroseconds(20);
    }else{
      cmd_without_stb(0b10001111); // This swithc On the VFD and define the Dimming Quantity Settings 
    delayMicroseconds(20);
    }
    digitalWrite(VFD_stb, HIGH);
    delayMicroseconds(20);
}
void dimming(unsigned int dimmer){
  digitalWrite(VFD_stb, LOW);
    delayMicroseconds(20);
    cmd_without_stb((0b10001000) | dimmer); // This swithc On the VFD and define the Dimming Quantity Settings 
    delayMicroseconds(20);
    digitalWrite(VFD_stb, HIGH);
    delayMicroseconds(20);
}
/************************ Setup Zone******************************************/
void setup() {
// This setup is defined to work with interrupts in case evoluate to a clock!!!
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(VFD_dataIn, OUTPUT);
  pinMode(VFD_clk, OUTPUT);
  pinMode(VFD_stb, OUTPUT);
  //pinMode(VFD_dataOut, INPUT_PULLUP);
  Serial.begin(115200);
 
V63111_init();

test_VFD();

clear_VFD();

}
/********************** Loop Zone *********************************/
void loop() {
  unsigned int arrayPosition=0;
    vfdOn=!vfdOn;
    Serial.println(vfdOn, BIN);
    //onOff(vfdOn);
     fillAll_VFD();
     delay(750);
     swapLeds();
     delay(1000);
    test_VFD();
    //msgHiFolks();
    //  delay(1500);
  
       for (int n=0; n < 11; n++){  // Write number by forward sequency
          arrayPosition=n*3; //
          send14segmNum(arrayPosition);
          delay(50);
          }
    clear_VFD();
   
        for (int n=11; n >= 0; n--){ // Write the number's by backward sequency
          arrayPosition=n*3;
          send14segmNum(arrayPosition);
          delay(50);
          }
          
        for (int n=7; n >= 0; n--){  // Adjusting the bright of VFD 
         dimming(n);
         delay(750);
        }
        
      clear_VFD();
   
        for (int n=0; n < 11; n++){  // Write message "HI FOLKS"
         arrayPosition=n*3;
         send14segmAlpha(arrayPosition);
         delay(50);
        }
        
        for (int i=0; i<6; i++){    // Switch on/off of VFD
          onOff(vfdOn);
          delay(500);
          vfdOn=!vfdOn;
          delay(500);
        }
}

