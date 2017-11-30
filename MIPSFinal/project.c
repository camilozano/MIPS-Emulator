// MIPS Simulator project, MySPIM
// Group members: Deepak Kumar Gunturu, Camilo Lozano
#include "spimcore.h"

/* ALU */
/* 10 Points */
// Function written by Deepak Kumar Gunturu
// PID: 3924312

void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero)
{
	// If control is 000, A + B is stored in the result
	if (ALUControl == 0)
	{
		*ALUresult = A + B;
	}

	// If control is 001, A - B is stored in the result
	else if (ALUControl == 1)
	{
		*ALUresult = A - B;
	}

	// If control is 010, A and B are compared
	// If A < B, the result stores 1. If not, the result stores 0
	// A and B are type-casted to integers and compared in this function
	else if (ALUControl == 2)
	{
		*ALUresult = ((int)A < (int)B) ? 1 : 0;
	}

	// If control is 011, A and B are compared
	// If A < B, the result stores 1. If not, the result stores 0
	// In this case, the unsigned values of A and B are compared
	else if (ALUControl == 3)
	{
		*ALUresult = (A < B) ? 1 : 0;
	}

	// If control is 100, the AND operation is performed between A and B
	// The result of this AND operation is stored in the result
	else if (ALUControl == 4)
	{
		*ALUresult = A & B;
	}

	// If control is 101, the OR operation is performed between A and B
	// The result of this OR operation is stored in the result
	else if (ALUControl == 5)
	{
		*ALUresult = A | B;
	}

	// If control is 110, B is shifted to the left by 16 
	else if (ALUControl == 6)
	{
		*ALUresult = B << 16;
	}

	// If control is 111, the negation of A is stored in the result
	else if (ALUControl == 7)
	{
		*ALUresult = ~A;
	}

	// After every control condition is examined and if the result is 0, the Zero is 1
	if (*ALUresult == 0)
	{
		*Zero = 1;
	}

	// After every control condition is examined and if the result is 1, the Zero is 0
	else if (*ALUresult == 1)
	{
		*Zero = 0;
	}
}

/* instruction fetch */
/* 10 Points */
// Function written by Deepak Kumar Gunturu
// PID: 3924312
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction)
{
	// When the halt condition takes place, the function returns 1
	if (PC % 4 != 0)
		return 1;

	// If the halt condition does not occur, the instruction addressed by PC from Mem is written into the variable instruction 
	*instruction = *(Mem + (PC >> 2));
	return 0;
}

/* instruction partition */
/* 10 Points */
// Function written by Deepak Kumar Gunturu
// PID: 3924312
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	*op = (instruction >> 26) & 0x0000003f;
	*r1 = (instruction >> 21) & 0x1f;
	*r2 = (instruction >> 16) & 0x1f;
	*r3 = (instruction >> 11) & 0x1f;
	*funct = (instruction & 0x0000003f);
	*offset = (instruction & 0x0000ffff);
	*jsec = (instruction & 0x03ffffff);
}

/* instruction decode */
/* 15 Points */
// Function written by Deepak Kumar Gunturu
// PID: 3924312
int instruction_decode(unsigned op, struct_controls *controls)
{
	// Initializing each of the controls to 0
	controls->RegDst = 0;
	controls->Jump = 0;
	controls->Branch = 0;
	controls->MemRead = 0;
	controls->MemtoReg = 0;
	controls->ALUOp = 0;
	controls->MemWrite = 0;
	controls->ALUSrc = 0;
	controls->RegWrite = 0;

	// Op code for R type instruction
	if (op == 0)
	{
		controls->RegDst = 1;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 7;
		controls->MemWrite = 0;
		controls->ALUSrc = 0;
		controls->RegWrite = 1;
	}

	// Op code for jump type instructions
	else if (op == 2)
	{
		controls->RegDst = 0;
		controls->Jump = 1;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 0;
		controls->MemWrite = 0;
		controls->ALUSrc = 0;
		controls->RegWrite = 0;
	}

	// Op code for when a branch is equal
	else if (op == 4)
	{
		controls->RegDst = 2;
		controls->Jump = 0;
		controls->Branch = 1;
		controls->MemRead = 0;
		controls->MemtoReg = 2;
		controls->ALUOp = 1;
		controls->MemWrite = 0;
		controls->ALUSrc = 0;
		controls->RegWrite = 0;
	}

	// Op code for addi
	else if (op == 8)
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 0;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
	}

	// Op code for shift left immediate
	else if (op == 10)
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 2;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
	}

	// Op code for shift left immediate unsigned
	else if (op == 11)
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 3;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
	}

	// Op code for load unsigned word immediate
	else if (op == 15)
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 6;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
	}

	// Op code for load word
	else if (op == 35)
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 1;
		controls->MemtoReg = 1;
		controls->ALUOp = 0;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
	}

	// Op code for store word
	else if (op == 43)
	{
		controls->RegDst = 2;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 2;
		controls->ALUOp = 0;
		controls->MemWrite = 1;
		controls->ALUSrc = 1;
		controls->RegWrite = 0;
	}

	// If the halt condition occurs
	else
	{
		return 1;
	}

	// If the halt condition does not occur
	return 0;
}

