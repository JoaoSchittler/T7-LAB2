//Chip8.hpp
struct chip8 
{
	unsigned short opcode;
	/*
	 Memory map:
		0x000-0x1FF (Font set)
		0x050-0xA0  (For the 4x5 Font set)
		0x200-0xFFF (ROM e RAM)	
	
	*/
	unsigned char memory[4096];//Memória de 4k bytes	
	unsigned char reg_v[16]; // Banco de registradores (V0 -> VF)
	unsigned short i;
	unsigned short pc;

	unsigned char gfx[64*32];
	unsigned short sound_timer;
	unsigned short delay_timer; // 60HZ

	unsigned short stack[16];
	unsigned short sp;

	unsigned char key[16];

	void init();
	// chip8();

	void loadGame(String str);

	void emulate_cycle();

	void draw_flag();
	
	void setKeys();

}

