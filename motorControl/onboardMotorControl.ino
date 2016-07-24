//--------------------------------------------------------------
// Author: Ashish Choudhari
// Email: choudhariashish@gmail.com
//
//--------------------------------------------------------------
// Platform: Pluto
// Program that runs on DfRobot romeo 1.3 board to control
// left and right motors.
//--------------------------------------------------------------

//--------------------------------------------------------------
//Standard PWM DC control
//--------------------------------------------------------------
#define LEFT_MOTOR    0
#define RIGHT_MOTOR   0

#define SPD_M_LEFT    6     // M left Speed Control
#define SPD_M_RIGHT   5     // M right Speed Control
#define DIR_M_LEFT    7     // M left Direction Control
#define DIR_M_RIGHT   4     // M right Direction Control

//--------------------------------------------------------------
// Motor encoder pin configuration
//--------------------------------------------------------------
#define ENC_M_LEFT_A    2     // Encoder A pin for left motor
#define ENC_M_RIGHT_A   3     // Encoder A pin for right motor

#define ENC_M_LEFT_B    8     // Encoder B pin for left motor
#define ENC_M_RIGHT_B  12     // Encoder B pin for right motor

//--------------------------------------------------------------
// Standard arduino setup routine
//--------------------------------------------------------------
void setup(void) 
{ 
  //------------------------------------------------------------
  // Configure all required pwm control pins to be output
  for(int i=4;i<=7;i++)
  {
      pinMode(i, OUTPUT);  
  }
  //Initialize serial port  
  Serial.begin(115200);
  Serial.println("[INFO]:Initialized serial console");
  
  //------------------------------------------------------------
  //Initialize encoder requirements
  Serial.println("[INFO]:Initializing encoder interrupts..");

  // LEFT motor
  // Encoder pin B as input
  pinMode(ENC_M_LEFT_A, INPUT);
  pinMode(ENC_M_LEFT_B, INPUT);

  // Attach interrupt. Gets called everytime a signal is
  // detected
  attachInterrupt(0, leftMotorMovedISR, CHANGE);// int.0

  // RIGHT motor
  // Encoder pin B as input
  pinMode(ENC_M_RIGHT_A, INPUT);
  pinMode(ENC_M_RIGHT_B, INPUT);  

  // Attach interrupt. Gets called everytime a signal is
  // detected
  attachInterrupt(1, rightMotorMovedISR, CHANGE);// int.1
  Serial.println("[INFO]:done"); 
}

volatile unsigned int LeftMPos       = 0;
volatile unsigned int RightMPos      = 0;
volatile unsigned int TargetPosLeft  = 0;
volatile unsigned int TargetPosRight = 0;
volatile bool trackLeft  = false;
volatile bool trackRight = false;
volatile unsigned int temp = 300;

//--------------------------------------------------------------
// Standard arduino loop routine
//--------------------------------------------------------------
void loop()
{
  if(trackLeft)
  {
    if(LeftMPos > TargetPosLeft)
    {
      stopMotors(LEFT_MOTOR);
      LeftMPos = 0;
      TargetPosLeft = 0;
      trackLeft = false;
    }
    else
    {
      Serial.print(LeftMPos);
      Serial.println(" LDONE");
    }
  }  
  if(trackRight)
  {
    if(RightMPos > TargetPosRight)
    {
      stopMotors(RIGHT_MOTOR);
      RightMPos = 0;
      TargetPosRight = 0;
      trackRight = false;
    }
    else
    {
      Serial.print(RightMPos);
      Serial.println(" RDONE");
    }    
  }

}

//--------------------------------------------------------------
// stopMotors()
// Stops motor
//--------------------------------------------------------------
void stopMotors(bool motor)
{
  if(motor == LEFT_MOTOR)
  {
    digitalWrite(SPD_M_LEFT,LOW);   
  }
  if(motor == RIGHT_MOTOR)
  {    
    digitalWrite(SPD_M_RIGHT,LOW);      
  }
}

//--------------------------------------------------------------
// turn_L()
// Turns left
//--------------------------------------------------------------
void turn_L (char a,char b)
{
  analogWrite (SPD_M_LEFT,a);
  digitalWrite(DIR_M_LEFT,HIGH);    
  analogWrite (SPD_M_RIGHT,b);    
  digitalWrite(DIR_M_RIGHT,LOW);
}

