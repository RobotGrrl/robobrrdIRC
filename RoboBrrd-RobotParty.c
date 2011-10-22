/* RoboBrrd and IRC ::
   :::::::::::::::::::
  
  robotgrrl.com Oct. 6, 2011
  
  Communicate with RoboBrrd through IRC!
  For the Robot Party! Every Thursday @ 8PM ET
  robotgrrl.com/robotparty
  
  Setup: Mini RoboBrrd robot using Google Android 
         ADK board + shield
 
  Uses the IRC and Arduino code from CMD-c && CMD-v
  http://blog.datasingularity.com/?p=148
 
  License: CC BY-SA
  http://creativecommons.org/licenses/by-nc-sa/3.0/
 
*/

#include <Wire.h>
#include <Servo.h>
#include <Streaming.h>

// LEDs
#define  LED3_RED       2
#define  LED3_GREEN     4
#define  LED3_BLUE      3

#define  LED2_RED       5
#define  LED2_GREEN     7
#define  LED2_BLUE      6

#define  LED1_RED       8       // eyes
#define  LED1_GREEN     10      // eyes
#define  LED1_BLUE      9       // eyes

// ADK Related
#define  TOUCH_RECV     14
#define  TOUCH_SEND     15

#define  RELAY1         A0
#define  RELAY2         A1

#define  LIGHT_SENSOR   A2
#define  TEMP_SENSOR    A3

#define  BUTTON1        A6
#define  BUTTON2        A7
#define  BUTTON3        A8

#define  JOY_SWITCH     A9      // pulls line down when pressed
#define  JOY_nINT       A10     // active low interrupt input
#define  JOY_nRESET     A11     // active low reset output

#define  ULTRASONIC     A14     // ultrasonic sensor (plug 2)
#define  ANSWER_SWITCH  A13      // answer switch (plug 2)

byte b1, b2, b3, b4, c;

// Servos
#define  SERVO1         11      // right wing
#define  SERVO2         12      // left wing
#define  SERVO3         13      // beak
#define  SERVO4         27      // rotation

#define  WING_R_UPPER   30;
#define  WING_R_LOWER   90;

#define  WING_L_UPPER   110
#define  WING_L_LOWER   70      // accounts for the ultrasonic sensor height

#define  BEAK_OPEN      140
#define  BEAK_CLOSED    10

Servo servos[4];

// LEDs
int R_start = 0;
int G_start = 0;
int B_start = 0;
int R_pre = 0;
int G_pre = 0;
int B_pre = 0;

bool receivedMessage = false;
int messageLength = 0;

// Commands
char fwCmd[] = {'F', 'W'};
char dance1Cmd[] = {'D', '1'};
char dance2Cmd[] = {'D', '2'};
char blinkCmd[] = {'B', 'L'};
char irCmd[] = {'I', 'R'};
char chirp1Cmd[] = {'C', '1'};
char wordCmd[] = {'W', 'O', 'R', 'D'};
char waveCmd[] = {'W', 'A', 'V'};

// Command handling
int it = 0;
byte newByte = 0;
int said = 0;
char msg[141];


void setup() {
	
	Serial.begin(9600);
    
    // ADK related
	init_leds();
    init_relays();
	init_buttons();
    
    // Servos (attach beak when needed)
    servos[0].attach(SERVO1);
    int p = WING_R_UPPER; // idk why i have to do this for R but not L o_O
	servos[0].write(p);
	servos[1].attach(SERVO2);
	servos[1].write(WING_L_UPPER);
    servos[3].attach(SERVO4);
    servos[3].write(90);
    
    // Buttons
	b1 = digitalRead(BUTTON1);
	b2 = digitalRead(BUTTON2);
	b3 = digitalRead(BUTTON3);
	b4 = digitalRead(JOY_SWITCH);
	c = 0;
    
    // LEDs
    fade2(128, 128, 128,
          128, 128, 128,
          1);
	
}

void loop() {
	
    // Clear out the msg buffer each time
	for(int i=0; i<141; i++) {
		msg[i] = ' ';
	}
	
    // Reset everyone
	it = 0;
	newByte = nextByte();
	receivedMessage = false;
	messageLength = 0;
	
	// If there is some data waiting...
	if(newByte == '*') {
		
		byte byteIn = 0;
		
		// Getting the command details
		while (byteIn != '*') {
			byteIn = nextByte();
			msg[it] = byteIn;
			it++;
		}
		
		// Checking to see if we received the message
		// and seeing how long it is
		if(it>0) {
			receivedMessage = true;
			messageLength = it-1;
		}
		
        // Received a message, let's pair it up with the right
        // command for it
        if(receivedMessage) {
            // Hmm I wonder if there is a way to enumerate this better...
            if(messageLength == sizeof(fwCmd)) checkFwCmd();
            if(messageLength == sizeof(dance1Cmd)) checkDance1Cmd();
            if(messageLength == sizeof(dance2Cmd)) checkDance2Cmd();
            if(messageLength == sizeof(blinkCmd)) checkBlinkCmd();
            if(messageLength == sizeof(irCmd)) checkIRCmd();
            if(messageLength == sizeof(chirp1Cmd)) checkChirp1Cmd();
            if(messageLength == sizeof(wordCmd)) checkWordCmd();
            if(messageLength == sizeof(waveCmd)) checkWaveCmd();
        }
         		
		said++;

	} else {		
		receivedMessage = false;
	}
	
    // Clear everything
	delay(10);
	Serial.flush();
	
}


