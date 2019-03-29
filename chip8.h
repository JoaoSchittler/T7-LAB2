//Chip8.cpp
void chip8::init()
{
	// Initialize registers and memory once
	opcode = 0;
	int i;
	for(i=0;i<4096;i++)
	{
		if(i<16)
		{
			reg_v[i]=0;
			stack[i]=0;
			key[i]=0;
		} 
		memory[i] = 0;
	}
}
void chip8::loadGame(String str)
{

	//Load the ROM into memory 


{
void chip8::emulate_cycle()
{
 	// Fetch Opcode
	
  	// Decode Opcode

  	// Execute Opcode
 
  	// Update timers

}

