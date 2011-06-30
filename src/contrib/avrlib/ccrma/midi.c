//  Midi.c
//
//  Midi output routines for the atmel atmega163 (and others)
//  
//  depends on avrlib for buffer
//  

#include "uart.h"
#include "midi.h"
//#include "debug.h"


void midiInit() {
	uartInit();
	uartSetBaudRate(MIDI_BAUD_RATE);
}

// send a midi NOTE ON message from the uart of the form [0x9n, note, vel]
// where n is the midi channel from 0-F, note and vel are 7-bit numbers
u08 midiNoteOnOut(u08 note, u08 vel, u08 channel) {
	uartSendByte(MIDI_NOTE_ON | (channel & MIDI_CHANNEL_MASK));
	uartSendByte(MIDI_DATA_MASK & note);
	uartSendByte(MIDI_DATA_MASK & vel);

	return 0;
}

// send a midi NOTE OFF  message from the uart of the form [0x8n, note, vel]
// where n is the midi channel from 0-F, note and vel are 7-bit numbers
u08 midiNoteOffOut(u08 note, u08 vel, u08 channel) {
	uartSendByte(MIDI_NOTE_OFF | (channel & MIDI_CHANNEL_MASK));
	uartSendByte(MIDI_DATA_MASK & note);
	uartSendByte(MIDI_DATA_MASK & vel);

	return 0;
}

// send a midi CONTROL CHANGE message from the uart of the form [0xBn, controller, value]
// where n is the midi channel from 0-F, controller and value are 7-bit numbers
u08 midiControlChangeOut(u08 controller, u08 value, u08 channel) {
	uartSendByte(MIDI_CONTROL_CHANGE | (channel & MIDI_CHANNEL_MASK));
	uartSendByte(MIDI_DATA_MASK & controller);
	uartSendByte(MIDI_DATA_MASK & value);

	return 0;
}

// send a midi PROGRAM CHANGE message from the uart of the form [0xCn, program]
// where n is the midi channel from 0-F, program is a 7-bit number
u08 midiProgramChangeOut(u08 program, u08 channel) {
	uartSendByte(MIDI_PROGRAM_CHANGE | (channel & MIDI_CHANNEL_MASK));
	uartSendByte(MIDI_DATA_MASK & program);

	return 0;
}

// send a midi POLYPHONIC AFTERTOUCH message from the uart of the form [0xCn, controller, value]
// where n is the midi channel from 0-F, note and pressure are 7-bit numbers
u08 midiPolyTouchOut(u08 note, u08 pressure, u08 channel) {
	uartSendByte(MIDI_POLY_TOUCH | (channel & MIDI_CHANNEL_MASK));
	uartSendByte(MIDI_DATA_MASK & note);
	uartSendByte(MIDI_DATA_MASK & pressure);

	return 0;
}

// send a midi CHANNEL AFTERTOUCH message from the uart of the form [0xDn, pressure]
// where n is the midi channel from 0-F, and pressure is a 7-bit number
u08 midiChannelTouchOut(u08 pressure, u08 channel) {
	uartSendByte(MIDI_CHANNEL_TOUCH | (channel & MIDI_CHANNEL_MASK));
	uartSendByte(MIDI_DATA_MASK & pressure);

	return 0;
}

// send a midi PITCH BEND message from the uart of the form [0xEn, bendLSB, bendMSB ]
// where n is the midi channel from 0-F, and bendLSB and bendMSB are 7-bit numbers
// note that MIDI devices normally pack together bendLSB and bendMSB to make a 14-bit number
u08 midiPitchBendOut(u08 bendLSB, u08 bendMSB, u08 channel) {
	uartSendByte(MIDI_PITCH_BEND | (channel & MIDI_CHANNEL_MASK));
	uartSendByte(MIDI_DATA_MASK & bendLSB);
	uartSendByte(MIDI_DATA_MASK & bendMSB);
	return 0;
}
