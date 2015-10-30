/********************************************************************
 File: dbDataType.h
 Function: ehome data type defines
 Author: Luobihe
 Date: 2015-10-27
*********************************************************************/

#ifndef __DBDATATYPE_H__
#define __DBDATATYPE_H__


#define EH_TBL_REGION   "tblRegion"
#define EH_TBL_DEVICE   "tblDevice"
#define EH_TBL_OPPOINT  "tblOpPoint"

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

//数据点的数据类型
enum DevDataType {
    DEV_TYPE_BOOL = 1,
    DEV_TYPE_BYTE,
    DEV_TYPE_SHORT,
    DEV_TYPE_INT,
    DEV_TYPE_LONG,
    DEV_TYPE_FLOAT,
    DEV_TYPE_DOUBLE,
};

typedef struct {
    char* dtdesc;
    int dataType;
} DataTypeMap;

#endif  //__DBDATATYPE_H__