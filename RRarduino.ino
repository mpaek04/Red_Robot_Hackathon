#include <AFMotor.h>
#include <PS2X_lib.h>   
#include <Servo.h>
Servo speaker;
Servo armLifty;
PS2X ps2x; 

//Default set-up process:
//1: Turn on controller (make sure no one else is also doing so at the same time)
//2: Turn on robot
//3: Wait for controller to connect to receiver. Different brands of controllers have different
//   ways of indicating this. For Blue Lake controllers, the controller's mode LED should stay
//   on and Red, while the power LED remains off.
//3b: Turn on Analog mode. For Blue Lake controllers, this means pressing the mode button forcefully
//    until the power LED turns on and Green.
//Congrats! The controller should be connected!

// these variables appear throughout the code to keep track of state
int error = 0; 
byte type = 0;
byte vibrate = 0;
byte LYprev=0;
byte RYprev=0;
byte LXprev = 0;
byte RXprev = 0;
int servoPosition = 0;
float slowDownFactor = 1;

// these are used to control the robot's motors
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

// executes code here once upon start-up/power-on
// open the serial monitor when booting up to ensure that the controller connected
void setup()
{
  // this initializes the baud rate; make sure the serial monitor is set to this value
  Serial.begin(57600);

  // this initializes the motor speed to 0, and allows you to update the motor speed
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor3.setSpeed(0);
  motor4.setSpeed(0);

  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);

  // delay to allow controller time to connect
  delay(3000);
  
  // GamePad(clock, command, attention, data, Pressures?, Rumble?)
  // pay attention to the analog pins!
  error = ps2x.config_gamepad(A2, A4, A5, A3, true, true); 

  if(error == 0)
  {
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
  }
  else if(error == 1) // Visit www.billporter.info for troubleshooting tips
    Serial.println("No controller found, check wiring, see readme.txt to enable debug.");
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug.");
  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
      
  type = ps2x.readType(); 
  switch(type)
  {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
  } 
  armLifty.attach(10);
  speaker.attach(9);
}

// use this function to do something when the red button is pressed
void redPressed()
{



}

// use this function to do something when the green button is pressed
void greenPressed()
{
  tone(9,1000,500);
  tone(9,900,500);
  noTone(9);
// speaker.write(500);
//   delay(1000);
//   speaker.write(1000);
//   delay(1000);
  Serial.print("green button");
}

// use this function to do something when the blue button is pressed
void bluePressed()
{

}

// use this function to do something when the pink button is pressed
void pinkPressed()
{

}

// use this function to do something when the red button is released
void redReleased()
{
  
}

// use this function to do something when the green button is released
void greenReleased()
{
}

// use this function to do something when the blue button is released
void blueReleased()
{

}

// use this function to do something when the pink button is released
void pinkReleased()
{

}

