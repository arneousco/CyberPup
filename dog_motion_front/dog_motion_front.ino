#include <Servo.h>
#include <UARTClass.h>
#include<Wire.h>


TwoWire Wire(0);

UARTClass espData(1);

Servo servo1;  // create servo object to control a servo
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;
Servo servo7;
Servo servo8; 

int data;  //esp data

int n = 0;

int motCntr = 1;

int mode = 1;

//imu variables

const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
 
int minVal=265;
int maxVal=402;
 
double x;
double y;
double z;

//imu end

double upArm = 5.5;
double downArm = 6.85;

double shoulderAngle = 0;
double kneeAngle= 0;

double shoulderAngleDeg = 0;
double kneeAngleDeg = 0;

double shoulderOffset = 0;
double shoulderOffsetDeg = 0;

double hLen = 8;
double Y = 0;
double pitch = 0;

double l1z = 0;
double l2z = 0;
double l3z = 0;
double l4z = 0;

double l1y = 0;
double l2y = 0;
double l3y = 0;
double l4y = 0;


int gF = 0;


double legLen = 0;


double bodyHalfY = 5;

int front = 1;
int side = 1;

int i;

int dataMap(int inp){
  int t;
  
  if (inp == 1094795585){
    t = 1;
  }
  else if (inp == 1111638594){
    t = 2;
  }
  else if (inp == 1128481603){
    t = 3;
  }
  else if (inp == 1145324612){
    t = 4;
  }
  else if (inp == 1162167621){
    t = 5;
  }
  else if (inp == 1179010630){
    t = 6;
  }
  else if (inp == 1195853639){
    t = 7;
  }
  else if (inp == 1212696648){
    t = 8;
  }
  return t;
}

