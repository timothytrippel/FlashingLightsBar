//defines for ledcontrol
#define SEC_PER_CYCLE 6
#define MS_PER_PERIOD 1000
#define ALLON 150
#define ALLOFF 10
#define STAGE1 20
#define STAGE2 30
#define STAGE3 40
#define STAGE4 50
#define STAGE5 60
#define STAGE6 70
#define STAGE7 80
#define STAGE8 90
#define STAGE9 100
#define STAGE10 105
#define CHANGE_THRESH 300
#define CHANGE_OFFSET_THRESH 5

//------------------------------------------------------------------------------------------------------------
//Global Variables
//Pin Declarations
//FAR LEFT PWM PINS
int farLeft_BLUE  = 2;
int farLeft_GREEN = 3;
int farLeft_RED   = 4;
//MID LEFT PWM PINS
int midLeft_BLUE  = 5;
int midLeft_GREEN = 6;
int midLeft_RED   = 7;
//MID RIGHT PWM PINS
int midRight_BLUE  = 8;
int midRight_GREEN = 9;
int midRight_RED   = 10;
//FAR RIGHT PWM PINS
int farRight_BLUE  = 11;
int farRight_GREEN = 12;
int farRight_RED   = 13;
//White-LEDS
int white_1 = 46;
int white_2 = 44;
//Audio Input Pin
int audioIn = ;
//Cycle-Pushbutton Pin
int cyc_pushbutton = 60;

//Other Global Variables
//Color Scheme 1 Global Values
int colorMode = 2;       //Initial Color Mode on Boot
int audioAmplitude = 0;  //Stores sampled value of audio signal (0 to 1024)
int brightnessFactor = 0; //Stores value added to light intensities
int cyclePeriod = 0;
int counterColor = 0;
//Color Scheme 2 Global Variables
int RED_Val = 0;
int GREEN_Val = 0;
int BLUE_Val = 0;
int colorIndex = 0;
int audioAmplitudePast = 0;  //Stores sampled value of audio signal (0 to 1024)
//0-RED,1-Orange,2-Yellow,3-Yellow/Green,4-Green,5-Green/Teal,6-Teal,7-LightBlue,8-Blue,9-Violet,10-LightPurple,11-Magenta
int colorArray_RED[12]   = {255, 255, 255, 128, 0, 0, 0, 0, 0, 127, 255, 255};
int colorArray_GREEN[12] = {0, 128, 255, 255, 255, 255, 255, 128, 0, 0, 0, 0};
int colorArray_BLUE[12]  = {0, 0, 0, 0, 0, 128, 255, 255, 255, 255, 255, 127};

//Function Prototype
void UpdateLEDs_Scheme_1(void);
void SelectBrightness(void);
void SelectColor(void);
void UpdateLEDs_Scheme_2(void);

//------------------------------------------------------------------------------------------------------------
//Initializations Code
void setup() {
  //Declare all PWM pins as outputs
  //Far-Left PWM Pins
  pinMode(farLeft_BLUE, OUTPUT);
  pinMode(farLeft_GREEN, OUTPUT);
  pinMode(farLeft_RED, OUTPUT);
  //Mid-Left PWM Pins
  pinMode(midLeft_BLUE, OUTPUT);
  pinMode(midLeft_GREEN, OUTPUT);
  pinMode(midLeft_RED, OUTPUT);
  //Mid-Right PWM Pins
  pinMode(midRight_BLUE, OUTPUT);
  pinMode(midRight_GREEN, OUTPUT);
  pinMode(midRight_RED, OUTPUT);
  //Far-Right PWM Pins
  pinMode(farRight_BLUE, OUTPUT);
  pinMode(farRight_GREEN, OUTPUT);
  pinMode(farRight_RED, OUTPUT); 
  //White-LEDs
  pinMode(white_1, OUTPUT);
  pinMode(white_2, OUTPUT);

  //Declare analog audio input pin
  pinMode(audioIn, INPUT);

  //Declare pushbutton digital input pin (sense contact closure to ground)
  pinMode(cyc_pushbutton, INPUT);
}

