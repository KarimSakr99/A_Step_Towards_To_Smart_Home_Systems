//include the library code
#include <IRremote.h>
#include <Servo.h>
#include <LiquidCrystal.h>

//Define ir receiver pin
#define RECV_PIN         2

//Define needed lcd interface pin with the arduino pin number it is connected to
#define LCD_RS           8
#define LCD_EN           9
#define LCD_D4          10
#define LCD_D5          11
#define LCD_D6          12
#define LCD_D7          13

//Define servo pin
#define Servo_pin        3

//Define led pin
#define Led_pin          5

//Define ldr pin
#define Ldr_pin          A1

//Define dc_motor pin
#define DcMotor_pin      6

//Define lm35 pin
#define Lm35_pin         A2


//Define Servo angle
#define Servo_FullAngle   180
#define Servo_ZeroAngle   0

//Define fan speed
#define SpeedLevel_0       0 
#define SpeedLevel_1       125 
#define SpeedLevel_2       255 

//Define Remote Serial
#define Switch_0          0xFFB04F
#define Switch_1          0xFF30CF
#define Switch_2          0xFF18E7
#define Switch_3          0xFF7A85
#define Switch_4          0xFF10EF
#define Switch_5          0xFF38C7
#define Switch_6          0xFF5AA5
#define Switch_7          0xFF42BD
#define Switch_Reset      0xFF629D
#define Switch_Off        0xFFA25D



#define NumberDigits      4
#define FirstTry          1
#define SecondTry         2
#define ThirdTry          3


// attaches the ir on pin 2 to the ir object
IRrecv irrecv(RECV_PIN);
// create ir object to store results
decode_results results;

// create servo object to control a servo
Servo myservo;

//initialize the library by associating ane needed lcd interface pin with the arduino pin number it is connected to
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);


void setup()
{
  Serial.begin(9600);

  //start the receiver
  irrecv.enableIRIn();

  // attaches the servo on pin 3 to the servo object
  myservo.attach(Servo_pin);

  pinMode(Led_pin, OUTPUT);

  pinMode(Ldr_pin , INPUT);

  pinMode(DcMotor_pin , OUTPUT);

  pinMode(Lm35_pin, INPUT);

  //set up the LCD'S number of columns amd rows:
  lcd.begin(16, 2);

  delay(500);
  Serial.println("Welcome");
  lcd.setCursor(4, 0);
  lcd.print("Welcome");
  Serial.println("May i help you");
  lcd.setCursor(1, 1);
  lcd.print("May i help you");
  delay(2000);
  lcd.clear();
}


int level;

int value = 0 , temp = 0;
float volt = 0;

char data1 = '0';
char data2 = '0';
char data3 = '0';
char data4 = '0';
char data5 = '0';

int check_pass;
int iterration_exit = 0;

char input_order;

void begining(void)
{
  delay(1000);
  Serial.println("Enter your code:");
  lcd.setCursor(0, 0);
  lcd.print("Enter your code:");
}

void pass(void)
{
  while (Serial.available() == 0);
  data1 = Serial.read();
  lcd.setCursor(0, 1);
  lcd.print(data1);
  delay(100);
  lcd.setCursor(0, 1);
  lcd.print("*");
  check_pass++;
  while (Serial.available() == 0);
  data2 = Serial.read();
  lcd.setCursor(1, 1);
  lcd.print(data2);
  delay(100);
  lcd.setCursor(1, 1);
  lcd.print("*");
  check_pass++;
  while (Serial.available() == 0);
  data3 = Serial.read();
  lcd.setCursor(2, 1);
  lcd.print(data3);
  delay(100);
  lcd.setCursor(2, 1);
  lcd.print("*");
  check_pass++;
  while (Serial.available() == 0);
  data4 = Serial.read();
  lcd.setCursor(3, 1);
  lcd.print(data4);
  delay(100);
  lcd.setCursor(3, 1);
  lcd.print("*");
  delay(100);
  check_pass++;
}