//--------------------------------------------------------------
// turn_R()
// Turns right
//--------------------------------------------------------------
void turn_R (char a,char b)
{
  analogWrite (SPD_M_LEFT,a);
  digitalWrite(DIR_M_LEFT,LOW);    
  analogWrite (SPD_M_RIGHT,b);    
  digitalWrite(DIR_M_RIGHT,HIGH);
}

//--------------------------------------------------------------
// turn_L_only_advance()
// Advances left motor only
//--------------------------------------------------------------
void turn_L_only_advance(char a)
{
  analogWrite (SPD_M_LEFT,a);
  digitalWrite(DIR_M_LEFT,LOW);    
}

//--------------------------------------------------------------
// turn_L_only_backoff()
// Backsoff left motor only
//--------------------------------------------------------------
void turn_L_only_backoff(char a)
{
  analogWrite (SPD_M_LEFT,a);
  digitalWrite(DIR_M_LEFT,HIGH);    
}

//--------------------------------------------------------------
// turn_R_only_advance()
// Advances right motor only
//--------------------------------------------------------------
void turn_R_only_advance(char a)
{
  analogWrite (SPD_M_RIGHT,a);
  digitalWrite(DIR_M_RIGHT,LOW);
}

//--------------------------------------------------------------
// turn_R_only_backoff()
// Backsoff right motor only
//--------------------------------------------------------------
void turn_R_only_backoff(char a)
{
  analogWrite (SPD_M_RIGHT,a);
  digitalWrite(DIR_M_RIGHT,HIGH);
}

//--------------------------------------------------------------
// advance()
// Advances both motors in forward direction
//--------------------------------------------------------------
void advance(char a,char b)
{
  analogWrite (SPD_M_LEFT,a);
  digitalWrite(DIR_M_LEFT,LOW);    
  analogWrite (SPD_M_RIGHT,b);
  digitalWrite(DIR_M_RIGHT,LOW);
}

//--------------------------------------------------------------
// backoff()
// backsoff both motors
//--------------------------------------------------------------
void backoff(char a,char b)
{
  analogWrite (SPD_M_LEFT,a);
  digitalWrite(DIR_M_LEFT,HIGH);    
  analogWrite (SPD_M_RIGHT,b);
  digitalWrite(DIR_M_RIGHT,HIGH);
}

void serialEvent()
{  
  char command = Serial.read();
  Serial.println(command);

  if(command == 'w')
  {
    TargetPosLeft = temp;    
    TargetPosRight = temp;    
    LeftMPos = 0;
    RightMPos = 0;    
    trackLeft = true;
    trackRight = true;    
    advance(255, 255);
  }
  if(command == 's')
  {
    TargetPosLeft = temp;    
    TargetPosRight = temp; 
    LeftMPos = 0;
    RightMPos = 0;        
    trackLeft = true;
    trackRight = true;    
    backoff(255, 255); 
  }
  if(command == 'a')
  {
    TargetPosLeft = temp;    
    TargetPosRight = temp;    
    LeftMPos = 0;
    RightMPos = 0;    
    trackLeft = true;
    trackRight = true;    
    turn_L(255, 255);
  }
  if(command == 'd')
  {
    TargetPosLeft = temp;    
    TargetPosRight = temp; 
    LeftMPos = 0;
    RightMPos = 0;        
    trackLeft = true;
    trackRight = true;    
    turn_R(255, 255); 
  }
  if(command == 'q')
  {
    TargetPosLeft = temp;
    LeftMPos = 0;
    trackLeft = true;
    turn_L_only_advance(255);
  }
  if(command == 'e')
  {
    TargetPosRight = temp;
    RightMPos = 0;
    trackRight = true;    
    turn_R_only_advance(255);
  }  
  if(command == 'z')
  {
    TargetPosLeft = temp;
    LeftMPos = 0;
    trackLeft = true;
    turn_L_only_backoff(255);
  }
  if(command == 'c')
  {
    TargetPosRight = temp;
    RightMPos = 0;
    trackRight = true;    
    turn_R_only_backoff(255);
  }    
  if(command == 'x')
  {
    TargetPosLeft  = 0;
    TargetPosRight = 0;
  }
}

void leftMotorMovedISR()
{
  LeftMPos++;
}

void rightMotorMovedISR()
{
  RightMPos++;
}
