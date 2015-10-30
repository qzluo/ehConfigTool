/********************************************************************
 File: ehDbManager.cpp
 Function: define the interface to read ehome data file
 Author: Luobihe
 Date: 2015-10-26
********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ehDbManager.h"

static DataTypeMap s_dtMap[] = {
    {JSON_ARRD_DATATYPE_BOOL, DEV_TYPE_BOOL},
    {JSON_ARRD_DATATYPE_BYTE, DEV_TYPE_BYTE},
    {JSON_ARRD_DATATYPE_SHROT, DEV_TYPE_SHORT},
    {JSON_ARRD_DATATYPE_INT, DEV_TYPE_INT},
    {JSON_ARRD_DATATYPE_LONG, DEV_TYPE_LONG},
    {JSON_ARRD_DATATYPE_FLOAT, DEV_TYPE_FLOAT},
    {JSON_ARRD_DATATYPE_DOUBLE, DEV_TYPE_DOUBLE},
};

CEhJsonTransfer::CEhJsonTransfer(void)
{
    m_jsonRoot = NULL;
    m_jsonRegionData = NULL;
    m_jsonDeviceData = NULL;
    m_jsonOpPointData = NULL;
}

CEhJsonTransfer::~CEhJsonTransfer(void)
{
    freeAll();
}

void CEhJsonTransfer::freeAll(void)
{
    freeRegionData();
    freeDeviceData();
    freeOpPointData();
    freeRoot();
}

void CEhJsonTransfer::freeRoot(void)
{
    if (m_jsonRoot) {
        cJSON_Delete(m_jsonRoot);
        m_jsonRoot = NULL;
    }
}

void CEhJsonTransfer::freeRegionData(void)
{
    if (m_jsonRegionData) {
        cJSON_Delete(m_jsonRegionData);
        m_jsonRegionData = NULL;
    }
}

void CEhJsonTransfer::freeDeviceData(void)
{
    if (m_jsonDeviceData) {
        cJSON_Delete(m_jsonDeviceData);
        m_jsonDeviceData = NULL;
    }
}

void CEhJsonTransfer::freeOpPointData(void)
{
    if (m_jsonOpPointData) {
        cJSON_Delete(m_jsonOpPointData);
        m_jsonOpPointData = NULL;
    }
}

/*-------------------------------------------------------------------
 Function: setDB(filename);
           read json file and save as json data 
 Parameters: filename -- [IN], file name
 Return: 0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int CEhJsonTransfer::setDB(const char* filename)
{
    FILE* p_file = NULL;
    long len = 0;
    char *data = NULL;
    int ret = 0;
	
	p_file = fopen(filename,"rb");
    if (!p_file) 
        return -1;

    fseek(p_file, 0, SEEK_END);
    len = ftell(p_file);
    fseek(p_file, 0, SEEK_SET);

    data = (char*)malloc(len + 1);
    fread(data, 1, len, p_file);
    fclose(p_file);
    p_file = NULL;

    //transfer text to json struct
    ret = genTransJsonFromText(data);

    ::free(data);

    return ret;
}

/*-------------------------------------------------------------------
 Function: genTransJsonFromText(text);
           generate json data from text
 Parameters: text -- [IN], file name
 Return: 0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int CEhJsonTransfer::genTransJsonFromText(const char* text)
{
    cJSON* json_region   = NULL;
    cJSON* json_devArray = NULL;
    int regionCount = 0;
    int ret = 0;

    //initial
    freeAll();

    m_jsonRoot = cJSON_Parse(text);
	if (!m_jsonRoot) 
        return -1;

    if (!m_jsonRegionData)
        m_jsonRegionData = cJSON_CreateArray();

    if (!m_jsonDeviceData)
        m_jsonDeviceData = cJSON_CreateArray();

    if (!m_jsonOpPointData)
        m_jsonOpPointData = cJSON_CreateArray();

    regionCount = cJSON_GetArraySize(m_jsonRoot);
    for (int i = 0; i < regionCount; i++) {
        json_region = cJSON_GetArrayItem(m_jsonRoot, i);
        if (!json_region) {
            ret = -1;
            break;
        }

        //generate region record
        cJSON* json_region_toadd = cJSON_Duplicate(json_region, 1);
        cJSON_DeleteItemFromObject(json_region_toadd, JSON_DEVICE);
        cJSON_AddItemToArray(m_jsonRegionData, json_region_toadd);


        //generate device record
        ret = genDeviceJsonFromRegionJson(json_region);
        if (ret)
            break;
    }

    return ret;
}

/*-------------------------------------------------------------------
 Function: genDeviceJsonFromRegionJson(json_region);
           generate device json data from region json
 Parameters: json_region -- [IN], region json
 Return: 0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int CEhJsonTransfer::genDeviceJsonFromRegionJson(cJSON* json_region)
{
    cJSON* json_devArray   = NULL;
    cJSON* json_dev        = NULL;
    cJSON* json_deviceId   = NULL;
    cJSON* json_deviceName = NULL;
    cJSON* json_regionId   = NULL;
    cJSON* json_dev_toAdd  = NULL;

    if (!json_region)
        return 0;

    json_devArray = cJSON_GetObjectItem(json_region, JSON_DEVICE);
    if (!json_devArray)
        return 0;
    
    int iDevCount = cJSON_GetArraySize(json_devArray);
    for (int i = 0; i < iDevCount; i++) {
        json_dev = cJSON_GetArrayItem(json_devArray, i);
        if (!json_dev) return -1;

        json_deviceId = cJSON_GetObjectItem(json_dev, JSON_DEVICE_ID);
        if (!json_deviceId) return -1;

        json_deviceName = cJSON_GetObjectItem(json_dev, JSON_DEVICE_NAME);
        if (!json_deviceName) return -1;

        json_regionId = cJSON_GetObjectItem(json_region, JSON_REGION_ID);
        if (!json_regionId) return -1;

        json_dev_toAdd = cJSON_CreateObject();
        cJSON_AddStringToObject(json_dev_toAdd, JSON_DEVICE_ID, json_deviceId->valuestring);
        cJSON_AddStringToObject(json_dev_toAdd, JSON_DEVICE_NAME, json_deviceName->valuestring);
        cJSON_AddStringToObject(json_dev_toAdd, JSON_REGION_ID, json_regionId->valuestring);

        cJSON_AddItemToArray(m_jsonDeviceData, json_dev_toAdd);

        //generate opPoint record
        if (genOpPointJsonFromDeviceJson(json_dev))
            return -1;
    }

    return 0;
}

/*-------------------------------------------------------------------
 Function: genOpPointJsonFromDeviceJson(json_device);
           generate oppoint json data from device json
 Parameters: json_device -- [IN], device json
 Return: 0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int CEhJsonTransfer::genOpPointJsonFromDeviceJson(cJSON* json_device)
{
    cJSON* json_opPointtArray = NULL;
    cJSON* json_opPoint       = NULL;
    cJSON* json_opPointId     = NULL;
    cJSON* json_opPointName   = NULL;
    cJSON* json_dataType      = NULL;
    cJSON* json_addr          = NULL;
    cJSON* json_statusAddr    = NULL;
    cJSON* json_deviceId      = NULL;
    cJSON* json_opPoint_toAdd = NULL;

    if (!json_device)
        return 0;

    json_opPointtArray = cJSON_GetObjectItem(json_device, JSON_ADDRESS);
    if (!json_opPointtArray)
        return 0;
    
    int iOpPointCount = cJSON_GetArraySize(json_opPointtArray);
    for (int i = 0; i < iOpPointCount; i++) {
        json_opPoint = cJSON_GetArrayItem(json_opPointtArray, i);
        if (!json_opPoint) return -1;

        json_opPointId = cJSON_GetObjectItem(json_opPoint, JSON_ADDR_ID);
        if (!json_opPointId) return -1;

        json_opPointName = cJSON_GetObjectItem(json_opPoint, JSON_ADDR_DESC);
        if (!json_opPointName) return -1;

        json_dataType = cJSON_GetObjectItem(json_opPoint, JSON_ADDR_DATATYPE);
        if (!json_dataType) return -1;

        json_addr = cJSON_GetObjectItem(json_opPoint, JSON_ADDR_ADDR);
        if (!json_addr) return -1;

        json_statusAddr = cJSON_GetObjectItem(json_opPoint, JSON_ADDR_STATUSADDR);
        if (!json_statusAddr) return -1;

        json_deviceId = cJSON_GetObjectItem(json_device, JSON_DEVICE_ID);
        if (!json_deviceId) return -1;

        json_opPoint_toAdd = cJSON_CreateObject();
        cJSON_AddNumberToObject(json_opPoint_toAdd, JSON_ADDR_ID, json_opPointId->valueint);
        cJSON_AddStringToObject(json_opPoint_toAdd, JSON_ADDR_DESC, json_opPointName->valuestring);
        cJSON_AddStringToObject(json_opPoint_toAdd, JSON_ADDR_DATATYPE, json_dataType->valuestring);
        cJSON_AddStringToObject(json_opPoint_toAdd, JSON_ADDR_ADDR, json_addr->valuestring);
        cJSON_AddStringToObject(json_opPoint_toAdd, JSON_ADDR_STATUSADDR, json_statusAddr->valuestring);
        cJSON_AddStringToObject(json_opPoint_toAdd, JSON_DEVICE_ID, json_deviceId->valuestring);

        cJSON_AddItemToArray(m_jsonOpPointData, json_opPoint_toAdd);
    }

    return 0;
}

int CEhJsonTransfer::getDataCount(const char* jsonDesc)
{
    if (strcmp(jsonDesc, EH_TBL_REGION) == 0) {
        if (!m_jsonRegionData)
            return 0;
        
        return cJSON_GetArraySize(m_jsonRegionData);
    }
    else if (strcmp(jsonDesc, EH_TBL_DEVICE) == 0) {
        if (!m_jsonDeviceData)
            return 0;
        
        return cJSON_GetArraySize(m_jsonDeviceData);
    }
    else if (strcmp(jsonDesc, EH_TBL_OPPOINT) == 0) {
        if (!m_jsonOpPointData)
            return 0;

        return cJSON_GetArraySize(m_jsonOpPointData);   
    }

    return -1;
}

int CEhJsonTransfer::fetchData(const char* jsonDesc, int pos, void* p_data)
{
    if (strcmp(jsonDesc, EH_TBL_REGION) == 0) 
        return fetchRegionData(pos, p_data);
    else if (strcmp(jsonDesc, EH_TBL_DEVICE) == 0)
        return fetchDeviceData(pos, p_data);
    else if (strcmp(jsonDesc, EH_TBL_OPPOINT) == 0) 
        return fetchOpPointData(pos, p_data);

    return -1;
}

int CEhJsonTransfer::fetchRegionData(int pos, void* p_data)
{
    cJSON* json_item = NULL;
    cJSON* json_temp = NULL;
    TagRegion* p_regionObj = (TagRegion*)p_data;

    if (pos < 0)
        return -1;

    if (pos >= getDataCount(EH_TBL_REGION))
        return -1;

    json_item = cJSON_GetArrayItem(m_jsonRegionData, pos);
    if (!json_item)
        return -1;

    //id
    json_temp = cJSON_GetObjectItem(json_item, JSON_REGION_ID);
    if (!json_temp)
        return -1;

    p_regionObj->id = getRegionIdIntFromString(json_temp->valuestring);

    //description
    json_temp = cJSON_GetObjectItem(json_item, JSON_REGION_NAME);
    if (!json_temp)
        return -1;
    
    strcpy(p_regionObj->desc, json_temp->valuestring);

    return 0;
}

int CEhJsonTransfer::fetchDeviceData(int pos, void* p_data)
{
    cJSON* json_item = NULL;
    cJSON* json_temp = NULL;
    TagDevice* p_deviceObj = (TagDevice*)p_data;

    if (pos < 0)
        return -1;

    if (pos >= getDataCount(EH_TBL_DEVICE))
        return -1;

    json_item = cJSON_GetArrayItem(m_jsonDeviceData, pos);
    if (!json_item)
        return -1;
   
    //id
    json_temp = cJSON_GetObjectItem(json_item, JSON_DEVICE_ID);
    if (!json_temp)
        return -1;

    p_deviceObj->id = getDeviceIdIntFromString(json_temp->valuestring);

    //description
    json_temp = cJSON_GetObjectItem(json_item, JSON_DEVICE_NAME);
    if (!json_temp)
        return -1;

    strcpy(p_deviceObj->desc, json_temp->valuestring);

    //region id
    json_temp = cJSON_GetObjectItem(json_item, JSON_REGION_ID);
    if (!json_temp)
        return -1;

    p_deviceObj->regionId = getRegionIdIntFromString(json_temp->valuestring);

    return 0;
}

int CEhJsonTransfer::fetchOpPointData(int pos, void* p_data)
{    
    cJSON* json_item = NULL;
    cJSON* json_temp = NULL;
    TagOpPoint* p_opPointObj = (TagOpPoint*)p_data;

    if (pos < 0)
        return -1;

    if (pos >= getDataCount(EH_TBL_OPPOINT))
        return -1;

    json_item = cJSON_GetArrayItem(m_jsonOpPointData, pos);
    if (!json_item)
        return -1;
   
    //id
    json_temp = cJSON_GetObjectItem(json_item, JSON_ADDR_ID);
    if (!json_temp)
        return -1;

    p_opPointObj->id = json_temp->valueint;

    //description
    json_temp = cJSON_GetObjectItem(json_item, JSON_ADDR_DESC);
    if (!json_temp)
        return -1;

    strcpy(p_opPointObj->desc, json_temp->valuestring);

    //data type
    json_temp = cJSON_GetObjectItem(json_item, JSON_ADDR_DATATYPE);
    if (!json_temp)
        return -1;
    
    p_opPointObj->dataType = getDataTypeFromStr(json_temp->valuestring);

    //group addr
    json_temp = cJSON_GetObjectItem(json_item, JSON_ADDR_ADDR);
    if (!json_temp)
        return -1;
    
    strcpy(p_opPointObj->opAddr, json_temp->valuestring);

    //status group addr
    json_temp = cJSON_GetObjectItem(json_item, JSON_ADDR_STATUSADDR);
    if (!json_temp)
        return -1;
    
    strcpy(p_opPointObj->statusAddr, json_temp->valuestring);

    //devid
    json_temp = cJSON_GetObjectItem(json_item, JSON_DEVICE_ID);
    if (!json_temp)
        return -1;

    p_opPointObj->deviceId = getDeviceIdIntFromString(json_temp->valuestring);

    //value
    strcpy(p_opPointObj->strValue, "0");

    return 0;
}

/*-------------------------------------------------------------------
 Function: getRegionIdIntFromString(p_regionIdString);
           get region id from the string in json file.
           exmple 'R001' --> 1
 Parameters: p_regionIdString -- [IN], string wrote in json file
 Return: >0 -- result
         -1 -- failed
-------------------------------------------------------------------*/
int CEhJsonTransfer::getRegionIdIntFromString(const char* p_regionIdString)
{    
    if (strncmp(p_regionIdString, REGION_ID_PREFIX, 
        strlen(REGION_ID_PREFIX)) != 0)
        return -1;
    
    return atoi(p_regionIdString + strlen(REGION_ID_PREFIX));
}