void mobile_app(void)
{
  while (1)
  {
    while (Serial.available() == 0);
    input_order = Serial.read();

    //open the door(0)
    if (input_order == '0')
    {
      myservo.write(Servo_FullAngle);
      Serial.println("door open");
      order();
      lcd.print("door open");
      delay(1000);
    }

    //close the door(1)
    else if (input_order == '1')
    {
      myservo.write(Servo_ZeroAngle);
      Serial.println("door close");
      order();
      lcd.print("door closed");
      delay(1000);
    }

    //Turn on the led(2)
    else if (input_order == '2')
    {
      digitalWrite(Led_pin, HIGH);
      Serial.println("Led on");
      order();
      lcd.print("Led on");
      delay(1000);
    }

    //Turn off the led(3)
    else if (input_order == '3')
    {
      digitalWrite(Led_pin, LOW);
      Serial.println("Led off");
      order();
      lcd.print("Led off");
      delay(1000);
    }

    //Adjustbrithness(4)
    else if (input_order == '4')
    {
      level = analogRead(Ldr_pin);
      level = map(level, 0, 1023, 0, 255);
      delay(200);
      level = 255 - level;
      analogWrite(Led_pin , level);
      Serial.println("Auto led");
      order();
      lcd.print("Auto led");
      delay(1000);
    }

    //turn on the fan(5)
    else if (input_order == '5')
    {
      digitalWrite(DcMotor_pin, HIGH);
      Serial.println("Fan on");
      order();
      lcd.print("Fan on");
      delay(1000);
    }

    //turn off the fan(6)
    else if (input_order == '6')
    {
      digitalWrite(DcMotor_pin, LOW);
      Serial.println("Fan off");
      order();
      lcd.print("Fan off");
      delay(1000);
    }

    //Adjust speed(7)
    else if (input_order == '7')
    {
      value = analogRead(Lm35_pin);
      volt = (5.0 / 1023.0) * value;
      temp = volt / 0.01;
      delay(500);
      Serial.print("Temperature = ");
      Serial.println(temp);
      if (temp <= 23)
      {
        analogWrite(DcMotor_pin, SpeedLevel_0);
      }
      else if ( (temp > 23) && (temp < 27))
      {
        analogWrite(DcMotor_pin, SpeedLevel_1);
      }

      else
      {
        analogWrite(DcMotor_pin, SpeedLevel_2);
      }
      Serial.println("Auto fan ");
      order();
      lcd.print("Auto fan ");
      lcd.setCursor(9, 1);
      lcd.print(temp);
      delay(1000);
    }
    //reset(8)
    else if (input_order == '8')
    {
      Serial.println("program reset");
      order();
      lcd.print("program reset");
      delay(1000);
      lcd.clear();
      break;
    }

    //off(9)
    else if (input_order == '9')
    {
      Serial.println("Switching off System");
      order();
      lcd.print("Switching off");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Use manual");
      lcd.setCursor(0, 1);
      lcd.print("control, Bye!");
      Serial.println("Use manual control");
      delay(1000);
      Serial.println("Reset the program to use smart home again!");
      delay(2000);
      Serial.println("Bye Bye");
      delay(2000);
      while (1)
      {
      }
    }
    else
    {
      Serial.println("Wrong character");
    }
  }
}

void remote_app(void)
{

  while (1)
  {
    //avoid false reset
    while (results.value == Switch_Reset)
    {
      if (irrecv.decode(&results))
      {

        irrecv.resume();
      }
      delay(250);
    }

    if (irrecv.decode(&results))
    {
      //open the door(0)
      if (results.value == Switch_0)
      {
        myservo.write(Servo_FullAngle);
        order();
        lcd.print("door open");
      }

      //close the door(1)
      else if (results.value == Switch_1)
      {
        myservo.write(Servo_ZeroAngle);
        order();
        lcd.print("door closed");
      }

      //Turn on the led(2)
      else if (results.value == Switch_2)
      {
        digitalWrite(Led_pin, HIGH);
        order();
        lcd.print("Led on");
      }

      //Turn off the led(3)
      else if (results.value == Switch_3)
      {
        digitalWrite(Led_pin, LOW);
        order();
        lcd.print("Led off");
      }

      //Adjustbrithness(4)
      else if (results.value == Switch_4)
      {
        level = analogRead(Ldr_pin);
        level = map(level, 0, 1023, 0, 255);
        delay(200);
        level = 255 - level;
        analogWrite(Led_pin , level);
        order();
        lcd.print("Auto led");
      }

      //turn on the fan(5)
      else if (results.value == Switch_5)
      {
        digitalWrite(DcMotor_pin, HIGH);
        order();
        lcd.print("Fan on");
      }

      //turn off the fan(6)
      else if (results.value == Switch_6)
      {
        digitalWrite(DcMotor_pin, LOW);
        order();
        lcd.print("Fan off");
      }
      //Adjust speed(7)
      else if (results.value == Switch_7)
      {
        value = analogRead(Lm35_pin);
        volt = (5.0 / 1023.0) * value;
        temp = volt / 0.01;
        delay(500);
        if (temp <= 23)
        {
          analogWrite(DcMotor_pin, SpeedLevel_0);
        }
        else if ((temp > 23) && (temp < 27))
        {
          analogWrite(DcMotor_pin, SpeedLevel_1);
        }

        else
        {
          analogWrite(DcMotor_pin, SpeedLevel_2);
        }
        order();
        lcd.print("Auto fan");
        lcd.setCursor(9, 1);
        lcd.print(temp);
      }
      //reset
      else if (results.value == Switch_Reset)
      {
        order();
        lcd.print("program reset");
        delay(1000);
        lcd.clear();
        break;
      }

      //off
      else if (results.value == Switch_Off)
      {
        order();
        lcd.print("Switching off");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Use manual");
        lcd.setCursor(0, 1);
        lcd.print("control, Bye!");
        while (1)
        {

        }
      }
      irrecv.resume();
    }
    delay(250);
  }

}

