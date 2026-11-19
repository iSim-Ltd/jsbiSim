#pragma once
#include <stdint.h>

#define dllExport __declspec(dllexport)

extern "C" {

  struct modelInfo{
    uint32_t numEngines;
    uint32_t numTanks;
    uint32_t engineType;
    uint32_t numGear;
    uint32_t numContacts;
    uint8_t success;
  };

  struct groundContact{
    double agl;
    double normalX;
    double normalY;
    double normalZ;
  };

  // Callback implemented on the host (X-Plane plugin) side.
// Called synchronously by JSBSim's ground callback with  lat/lon (degrees)
// of the specific contact point being queried; host fills in *outResult
// (e.g. from a main-thread-updated, at-most-one-frame-old sample cache).
  typedef void (*GroundContactCallback)(double latitudeDeg, double longitudeDeg, groundContact* outResult);


  //lifeCycle Functions
  dllExport void* JSBSim_Create();
  dllExport void JSBSim_Destroy(void* ptr);


  //Loading Functions
  dllExport bool JSBSim_SetDirectories(void* ptr, const char* rootDir, const char* aircraftDir, const char* engineDir, const char* systemsDir);
  dllExport modelInfo JSBSim_LoadModel(void* ptr, const char* modelPath);

  //Setup Functions
  dllExport bool JSBSim_SetInitialCondition(void* ptr, double lat, double lon, double altft, double hdgTrue, double kts);
  dllExport bool JSBSim_RunIC(void* ptr);

  //Simulation Functions
  dllExport bool JSBSim_FlightLoop(void* ptr);

  //Property Accessors
  dllExport bool JSBSim_SetPropertyValue(void* ptr, const uint32_t propertyID, double value);
  dllExport double JSBSim_GetPropertyValue(void* ptr, const uint32_t propertyID);
  dllExport uint32_t JSBSim_PreFetch(void* ptr, const char* propertyPath);

  dllExport void JSBSim_SetGroundCallback(void* ptr, GroundContactCallback callback);

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
  decltype(&JSBSim_SetGroundCallback) setGroundCallback;
};