// repeats forever
void loop()
{
   /* You must Read Gamepad to get new values. Read GamePad and set vibration values
   ps2x.read_gamepad(small motor on/off, larger motor strength from 0-255)
   if you don't enable the rumble, use ps2x.read_gamepad(); with no values
   You should call this at least once a second
   */
  if(error == 1) 
    return; //quit-out
  
  if(type == 2){ 
    Serial.println("Error!: Why are you using a GuitarHero Controller?");
  }
  else { //DualShock Controller

    // read controller and set large motor to spin at 'vibrate' speed
    // note that vibrate is updated with new value on next loop
    ps2x.read_gamepad(false, vibrate);

    // sets the large motor vibrate speed based on how hard you press the blue (X) button
    // this can be used with other buttons, too!
    // not really useful for the robot, but still fun to have?
    vibrate = ps2x.Analog(PSAB_BLUE);
    
    // these tell you if the start or select buttons are held
    if(ps2x.Button(PSB_START)) //will be TRUE as long as button is pressed
    {
      Serial.println("Start is being held");
    }
    if(ps2x.Button(PSB_SELECT))
    {
      Serial.println("Select is being held");
    }
         
    // these show how to read how long the D-Pad buttons were pressed
    if(ps2x.Button(PSB_PAD_UP)) // will be TRUE as long as button is pressed
    {
      Serial.print("Up held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
    }
    if(ps2x.Button(PSB_PAD_RIGHT))
    {
      Serial.print("Right held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
    }
    if(ps2x.Button(PSB_PAD_LEFT))
    {
      Serial.print("LEFT held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
    }
    if(ps2x.Button(PSB_PAD_DOWN))
    {
      Serial.print("DOWN held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
    }   
    
    // see below for how you can get the state of the different buttons/triggers
    if (ps2x.NewButtonState()) // will be TRUE if any button changes state (on to off, or off to on)
    {
      if(ps2x.Button(PSB_L3))
      {
       Serial.println("L3 pressed");
      }
      if(ps2x.Button(PSB_R3))
       Serial.println("R3 pressed");
      if(ps2x.Button(PSB_L2))
       Serial.println("L2 pressed");
      if(ps2x.Button(PSB_R2))
      {
      if (slowDownFactor == 1)
        {
          slowDownFactor = 0.5;
        }
        else
        {
          slowDownFactor = 1;
        }
        delay(10);
       Serial.println("R2 pressed");
      }
    }

    // will be TRUE if button was JUST pressed
    if(ps2x.ButtonPressed(PSB_RED))
    {
      Serial.println("Circle just pressed");
      redPressed();
    }

    if(ps2x.ButtonPressed(PSB_GREEN))
    {
      Serial.println("Triangle just pressed");
      greenPressed();
    }

    if(ps2x.ButtonPressed(PSB_BLUE))
    {
      Serial.println("X just pressed");
      bluePressed();
    }

    if(ps2x.ButtonPressed(PSB_PINK))
    {
      Serial.println("Square just pressed");
      pinkPressed();
    }
    
    //will be TRUE if button was JUST released
    if(ps2x.ButtonReleased(PSB_RED))
    {
      Serial.println("Circle just released");
      redReleased(); 
    }

    if(ps2x.ButtonReleased(PSB_GREEN))
    {
      Serial.println("Triangle just released");
      greenReleased();
    }

    if(ps2x.ButtonReleased(PSB_BLUE))
    {
      Serial.println("X just released");
      blueReleased();
    }

    if(ps2x.ButtonReleased(PSB_PINK))
    {
      Serial.println("Square just released");
      pinkReleased();   
    }
    
    // print stick values if either is TRUE (for debugging)
    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1))
    {
      Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC); 
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC); 
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC);
    }

    // Get forward speed based on analog sticks
    byte LY = ps2x.Analog(PSS_LY);
    byte RY = ps2x.Analog(PSS_RY);
    byte LX = ps2x.Analog(PSS_LX);

    // byte LX = ps2x.Analog(PSS_LX);
    // byte RX = ps2x.Analog(PSS_RX);
    // byte LY = ps2x.Analog(PSS_LY);
    // byte RY = ps2x.Analog(PSS_RY);

    // // Maths implement for motors
    // double centre = 255/2;
    // double unitX = (RX-centre)/centre;
    // double unitY = (-(RY-centre))/centre;
    // double angle = atan(RY/RX);
    // double scaling = (sin(angle) - 0.5)*2;
    // int mag = 255;
    // int turningSpeed = mag*scaling;

    byte baseSpeed = (abs(LY-127)*2+(LY<128)-(LY>128));
    byte turnSpeed = (abs(LX-127 )*2+(LX<128)-(LX>128));
    byte leftSpeed, rightSpeed;

    // byte leftSpeed = LY+(255-LX)-200;
    // byte rightSpeed = LY+(LX)-200;

    // Drive the motors based on steering commands from the controller
    // You are advised against changing this code block, unless you know what you are doing!
    //LX is 0 at left, 255 at right, 128 in the middle
    //LY is 0 at top, 255 at bottom, 127 in the middle
    //used to be RY

    if(((LY != 127) & (LYprev != 127)) || ((RY != 127) & (RYprev != 128)))
    {


    //   // leftSpeed = LY+(255-LX)-200;
    //   // rightSpeed = LY+(LX)-200;
    
    //HERE
      motor1.setSpeed((abs(LY-127)*2+(LY<128)-(LY>128))* slowDownFactor);
      motor4.setSpeed((abs(LY-127)*2+(LY<128)-(LY>128))* slowDownFactor);
      motor2.setSpeed((abs(RY-127)*2+(RY<128)-(RY>128))* slowDownFactor);
      motor3.setSpeed((abs(RY-127)*2+(RY<128)-(RY>128))* slowDownFactor) ;
    //HERE

    //   if (-255 < LX < 128)
    //   {
    //     leftSpeed = abs(baseSpeed - turnSpeed);
    //     rightSpeed = baseSpeed;
    //     // leftSpeed = turnSpeed;
    //   }   
    //   else if (255 > LX > 128)
    //   {
    //     leftSpeed = baseSpeed;
    //     rightSpeed = abs(baseSpeed - turnSpeed);
    //     // rightSpeed = turnSpeed;
    //   } 
    //   else if (LX == 255 || LX == -255)
    //   {
    //     leftSpeed = rightSpeed = turnSpeed;
    //   }
    //   else
    //   {
    //     leftSpeed = rightSpeed = baseSpeed;
    //   }
    //   if (leftSpeed < 70)
    //   {
    //     leftSpeed = 70;
    //   }
    //   if (rightSpeed < 70)
    //   {
    //     rightSpeed = 70;
    //   }

    // //   motor1.setSpeed(leftSpeed);
    // //   motor4.setSpeed(leftSpeed);
    // //   motor2.setSpeed(rightSpeed);
    // //   motor3.setSpeed(rightSpeed);
    //   motor1.setSpeed(abs(leftSpeed));
    //   motor4.setSpeed(abs(leftSpeed));
    //   motor2.setSpeed(abs(rightSpeed));
    //   motor3.setSpeed(abs(rightSpeed));


      //HERE
      motor1.run(LY < 127 ? FORWARD : BACKWARD);
      motor4.run(LY < 127 ? FORWARD : BACKWARD);
      motor2.run(RY < 127 ? FORWARD : BACKWARD);
      motor3.run(RY < 127 ? FORWARD : BACKWARD);
      //HERE


      // if (LY < 127)
      // {
      //   if (LX < 128)
      //   {
      //     Serial.println("left up");
      //     motor1.run(LX > 64 ? FORWARD : BACKWARD);
      //     motor4.run(LX > 64 ? FORWARD : BACKWARD);
      //     motor2.run(FORWARD);
      //     motor3.run(FORWARD);
      //   }
      //   else if (LX > 128)
      //   {
      //     Serial.println("right up");
      //     motor1.run(FORWARD);
      //     motor4.run(FORWARD);
      //     motor2.run(LX < 192 ? FORWARD : BACKWARD);
      //     motor3.run(LX < 192 ? FORWARD : BACKWARD);
      //     Serial.println("Left speed");
      //     Serial.println(leftSpeed);
      //     Serial.println("Right Speed");
      //     Serial.println(rightSpeed);          

      //   }
      //   else
      //   {
      //     Serial.println("forward");
      //     motor1.run(FORWARD);
      //     motor4.run(FORWARD);
      //     motor2.run(FORWARD);
      //     motor3.run(FORWARD);            
      //   }
      // }
      // else if (LY > 127)
      // {
      //  if (LX < 128)
      //   {
      //     Serial.println("left down");
      //     motor1.run(LX > 64 ? FORWARD : BACKWARD);
      //     motor4.run(LX > 64 ? FORWARD : BACKWARD);
      //     motor2.run(BACKWARD);
      //     motor3.run(BACKWARD);
      //   }
      //   else if (LX > 128)
      //   {
      //     Serial.println("right down");
      //     motor1.run(BACKWARD);
      //     motor4.run(BACKWARD);
      //     motor2.run(LX < 192 ? FORWARD : BACKWARD);
      //     motor3.run(LX < 192 ? FORWARD : BACKWARD);

      //   }
      //   else
      //   {
      //     Serial.println("back");
      //     motor1.run(BACKWARD);
      //     motor4.run(BACKWARD);
      //     motor2.run(BACKWARD);
      //     motor3.run(BACKWARD);            
      //   }

      // }
      // else
      // {
      //   motor1.setSpeed(turnSpeed);
      //   motor4.setSpeed(turnSpeed);
      //   motor2.setSpeed(turnSpeed);
      //   motor3.setSpeed(turnSpeed);
      //   motor1.run(LX > 128 ? FORWARD : BACKWARD);
      //   motor4.run(LX > 128 ? FORWARD : BACKWARD);
      //   motor2.run(LX < 128 ? FORWARD : BACKWARD);
      //   motor3.run(LX < 128 ? FORWARD : BACKWARD);
        
      // }
    } else

    
    {
      motor1.setSpeed(0);
      motor4.setSpeed(0);
      motor2.setSpeed(0);
      motor3.setSpeed(0);
      motor1.run(RELEASE);
      motor4.run(RELEASE);
      motor2.run(RELEASE);
      motor3.run(RELEASE);
    }
   

    LYprev = LY;
    RYprev = RY;
    // LYprev = LY;
    // RYprev = LX;
  }

  // //motors implement
  // if(((LY != 127) & (LYprev != 127)) || ((RY != 127) & (RYprev != 127)) || ((LX != 127) & (LXprev != 127)) || ((RX != 127) & (RXprev != 127)))
  //   {
  //     //Set turning motor
  //     if (unitX > 0)
  //     {
  //       //turning motors = left = 1,4
  //       motor2.setSpeed(mag);
  //       motor3.setSpeed(mag);
  //       motor1.setSpeed(abs(turningSpeed));
  //       motor4.setSpeed(abs(turningSpeed));
  //       motor2.run(FORWARD);
  //       motor3.run(FORWARD);
  //       motor1.run(turningSpeed > 0 ? FORWARD : BACKWARD);
  //       motor4.run(turningSpeed > 0 ? FORWARD : BACKWARD);
  //     }
  //     if (unitX < 0)
  //     {
  //       //turning motors = right = 2,3
  //       motor1.setSpeed(mag);
  //       motor4.setSpeed(mag);
  //       motor2.setSpeed(abs(turningSpeed));
  //       motor3.setSpeed(abs(turningSpeed));
  //       motor1.run(FORWARD);
  //       motor4.run(FORWARD);
  //       motor2.run(turningSpeed > 0 ? FORWARD : BACKWARD);
  //       motor3.run(turningSpeed > 0 ? FORWARD : BACKWARD);
  //     }
  //   } else
  //   {
  //     motor1.setSpeed(0);
  //     motor4.setSpeed(0);
  //     motor2.setSpeed(0);
  //     motor3.setSpeed(0);
  //     motor1.run(RELEASE);
  //     motor4.run(RELEASE);
  //     motor2.run(RELEASE);
  //     motor3.run(RELEASE);
  //   }

  //   LYprev = LY;
  //   RYprev = RY;
  //   LXprev = LX;
  //   RXprev = RX;

  if(ps2x.Button(PSB_PAD_UP)) // will be TRUE as long as button is pressed
    {
      if (servoPosition<=90){
        servoPosition+=10;
      }
      armLifty.write(servoPosition);
    }
    
    if(ps2x.Button(PSB_PAD_DOWN))
    {
      if (servoPosition>=0){
        servoPosition-=10;
      }
      armLifty.write(servoPosition);
    }   

    if(ps2x.ButtonPressed(PSB_RED))
    {
      Serial.println("Circle just pressed");
      redPressed();
    }

  delay(50); // small added delay between checks to avoid overloading  
}

