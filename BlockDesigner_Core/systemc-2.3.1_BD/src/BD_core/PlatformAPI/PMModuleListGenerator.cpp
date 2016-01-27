//-----------------------------------------------------------------------------
// Design								: json file generator for sc_module List 
// Author								: Bryan Choi 
// Email								: bryan.choi@twoblocktech.com 
// File		     					: PMModuleListGenerator.cpp
// Date	       					: 2016/1/19
// Reference            :
// ----------------------------------------------------------------------------
// Copyright (c) 2015-2016 TwoBlock Techinologies Co.
// ----------------------------------------------------------------------------
// Description	: get sc_module list and make json formate file 
// ----------------------------------------------------------------------------

#include "PMModuleListGenerator.h"
#include "../SimulationAPI/BDDITypes.h"
#include <string.h>
#include <stdlib.h>
#include "json/json.h"
#include <iostream>
#include <vector>

#define Size 128

namespace BDapi
{
	/*
	 * function    	: GenerateJsonFile
	 * design	      :  
	 * param        : list<sc_module*> - sc_module list to parse informations
	 *                ( module name, type, port list, parameters ) 
	 * return       : string - string(json format) to pass them to GUI 
	 * caller		    : PMModuleListManager::GetOperationControl
	 */
	string PMModuleListGenerator::GenerateJsonFile(list<sc_module*> SCModuleList)
	{
		// json format entities
		Json::Value Root;
		Json::Value PMModuleList;
		Json::Value Module;
		Json::Value PortList;
		Json::Value Port;
		Json::Value ParameterList;
		Json::Value Parameter;
		Json::Value RegisterList;
		Json::Value Register;

		// Port list pointer from sc_module
		std::vector<sc_port_base*>* p_PortList = NULL;

		// variable to parse port list
		char a_PortTypeInfo[Size] = {0,};
		const char *p_PortName= NULL;
		const char *p_ScPortType = NULL;
		const char *p_DataType = NULL;

		char a_Temp[1024] = {0,};
		unsigned int dw_Index = 0;
		unsigned int dw_ParNum = 0;
		unsigned int dw_RegNum = 0;

		list<sc_module*>::iterator FirstModule = SCModuleList.begin();
		list<sc_module*>::iterator LastModule = SCModuleList.end();
		list<sc_module*>::iterator IndexOfModule = FirstModule;

		/********************************************
		 * Iterate sc_modules in sc_module list
		 ********************************************/
		for(IndexOfModule = FirstModule; IndexOfModule != LastModule; ++IndexOfModule){   

			p_PortList = (*IndexOfModule)->get_port_list();	

			std::vector<sc_port_base*>::iterator FirstPort = p_PortList->begin(); 
			std::vector<sc_port_base*>::iterator LastPort = p_PortList->end(); 
			std::vector<sc_port_base*>::iterator IndexOfPort = FirstPort;

			/********************************************
			 * Iterate ports in sc_module
			 ********************************************/
			for(IndexOfPort = FirstPort; IndexOfPort != LastPort; ++IndexOfPort){   
				// parse port information
				strcpy(a_PortTypeInfo, (*IndexOfPort)->kind());
				p_DataType = strtok(a_PortTypeInfo, "\n");
				p_ScPortType = strtok(NULL,"\n");
				p_PortName = (*IndexOfPort)->get_port_name();

				// add Port to PortList in json format
				Port["port_name"] = p_PortName;
				Port["sc_type"]   = p_ScPortType; 
				Port["data_type"] = p_DataType;
				PortList.append(Port);	
			} 
  
			/********************************************
			 * Iterate parameter in sc_module
			 ********************************************/
			dw_ParNum = (*IndexOfModule)->GetBDDI()->BDDIGetModuleTotalParNum();		

			for(dw_Index = 0; dw_Index < dw_ParNum; dw_Index++){

				BDDIParInfo *pst_ParInfo = (*IndexOfModule)->GetBDDI()->BDDIGetModuleParInfo(); 
				Parameter["parameter_name"] = pst_ParInfo[dw_Index].Name;	
				sprintf(a_Temp, "%u", pst_ParInfo[dw_Index].Bitswide);
				Parameter["bits_wide"] = a_Temp;
				sprintf(a_Temp, "%d", (int)pst_ParInfo[dw_Index].Type);	
				Parameter["type"] = a_Temp;
				Parameter["value"] = pst_ParInfo[dw_Index].Value;
				ParameterList.append(Parameter);
			}
	
			/********************************************
			 * Iterate register in sc_module
			 ********************************************/
			dw_RegNum = (*IndexOfModule)->GetBDDI()->BDDIGetModuleTotalRegNum();		

			for(dw_Index = 0; dw_Index < dw_RegNum; dw_Index++){

				BDDIRegInfo *pst_RegInfo = (*IndexOfModule)->GetBDDI()->BDDIGetModuleRegInfo(); 
				Register["register_name"] = pst_RegInfo[dw_Index].Name;	
				sprintf(a_Temp, "%u", pst_RegInfo[dw_Index].Bitswide);
				Register["bits_wide"] = a_Temp;
				sprintf(a_Temp, "%d", (int)pst_RegInfo[dw_Index].Type);	
				Register["type"] = a_Temp;
				RegisterList.append(Register);
			}	

			// add Module to PMModuleList in json format
			Module["module_name"] = (*IndexOfModule)->GetModuleName();
			Module["module_type"] = (*IndexOfModule)->GetBDDI()->BDDIGetModuleType();
			Module["port"] = PortList;
			Module["parameter"] = ParameterList;
			Module["register"] = RegisterList;
			PMModuleList.append(Module);
		}

		Root["PMML"] = PMModuleList;

		Json::StyledWriter writer;
		JsonFileOfPMModuleList = writer.write(Root);
		cout<< endl << "JSON WriteTest" << endl << JsonFileOfPMModuleList << endl; 

		return JsonFileOfPMModuleList;
	}
}
