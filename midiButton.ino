// midiButton.ino
int flag = 0;

void setup() {
	Serial.begin(31250);
	pinMode(7, INPUT); 
	pinMode(13, OUTPUT);
}

void loop() {
	if((digitalRead(7) == HIGH) && (flag == 0)){
		digitalWrite(13, HIGH); 
		midiOUT(0xB0, 25, 127);
		delay(10);
		flag = 1;
	}
	if((digitalRead(7) == LOW) && (flag == 1)){
		digitalWrite(13, LOW); 
		midiOUT(0xB0, 25, 0);
		delay(10);
		flag = 0;	    
	}  

}


void midiOUT(char command, char value1, char value2) {
    Serial.write(command);
    Serial.write(value1);
    Serial.write(value2);
}