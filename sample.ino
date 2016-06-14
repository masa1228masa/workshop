#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"
#include<Servo.h>

String readinput();
void Enroll();
void Detect();
void Turn(bool);//true:open,false:close

Servo servo;
FPS_GT511C3 fps(4,5);
int ID = 0;//the granted finger ID to operate servo 


void setup(){
  Serial.begin(9600);
  delay(100);
  servo.attach(9);
  fps.Open();
  fps.SetLED(true);
  delay(1000);
  fps.SetLED(false);
}

bool turn = false;

void loop(){
  String str;

  str = readinput();
  
  if(str.length() > 0){
    if(str == "enroll"){
      fps.SetLED(true);  
      delay(1000);
      fps.SetLED(false);
      delay(1000);
      fps.SetLED(true);
      delay(1000);
      fps.SetLED(false);
      delay(1000);
      Enroll();
      Serial.println(str + " end!!");
    }else if(str == "detect"){
      fps.SetLED(true);
      delay(1000);
      fps.SetLED(false);
      delay(1000);
      fps.SetLED(true);
      delay(1000);
      fps.SetLED(false);
      delay(1000);
      fps.SetLED(true);
      delay(1000);
      fps.SetLED(false);
      delay(1000);
      fps.SetLED(true);
      Detect();
      Serial.println(str + " end!!");
    }else if(str == "turn right"){
      Turn(true);  
      Serial.println(str + " end!!");    
    }else if(str == "turn left"){
      Turn(false);
      Serial.println(str + " end!!");     
    }else{
      Serial.println("wrong command!!");  
    }
  }
}


String readinput(){
  String str;
  if(Serial.available()>0){
    delay(100);  
    while(Serial.available()>0){
      int input = Serial.read();
      str.concat((char)input);
    }
  }
  return str;
}

void Enroll()
{
	// Enroll test

	// find open enroll id
	int enrollid = 0;
	bool usedid = true;
	while (usedid == true)
	{
		usedid = fps.CheckEnrolled(enrollid);
		if (usedid==true) enrollid++;
	}
	fps.EnrollStart(enrollid);

	// enroll
	Serial.print("Press finger to Enroll #");
	Serial.println(enrollid);
	while(fps.IsPressFinger() == false) delay(100);
	bool bret = fps.CaptureFinger(true);
	int iret = 0;
	if (bret != false)
	{
		Serial.println("Remove finger");
		fps.Enroll1(); 
		while(fps.IsPressFinger() == true) delay(100);
		Serial.println("Press same finger again");
		while(fps.IsPressFinger() == false) delay(100);
		bret = fps.CaptureFinger(true);
		if (bret != false)
		{
			Serial.println("Remove finger");
			fps.Enroll2();
			while(fps.IsPressFinger() == true) delay(100);
			Serial.println("Press same finger yet again");
			while(fps.IsPressFinger() == false) delay(100);
			bret = fps.CaptureFinger(true);
			if (bret != false)
			{
				Serial.println("Remove finger");
				iret = fps.Enroll3();
				if (iret == 0)
				{
					Serial.println("Enrolling Successfull");
				}
				else
				{
					Serial.print("Enrolling Failed with error code:");
					Serial.println(iret);
				}
			}
			else Serial.println("Failed to capture third finger");
		}
		else Serial.println("Failed to capture second finger");
	}
	else Serial.println("Failed to capture first finger");
}

void Detect(){
  while(readinput() != "quit"){
    if (fps.IsPressFinger())
    {
      fps.CaptureFinger(false);
      int id = fps.Identify1_N();
      if (id <200)
      {
        Serial.print("Verified ID:");
        Serial.println(id);
        if(servo.read() == 180){
          if(ID == id){
            Serial.println("access granted");
            Turn(false);
            Serial.println("door is close");
          }else{
            Serial.println("access denied");
            Serial.println("only who opened the door can access");
          }
        }else{
          Serial.println("access granted");
          Turn(true);
          ID = id;
          Serial.println("door is open");
        }

      }
      else
      {
        Serial.println("Finger not found");
      }
    }
    else
    {
      Serial.println("Please press finger");
    }
    delay(100);
  }
}

void Turn(bool direction){
  if(direction){
    delay(1000);
    servo.write(180);
  }else{
    delay(1000);
    servo.write(0);
  }
}
