#include "ahbl2mem.h"

BDDI* AHBL2MEM::GetBDDI()
{
	return bddi;
}

char* AHBL2MEM::GetModuleName()
{
	return (char*)"AHBL2MEM";
}

void AHBL2MEM::BDInit()
{
	HCLK.set_port_name("HCLK");
	HRESETn.set_port_name("HRESETn");

	AHB_SS = new BD_AHBLitePort_SS((char*)"SS_S0");

	base_addr = 0;
	addr_size = 0x100000;

	memset(memory, 0, sizeof(memory));

	bddi = new AHBL2MEM_BDDI(this);
}

extern "C" sc_module* CreateInstance(const char *ModuleInstanceName)
{
	return new AHBL2MEM(ModuleInstanceName);
}