//------------------------------------------------------------------------------------------------------------
void loop() {
  analogWrite(white_1, 255);
  analogWrite(white_2, 255);
  if(colorMode == 1){
    if((cyclePeriod % 2) == 0 ){
      counterColor++;
      if(counterColor == ALLON){
        cyclePeriod++; //increment cylce period
      }    
    }
    else{
      counterColor--;
      if((cyclePeriod == 5) && (counterColor == ALLOFF)){
        cyclePeriod = 0;
      }
      else if(counterColor == ALLOFF){
        cyclePeriod++;
      }
    }
    audioAmplitude = analogRead(audioIn);
    SelectBrightness();  
    UpdateLEDs_Scheme_1();
    delayMicroseconds(15000); // pauses for 15 millisecond 
  }
  else if(colorMode == 2){
    audioAmplitudePast = audioAmplitude;
    audioAmplitude = analogRead(audioIn);
    SelectColor();
    UpdateLEDs_Scheme_2();
    delayMicroseconds(25000); // pauses for 25 millisecond 
  }
}

//------------------------------------------------------------------------------------------------------------
//Color Scheme 1 - Functions
void SelectBrightness(void){
  if (audioAmplitude < 100){
    brightnessFactor = STAGE1;
  }
  else if((audioAmplitude >= 200) && (audioAmplitude < 300)){
    brightnessFactor = STAGE2;
  }
  else if((audioAmplitude >= 300) && (audioAmplitude < 400)){
    brightnessFactor = STAGE3;
  }
  else if((audioAmplitude >= 400) && (audioAmplitude < 500)){
    brightnessFactor = STAGE4;
  }
  else if((audioAmplitude >= 500) && (audioAmplitude < 600)){
    brightnessFactor = STAGE5;
  }
  else if((audioAmplitude >= 600) && (audioAmplitude < 700)){
    brightnessFactor = STAGE6;
  }
  else if((audioAmplitude >= 700) && (audioAmplitude < 800)){
    brightnessFactor = STAGE7;
  }
  else if((audioAmplitude >= 800) && (audioAmplitude < 900)){
    brightnessFactor = STAGE8;
  }
  else if((audioAmplitude >= 900) && (audioAmplitude < 1000)){
    brightnessFactor = STAGE9;
  }  
  else{
    brightnessFactor = STAGE10;
  }
  return;
}

