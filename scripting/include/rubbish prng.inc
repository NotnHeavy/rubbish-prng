//////////////////////////////////////////////////////////////////////////////
// MADE BY NOTNHEAVY. USES GPL-3, AS PER REQUEST OF SOURCEMOD               //
//////////////////////////////////////////////////////////////////////////////

// amongus happy meal on the ibm 701

#if defined __notnheavy_rubbish_prng_
	#endinput
#endif
#define __notnheavy_rubbish_prng_

#include <sourcemod>
#include <sdktools>

//////////////////////////////////////////////////////////////////////////////
// GLOBALS                                                                  //
//////////////////////////////////////////////////////////////////////////////

static const char buffer[] = "\x8B\x44\x24\x08\x0F\xC7\xF8\xC3\x0F\xC7\xF0\xC3";	// rdseed:
																					//   mov eax, [esp/rsp + 0x08]
																					//   rdseed eax
																					//   ret
																					// rdrand:
																					//  rdrand eax


static Handle SDKCall_rdseed;
static Handle SDKCall_rdrand;

enum OSType
{
	Windows,
	Windows64,
	Linux,
	Linux64
}

//////////////////////////////////////////////////////////////////////////////
// INTERNAL                                                                 //
//////////////////////////////////////////////////////////////////////////////

static void AllocateSubroutine(Address address, const char[] subroutine, int maxlength)
{
	for (int i = 0; i < maxlength; ++i)
		StoreToAddress(address + view_as<Address>(i), subroutine[i], NumberType_Int8);
}


//////////////////////////////////////////////////////////////////////////////
// STOCKS                                                                   //
//////////////////////////////////////////////////////////////////////////////

stock void srand(int seed)
{
	// Check if we need to create the subroutines for RDRAND and RDSEED.
	if (!SDKCall_rdseed || !SDKCall_rdrand)
	{
		// Get the image base.
		GameData config = new GameData("rubbish prng");
		Address imageBase = config.GetMemSig("ImageBase");
		OSType os = view_as<OSType>(config.GetOffset("OS"));
		delete config;

		// this works trust me
		Address rdseed;
		Address rdrand;
		if (os == Windows || os == Windows64)
		{
			rdseed = imageBase + view_as<Address>(0x28); // reserved space that can be tampered with
			rdrand = rdseed + view_as<Address>(8);
			AllocateSubroutine(rdseed, buffer, sizeof(buffer) - 1);
		}
		else if (os == Linux || os == Linux64)
		{
			rdseed = imageBase + view_as<Address>(0x04); // DANGEROUS?
			rdrand = rdseed + view_as<Address>(8);
			AllocateSubroutine(rdseed, buffer, sizeof(buffer) - 1);
		}

		// Set up the SDKCalls.
		StartPrepSDKCall(SDKCall_Static);
		PrepSDKCall_SetAddress(rdseed);
		PrepSDKCall_AddParameter(SDKType_PlainOldData, SDKPass_Plain);
		SDKCall_rdseed = EndPrepSDKCall();
		StartPrepSDKCall(SDKCall_Static);
		PrepSDKCall_SetAddress(rdrand);
		PrepSDKCall_SetReturnInfo(SDKType_PlainOldData, SDKPass_Plain);
		SDKCall_rdrand = EndPrepSDKCall();
	}

	// Set the prng seed.
	SDKCall(SDKCall_rdseed, seed);
}

stock int rand()
{
	if (!SDKCall_rdrand)
		return -1;
	return SDKCall(SDKCall_rdrand);
}