// KeyGen.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <winnt.h>

static const uint8_t table[] =
"Wj5Lr)A^3hO&[=tIb7U+nCX1|Fl:Ri.Dq9]Ke(s?Vk2N`%zB\\6mH*Tp>Yf0Mx#Z@o"
"4S~Ga,v;QgE_'u8Jc/}Pj<w)UdAy.Lh5^rC{1Ol&Xt=RiF[3|Hb+n7WDq9e"
"Ks(]k2V?`N%zB\\6mT*p>fY0xM#Z@o4~SGav,Q;gEu'_8cJ/}Pw<d)yUAlO1"
"{C^r5h&Xt=RnF|3[bH+iW7Dqe9K]s(k?V`Nz%2Bm6\\Tp*>fxY0M@Z#o~4SaGv,Q;g"
"E_u'8c}P";

void generate_XXXX(uint8_t* XXXX, const uint8_t* guid)
{
	for (int i = 0; i < 4; i++)
	{
		uint8_t  val = 31 ^ guid[4] ^ ((guid[i] + 3) & 0xFF);
		uint8_t  r = val % 93;
		uint8_t  XXXX_i = r + 34;
		if ((uint8_t)(r + 33) < 0x2DU)
		{
			XXXX_i = r + 33;
		}
		
		XXXX[i] = XXXX_i;
	}

	printf("XXXX generated: %c%c%c%c\n\n", XXXX[3], XXXX[2], XXXX[1], XXXX[0]);
}

void generate_YYYY(uint8_t* YYYY, const uint8_t* XXXX)
{
	// We need to sum all XXXX bytes
	uint8_t seed = XXXX[0] + XXXX[1] + XXXX[2] + XXXX[3];
	
	YYYY[0] = table[seed % 256];

	for (int i = 1; i < 4; i++)
	{
		YYYY[i] = table[(seed + YYYY[i - 1]) % 256];
	}

	printf("YYYY generated: %c%c%c%c\n\n", YYYY[3], YYYY[2], YYYY[1], YYYY[0]);
}

void generate_ZZZZ(uint8_t* ZZZZ)
{
	ZZZZ[0] = 70;
	ZZZZ[1] = 72;   
	ZZZZ[2] = 75;   
	ZZZZ[3] = 75;   

	printf("ZZZZ generated: %c%c%c%c\n\n", ZZZZ[3], ZZZZ[2], ZZZZ[1], ZZZZ[0]);
}

void generate_WWWW(uint8_t* WWWW, const uint8_t* XXXX, const uint8_t* YYYY, const uint8_t* ZZZZ)
{

	char Buffer[24] = {0};
	uint32_t value = 0;
	for (int i = 0; i < 4; i++) value = (value * 31 + XXXX[i]);
	for (int i = 0; i < 4; i++) value = (value * 31 + YYYY[i]);
	for (int i = 0; i < 4; i++) value = (value * 31 + ZZZZ[i]);


	uint32_t final_val = value % 10000;

	sprintf_s(Buffer, sizeof(Buffer), "%04u", final_val);

	for (int i = 0; i < 4; i++) WWWW[i] = Buffer[i];

	printf("WWWW generated: %c%c%c%c\n\n", WWWW[3], WWWW[2], WWWW[1], WWWW[0]);
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: KeyGen <GUID>" << std::endl;
		return 1;
	}

	const uint8_t* guid = (const uint8_t*)argv[1];
	printf("Generating serial key from GUID: %s\n", guid);

	uint8_t XXXX[4] = { 0 };
	uint8_t YYYY[4] = { 0 };
	uint8_t ZZZZ[4] = { 0 };
	uint8_t WWWW[4] = { 0 };


	generate_XXXX(XXXX, guid);
	generate_YYYY(YYYY, XXXX);
	generate_ZZZZ(ZZZZ);
	generate_WWWW(WWWW, XXXX, YYYY, ZZZZ);

	printf("Key: %c%c%c%c-%c%c%c%c-%c%c%c%c-%c%c%c%c\n",
		XXXX[0], XXXX[1], XXXX[2], XXXX[3],
		YYYY[0], YYYY[1], YYYY[2], YYYY[3],
		ZZZZ[0], ZZZZ[1], ZZZZ[2], ZZZZ[3],
		WWWW[0], WWWW[1], WWWW[2], WWWW[3]);
}