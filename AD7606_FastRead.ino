#define IMXRT_GPIO6_DIRECT  (*(volatile uint32_t *)0x42000000) //register address of GPIO6 is 0x4200 0000
#define IO_BLOCK_A ((IMXRT_GPIO6_DIRECT & 0xFFFF0000)>>16) //data block A given by end 16 bits of GPIO6
#define NOP10 "nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t"   //delay macro. Timings for teensy 4.1.
#define PAUSECV __asm__(NOP10 NOP10 NOP10 NOP10)                  //pauses for roughly 30ns for convSt pulses
#define PAUSERD __asm__(NOP10 NOP10 NOP10 NOP10 NOP10 NOP10)        //pauses roughly 40ns for RD pulses
#define PAUSERDH __asm__(NOP10 NOP10 NOP10)                           //pauses roughly 20ns for RD high

#define pinCV     24
#define pinRD     25
#define pinBUSY   35
#define pinBUSY2  37
#define pinCS     34
#define pinRST    36

#define numSamples 200000

void convDone(); //ISR detecting falling edge of BUSY, indicating that conversion is complete. Triggers next conversion. 
                 //track/hold switches to HOLD on rising edge of CONV, timing should be done at end of this ISR
//void startRead(); //ISR detecting rising edge of BUSY, tells teensy 4.1 to start reading
void startConv(); //Function that toggles pinCV to start conversion
short readOne(); //Single read, returns read-in value
volatile int convD=1, readD=0; //flags for convDone and readDone
volatile short data[8] = {0,0,0,0,0,0,0,0};
EXTMEM unsigned long timeBuffer[numSamples];    //Stores time data in microseconds
EXTMEM short bigBuffer[numSamples];             //Teensy used for testing had additional 16MB of PSRAM. 
unsigned long timeTaken=0, currentTime = 0;
int i=0, counter=0, countFlag = 1;

elapsedMillis sysTime = 0;
elapsedMicros sampleTime = 0;

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

  pinMode(pinCV, OUTPUT); //ConvStart, both pins tied together
  pinMode(pinRD, OUTPUT); //~RD
  pinMode(pinRST, OUTPUT); //RESET
  pinMode(pinBUSY, INPUT); //Busy - attach ISR here
  pinMode(pinCS, OUTPUT); //~CS tie low to enable. For multiple chips, delay 40ns after CS low before read.

  digitalWrite(pinCV, 1); //CV default high, rising edge triggers conversions
  digitalWrite(pinRD, 1); //~RD default high, falling edge triggers data output, wait 40ns after falling edge to measure

  digitalWrite(pinCS, 0);

  Serial.begin(115200);
  for(i=0;i<100000;i++){
    bigBuffer[i] = 0;
    timeBuffer[i] = 0;
  }
  attachInterrupt(digitalPinToInterrupt(pinBUSY2),startRead,RISING);
  startConv();                                                          //Starts conversion 
  sampleTime = 0;
  attachInterrupt(digitalPinToInterrupt(pinBUSY),convDone,FALLING);
  Serial.println("Setup complete");
}

void loop() {
  if(convD && countFlag){                  //If conversion done flag is up, set next conversion ASAP
    startConv();
    timeBuffer[counter+1] = sampleTime;
    convD = 0;
    if(counter+1 == numSamples){
      countFlag = 0;
    }
  }
  if(readD){
    bigBuffer[counter++] = data[0];
    readD = 0;
    //Serial.println(counter);
  }

  if(counter == numSamples){
    timeTaken = sampleTime;
    currentTime = sysTime;
    for(i=0;i<numSamples;i++){
      Serial.print(timeBuffer[i]);
      Serial.print("\t");
      Serial.println(bigBuffer[i]);
    }
    Serial.print("Time: ");
    Serial.print(currentTime/1000.0);
    Serial.print(". Time taken: ");
    Serial.println(timeTaken);
    counter = 0;
    countFlag = 1;
    sampleTime = 0;
  }
  if(!digitalRead(pinBUSY) && !convD){
    Serial.println("Skipped");
    Serial.println(sysTime);
    counter = 0;
    countFlag = 1;
    startConv();
    sampleTime = 0;
  }
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

void startConv(){
  digitalWriteFast(pinCV, 0);
  PAUSECV;
  digitalWriteFast(pinCV, 1);
  PAUSECV;
}

void convDone(){    //ISR detecting falling edge of BUSY, indicating that conversion is complete.
  convD = 1;
}
void startRead(){//ISR detecting rising edge of BUSY, tells teensy 4.1 to start reading
  for(i=0;i<8;i++){
    data[i] = readOne();
  }
  readD=1;
}