byte nextByte() { 
    while(1) {
		if(Serial.available() > 0) {
			byte b =  Serial.read();
			//Serial << b;
			return b;
		}
    }
}


// ---------------
// Command Parsing
// ---------------

bool validCmd(char theCmd[]) {
    
    bool valid = true;
    int ii=0;
    
    // Check each letter to see if it's right
    while(validCmd && ii<messageLength) {
        if(msg[ii] != theCmd[ii]) {
            valid = false;
        }
        ii++;
    }
    
    return valid;
}

void checkWordCmd() {
    if(validCmd(wordCmd)) {
        shake(5);
        rightWing(3, 50);
        shake(5);
        leftWing(3, 50);
        shake(5);
        bothWings(10, 50);
        shake(5);
    }
}

void checkFwCmd() {
    if(validCmd(fwCmd)) {
        bothWings(10, 50);
    }
}

void checkDance1Cmd() {
    if(validCmd(dance1Cmd)) {
        leftWing(3, 50);
        shake(3);
        rightWing(3, 50);
        shake(3);
    }
}

void checkDance2Cmd() {
    if(validCmd(dance2Cmd)) {
        bothWings(10, 50);
        shake(5);
        rightWing(3, 50);
    }
}

void checkBlinkCmd() {
    if(validCmd(blinkCmd)) {
        updateLights();
    }
}

void checkIRCmd() {
    if(validCmd(irCmd)) {
        int irval = analogRead(ULTRASONIC);
        Serial << "&";
        if(irval < 10) {
            Serial << "000" << irval;
        } else if(irval < 100) {
            Serial << "00" << irval;
        } else if(irval < 1000) {
            Serial << "0" << irval;
        } else {
            Serial << irval;
        }
    }
}

void checkChirp1Cmd() {
    if(validCmd(chirp1Cmd)) {
        // TODO: Chirp!
    }
}

void checkWaveCmd() {
    if(validCmd(waveCmd)) {
        leftWing(3, 50);
    }
}


// --------------------
// ADK Accessory Shield
// --------------------

void init_buttons() {
	pinMode(BUTTON1, INPUT);
	pinMode(BUTTON2, INPUT);
	pinMode(BUTTON3, INPUT);
	pinMode(JOY_SWITCH, INPUT);
    //pinMode(ANSWER_SWITCH, INPUT);
    
	// enable the internal pullups
	digitalWrite(BUTTON1, HIGH);
	digitalWrite(BUTTON2, HIGH);
	digitalWrite(BUTTON3, HIGH);
	digitalWrite(JOY_SWITCH, HIGH);
    //digitalWrite(ANSWER_SWITCH, HIGH);
}

void init_relays() {
	pinMode(RELAY1, OUTPUT);
	pinMode(RELAY2, OUTPUT);
}

void init_leds() {
	digitalWrite(LED1_RED, 1);
	digitalWrite(LED1_GREEN, 1);
	digitalWrite(LED1_BLUE, 1);
    
	pinMode(LED1_RED, OUTPUT);
	pinMode(LED1_GREEN, OUTPUT);
	pinMode(LED1_BLUE, OUTPUT);
    
    analogWrite(LED1_RED, 128);
    analogWrite(LED1_GREEN, 128);
    analogWrite(LED1_BLUE, 128);
    
	digitalWrite(LED2_RED, 1);
	digitalWrite(LED2_GREEN, 1);
	digitalWrite(LED2_BLUE, 1);
    
	pinMode(LED2_RED, OUTPUT);
	pinMode(LED2_GREEN, OUTPUT);
	pinMode(LED2_BLUE, OUTPUT);
    
	digitalWrite(LED3_RED, 1);
	digitalWrite(LED3_GREEN, 1);
	digitalWrite(LED3_BLUE, 1);
    
	pinMode(LED3_RED, OUTPUT);
	pinMode(LED3_GREEN, OUTPUT);
	pinMode(LED3_BLUE, OUTPUT);
}