/* Read Register */
/* 5 Points */
// Function written by Deepak Kumar Gunturu
// PID: 3924312
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2)
{
	// Reading register from the offset r1 of Reg into data1
	*data1 = *(Reg + r1);

	// Reading register from the offset r2 of Reg into data2
	*data2 = *(Reg + r2);
}


/* Sign Extend */
/* 10 Points */
/* Camilo 3794895*/
void sign_extend(unsigned offset, unsigned *extended_value)
{
	//Grabs the first digit of the the 16 bit num to check for sign
	if (offset >> 15) *extended_value = offset | 0xFFFF0000;	//If it is one add 1's to the beginning
	else *extended_value = offset & 0x0000FFFF;	//Otherwise adds zeroes
}


/* ALU operations */
/* 10 Points */
/* Camilo 3794895*/
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult, char *Zero)
{
	//ALUSrc multiplexor selects the extended value if it is one
	if (ALUSrc) data2 = extended_value;
	//Changes ALUOp depending on the function code if OP code is an R-type
	if (ALUOp == 7) {
			 if (funct == 32) ALUOp = 0; //Add
		else if (funct == 34) ALUOp = 1; //Sub
		else if (funct == 42) ALUOp = 2; //Sls
		else if (funct == 43) ALUOp = 3; //Slu
		else if (funct == 36) ALUOp = 4; //And
		else if (funct == 37) ALUOp = 5; //Or
		else if (funct ==  6) ALUOp = 6; //Sle16
		else if (funct == 39) ALUOp = 7; //Nor
		else return 1;
	}
	//Run ALU command once ALUOp has been corrected if needed
	ALU(data1, data2, ALUOp, ALUresult, Zero);
	return 0;
}

/* Read / Write Memory */
/* 10 Points */
/* Camilo 3794895*/
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem)
{
	//printf("MemRead: %u MemWrite: %u ALUresult: %u\n", MemRead, MemWrite, ALUresult);
	if ((ALUresult % 4!=0) && (MemRead || MemWrite)) return 1;//Halt if address is not bbyte addressed 
	if (MemRead) *memdata = Mem[ALUresult >> 2];	//Read from memory if MemRead, shift is to make ALUresult byte addressed
	if (MemWrite) Mem[ALUresult >> 2] = data2;		//Write to memory if MemWrite from data2
	return 0;

}


/* Write Register */
/* 10 Points */
/* Camilo 3794895*/
void write_register(unsigned r2, unsigned r3, unsigned memdata, unsigned ALUresult, char RegWrite, char RegDst, char MemtoReg, unsigned *Reg)
{
	unsigned *RegAns = RegDst ? &Reg[r3] : &Reg[r2];		//Multiplexor RegDst chooses which register to write to
	unsigned DataAns = MemtoReg ? memdata : ALUresult;	//Multiplexor MemtoReg chooses which value to store in register
	*RegAns = DataAns;	//Writes to register
}

/* PC update */
/* 10 Points */
/* Camilo 3794895*/
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC)
{
	*PC += 4;	//Always update program counter
	if (Zero && Branch) *PC + extended_value;	// If branch instruction add the extended value
	if (Jump) *PC = (jsec << 2) | (*PC & 0xf0000000);
}