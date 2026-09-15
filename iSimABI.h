#pragma once
#include <stdint.h>
#define dllExport __declspec(dllexport)

extern "C" {

  //lifeCycle Functions
  dllExport void* JSBSim_Create();
  dllExport void JSBSim_Destroy(void* ptr);


  //Loading Functions
  dllExport bool JSBSim_SetDirectories(void* ptr, const char* rootDir, const char* aircraftDir, const char* engineDir, const char* systemsDir);
  dllExport bool JSBSim_LoadModel(void* ptr, const char* modelPath);

  //Setup Functions
  dllExport bool JSBSim_SetInitialCondition(void* ptr, double lat, double lon, double altft, double hdgTrue, double kts);
  dllExport bool JSBSim_RunIC(void* ptr);

  //Simulation Functions
  dllExport bool JSBSim_FlightLoop(void* ptr);

  //Property Accessors
  dllExport bool JSBSim_SetPropertyValue(void* ptr, const uint32_t propertyID, double value);
  dllExport double JSBSim_GetPropertyValue(void* ptr, const uint32_t propertyID);
  dllExport uint32_t JSBSim_PreFetch(void* ptr, const char* propertyPath);

}

struct JSB_Functions{
  decltype(&JSBSim_Create)              create;
  decltype(&JSBSim_Destroy)             destroy;

  decltype(&JSBSim_SetDirectories)      setDirectories;
  decltype(&JSBSim_LoadModel)           loadModel;

  decltype(&JSBSim_SetInitialCondition) setInitialCondition;
  decltype(&JSBSim_RunIC)               runIC;

  decltype(&JSBSim_FlightLoop)          flightLoop;

  decltype(&JSBSim_SetPropertyValue)         setPropertyValue;
  decltype(&JSBSim_GetPropertyValue)         getPropertyValue;
  decltype(&JSBSim_PreFetch)              preFetch;

};