// -----------------------
// Mini RoboBrrd Movements
// -----------------------

void shake(int repeat) {
    
    for(int j=0; j<repeat; j++) {
        
        for(int i=90; i>60; i--) {
            servos[3].write(i);
        }
        delay(100);
        
        for(int i=60; i<120; i++) {
            servos[3].write(i);
        }
        delay(100);
        
        for(int i=120; i>90; i--) {
            servos[3].write(i);
        }
        delay(10);
        
    }
}

void leftWing(int repeat, int speed) {
    
    for(int j=0; j<repeat; j++) {
        
        for(int i=WING_L_LOWER; i<WING_L_UPPER; i++) {
            servos[1].write(i);
        }
        delay(speed);
        
        for(int i=WING_L_UPPER; i>WING_L_LOWER; i--) {
            servos[1].write(i);
        }
        delay(speed);
        
    }
}

void rightWing(int repeat, int speed) {
    
    int l = WING_R_LOWER;
    int u = WING_R_UPPER;
    
    for(int j=0; j<repeat; j++) {
        
        for(int i=u; i<l; i++) {
            servos[0].write(i);
        }
        delay(speed);
        
        for(int i=l; i>u; i--) {
            servos[0].write(i);
        }
        delay(speed);
        
    }
}

void bothWings(int repeat, int speed) {
    
    int rl = WING_R_LOWER;
    int ll = WING_L_LOWER;
    
    for(int j=0; j<repeat; j++) {
        
        for(int i=0; i<40; i++) {
            servos[0].write(rl-20-i);
            servos[1].write(ll+i);
        }
        delay(speed);
        
        for(int i=40; i>0; i--) {
            servos[0].write(rl-20-i);
            servos[1].write(ll+i);
        }
        delay(speed);
        
    }
}

void openBeak(int speed, int step) {
    
    int b = BEAK_OPEN;
    int currentPos = servos[2].read();
    
    servos[2].attach(SERVO3);
    
    if(currentPos > b) {
        for(int i=currentPos; i>b; i-=step) {
            servos[2].write(i);
            delay(speed);
        }
    } else {
        for(int i=currentPos; i<b; i+=step) {
            servos[2].write(i);
            delay(speed);
        }
    }
    
    servos[2].detach();
}

void closeBeak(int speed, int step) {
    
    int b = BEAK_CLOSED;
    int currentPos = servos[2].read();
    
    servos[2].attach(SERVO3);
    
    if(currentPos > b) {
        for(int i=currentPos; i>b; i-=step) {
            servos[2].write(i);
            delay(speed);
        }
    } else {
        for(int i=currentPos; i<b; i+=step) {
            servos[2].write(i);
            delay(speed);
        }
    }
    
    servos[2].detach();
}

void updateLights() {
    
    R_start = int(random(50, 255));
    G_start = int(random(50, 255));
    B_start = int(random(50, 255));
    
    fade2( R_pre,    G_pre,      B_pre, 
          R_start,  G_start,    B_start, 
          1);
    
    R_pre = R_start;
    G_pre = G_start;
    B_pre = B_start;
}

void fade2 (int start_R,  int start_G,  int start_B, 
            int finish_R, int finish_G, int finish_B,
            int stepTime) {
    
    int skipEvery_R = 256/abs(start_R-finish_R); 
    int skipEvery_G = 256/abs(start_G-finish_G);
    int skipEvery_B = 256/abs(start_B-finish_B);
    
    for(int i=0; i<256; i++) {
        
        if(start_R<finish_R) {
            if(i<=finish_R) {
                if(i%skipEvery_R == 0) {
                    analogWrite(LED1_RED, i);
                } 
            }
        } else if(start_R>finish_R) {
            if(i>=(256-start_R)) {
                if(i%skipEvery_R == 0) {
                    analogWrite(LED1_RED, 256-i); 
                }
            } 
        }
        
        if(start_G<finish_G) {
            if(i<=finish_G) {
                if(i%skipEvery_G == 0) {
                    analogWrite(LED1_GREEN, i);
                } 
            }
        } else if(start_G>finish_G) {
            if(i>=(256-start_G)) {
                if(i%skipEvery_G == 0) {
                    analogWrite(LED1_GREEN, 256-i); 
                }
            } 
        }
        
        if(start_B<finish_B) {
            if(i<=finish_B) {
                if(i%skipEvery_B == 0) {
                    analogWrite(LED1_BLUE, i);
                } 
            }
        } else if(start_B>finish_B) {
            if(i>=(256-start_B)) {
                if(i%skipEvery_B == 0) {
                    analogWrite(LED1_BLUE, 256-i); 
                }
            } 
        }
        
        delay(stepTime);
        
    }
}