void UpdateLEDs_Scheme_1(void) {
  //blue -> blue/green -> green -> green/red -> red -> red/blue -> blue
  switch (cyclePeriod)
  {
    //BLUE->BLUE/GREEN->GREEN->GREEN/RED->RED->RED/BLUE->BLUE
  case 0:
    //BLUE, count up GREEN
    analogWrite(farLeft_BLUE, ALLON + brightnessFactor);
    analogWrite(midLeft_BLUE, ALLON + brightnessFactor);
    analogWrite(midRight_BLUE, ALLON + brightnessFactor);
    analogWrite(farRight_BLUE, ALLON + brightnessFactor);
    analogWrite(farLeft_GREEN, counterColor + brightnessFactor);
    analogWrite(midLeft_GREEN, counterColor + brightnessFactor);
    analogWrite(midRight_GREEN, counterColor + brightnessFactor);
    analogWrite(farRight_GREEN, counterColor + brightnessFactor);
    //RED off
    analogWrite(farLeft_RED, ALLOFF);
    analogWrite(midLeft_RED, ALLOFF);
    analogWrite(midRight_RED, ALLOFF);
    analogWrite(farRight_RED, ALLOFF);
    break;
  case 1:
    //GREEN, count down BLUE
    analogWrite(farLeft_GREEN, ALLON + brightnessFactor);
    analogWrite(midLeft_GREEN, ALLON + brightnessFactor);
    analogWrite(midRight_GREEN, ALLON + brightnessFactor);
    analogWrite(farRight_GREEN, ALLON + brightnessFactor);
    analogWrite(farLeft_BLUE, counterColor + brightnessFactor);
    analogWrite(midLeft_BLUE, counterColor + brightnessFactor);
    analogWrite(midRight_BLUE, counterColor + brightnessFactor);
    analogWrite(farRight_BLUE, counterColor + brightnessFactor);
    //RED off
    analogWrite(farLeft_RED, ALLOFF);
    analogWrite(midLeft_RED, ALLOFF);
    analogWrite(midRight_RED, ALLOFF);
    analogWrite(farRight_RED, ALLOFF);
    break;
  case 2:
    //GREEN, count up RED
    analogWrite(farLeft_GREEN, ALLON + brightnessFactor);
    analogWrite(midLeft_GREEN, ALLON + brightnessFactor);
    analogWrite(midRight_GREEN, ALLON + brightnessFactor);
    analogWrite(farRight_GREEN, ALLON + brightnessFactor);
    analogWrite(farLeft_RED, counterColor + brightnessFactor);
    analogWrite(midLeft_RED, counterColor + brightnessFactor);
    analogWrite(midRight_RED, counterColor + brightnessFactor);
    analogWrite(farRight_RED, counterColor + brightnessFactor);
    //BLUE off
    analogWrite(farLeft_BLUE, ALLOFF);
    analogWrite(midLeft_BLUE, ALLOFF);
    analogWrite(midRight_BLUE, ALLOFF);
    analogWrite(farRight_BLUE, ALLOFF);      
    break;
  case 3:
    //RED, count down GREEN
    analogWrite(farLeft_RED, ALLON + brightnessFactor);
    analogWrite(midLeft_RED, ALLON + brightnessFactor);
    analogWrite(midRight_RED, ALLON + brightnessFactor);
    analogWrite(farRight_RED, ALLON + brightnessFactor);
    analogWrite(farLeft_GREEN, counterColor + brightnessFactor);
    analogWrite(midLeft_GREEN, counterColor + brightnessFactor);
    analogWrite(midRight_GREEN, counterColor + brightnessFactor);
    analogWrite(farRight_GREEN, counterColor + brightnessFactor);
    //BLUE off
    analogWrite(farLeft_BLUE, ALLOFF);
    analogWrite(midLeft_BLUE, ALLOFF);
    analogWrite(midRight_BLUE, ALLOFF);
    analogWrite(farRight_BLUE, ALLOFF);       
    break;
  case 4:
    //RED, count up BLUE
    analogWrite(farLeft_RED, ALLON + brightnessFactor);
    analogWrite(midLeft_RED, ALLON + brightnessFactor);
    analogWrite(midRight_RED, ALLON + brightnessFactor);
    analogWrite(farRight_RED, ALLON + brightnessFactor);
    analogWrite(farLeft_BLUE, counterColor + brightnessFactor);
    analogWrite(midLeft_BLUE, counterColor + brightnessFactor);
    analogWrite(midRight_BLUE, counterColor + brightnessFactor);
    analogWrite(farRight_BLUE, counterColor + brightnessFactor);
    //GREEN off
    analogWrite(farLeft_GREEN, ALLOFF);
    analogWrite(midLeft_GREEN, ALLOFF);
    analogWrite(midRight_GREEN, ALLOFF);
    analogWrite(farRight_GREEN, ALLOFF);
    break;
  case 5:
    //BLUE, count down RED
    analogWrite(farLeft_BLUE, ALLON + brightnessFactor);
    analogWrite(midLeft_BLUE, ALLON + brightnessFactor);
    analogWrite(midRight_BLUE, ALLON + brightnessFactor);
    analogWrite(farRight_BLUE, ALLON + brightnessFactor);
    analogWrite(farLeft_RED, counterColor + brightnessFactor);
    analogWrite(midLeft_RED, counterColor + brightnessFactor);
    analogWrite(midRight_RED, counterColor + brightnessFactor);
    analogWrite(farRight_RED, counterColor + brightnessFactor);
    //GREEN off
    analogWrite(farLeft_GREEN, ALLOFF);
    analogWrite(midLeft_GREEN, ALLOFF);
    analogWrite(midRight_GREEN, ALLOFF);
    analogWrite(farRight_GREEN, ALLOFF);
    break;
  default:
    break;
  }  
  return;
}

//------------------------------------------------------------------------------------------------------------
//Color Scheme 2 - Functions
void SelectColor(void){
  if((audioAmplitude > CHANGE_THRESH) && (abs(audioAmplitudePast - audioAmplitude) > CHANGE_OFFSET_THRESH)){
    colorIndex = random(0, 12);
    RED_Val = colorArray_RED[colorIndex];
    GREEN_Val = colorArray_GREEN[colorIndex];
    BLUE_Val = colorArray_BLUE[colorIndex];
  }
  return;
}

void UpdateLEDs_Scheme_2(void){
  //Update Far-Left
  analogWrite(farLeft_RED, RED_Val);
  analogWrite(farLeft_GREEN, GREEN_Val);
  analogWrite(farLeft_BLUE, BLUE_Val);
  //Update Mid-Left
  analogWrite(midLeft_RED, RED_Val);
  analogWrite(midLeft_GREEN, GREEN_Val);
  analogWrite(midLeft_BLUE, BLUE_Val);
  //Update Mid_Right
  analogWrite(midRight_RED, RED_Val);
  analogWrite(midRight_GREEN, GREEN_Val);
  analogWrite(midRight_BLUE, BLUE_Val);
  //Update Far-Right
  analogWrite(farRight_RED, RED_Val);
  analogWrite(farRight_GREEN, GREEN_Val);
  analogWrite(farRight_BLUE, BLUE_Val);
  return; 
}