int CEhJsonTransfer::getDeviceIdIntFromString(const char* p_deviceIdString)
{
    if (strncmp(p_deviceIdString, DEVICE_ID_PREFIX, 
        strlen(DEVICE_ID_PREFIX)) != 0)
        return -1;
    
    return atoi(p_deviceIdString + strlen(DEVICE_ID_PREFIX));
}

int CEhJsonTransfer::getDataTypeFromStr(const char* p_str)
{
    for (int i = 0; i < sizeof(s_dtMap) / sizeof(DataTypeMap); i++) {
        if (strcmp(p_str, s_dtMap[i].dtdesc) == 0)
            return s_dtMap[i].dataType;
    }

    return -1;
}

/*-------------------------------------------------------------------
 Function: genRegionIdStringFromInt(iRegionId, p_regionIdString);
           generate region id string from the id.
           exmple 1 -- > 'R001'
 Parameters: iRegionId -- [IN], id value
             p_regionIdString -- [OUT], string result
 Return:  0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int CEhJsonTransfer::genRegionIdStringFromInt(int iRegionId, 
                                              char* p_regionIdString)
{
    if (iRegionId <= 0)
        return -1;

    sprintf(p_regionIdString, "%s%03d",
        REGION_ID_PREFIX, iRegionId);
    
    return 0;
}

int CEhJsonTransfer::genDeviceIdStringFromInt(int iDeviceId, 
                                              char* p_deviceIdString)
{
    if (iDeviceId <= 0)
        return -1;

    sprintf(p_deviceIdString, "%s%03d",
        DEVICE_ID_PREFIX, iDeviceId);
    
    return 0;
}

int CEhJsonTransfer::genDataTypeStringFromInt(int dataType, 
                                              char* p_DataTypeString)
{
    for (int i = 0; i < sizeof(s_dtMap) / sizeof(DataTypeMap); i++) {
        if (dataType == s_dtMap[i].dataType) {
            strcpy(p_DataTypeString, s_dtMap[i].dtdesc);
            return 0;
        }
    }

    return -1;
}

int CEhJsonTransfer::saveDB(const char* filename)
{
    FILE* p_file = NULL;
    char* out = NULL;

    if (!filename)
        return -1;

    p_file = fopen(filename, "wb");
    if (!p_file)
        return -1;

    if (!m_jsonRoot)
        return -1;

    out = cJSON_Print(m_jsonRoot);
    fwrite(out, sizeof(char), strlen(out), p_file);
    free(out);
    out = NULL;

    fclose(p_file);
    p_file = NULL;

    return 0;
}

int CEhJsonTransfer::freeTbl(const char* jsonDesc)
{
    if (strcmp(jsonDesc, EH_TBL_REGION) == 0) {
        freeRegionData();
        return 0;
    }
    else if (strcmp(jsonDesc, EH_TBL_DEVICE) == 0) {
        freeDeviceData();
        return 0;
    }
    else if (strcmp(jsonDesc, EH_TBL_OPPOINT) == 0) {
        freeOpPointData();
        return 0;
    }

    return -1;
}

int CEhJsonTransfer::addData(const char* jsonDesc, void* p_data)
{
    if (strcmp(jsonDesc, EH_TBL_REGION) == 0) 
        return addRegionData(p_data);
    else if (strcmp(jsonDesc, EH_TBL_DEVICE) == 0)
        return addDeviceData(p_data);
    else if (strcmp(jsonDesc, EH_TBL_OPPOINT) == 0) 
        return addOpPointData(p_data);

    return -1;
}

int CEhJsonTransfer::addRegionData(void* p_data)
{
    char szRegionId[16] = {0};
    cJSON* json_region_toadd = NULL;
    TagRegion* p_regionObj = (TagRegion*)p_data;
    if (!p_data)
        return -1;

    if (!m_jsonRegionData)
        m_jsonRegionData = cJSON_CreateArray();

    if (genRegionIdStringFromInt(p_regionObj->id, szRegionId))
        return -1;

    json_region_toadd = cJSON_CreateObject();
    cJSON_AddStringToObject(json_region_toadd, JSON_REGION_ID, szRegionId);
    cJSON_AddStringToObject(json_region_toadd, JSON_REGION_NAME, p_regionObj->desc);

    cJSON_AddItemToArray(m_jsonRegionData, json_region_toadd);

    return 0;
}

int CEhJsonTransfer::addDeviceData(void* p_data)
{
    char szDeviceId[16] = {0};
    char szRegionId[16] = {0};
    cJSON* json_device_toadd = NULL;
    TagDevice* p_deviceObj = (TagDevice*)p_data;
    if (!p_data)
        return -1;

    if (!m_jsonDeviceData)
        m_jsonDeviceData = cJSON_CreateArray();

    if (genDeviceIdStringFromInt(p_deviceObj->id, szDeviceId))
        return -1;

    if (genRegionIdStringFromInt(p_deviceObj->regionId, szRegionId))
        return -1;

    json_device_toadd = cJSON_CreateObject();
    cJSON_AddStringToObject(json_device_toadd, JSON_DEVICE_ID, szDeviceId);
    cJSON_AddStringToObject(json_device_toadd, JSON_DEVICE_NAME, p_deviceObj->desc);
    cJSON_AddStringToObject(json_device_toadd, JSON_REGION_ID, szRegionId);

    cJSON_AddItemToArray(m_jsonDeviceData, json_device_toadd);

    return 0;
}

int CEhJsonTransfer::addOpPointData(void* p_data)
{
    char szDeviceId[16] = {0};
    char szDataType[16] = {0};
    cJSON* json_opPoint_toAdd = NULL;
    TagOpPoint* p_opPointObj = (TagOpPoint*)p_data;
    if (!p_data)
        return -1;

    if (!m_jsonOpPointData)
        m_jsonOpPointData = cJSON_CreateArray();

    if (genDataTypeStringFromInt(p_opPointObj->dataType, szDataType))
        return -1;

    if (genDeviceIdStringFromInt(p_opPointObj->deviceId, szDeviceId))
        return -1;

    json_opPoint_toAdd = cJSON_CreateObject();
    cJSON_AddNumberToObject(json_opPoint_toAdd, JSON_ADDR_ID, p_opPointObj->id);
    cJSON_AddStringToObject(json_opPoint_toAdd, JSON_ADDR_DESC, p_opPointObj->desc);
    cJSON_AddStringToObject(json_opPoint_toAdd, JSON_ADDR_DATATYPE, szDataType);
    cJSON_AddStringToObject(json_opPoint_toAdd, JSON_ADDR_ADDR, p_opPointObj->opAddr);
    cJSON_AddStringToObject(json_opPoint_toAdd, JSON_ADDR_STATUSADDR, p_opPointObj->statusAddr);
    cJSON_AddStringToObject(json_opPoint_toAdd, JSON_DEVICE_ID, szDeviceId);

    cJSON_AddItemToArray(m_jsonOpPointData, json_opPoint_toAdd);

    return 0;
}

/*-------------------------------------------------------------------
 Function: genRootJsonFromDataJson();
           generate root json from other json source data
 Parameters: 
 Return:  0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int CEhJsonTransfer::genRootJsonFromDataJson(void)
{
    int regionCount = 0;
    cJSON* json_region = NULL;
    cJSON* json_device = NULL;

    freeRoot();

    if (!m_jsonRoot)
        m_jsonRoot = cJSON_CreateArray();

    //add region
    m_jsonRoot = cJSON_Duplicate(m_jsonRegionData, 1);
    regionCount = cJSON_GetArraySize(m_jsonRoot);
    for (int i = 0; i < regionCount; i++) {
        json_region = cJSON_GetArrayItem(m_jsonRoot, i);
        json_device = cJSON_CreateArray();
        cJSON_AddItemToObject(json_region, JSON_DEVICE, json_device);

        //add device to the region from device data json
        addDeviceToRegionJson(json_region);
    }

    return 0;
}

/*-------------------------------------------------------------------
 Function: addDeviceToRegionJson(json_region);
           add device to json_region from other json source data
 Parameters: json_region -- [IN, OUT]
 Return:  0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int CEhJsonTransfer::addDeviceToRegionJson(cJSON* json_region)
{
    if (!json_region)
        return -1;

    int deviceCount = 0;
    cJSON* json_regionIdFromRegion  = NULL;
    cJSON* json_regionIdFromDevData = NULL;    
    cJSON* json_deviceArray         = NULL;
    cJSON* json_device              = NULL;
    cJSON* json_device_toAdd        = NULL;
    cJSON* json_opPointArray        = NULL;

    json_regionIdFromRegion = cJSON_GetObjectItem(json_region, JSON_REGION_ID);
    if (!json_regionIdFromRegion)
        return -1;

    json_deviceArray = cJSON_GetObjectItem(json_region, JSON_DEVICE);

    deviceCount = cJSON_GetArraySize(m_jsonDeviceData);
    for (int i = 0; i < deviceCount; i++) {
        //device data
        json_device = cJSON_GetArrayItem(m_jsonDeviceData, i);
        json_regionIdFromDevData = cJSON_GetObjectItem(json_device, JSON_REGION_ID);
        if (strcmp(json_regionIdFromDevData->valuestring,
            json_regionIdFromRegion->valuestring) == 0) { //device should be added
                json_device_toAdd = cJSON_Duplicate(json_device, 1);
                cJSON_DeleteItemFromObject(json_device_toAdd, JSON_REGION_ID);

                //add device to array
                cJSON_AddItemToArray(json_deviceArray, json_device_toAdd);

                //add opPoint to device
                json_opPointArray = cJSON_CreateArray();
                cJSON_AddItemToObject(json_device_toAdd, JSON_ADDRESS, json_opPointArray);
                addOpPointToDeviceJson(json_device_toAdd);
        }
    }

    return 0;
}

/*-------------------------------------------------------------------
 Function: addOpPointToDeviceJson(json_device);
           add oppoint to json_device from opPoint json source data
 Parameters: json_device -- [IN, OUT]
 Return:  0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int CEhJsonTransfer::addOpPointToDeviceJson(cJSON* json_device)
{
    if (!json_device)
        return -1;

    int opPointCount = 0;
    cJSON* json_deviceIdFromDevice      = NULL;
    cJSON* json_deviceIdFromOpPointData = NULL;
    cJSON* json_opPointArray            = NULL;
    cJSON* json_opPoint_fromOpPointData = NULL;
    cJSON* json_opPoint_toAdd           = NULL;
    cJSON* json_temp                    = NULL;

    json_deviceIdFromDevice = cJSON_GetObjectItem(json_device, JSON_DEVICE_ID);
    if (!json_deviceIdFromDevice)
        return -1;

    json_opPointArray = cJSON_GetObjectItem(json_device, JSON_ADDRESS);

    opPointCount = cJSON_GetArraySize(m_jsonOpPointData);
    for (int i = 0; i < opPointCount; i++) {
        //opPoint data
        json_opPoint_fromOpPointData = cJSON_GetArrayItem(m_jsonOpPointData, i);
        json_deviceIdFromOpPointData = cJSON_GetObjectItem(json_opPoint_fromOpPointData, 
            JSON_DEVICE_ID);
        if (strcmp(json_deviceIdFromDevice->valuestring,
            json_deviceIdFromOpPointData->valuestring) == 0) { //opPoint should be added
                json_opPoint_toAdd = cJSON_CreateObject();

                //id
                json_temp = cJSON_GetObjectItem(json_opPoint_fromOpPointData, JSON_ADDR_ID);
                cJSON_AddNumberToObject(json_opPoint_toAdd, JSON_ADDR_ID, json_temp->valueint);

                //type
                cJSON_AddStringToObject(json_opPoint_toAdd, JSON_ADDR_TYPE, JSON_ADDR_TYPE_DEFAULT);

                //desc
                json_temp = cJSON_GetObjectItem(json_opPoint_fromOpPointData, JSON_ADDR_DESC);
                cJSON_AddStringToObject(json_opPoint_toAdd, JSON_ADDR_DESC, json_temp->valuestring);

                //dataType
                json_temp = cJSON_GetObjectItem(json_opPoint_fromOpPointData, JSON_ADDR_DATATYPE);
                cJSON_AddStringToObject(json_opPoint_toAdd, JSON_ADDR_DATATYPE, json_temp->valuestring);

                //value
                cJSON_AddStringToObject(json_opPoint_toAdd, JSON_ADDR_VALUE, JSON_ADDR_VALUE_DEFAULT);

                //op addr
                json_temp = cJSON_GetObjectItem(json_opPoint_fromOpPointData, JSON_ADDR_ADDR);
                cJSON_AddStringToObject(json_opPoint_toAdd, JSON_ADDR_ADDR, json_temp->valuestring);

                //status addr
                json_temp = cJSON_GetObjectItem(json_opPoint_fromOpPointData, JSON_ADDR_STATUSADDR);
                cJSON_AddStringToObject(json_opPoint_toAdd, JSON_ADDR_STATUSADDR, json_temp->valuestring);

                //add opPoint to array
                cJSON_AddItemToArray(json_opPointArray, json_opPoint_toAdd);
        }
    }

    return 0;
}

char* CEhJsonTransfer::printRootData(void)
{
    return printJson(m_jsonRoot);
}

char* CEhJsonTransfer::printRegionData(void)
{
    return printJson(m_jsonRegionData);
}

char* CEhJsonTransfer::printDeviceData(void)
{
    return printJson(m_jsonDeviceData);
}

char* CEhJsonTransfer::printOpPointData(void)
{
    return printJson(m_jsonOpPointData);
}

char* CEhJsonTransfer::printJson(cJSON* json)
{
    if (json)
        return cJSON_Print(json);
    else 
        return NULL;
}

