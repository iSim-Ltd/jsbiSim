#pragma once
extern "C" {

  __declspec(dllexport) void* JSBSim_Create();
  __declspec(dllexport) void JSBSim_Destroy(void* ptr);
  __declspec(dllexport) bool JSBSim_LoadModel(void* ptr, const char* modelPath);
  __declspec(dllexport) bool JSBSim_FirstRun(void* ptr);
  __declspec(dllexport) bool JSBSim_FlightLoop(void* ptr);

}