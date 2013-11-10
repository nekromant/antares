
struct circ_buffer { 
	int head; 
	int tail;
	int size;
	char buffer[];
}


struct console { 
	void (*init)(void);
	
}