double servo_Map(int x, int in_min, int in_max, int out_min, int out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

int zMot(int pos) {
  int z;

  if (pos == 2 || pos == 3) {
    z = 7;
  }
  else {
    z = 8;
  }
  return z;
}

float yMot(int pos, float curPos, float stick){
  float y;
  
  // change stick value
  
  if (pos == 1) {
    y = curPos - (stick*-1);
  }
  else if (pos == 2) {
    y = curPos + (stick * -1);
  }
  else if (pos == 3) {
    y = stick;
  }
  else if (pos == 4) {
    y = curPos + (stick * -1);
  }
  else if (pos == 5) {
    y = curPos - (stick * -1);
  }
  else if (pos == 6) {
    y = curPos - (stick * -1);
  }
  else if (pos == 7) {
    y = curPos - (stick * -1);
  }


  return y;
}

void leg(double hlen, double Y, double pitch, int side, int front){
  pitch = pitch*(PI/180);
        if (front == 1){
            pitch = pitch*(-1);
        }
        else if (front == 0){
            pitch = pitch;
        }

        double differenceZ = sin(pitch) * bodyHalfY;
        double distanceY = cos(pitch) * bodyHalfY;
        double shoulderHeight = hlen + differenceZ;
        double differenceY = Y + (bodyHalfY - distanceY);

        if (front == 1){
            differenceY = differenceY*(-1);
        }

        double shoulderAngle1 = atan(differenceY/shoulderHeight);
        double legLen1 = shoulderHeight/cos(shoulderAngle1);

        double shoulderAngle2 = pitch+shoulderAngle1;

        if (front == 1){
            shoulderAngle2 = shoulderAngle2*(-1);
        }

        double shoulderAngle2Deg = shoulderAngle2*(180/PI);

        hlen = legLen1*cos(shoulderAngle2);
        Y = legLen1*sin(shoulderAngle2);

        if (Y == 0){
            Y = 0.1;
        }

        double shoulderOffset = atan(Y / hlen);
        double shoulderOffsetDeg = shoulderOffset *(180/PI);
        double legLen = Y / sin(shoulderOffset);
        double kneeAngle = acos(((pow(upArm,2))+(pow(downArm,2))-(pow(legLen,2)))/(2*upArm*downArm));
        double shoulderAngle = acos(((pow(legLen,2))+(pow(upArm,2))-(pow(downArm,2)))/(2*legLen*upArm));
        double kneeAngleDeg = kneeAngle * (180/PI);
        double shoulderAngleDeg = shoulderAngle * (180/PI) + shoulderOffsetDeg;

        

        if (side == 1 && front == 1){
          servo7.write(kneeAngleDeg);
          double s7x = servo_Map(shoulderAngleDeg - 15,90,0,0,90);
          servo3.write(s7x); //shoulder angle
        }
        else if (side == 1 && front == 0){
          servo5.write(kneeAngleDeg+20);
          double s9x = servo_Map(shoulderAngleDeg - 30,90,0,0,90);
          servo1.write(s9x);
        }
        else if (side == 0 && front == 1){
          double s11x = servo_Map(kneeAngleDeg,0,180,180,0);
          servo8.write(s11x);
          double s8x = servo_Map(shoulderAngleDeg - 15,90,0,180,90);
          servo4.write(s8x);
        }
        else if (side == 0 && front == 0){
          double s6x = servo_Map(kneeAngleDeg,0,180,180,0);
          servo6.write(s6x);
          double s10x = servo_Map(shoulderAngleDeg - 15,90,0,180,90);
          servo2.write(s10x);
        }
}



void setup() {
  Serial.begin(115200);
  espData.begin(115200); 

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
  servo1.attach(0); 
  servo2.attach(1);
  servo3.attach(2);
  servo4.attach(3);
  servo5.attach(4);
  servo6.attach(5);
  servo7.attach(6);
  servo8.attach(7);
  delay(1000);
}

void loop() {
//   while (true) {
//   if(espData.available() > 0)  {  
//     data = espData.read();
//     Serial.println("1.");
//    Serial.println(data);
//    Serial.print("\n \n");
//     if (data == 168430090){
//       continue;
//     }
//     else {
//       break;
//     }
//     }
//   }
//     if (data != 168430090){
//      
//       int finDat = dataMap(data);
//       Serial.println("2.");
//       Serial.println(finDat);
//       Serial.print("\n \n");
//
//    
//
//     if (finDat == 1 && hLen < 8) {
//       hLen += 2;
//     }
//     else if (finDat == 2 && hLen > 6) {
//       hLen -= 2;
//     }
//     else if (finDat == 3 && Y < 4) {
//       Y += 5;
//     }
//     else if (finDat == 4 && Y > -4) {
//       Y -= 5;
//     }
//     else if (finDat == 5 && pitch < 40) {
//       pitch += 5;
//     }
//     else if (finDat == 6 && pitch > -40) {
//       pitch -= 5;
//     } 
//     else if (finDat == 7) {
//       motCntr = 1;
//     } 
//     else if (finDat == 8 && motCntr > -1) {
//       motCntr = 0;
//     } 
//     }  
  

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  int xAng = map(AcX,minVal,maxVal,-90,90);
  int yAng = map(AcY,minVal,maxVal,-90,90);
  int zAng = map(AcZ,minVal,maxVal,-90,90);
   
  x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
  z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
   
  // Serial.print("AngleX= ");
  // Serial.println(x);
   
  // Serial.print("AngleY= ");
  // Serial.println(y);
   
  // Serial.print("AngleZ= ");
  // Serial.println(z);

//Serial.print("Mode: ");
//Serial.println(mode);


          //For movement


        if (gF == 0 && n > 100) {
          l1z = zMot(1);
          l2z = zMot(6);
          l3z = zMot(5);
          l4z = zMot(4);
          gF = 1;
        }

        else if (gF == 1 && n > 100){
          l1z = zMot(1);
          l2z = zMot(6);
          l3z = zMot(5);
          l4z = zMot(4);
          

          l4y = yMot(4, l4y, 1);
          gF = 2;

        }
        
        else if (gF == 2 && n > 100){
          l1z = zMot(2);
          l2z = zMot(6);
          l3z = zMot(5);
          l4z = zMot(4);

          l1y = yMot(2, l1y, 1);
          gF = 3;

        }
        
        else if (gF == 3 && n > 100){
          l1z = zMot(3);
          l2z = zMot(1);
          l3z = zMot(6);
          l4z = zMot(5);

          l1y = yMot(3, l1y, 1);
          l2y = yMot(1, l2y, 1);
          l3y = yMot(6, l3y, 1);
          l4y = yMot(5, l4y, 1);
          gF = 4;

        }
        
        else if (gF == 4 && n > 100){
          l1z = zMot(4);
          l2z = zMot(1);
          l3z = zMot(6);
          l4z = zMot(5);

          l1y = yMot(4, l1y, 1);
          gF = 5;
        }
        
        else if (gF == 5 && n > 100){
          l1z = zMot(4);
          l2z = zMot(2);
          l3z = zMot(6);
          l4z = zMot(5);

          l2y = yMot(2, l2y, 1);
          gF = 6;
        }
        
        else if (gF == 6 && n > 100){
          l1z = zMot(5);
          l2z = zMot(3);
          l3z = zMot(1);
          l4z = zMot(6);

          l1y = yMot(5, l1y, 1);
          l2y = yMot(3, l2y, 1);
          l3y = yMot(1, l3y, 1);
          l4y = yMot(6, l4y, 1);
          gF = 7;
        }
        
        else if (gF == 7 && n > 100){
          l1z = zMot(5);
          l2z = zMot(4);
          l3z = zMot(1);
          l4z = zMot(6);

          l2y = yMot(4, l2y, 1);
          gF = 8;
        }
        
        else if (gF == 8 && n > 100){
          l1z = zMot(5);
          l2z = zMot(4);
          l3z = zMot(2);
          l4z = zMot(6);

          l3y = yMot(2, l3y, 1);
          gF = 9;
        }
        
        else if (gF == 9 && n >100){
          l1z = zMot(6);
          l2z = zMot(5);
          l3z = zMot(3);
          l4z = zMot(1);

          l1y = yMot(6, l1y, 1);
          l2y = yMot(5, l2y, 1);
          l3y = yMot(3, l3y, 1);
          l4y = yMot(1, l4y, 1);
          gF = 10;
        }
        
        else if (gF == 10 && n > 100){
          l1z = zMot(6);
          l2z = zMot(5);
          l3z = zMot(4);
          l4z = zMot(1);

          l3y = yMot(4, l3y, 1);
          gF = 11;
        }
        
        else if (gF == 11 && n > 100){
          l1z = zMot(6);
          l2z = zMot(5);
          l3z = zMot(4);
          l4z = zMot(2);

          l4y = yMot(2, l4y, 1);
          gF = 12;
        }
        
        else if (gF == 12 && n > 100){
          l1z = zMot(1);
          l2z = zMot(6);
          l3z = zMot(5);
          l4z = zMot(3);

          l1y = yMot(1, l1y, 1);
          l2y = yMot(6, l2y, 1);
          l3y = yMot(5, l3y, 1);
          l4y = yMot(3, l4y, 1);
          gF = 1;
        }


//
//  leg(l2z, 0, pitch,0,0);
//  leg(l1z, 0, pitch,0,1);
//  leg(l4z, 0, pitch,1,0);
//  leg(l3z, 0, pitch,1,1);

//
leg(l2z, l2y, pitch,0,0);
  leg(l1z, l1y, pitch,0,1);
  leg(l4z, l4y, pitch,1,0);
  leg(l3z, l3y, pitch,1,1);
  n = n+1;



//else if (motCntr == 0){ 
//  leg(hLen, Y, pitch,0,0);
//  leg(hLen, Y, pitch,0,1);
//  leg(hLen, Y, pitch,1,0);
//  leg(hLen, Y, pitch,1,1);
//}
}