void order(void)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Choose an order");
  lcd.setCursor(0, 1);
}

void loop()
{

  check_pass = 0;

  begining();

  pass();

  if (check_pass == NumberDigits)
  {
    // Correct password is 1234
    if ((data1 == '1') && (data2 == '2') && (data3 == '3') && (data4 == '4'))
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Correct password");
      Serial.println("Correct password");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Choose your");
      lcd.setCursor(0, 1);
      lcd.print("controller");
      Serial.println("Choose your controller");
      delay(2000);
label1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("1 mobile phone");
      Serial.println("1 mobile phone");
      lcd.setCursor(0, 1);
      lcd.print("2 remote control");
      Serial.println("2 remote control");
      while (Serial.available() == 0);
      data5 = Serial.read();
      lcd.clear();
      if (data5 == '1')
      {
        lcd.setCursor(0, 0);
        lcd.print("Your are now on");
        lcd.setCursor(0, 1);
        lcd.print("mobile mode");
        Serial.println("You are now on mobile mode");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Choose an order");
        Serial.println("Choose an order from the menu");
        delay(2000);
        Serial.println("For door opening press 0");
        Serial.println("For door closing press 1");
        Serial.println("For light on press 2");
        Serial.println("For light off press 3");
        Serial.println("For automatic light press 4");
        Serial.println("For fan on press 5");
        Serial.println("For fan off press 6");
        Serial.println("For automatic fan press 7");
        Serial.println("To reset the system press 8");
        Serial.println("To turn off the system press 9");
        mobile_app();
      }
      else if (data5 == '2')
      {
        lcd.setCursor(0, 0);
        lcd.print("Your are now on");
        lcd.setCursor(0, 1);
        lcd.print("remote mode");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Choose an order");
        remote_app();
      }
      else
      {
        lcd.clear();
        Serial.println("Wrong Character");
        lcd.setCursor(0, 0);
        lcd.print("Wrong Character");
        goto label1;
      }
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Wrong password");
      Serial.println("Wrong password");
      delay(2000);
      lcd.clear();
      iterration_exit++;
    }
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wrong password");
    Serial.println("Incorrect pass");
    delay(2000);
    lcd.clear();
    iterration_exit++;
  }
  data1 = '0';
  data2 = '0';
  data3 = '0';
  data4 = '0';
  if (iterration_exit == FirstTry)
  {
    lcd.clear();
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("You have 2");
    lcd.setCursor(0, 1);
    lcd.print("more attempts");
    Serial.println("You have 2 more attempts");
    delay(2000);
    lcd.clear();
  }
  else if (iterration_exit == SecondTry)
  {
    lcd.clear();
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("You have 1");
    lcd.setCursor(0, 1);
    lcd.print("more attempt");
    Serial.println("You have 1 more attempt");
    delay(2000);
    lcd.clear();
  }
  else if (iterration_exit == ThirdTry)
  {
    lcd.clear();
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("You have no");
    lcd.setCursor(0, 1);
    lcd.print("attempts left");
    Serial.println("You have no attempts left");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reset system");
    Serial.println("Reset system");
    delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("Good luck");
    Serial.println("Good luck");
    delay(2000);
    lcd.clear();
    while (1)
    {
    }
  }


}
