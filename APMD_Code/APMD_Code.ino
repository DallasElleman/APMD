//This code has been modified from the original found at https://www.atlas-scientific.com/_files/code/multi-UART-mega-4.pdf 
//Author of modified code: Dallas Elleman serotonin@cox.net, 1/7/2017. Current UART code allows serial input from keyboard, new code will automate the process of command entry
//and allow for automatic initialization of sensors, averaging of data, and output of average pH, EC, ORP and DO readings.

//This sample code was written on an Arduino MEGA 2560.
//It will allow you to control up to 4 Atlas Scientific devices through 1 serial RX/TX line.

int Pin_x = 2;                      //Arduino pin 2 to control pin x
int Pin_y = 3;                      //Arduino pin 3 to control pin y

char computerdata[20];               //A 20 byte character array to hold incoming data from a pc/mac/other 
char sensordata[30];                 //A 30 byte character array to hold incoming data from the sensors
byte computer_bytes_received=0;      //We need to know how many characters bytes have been received         
byte sensor_bytes_received=0;        //We need to know how many characters bytes have been received

char *channel;                       //Char pointer used in string parsing
char *cmd;                           //Char pointer used in string parsing

void setup(){
     Serial.begin(38400);              //set baud rate for the hardware serial port 0 to 38400
     Serial1.begin(38400);             //set baud rate for the hardware serial port 1 to 38400
     pinMode(Pin_x, OUTPUT);           //Set the digital pin as output.
     pinMode(Pin_y, OUTPUT);           //Set the digital pin as output. 
     }
    
void serialEvent(){                                                             //This interrupt will trigger when the data coming from the serial monitor(pc/mac/other) is received   
           computer_bytes_received=Serial.readBytesUntil(13,computerdata,20);   //We read the data sent from the serial monitor(pc/mac/other) until we see a <CR>. We also count how many characters have been received    
           computerdata[computer_bytes_received]=0;                             //We add a 0 to the spot in the array just after the last character we received. This will stop us from transmitting incorrect data that may have been left in the buffer
           }            
       
void serialEvent1(){                                                           //This interrupt will trigger when the data coming from the serial monitor(pc/mac/other) is received   
           sensor_bytes_received=Serial1.readBytesUntil(13,sensordata,30);      //we read the data sent from the Atlas Scientific device until we see a <CR>. We also count how many character have been received    
           sensordata[sensor_bytes_received]=0;                                 //we add a 0 to the spot in the array just after the last character we received. This will stop us from transmitting incorrect data that may have been left in the buffer 
           }        

void loop(){
     
      if(computer_bytes_received!=0){                 //If computer_bytes_received does not equal zero  
        channel=strtok(computerdata, ":");            //Let's parse the string at each colon
        cmd=strtok(NULL, ":");                        //Let's parse the string at each colon 
        open_channel();                               //Call the function "open_channel" to open the correct data path
        Serial1.print(cmd);                           //Send the command from the computer to the Atlas Scientific device using serial port 1 
        Serial1.print("\r");                          //After we send the command we send a carriage return <CR> 
        computer_bytes_received=0;                    //Reset the var computer_bytes_received to equal 0 
      }
  
      else if(sensor_bytes_received!=0){              //If data has been transmitted from an Atlas Scientific device
       Serial.println(sensordata);                    //let’s transmit the data received from the Atlas Scientific device to the serial monitor   
       sensor_bytes_received=0;                       //Reset the var sensor_bytes_received to equal 0 
      } 

      else{
      /*  Serial.println("DO Readings:");
        for (int a=0; a <= 10; a++){                  //Get 10 readings
          DOtest();
          delay(600);      
        }
*/
        Serial.println("EC Readings:");
        for (int a=0; a <= 10; a++){                  //Get 10 readings
          ECtest();
          delay(1500);      
        }
       
      }
}
void DOtest(){
    digitalWrite(Pin_x, HIGH);                        //Set channel to 4
    digitalWrite(Pin_y, HIGH);                        //Set channel to 4
    Serial1.write("r");                               //Give command to perform 1 test
    Serial1.write(13);
    bool sensor_response=false;                       //Haven't received a response yet
    while(sensor_response=false){    
      if(computer_bytes_received!=0){                 //If computer_bytes_received does not equal zero  
        channel=strtok(computerdata, ":");            //Let's parse the string at each colon
        cmd=strtok(NULL, ":");                        //Let's parse the string at each colon 
        Serial1.print(cmd);                           //Send the command from the computer to the Atlas Scientific device using serial port 1 
        Serial1.print("\r");                          //After we send the command we send a carriage return <CR> 
        computer_bytes_received=0;                    //Reset the var computer_bytes_received to equal 0 
        sensor_response=true;
      }
    }
}
    
void ECtest(){
    digitalWrite(Pin_x, HIGH);                        //Set channel to 3
    digitalWrite(Pin_y, LOW);                         //Set channel to 3
    Serial1.write("r");                               //Give command to perform 1 test
    Serial1.write(13);
    bool sensor_response=false;                       //Haven't received a response yet
    while(sensor_response=false){    
      if(computer_bytes_received!=0){                 //If computer_bytes_received does not equal zero  
        channel=strtok(computerdata, ":");            //Let's parse the string at each colon
        cmd=strtok(NULL, ":");                        //Let's parse the string at each colon 
        computer_bytes_received=0;                    //Reset the var computer_bytes_received to equal 0 
        sensor_response=true;
      }
    }
}

 void open_channel(){                               //This function controls what UART port is opened. 
      
     switch (*channel) {                             //Looking to see what channel to open   
   
       case '1':                                     //If *channel==1 then we open channel 1     
         digitalWrite(Pin_x, LOW);                   //Pin_x and pin_y control what channel opens 
         digitalWrite(Pin_y, LOW);                   //Pin_x and pin_y control what channel opens 
       break;                                        //Exit switch case
        
       case '2':
         digitalWrite(Pin_x, LOW);
         digitalWrite(Pin_y, HIGH);
       break;

       case '3':
         digitalWrite(Pin_x, HIGH);
         digitalWrite(Pin_y, LOW);
       break;

       case '4':
         digitalWrite(Pin_x, HIGH);
         digitalWrite(Pin_y, HIGH);
       break;
      }
  
   
}    
















