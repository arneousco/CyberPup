#include <UARTClass.h>

UARTClass espData(1);

int data;  
void setup( )  
{  
  Serial.begin(9600); 
  espData.begin(9600); 
}  

void loop( )  
{  
  if(espData.available() > 0)  {  
    data = espData.read();
    Serial.print("data: ");  
    Serial.println(data);  
    }  
}  
