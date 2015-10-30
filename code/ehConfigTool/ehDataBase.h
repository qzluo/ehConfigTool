/********************************************************************
 File: ehDataBase.h
 Function: ehome data base
 Author: Luobihe
 Date: 2015-10-12
*********************************************************************/

#ifndef __EHDATABASE_H__
#define __EHDATABASE_H__

#include "dbDataType.h"
#include "ehDbManager.h"
#include <list>
using namespace std;


class CEhDB {
private:
    std::list<TagRegion* > m_tblRegion;
    std::list<TagDevice* > m_tblDevice;
    std::list<TagOpPoint*> m_tblOpPoint;
    static CEhDB* m_instance;

private:
    CEhDB();
    ~CEhDB();

public:
    void Free(void);

    void freeTblRegion(void);
    void freeTblDevice(void);
    void freeTblOpPoint(void);

    int initTblRegion(void);
    int initTblDevice(void);
    int initTblOpPoint(void);

    int initTblRegion(CEhJsonTransfer* p_jsonTrans);
    int initTblDevice(CEhJsonTransfer* p_jsonTrans);
    int initTblOpPoint(CEhJsonTransfer* p_jsonTrans);

    int saveTblRegion(CEhJsonTransfer* p_jsonTrans);
    int saveTblDevice(CEhJsonTransfer* p_jsonTrans);
    int saveTblOpPoint(CEhJsonTransfer* p_jsonTrans);

    int fetchRegionData(int pos, void* p_data);
    int fetchDeviceData(int pos, void* p_data);
    int fetchOpPointData(int pos, void* p_data);
    
    int getNewRegionID(void);
    int getNewDeviceID(void);
    int getNewOpPointID(void);

    
    //0 -- success, -1 -- failed
    int findRegionDataById(int iId, void* p_data);
    int findDeviceDataById(int iId, void* p_data);
    int findOpPointDataById(int iId, void* p_data);

    //0 -- success, -1 -- failed
    int findRegionDataByDesc(const char* pDesc, void* p_data);
    int findDeviceDataByDescAndRegionId(const char* pDesc, int iRegionId, void* p_data);

    int findOpPointDataByDescAndDeviceId(const char* pDesc, int iDeviceId, void* p_data);

    //add record
    int addRegion(const char* pRegionName);
    int addDevice(const char* pDeviceName, int iRegionId);
    int addOpPoint(const char* pOpName, int iDeviceId, int iDataType,
        const char* pOpAddr, const char* pStatusAddr);

    //check whether operation can be add, 1 -- yes, 0 -- no
    int isOpPointCanAdd(const char* pOpName, int iDeviceId, int iDataType,
        const char* pOpAddr, const char* pStatusAddr);

    //delete
    int deleteRegionAndRefDeviceById(int iRegionId);
    int deleteRegionById(int iId);
    int deleteDeviceAndRefOpPointByRegionId(int iRegionId);
    int deleteDeviceAndRefOpPointById(int iId);
    int deleteDeviceById(int iId);
    int deleteOpPointById(int iId);
    int deleteOpPointByDeviceId(int iDeviceId);

    //update, by id
    int updateRegion(void* p_data);
    int updateDevice(void* p_data);
    int updateOpPoint(void* p_data);

    //0 -- can not update, 1 -- can update
    int checkRegionCanUpdate(TagRegion* p_data);
    int checkDeviceCanUpdate(TagDevice* p_data);
    int checkOpPointCanUpdate(TagOpPoint* p_data);    

public:
    static CEhDB * getInstance(void);

    int init(void);    
    int init(const char* fileName);

    int save(const char* fileName);

    int getDataCount(const char* pTblName);
    int fetchData(const char* pTblName, int pos, void* p_data);

    int findDataById(const char* pTblName, int iId, void* p_data);
    int findDataByDesc(const char* pTblName, const char* pDesc, void* p_data);    

    //delete record
    int deleteDataById(const char*  pTblName, int iId);

    //update
    int updateTblData(const char* pTblName, void* p_data);
};

int init_Db(void);
int init_Db(const char* fileName);
int save_Db(const char* fileName);
int getTblRecordCount(const char* pTblName);
int fetchRecordData(const char* pTblName, int pos, void* p_data);

int findDataById(const char* pTblName, int iId, void* p_data);
int findDataByDesc(const char* pTblName, const char* pDesc, void* p_data);
int findDeviceDataByDescAndRegionId(const char* pDesc, int iRegionId, void* p_data);
int findOpPointDataByDescAndDeviceId(const char* pDesc, int iDeviceId, void* p_data);

//add record
//0 -- success, -1 -- failed
int addRegion(const char* pRegionName);
int addDevice(const char* pDeviceName, int iRegionId);
int addOpPoint(const char* pOpName, int iDeviceId, int iDataType, 
               const char* pOpAddr, const char* pStatusAddr);

//delete record
int deleteDataById(const char* pTblName, int iId);

//update record
int updateTblData(const char* pTblName, void* p_data);

#endif  //__EHDATABASE_H__