#define IMXRT_GPIO6_DIRECT  (*(volatile uint32_t *)0x42000000) //register address of GPIO6 is 0x4200 0000
#define IO_BLOCK_A ((IMXRT_GPIO6_DIRECT & 0xFFFF0000)>>16) //data block A given by end 16 bits of GPIO6
#define NOP10 "nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t"   //delay macro. Timings for teensy 4.1.
#define PAUSECV __asm__(NOP10 NOP10 NOP10 NOP10)                  //pauses for roughly 30ns for convSt pulses
#define PAUSERD __asm__(NOP10 NOP10 NOP10 NOP10 NOP10 NOP10)        //pauses roughly 40ns for RD pulses
#define PAUSERDH __asm__(NOP10 NOP10 NOP10)                           //pauses roughly 20ns for RD high

#define pinCV     24
#define pinRD     25
#define pinBUSY   35
#define pinCS     34
#define pinRST    36

void convDone(); //ISR detecting falling edge of BUSY, indicating that conversion is complete. Triggers next conversion. 
                 //track/hold switches to HOLD on rising edge of CONV, timing should be done at end of this ISR
//void startRead(); //ISR detecting rising edge of BUSY, tells teensy 4.1 to start reading
short readOne(); //Single read, returns read-in value
volatile int convD=0, startR=0; //flags for convDone and startRead
volatile short data[8] = {0,0,0,0,0,0,0,0};
int i=0;

void setup() {
  pinMode(19, INPUT);
  pinMode(18, INPUT);
  pinMode(14, INPUT);
  pinMode(15, INPUT);
  pinMode(40, INPUT);
  pinMode(41, INPUT);
  pinMode(17, INPUT);
  pinMode(16, INPUT);
  
  pinMode(22, INPUT);
  pinMode(23, INPUT);
  pinMode(20, INPUT);
  pinMode(21, INPUT);
  pinMode(38, INPUT);
  pinMode(39, INPUT);
  pinMode(26, INPUT);
  pinMode(27, INPUT); //These are the GPIO6 pins being used, in the above order

  pinMode(pinCV, OUTPUT); //ConvStart
  pinMode(pinRD, OUTPUT); //~RD
  pinMode(pinRST, OUTPUT); //RESET
  pinMode(pinBUSY, INPUT); //Busy - attach ISR here
  pinMode(pinCS, OUTPUT); //~CS tie low to enable. For multiple chips, delay 40ns after CS low before read.

  digitalWrite(pinCV, 1); //CV default high, rising edge triggers conversions
  digitalWrite(pinRD, 1); //~RD default high, falling edge triggers data output, wait 40ns after falling edge to measure

  digitalWrite(pinCS, 0);

  Serial.begin(115200);

  attachInterrupt(digitalPinToInterrupt(pinBUSY),convDone,FALLING);
//  attachInterrupt(digitalPinToInterrupt(pinBUSY),startRead,RISING);
  
}

void loop() {
  if(!digitalRead(pinBUSY)){  //if not busy, either conv finished or not started
    if(convD){                //conv flag means conv finished. Store data in array.
      Serial.println("Conversion complete!");
      for(i=0;i<8;i++){
        data[i] = readOne();
        Serial.println(data[i]);
      }
      convD = 0;
    }
    //Start new conversion
    digitalWriteFast(pinCV, 0);
    PAUSECV;
    digitalWriteFast(pinCV, 1);
  }
  delay(1000);
}

short readOne(){ //Single read
  short datum;
  digitalWriteFast(pinRD, 0);
  PAUSERD;
  datum = IO_BLOCK_A;
  digitalWriteFast(pinRD, 1);
  PAUSERDH;
  return datum;
}

void convDone(){    //ISR detecting falling edge of BUSY, indicating that conversion is complete.
  convD = 1;
}
/*void startRead(){//ISR detecting rising edge of BUSY, tells teensy 4.1 to start reading
  digitalWriteFast(pinRD, 0);
  PAUSERD;
  data[0] = IO_BLOCK_A;
  digitalWriteFast(pinRD, 1);
  PAUSERDH;
  digitalWriteFast(pinRD, 0);
  PAUSERD;
  data[1] = IO_BLOCK_A;
  digitalWriteFast(pinRD, 1);
  PAUSERDH;
  digitalWriteFast(pinRD, 0);
  PAUSERD;
  data[2] = IO_BLOCK_A;
  digitalWriteFast(pinRD, 1);
  PAUSERDH;
  digitalWriteFast(pinRD, 0);
  PAUSERD;
  data[3] = IO_BLOCK_A;
  digitalWriteFast(pinRD, 1);
  PAUSERDH;
  digitalWriteFast(pinRD, 0);
  PAUSERD;
  data[4] = IO_BLOCK_A;
  digitalWriteFast(pinRD, 1);
  PAUSERDH;
  digitalWriteFast(pinRD, 0);
  PAUSERD;
  data[5] = IO_BLOCK_A;
  digitalWriteFast(pinRD, 1);
  PAUSERDH;
  digitalWriteFast(pinRD, 0);
  PAUSERD;
  data[6] = IO_BLOCK_A;
  digitalWriteFast(pinRD, 1);
  PAUSERDH;
  digitalWriteFast(pinRD, 0);
  PAUSERD;
  data[7] = IO_BLOCK_A;
  digitalWriteFast(pinRD, 1);
}*/
