/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2019 The Johns Hopkins University Applied Physics Laboratory LLC (JHU/APL).  All Rights Reserved.
//
// This material may be only be used, modified, or reproduced by or for the U.S. Government pursuant to the license
// rights granted under the clauses at DFARS 252.227-7013/7014 or FAR 52.227-14. For any other permission, please
// contact the Office of Technology Transfer at JHU/APL: Telephone: 443-778-2792, Internet: www.jhuapl.edu/ott
//
// NO WARRANTY, NO LIABILITY. THIS MATERIAL IS PROVIDED "AS IS." JHU/APL MAKES NO REPRESENTATION OR WARRANTY WITH
// RESPECT TO THE PERFORMANCE OF THE MATERIALS, INCLUDING THEIR SAFETY, EFFECTIVENESS, OR COMMERCIAL VIABILITY, AND
// DISCLAIMS ALL WARRANTIES IN THE MATERIAL, WHETHER EXPRESS OR IMPLIED, INCLUDING (BUT NOT LIMITED TO) ANY AND ALL
// IMPLIED WARRANTIES OF PERFORMANCE, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT OF
// INTELLECTUAL PROPERTY OR OTHER THIRD PARTY RIGHTS. ANY USER OF THE MATERIAL ASSUMES THE ENTIRE RISK AND LIABILITY
// FOR USING THE MATERIAL. IN NO EVENT SHALL JHU/APL BE LIABLE TO ANY USER OF THE MATERIAL FOR ANY ACTUAL, INDIRECT,
// CONSEQUENTIAL, SPECIAL OR OTHER DAMAGES ARISING FROM THE USE OF, OR INABILITY TO USE, THE MATERIAL, INCLUDING,
// BUT NOT LIMITED TO, ANY DAMAGES FOR LOST PROFITS.
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// wrapper for uldaq.h

#include <napi.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include "cbw.h"
#include "mixed_types.h"
#else
#include "uldaq.h"
#include "utility.h"
#endif

#include "node_errors.h"
#include <list>
#include <iostream>

#include "config.cpp"

std::list<DaqDeviceHandle> openHandles = {};
std::list<DaqDeviceHandle> openConnections = {};

#define MAX_DEV_COUNT 100
#define MAX_STR_LENGTH 64

DaqDeviceDescriptor devDescriptors[MAX_DEV_COUNT];
DaqDeviceInterface interfaceType = ANY_IFC;
unsigned int numDevs = MAX_DEV_COUNT;

using namespace Napi;

DaqDeviceHandle validateDeviceHandle(const CallbackInfo &info)
{
  Env env = info.Env();
  if (info.Length() < 1)
  {
    TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return WRONG_NUM_ARGS;
  }

  if (!info[0].IsNumber())
  {
    TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return WRONG_ARGS;
  }

#ifdef _WIN32
  UlError err = NOERRORS;
#else
  UlError err = ERR_NO_ERROR;
#endif
  int descriptorIndex = info[0].As<Number>().Uint32Value();

  // ensure that available devices have been inventoried
  if (numDevs == MAX_DEV_COUNT)
  {
#ifdef _WIN32
    err = cbGetDaqDeviceInventory(interfaceType, devDescriptors, (INT*) &numDevs);
    if (err != NOERRORS)
    {
      TypeError::New(env, "Error getting DAQ device inventory").ThrowAsJavaScriptException();
      return -err;
    }
#else
    err = ulGetDaqDeviceInventory(interfaceType, devDescriptors, &numDevs);
    if (err != ERR_NO_ERROR)
    {
      TypeError::New(env, "Error getting DAQ device inventory").ThrowAsJavaScriptException();
      return -err;
    }
#endif
  }

  if (descriptorIndex >= (int)numDevs)
  {
    TypeError::New(env, "Invalid device index").ThrowAsJavaScriptException();
    return INVALID_INDEX;
  }

  return descriptorIndex;
}

DaqDeviceHandle getDaqDeviceHandle(const CallbackInfo &info)
{
  int descriptorIndex = validateDeviceHandle(info);
  if (descriptorIndex < 0)
  {
    return descriptorIndex;
  }
#ifdef _WIN32
  UlError err = NOERRORS;
  err = cbCreateDaqDevice(descriptorIndex, devDescriptors[descriptorIndex]);
  // the descriptorIndex becomes the DaqDeviceHandle
  return (DaqDeviceHandle)descriptorIndex;
#else
  return ulCreateDaqDevice(devDescriptors[descriptorIndex]);
#endif
}

