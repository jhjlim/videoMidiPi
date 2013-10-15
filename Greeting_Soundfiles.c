// Front Door Greeting Program 
// Andrew Ikenberry
// andrewikenberry@gmail.com
// August 2013

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "portmidi.h"

//#define ANDREW 1
//#define JASON 2

int main(int argc, const char * argv[])
{
    int i, numDevices, selectedDeviceIndex;
    long channel;
    const PmDeviceInfo *info;
    PortMidiStream *stream;
    PmEvent event;
    
    Pm_Initialize();
    
    numDevices = Pm_CountDevices();
    if (numDevices == 0) {
        printf("No MIDI devices found.\n");
        return 0;
    }
    for (i = 0; i < numDevices; i++) {
        info = Pm_GetDeviceInfo(i);
        if (info->input) {
            printf("%d: %s\n", i, info->name);
        }
    }
    
    printf("Select the MIDI device: ");
    scanf("%d", &selectedDeviceIndex);
    info = Pm_GetDeviceInfo(selectedDeviceIndex);
    printf("You chose \"%s\".\n", info->name);
    
    channel = -1;
    while (channel < 1 || channel > 16) {
        printf("Please select a MIDI channel: ");
        scanf("%ld", &channel);
        if (channel < 1 || channel > 16) {
            printf("Invalid channel.\n");
        }
    }
    channel--;
    
    Pm_OpenInput(&stream,
                 selectedDeviceIndex,
                 NULL,
                 8192,
                 NULL,
                 NULL);
    
    printf("Setup complete! Waiting for MIDI data...\n");
    while (1) {
        
        long status, data1, data2;//, AndrewCC=1, AndrewVal, JasonCC=2, JasonVal, TriggerCC=3, TriggerVal;
        int length, person, audioFile;
	//const char *AndrewGreetings[] = {"aplay Andrew_1.wav", "aplay Andrew_2.wav", "aplay Andrew_3.wav", "aplay Andrew_4.wav", "aplay Andrew_5.wav"};
	//const char *JasonGreetings[] = {"aplay Jason_1.wav", "aplay Jason_2.wav", "aplay Jason_3.wav", "aplay Jason_4.wav", "aplay Jason_5.wav"};
       	 
        length = Pm_Read(stream, &event, sizeof(long));
        if (length > 0) {
            
            status = Pm_MessageStatus(event.message);
            data1 = Pm_MessageData1(event.message);
            data2 = Pm_MessageData2(event.message);
            
            if (status == (0x80 + channel)) {
                printf("Note off: %ld, %ld\n", data1, data2);
            } else if (status == (0x90 + channel)) {
                printf("Note on: %ld, %ld\n", data1, data2);
            } else if (status == (0xB0 + channel)) {
                printf("CC: %ld, %ld\n", data1, data2);
            }
        }
/*
       if (data1 == TriggerCC)
		TriggerVal = data2;
	else if (data1 == AndrewCC)
		AndrewVal = data2;
	else if (data1 == JasonCC)
		JasonVal = data2;
	if (AndrewVal == 127)
		person = 1;
	if (JasonVal == 127)
		person = 2;

	// Play one of Andrew's Greetings
	if (person == ANDREW && TriggerVal == 127) {

		audioFile = (random()%5)+1;
		system(AndrewGreetings[audioFile]);

		TriggerVal = 0;
		data1 = 0;
		data2 = 0;
		
	}

	// Play one of Jason's Greetings
	if (person == JASON && TriggerVal == 127) {

		audioFile = (random()%5)+1;
		system(JasonGreetings[audioFile]);

		TriggerVal = 0;
		data1 = 0;
		data2 = 0;
	}

        sleep(0.050);
    }
  */  
}
    Pm_Close(stream);
    
    Pm_Terminate();
    
    return 0;
}

