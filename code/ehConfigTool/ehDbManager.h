/********************************************************************
 File: ehDbManager.h
 Function: define the interface to read ehome data file
 Author: Luobihe
 Date: 2015-10-26
********************************************************************/

#ifndef __EHDBMANAGER_H__
#define __EHDBMANAGER_H__

#include "cJSON.h"
#include "dbDataType.h"

#define JSON_REGION_ID        "region id"
#define JSON_REGION_NAME      "region name"
#define JSON_DEVICE           "devices"
#define JSON_ADDRESS          "address"

#define JSON_DEVICE_ID        "device id"
#define JSON_DEVICE_NAME      "device name"

#define JSON_ADDR_ID          "addr id"
#define JSON_ADDR_DESC        "description"
#define JSON_ADDR_DATATYPE    "data type"
#define JSON_ADDR_TYPE        "type"
#define JSON_ADDR_VALUE       "value"
#define JSON_ADDR_ADDR        "address"
#define JSON_ADDR_STATUSADDR  "status address"

#define JSON_ADDR_TYPE_DEFAULT        "knx"
#define JSON_ADDR_VALUE_DEFAULT       "0,1"

#define REGION_ID_PREFIX     "R"
#define DEVICE_ID_PREFIX     "dev"


#define JSON_ARRD_DATATYPE_BOOL     "bool"
#define JSON_ARRD_DATATYPE_BYTE     "byte"
#define JSON_ARRD_DATATYPE_SHROT    "short"
#define JSON_ARRD_DATATYPE_INT      "integer"
#define JSON_ARRD_DATATYPE_LONG     "long"
#define JSON_ARRD_DATATYPE_FLOAT    "float"
#define JSON_ARRD_DATATYPE_DOUBLE   "double"


class CEhDbInterface
{
public:
    virtual int setDB(const char* filename) = 0;
    virtual int getDataCount(const char* tblName) = 0;
    virtual int fetchData(const char* tblName, int pos, void* p_data) = 0;
    
    virtual int saveDB(const char* filename) = 0;
    virtual int freeTbl(const char* tblName) = 0;
    virtual int addData(const char* tblName, void* p_data) = 0;
};

/////////////////////////////////////////////////////////////////////

/*
class CEhJsonTransfer: to generate ehome database 

//region table
typedef struct {
    int id;
    char desc[256]; 
} TagRegion;

//device table
typedef struct {
    int id;
    char desc[256]; 
    int regionId;
} TagDevice;

//opetation point table
typedef struct {
    int id;
    char desc[256]; 
    int deviceId;
    int dataType;
    char opAddr[16];
    char statusAddr[16];
    char strValue[16];
} TagOpPoint;

variable:
    m_jsonRegionData -- to save region data, 
        [
            {"region id":"R001",
            "region name":"办公区"},
            ...
        ]
    m_jsonDeviceData -- to save device data
    m_jsonOpPointData -- to save opPoint data
*/


class CEhJsonTransfer : public CEhDbInterface
{
private:
    cJSON* m_jsonRoot;

    /* region json
      [{"region id":"R001",
        "region name":"办公区"}, 
        { ... }]
    */
    cJSON* m_jsonRegionData;  
    
    /* device json
      [{"device id":"dev001",
        "device name":"iec照明",
        "region id":"R001"}, 
       { ... }]
    */
    cJSON* m_jsonDeviceData;

    /* opPoint json
      [{"addr id":1,
        "description":"iec照明开关",
        "data type": "bool",
        "address":"10/1/11",
        "status address":"10/1/11",
        "device id":"dev001"}, 
       { ... }]
    */
    cJSON* m_jsonOpPointData;

public:
    CEhJsonTransfer(void);
    ~CEhJsonTransfer(void);

    void freeAll(void);
    void freeRoot(void);
    void freeRegionData(void);
    void freeDeviceData(void);
    void freeOpPointData(void);

    char* printRootData(void);
    char* printRegionData(void);
    char* printDeviceData(void);
    char* printOpPointData(void);
    char* printJson(cJSON* json);

    //0 -- success, -1 -- failed
    int genTransJsonFromText(const char* text);
    int genDeviceJsonFromRegionJson(cJSON* json_region);
    int genOpPointJsonFromDeviceJson(cJSON* json_device);

    int fetchRegionData(int pos, void* p_data);
    int fetchDeviceData(int pos, void* p_data);
    int fetchOpPointData(int pos, void* p_data);

    int getRegionIdIntFromString(const char* p_regionIdString);
    int getDeviceIdIntFromString(const char* p_deviceIdString);
    int getDataTypeFromStr(const char* p_str);

    int genRegionIdStringFromInt(int iRegionId, char* p_regionIdString);
    int genDeviceIdStringFromInt(int iDeviceId, char* p_deviceIdString);
    int genDataTypeStringFromInt(int dataType, char* p_DataTypeString);

    //for save to file
    int addRegionData(void* p_data);
    int addDeviceData(void* p_data);
    int addOpPointData(void* p_data);

    int genRootJsonFromDataJson(void);
    int addDeviceToRegionJson(cJSON* json_region);
    int addOpPointToDeviceJson(cJSON* json_device);

public:
    int setDB(const char* filename);
    int getDataCount(const char* jsonDesc);
    int fetchData(const char* jsonDesc, int pos, void* p_data);
    
    int saveDB(const char* filename);
    int freeTbl(const char* jsonDesc);
    int addData(const char* jsonDesc, void* p_data);

};


#endif  //__EHDBMANAGER_H__