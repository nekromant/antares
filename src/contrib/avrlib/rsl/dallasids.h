
#ifndef DALLASIDS_H
#define DALLASIDS_H

// subsystem dallas ID structure

typedef struct
{
	int serialnum;
	dallas_rom_id_T V1;
	dallas_rom_id_T S;
	dallas_rom_id_T V2;
	dallas_rom_id_T T;
} DallasSubsysId;

#endif
