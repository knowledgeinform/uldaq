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

#include <napi.h>

#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include "cbw.h"
#else
#include "uldaq.h"
#endif

using namespace Napi;

Object initializeConfigObjectsArray(Env env)
{
    Object config = Object::New(env);

    Object options = Object::New(env);
    Object multiLevel = Object::New(env);
    multiLevel.Set("value", (int)123409);
    multiLevel.Set("options", 123409);

    Object AiConfigItemObj = Object::New(env);

    options = Object::New(env);
#ifdef _WIN32
    options.Set("AI_VOLTAGE", (int)AI_CHAN_TYPE_VOLTAGE);
    options.Set("AI_TC", (int)AI_CHAN_TYPE_TC);
    options.Set("AI_RTD", (int)AI_CHAN_TYPE_RTD_100OHM_4W);
    options.Set("AI_CHAN_TYPE_RTD_1000OHM_4W", (int)AI_CHAN_TYPE_RTD_1000OHM_4W);
    options.Set("AI_CHAN_TYPE_RTD_100OHM_4W", (int)AI_CHAN_TYPE_RTD_100OHM_4W);
    options.Set("AI_CHAN_TYPE_RTD_1000OHM_3W", (int)AI_CHAN_TYPE_RTD_1000OHM_3W);
    options.Set("AI_CHAN_TYPE_RTD_100OHM_3W", (int)AI_CHAN_TYPE_RTD_100OHM_3W);
    options.Set("AI_THERMISTOR", (int)AI_CHAN_TYPE_RESISTANCE_1K4W);
    options.Set("AI_CHAN_TYPE_RESISTANCE_10K4W", (int)AI_CHAN_TYPE_RESISTANCE_10K4W);
    options.Set("AI_CHAN_TYPE_RESISTANCE_1K4W", (int)AI_CHAN_TYPE_RESISTANCE_1K4W);
    options.Set("AI_CHAN_TYPE_RESISTANCE_10K2W", (int)AI_CHAN_TYPE_RESISTANCE_10K2W);
    options.Set("AI_CHAN_TYPE_RESISTANCE_1K2W", (int)AI_CHAN_TYPE_RESISTANCE_1K2W);
    options.Set("AI_SEMICONDUCTOR", (int)AI_CHAN_TYPE_QUART_BRIDGE_350OHM);
    options.Set("AI_CHAN_TYPE_QUART_BRIDGE_350OHM", (int)AI_CHAN_TYPE_QUART_BRIDGE_350OHM);
    options.Set("AI_CHAN_TYPE_QUART_BRIDGE_120OHM", (int)AI_CHAN_TYPE_QUART_BRIDGE_120OHM);
    options.Set("AI_CHAN_TYPE_HALF_BRIDGE", (int)AI_CHAN_TYPE_HALF_BRIDGE);
    options.Set("AI_CHAN_TYPE_FULL_BRIDGE_62PT5mVV", (int)AI_CHAN_TYPE_FULL_BRIDGE_62PT5mVV);
    options.Set("AI_CHAN_TYPE_FULL_BRIDGE_7PT8mVV", (int)AI_CHAN_TYPE_FULL_BRIDGE_7PT8mVV);
#else
    options.Set("AI_VOLTAGE", (int)AI_VOLTAGE);
    options.Set("AI_TC", (int)AI_TC);
    options.Set("AI_RTD", (int)AI_RTD);
    options.Set("AI_THERMISTOR", (int)AI_THERMISTOR);
    options.Set("AI_SEMICONDUCTOR", (int)AI_SEMICONDUCTOR);
    options.Set("AI_DISABLED", (int)AI_DISABLED);
#endif
    multiLevel = Object::New(env);
#ifdef _WIN32
    multiLevel.Set("value", (int)BIADCHANTYPE);
#else
    multiLevel.Set("value", (int)AI_CFG_CHAN_TYPE);
#endif
    multiLevel.Set("options", options);
    AiConfigItemObj.Set("AI_CFG_CHAN_TYPE", multiLevel);

    options = Object::New(env);
#ifdef _WIN32
    options.Set("TC_J", (int)TC_TYPE_J);
    options.Set("TC_K", (int)TC_TYPE_K);
    options.Set("TC_T", (int)TC_TYPE_T);
    options.Set("TC_E", (int)TC_TYPE_E);
    options.Set("TC_R", (int)TC_TYPE_R);
    options.Set("TC_S", (int)TC_TYPE_S);
    options.Set("TC_B", (int)TC_TYPE_B);
    options.Set("TC_N", (int)TC_TYPE_N);
#else
    options.Set("TC_J", (int)TC_J);
    options.Set("TC_K", (int)TC_K);
    options.Set("TC_T", (int)TC_T);
    options.Set("TC_E", (int)TC_E);
    options.Set("TC_R", (int)TC_R);
    options.Set("TC_S", (int)TC_S);
    options.Set("TC_B", (int)TC_B);
    options.Set("TC_N", (int)TC_N);
#endif
    multiLevel = Object::New(env);

#ifdef _WIN32
    multiLevel.Set("value", (int)BITCCHANTYPE); // best guess here
#else
    multiLevel.Set("value", (int)AI_CFG_CHAN_TC_TYPE);
#endif
    multiLevel.Set("options", options);
    AiConfigItemObj.Set("AI_CFG_CHAN_TC_TYPE", multiLevel);

    options = Object::New(env);
#ifdef _WIN32
    options.Set("TU_CELSIUS", (int)CELSIUS);
    options.Set("TU_FAHRENHEIT", (int)FAHRENHEIT);
    options.Set("TU_KELVIN", (int)KELVIN);
#else
    options.Set("TU_CELSIUS", (int)TU_CELSIUS);
    options.Set("TU_FAHRENHEIT", (int)TU_FAHRENHEIT);
    options.Set("TU_KELVIN", (int)TU_KELVIN);
#endif
    multiLevel = Object::New(env);
#ifdef _WIN32
    multiLevel.Set("value", (int)BITEMPSCALE);
#else
    multiLevel.Set("value", (int)AI_CFG_SCAN_CHAN_TEMP_UNIT);
#endif
    multiLevel.Set("options", options);
    AiConfigItemObj.Set("AI_CFG_SCAN_CHAN_TEMP_UNIT", multiLevel);

#ifndef _WIN32
    options = Object::New(env);
    options.Set("TU_CELSIUS", (int)TU_CELSIUS);
    options.Set("TU_FAHRENHEIT", (int)TU_FAHRENHEIT);
    options.Set("TU_KELVIN", (int)TU_KELVIN);
    multiLevel = Object::New(env);
    multiLevel.Set("value", (int)AI_CFG_SCAN_TEMP_UNIT);
    multiLevel.Set("options", options);
    AiConfigItemObj.Set("AI_CFG_SCAN_TEMP_UNIT", multiLevel);

    options = Object::New(env);
    options.Set("ADC_TM_AUTO", (int)ADC_TM_AUTO);
    options.Set("ADC_TM_HIGH_RES", (int)ADC_TM_HIGH_RES);
    options.Set("ADC_TM_HIGH_SPEED", (int)ADC_TM_HIGH_SPEED);
    multiLevel = Object::New(env);
    multiLevel.Set("value", (int)AI_CFG_ADC_TIMING_MODE);
    multiLevel.Set("options", options);
    AiConfigItemObj.Set("AI_CFG_ADC_TIMING_MODE", multiLevel);

    options = Object::New(env);
    options.Set("AZM_NONE", (int)AZM_NONE);
    options.Set("AZM_EVERY_SAMPLE", (int)AZM_EVERY_SAMPLE);
    options.Set("AZM_ONCE", (int)AZM_ONCE);
    multiLevel = Object::New(env);
    multiLevel.Set("value", (int)AI_CFG_AUTO_ZERO_MODE);
    multiLevel.Set("options", options);
    AiConfigItemObj.Set("AI_CFG_AUTO_ZERO_MODE", multiLevel);

    options = Object::New(env);
    options.Set("FACTORY_DATE", (int)0);
    options.Set("FIELD_DATE", (int)1);
    multiLevel = Object::New(env);
    multiLevel.Set("value", (int)AI_CFG_CAL_DATE);
    multiLevel.Set("options", options);
    AiConfigItemObj.Set("AI_CFG_CAL_DATE", multiLevel);

    options = Object::New(env);
    options.Set("IEPE_DISABLED", (int)IEPE_DISABLED);
    options.Set("IEPE_ENABLED", (int)IEPE_ENABLED);
    multiLevel = Object::New(env);
    multiLevel.Set("value", (int)AI_CFG_CHAN_IEPE_MODE);
    multiLevel.Set("options", options);
    AiConfigItemObj.Set("AI_CFG_CHAN_IEPE_MODE", multiLevel);

    options = Object::New(env);
    options.Set("CM_DC", (int)CM_DC);
    options.Set("CM_AC", (int)CM_AC);
    multiLevel = Object::New(env);
    multiLevel.Set("value", (int)AI_CFG_CHAN_COUPLING_MODE);
    multiLevel.Set("options", options);
    AiConfigItemObj.Set("AI_CFG_CHAN_COUPLING_MODE", multiLevel);

    options = Object::New(env);
    multiLevel = Object::New(env);
    multiLevel.Set("value", (int)AI_CFG_CHAN_SENSOR_CONNECTION_TYPE);
    multiLevel.Set("options", options);
    AiConfigItemObj.Set("AI_CFG_CHAN_SENSOR_CONNECTION_TYPE", multiLevel);

    options = Object::New(env);
    options.Set("OTD_DISABLED", (int)OTD_DISABLED);
    options.Set("OTD_ENABLED", (int)OTD_ENABLED);
    multiLevel = Object::New(env);
    multiLevel.Set("value", (int)AI_CFG_CHAN_OTD_MODE);
    multiLevel.Set("options", options);
    AiConfigItemObj.Set("AI_CFG_CHAN_OTD_MODE", multiLevel);

    options = Object::New(env);
    options.Set("OTD_DISABLED", (int)OTD_DISABLED);
    options.Set("OTD_ENABLED", (int)OTD_ENABLED);
    multiLevel = Object::New(env);
    multiLevel.Set("value", (int)AI_CFG_OTD_MODE);
    multiLevel.Set("options", options);
    AiConfigItemObj.Set("AI_CFG_OTD_MODE", multiLevel);

    options = Object::New(env);
    options.Set("AI_CTT_FACTORY", (int)AI_CTT_FACTORY);
    options.Set("AI_CTT_FIELD", (int)AI_CTT_FIELD);
    multiLevel = Object::New(env);
    multiLevel.Set("value", (int)AI_CFG_CAL_TABLE_TYPE);
    multiLevel.Set("options", options);
    AiConfigItemObj.Set("AI_CFG_CAL_TABLE_TYPE", multiLevel);

    options = Object::New(env);
    options.Set("AI_RFT_60HZ", (int)AI_RFT_60HZ);
    options.Set("AI_RFT_50HZ", (int)AI_RFT_50HZ);
    multiLevel = Object::New(env);
    multiLevel.Set("value", (int)AI_CFG_REJECT_FREQ_TYPE);
    multiLevel.Set("options", options);
    AiConfigItemObj.Set("AI_CFG_REJECT_FREQ_TYPE", multiLevel);

    options = Object::New(env);
    options.Set("FACTORY_DATE", (int)0);
    options.Set("FIELD_DATE", (int)1);
    multiLevel = Object::New(env);
    multiLevel.Set("value", (int)AI_CFG_EXP_CAL_DATE);
    multiLevel.Set("options", options);
    AiConfigItemObj.Set("AI_CFG_EXP_CAL_DATE", multiLevel);

#endif

    config.Set("AiConfigItem", AiConfigItemObj);

    config.Freeze();
    return config;
}