Number closeAll(const CallbackInfo &info)
{
  Env env = info.Env();

  // close connections
  std::cout << "Closing connections ";
  for (size_t i = 0; i < openConnections.size(); i++)
  {
    DaqDeviceHandle daqDeviceHandle = openConnections.front();
    std::cout << daqDeviceHandle << ", ";
#ifndef _WIN32
    ulDisconnectDaqDevice(daqDeviceHandle);
#endif

    openConnections.erase(openConnections.begin());
  }
  std::cout << "\n";

  // release handles
  std::cout << "Releasing handles ";
  for (size_t i = 0; i < openHandles.size(); i++)
  {
    DaqDeviceHandle daqDeviceHandle = openHandles.front();
    std::cout << daqDeviceHandle << ", ";
#ifdef _WIN32
    cbReleaseDaqDevice(daqDeviceHandle);
#else
    ulReleaseDaqDevice(daqDeviceHandle);
#endif
    openHandles.erase(openHandles.begin());
  }
  std::cout << "\n";

  return Number::New(env, 0);
}

bool compareDaqDeviceHandle(DaqDeviceHandle daqDeviceHandle, std::list<DaqDeviceHandle> l)
{
  for (DaqDeviceHandle n : l)
  {
    if (n == daqDeviceHandle)
      return true;
  }

  return false;
}

bool handleOpen(DaqDeviceHandle daqDeviceHandle)
{
  return compareDaqDeviceHandle(daqDeviceHandle, openHandles);
}

bool connectionOpen(DaqDeviceHandle daqDeviceHandle)
{
  return compareDaqDeviceHandle(daqDeviceHandle, openConnections);
}

Number connectDaqDevice(const CallbackInfo &info)
{
  Env env = info.Env();
  DaqDeviceHandle descriptorIndex = validateDeviceHandle(info);
  if (descriptorIndex < 0)
  {
    return Number::New(env, descriptorIndex);
  }

#ifdef _WIN32
  UlError err = NOERRORS;
#else
  UlError err = ERR_NO_ERROR;
#endif
  DaqDeviceHandle daqDeviceHandle = descriptorIndex;
  if (!handleOpen(descriptorIndex))
  {
    daqDeviceHandle = getDaqDeviceHandle(info);
    openHandles.push_front(daqDeviceHandle);
  }

  if (!connectionOpen(daqDeviceHandle))
  {
    // establish a connection to the DAQ device
    openConnections.push_front(daqDeviceHandle);
#ifndef _WIN32
    err = ulConnectDaqDevice(daqDeviceHandle);
    if (err != ERR_NO_ERROR)
    {
      TypeError::New(env, "Error connecting DAQ device").ThrowAsJavaScriptException();
      return Number::New(env, err);
    }
#endif
  }
  return Number::New(env, daqDeviceHandle);
}

Array getDaqDeviceInventory(const CallbackInfo &info)
{
  Env env = info.Env();

#ifdef _WIN32
  UlError err = NOERRORS;
#else
  UlError err = ERR_NO_ERROR;
#endif

// Get descriptors for all of the available DAQ devices
#ifdef _WIN32
  err = cbGetDaqDeviceInventory(interfaceType, devDescriptors, (INT*) &numDevs);
  if (err != NOERRORS)
  {
    TypeError::New(env, "Error getting DAQ device inventory").ThrowAsJavaScriptException();
    return Array::New(env, 0);
  }
#else
  err = ulGetDaqDeviceInventory(interfaceType, devDescriptors, &numDevs);
  if (err != ERR_NO_ERROR)
  {
    TypeError::New(env, "Error getting DAQ device inventory").ThrowAsJavaScriptException();
    return Array::New(env, 0);
  }
#endif

  Array ret = Array::New(env, numDevs);
  for (int i = 0; i < (int)numDevs; i++)
  {
    Object description = Object::New(env);
#ifdef _WIN32
    description.Set("productName", devDescriptors[i].ProductName);
    description.Set("uniqueId", devDescriptors[i].UniqueID);
    description.Set("devString", devDescriptors[i].DevString);
#else
    description.Set("productName", devDescriptors[i].productName);
    description.Set("uniqueId", devDescriptors[i].uniqueId);
    description.Set("devString", devDescriptors[i].devString);
#endif

#ifdef _WIN32
    int bdnum = 0;
    err = cbCreateDaqDevice(bdnum, devDescriptors[i]);
#else
    DaqDeviceHandle daqDeviceHandle = ulCreateDaqDevice(devDescriptors[i]);
#endif

#ifndef _WIN32
    int hasAI = 0;
    err = getDevInfoHasAi(daqDeviceHandle, &hasAI);
    description.Set("hasAI", hasAI != 0);
#endif

#ifndef _WIN32
    int numberOfChannels = 0;
    int hasTemperatureChannel = 0;
    err = getAiInfoHasTempChan(daqDeviceHandle, &hasTemperatureChannel, &numberOfChannels);
    description.Set("hasTemperature", hasTemperatureChannel != 0);
    description.Set("numberOfChannels", numberOfChannels);
#endif

#ifdef _WIN32
    cbReleaseDaqDevice(bdnum);
#else
    if (daqDeviceHandle)
      ulReleaseDaqDevice(daqDeviceHandle);
#endif

    ret[i] = description;
  }

  return ret;
}

