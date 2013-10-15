
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "portmidi.h"


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
        
        long status, data1, data2;
        int length, person, audioFile;
       	 
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
}
    Pm_Close(stream);
    
    Pm_Terminate();
    
    return 0;
}