Boolean devInfoHasAi(const CallbackInfo &info)
{
  Env env = info.Env();
#ifdef _WIN32
  TypeError::New(env, "Function Unavailable on Windows").ThrowAsJavaScriptException();
  return Boolean::New(env, false);
#else
  DaqDeviceHandle daqDeviceHandle = getDaqDeviceHandle(info);
  if (daqDeviceHandle < 0)
  {
    return Boolean::New(env, false);
  }

#ifdef _WIN32
  UlError err = NOERRORS;
#else
  UlError err = ERR_NO_ERROR;
#endif
  int hasAI = 0;
  err = getDevInfoHasAi(daqDeviceHandle, &hasAI);
  if (err != ERR_NO_ERROR)
  {
    TypeError::New(env, "Error getting device info, has analog input").ThrowAsJavaScriptException();
    return Boolean::New(env, false);
  }

  if (daqDeviceHandle)
#ifdef _WIN32
    cbReleaseDaqDevice(daqDeviceHandle);
#else
    ulReleaseDaqDevice(daqDeviceHandle);
#endif

  if (hasAI == 0)
  {
    return Boolean::New(env, false);
  }
  else
  {
    return Boolean::New(env, true);
  }
#endif
}

Boolean aiInfoHasTempChan(const CallbackInfo &info)
{
  Env env = info.Env();
#ifdef _WIN32
  TypeError::New(env, "Function Unavailable on Windows").ThrowAsJavaScriptException();
  return Boolean::New(env, false);
#else
  DaqDeviceHandle daqDeviceHandle = getDaqDeviceHandle(info);
  if (daqDeviceHandle < 0)
  {
    return Boolean::New(env, false);
  }

#ifdef _WIN32
  UlError err = NOERRORS;
#else
  UlError err = ERR_NO_ERROR;
#endif
  int numberOfChannels = 0;
  int hasTemperatureChannel = 0;
  err = getAiInfoHasTempChan(daqDeviceHandle, &hasTemperatureChannel, &numberOfChannels);
  if (err != ERR_NO_ERROR)
  {
    TypeError::New(env, "Error getting device info, has temperature channel").ThrowAsJavaScriptException();
    return Boolean::New(env, false);
  }

  if (daqDeviceHandle)
#ifdef _WIN32
    cbReleaseDaqDevice(daqDeviceHandle);
#else
    ulReleaseDaqDevice(daqDeviceHandle);
#endif

  if (hasTemperatureChannel == 0)
  {
    return Boolean::New(env, false);
  }
  else
  {
    return Boolean::New(env, true);
  }
#endif
}

Number aiSetConfig(const CallbackInfo &info)
{
  Env env = info.Env();
  if (info.Length() < 4)
  {
    TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return Number::New(env, WRONG_NUM_ARGS);
  }

  if (!info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber() || !info[3].IsNumber())
  {
    TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return Number::New(env, WRONG_ARGS);
  }

#ifdef _WIN32
  UlError err = NOERRORS;
#else
  UlError err = ERR_NO_ERROR;
#endif
  DaqDeviceHandle daqDeviceHandle = getDaqDeviceHandle(info);
  if (daqDeviceHandle < 0)
  {
    return Number::New(env, daqDeviceHandle);
  }

  if (!connectionOpen(daqDeviceHandle))
  {
    TypeError::New(env, "NOT Connected").ThrowAsJavaScriptException();
    return Number::New(env, NOT_CONNECTED);
  }

  AiConfigItem configItem = (AiConfigItem)info[1].As<Number>().Int64Value();
#ifdef _WIN32
  int channel = info[2].As<Number>().Int64Value();
  int configValue = info[3].As<Number>().Int64Value();
  err = cbSetConfig(BOARDINFO, daqDeviceHandle, channel, configItem, configValue);
  if (err != NOERRORS)
  {
    TypeError::New(env, "Error setting analog input configuration").ThrowAsJavaScriptException();
  }
  return Number::New(env, err);
#else
  unsigned int channel = info[2].As<Number>().Uint32Value();
  long long configValue = (long long)info[3].As<Number>().Int64Value();
  err = ulAISetConfig(daqDeviceHandle, configItem, channel, configValue);

  if (err != ERR_NO_ERROR)
  {
    TypeError::New(env, "Error setting analog input configuration").ThrowAsJavaScriptException();
  }
  return Number::New(env, err);
#endif
}

/*
Arguments:
1. Device descriptior index (0, number of devices)
2. Channel (optional, defulat = 0)
*/

Number TIn(const CallbackInfo &info)
{
  Env env = info.Env();
  DaqDeviceHandle daqDeviceHandle = getDaqDeviceHandle(info);
  if (daqDeviceHandle < 0)
  {
    return Number::New(env, daqDeviceHandle);
  }

  if (!connectionOpen(daqDeviceHandle))
  {
    TypeError::New(env, "NOT Connected").ThrowAsJavaScriptException();
    return Number::New(env, NOT_CONNECTED);
  }

  unsigned int channel = 0;
  if (info.Length() == 2)
  {
    if (info[1].IsNumber())
    {
      channel = info[1].As<Number>().Uint32Value();
    }
    else
    {
      TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
      return Number::New(env, WRONG_ARGS);
    }
  }

#ifdef _WIN32
  int scale = CELSIUS;
  int flags = FILTER;
  float data = 0;
  UlError err = NOERRORS;
  err = cbTIn(daqDeviceHandle, channel, scale, &data, flags);
  if (err == NOERRORS)
  {
    return Number::New(env, data);
  }
  else if (err == OPENCONNECTION)
  {
    TypeError::New(env, "Open contact").ThrowAsJavaScriptException();
    return Number::New(env, NAN);
  }
  else
  {
    TypeError::New(env, "Unknown error: " + std::to_string(err)).ThrowAsJavaScriptException();
    return Number::New(env, NAN);
  }
#else
  TempScale scale = TS_CELSIUS;
  TInFlag flags = TIN_FF_DEFAULT;
  double data = 0;
  UlError err = ERR_NO_ERROR;
  err = ulTIn(daqDeviceHandle, channel, scale, flags, &data);

  if (err == ERR_NO_ERROR)
  {
    return Number::New(env, data);
  }
  else if (err == ERR_OPEN_CONNECTION)
  {
    TypeError::New(env, "Open contact").ThrowAsJavaScriptException();
    return Number::New(env, NAN);
  }
  else
  {
    TypeError::New(env, "Unknown error: " + std::to_string(err)).ThrowAsJavaScriptException();
    return Number::New(env, NAN);
  }
#endif
}

Object Init(Env env, Object exports)
{
  exports.Set(String::New(env, "config"),
              initializeConfigObjectsArray(env));

  exports.Set(String::New(env, "getDaqDeviceInventory"),
              Function::New(env, getDaqDeviceInventory));

  exports.Set(String::New(env, "devInfoHasAi"),
              Function::New(env, devInfoHasAi));

  exports.Set(String::New(env, "aiInfoHasTempChan"),
              Function::New(env, aiInfoHasTempChan));

  exports.Set(String::New(env, "closeAll"),
              Function::New(env, closeAll));

  exports.Set(String::New(env, "connectDaqDevice"),
              Function::New(env, connectDaqDevice));

  exports.Set(String::New(env, "aiSetConfig"),
              Function::New(env, aiSetConfig));

  exports.Set(String::New(env, "TIn"),
              Function::New(env, TIn));
  return exports;
}

NODE_API_MODULE(ul, Init)