/********************************************************************
 File: ehDataBase.cpp
 Function: ehome data base
 Author: Luobihe
 Date: 2015-10-12
*********************************************************************/

#include "stdafx.h"
#include "ehDataBase.h"
#include "knxcomm.h"

static TagRegion s_tblRegion[] = {
    {1, "办公区"},
    {2, "视听室"},
    {3, "测试区"}
};

static TagDevice s_tblDevice[] = {
    {1, "iec照明",     1},
    {2, "MSI单排照明", 1},
    {3, "MSI双排照明", 1},
    {4, "茶水区",      1},
    {5, "橱窗灯",      1},
    {6, "前台灯",      1},
    {7, "筒灯",        2},
    {8, "沙发射灯",    2},
    {9, "电视射灯",    2},
};

static TagOpPoint s_tblOpPoint[] = {
    {1, "iec照明开关",     1, DEV_TYPE_BOOL, "10/1/11", "10/1/11", "0"},
    {2, "MSI单排照明开关", 2, DEV_TYPE_BOOL, "10/1/12", "10/1/12", "0"},
    {3, "MSI双排照明开关", 3, DEV_TYPE_BOOL, "10/1/13", "10/1/13", "0"},
    {4, "茶水区开关",      4, DEV_TYPE_BOOL, "10/1/13", "10/1/31", "0"},
    {5, "橱窗灯开关",      5, DEV_TYPE_BOOL, "10/1/13", "10/1/32", "0"},
    {6, "前台灯开关",      6, DEV_TYPE_BOOL, "10/1/13", "10/1/33", "0"},
    {7, "筒灯开关",        7, DEV_TYPE_BOOL, "10/2/11", "10/2/11", "0"},
    {8, "沙发射灯开关",    8, DEV_TYPE_BOOL, "10/2/12", "10/2/12", "0"},
    {9, "电视射灯开关",    9, DEV_TYPE_BOOL, "10/2/13", "10/2/13", "0"},
    {10, "iec照明开关2",   1, DEV_TYPE_BOOL, "10/1/11", "10/1/11", "0"}
};


CEhDB* CEhDB::m_instance = NULL;

CEhDB::CEhDB()
{
}

CEhDB::~CEhDB()
{
    Free();

    if (m_instance) {
        delete m_instance;
        m_instance = NULL;
    }
}

void CEhDB::Free(void)
{
    freeTblRegion();
    freeTblDevice();
    freeTblOpPoint();
}

void CEhDB::freeTblRegion(void)
{
    //free tbl region
    TagRegion* pData = NULL;
    while (m_tblRegion.size() > 0) {
        pData = *(m_tblRegion.begin());
        m_tblRegion.pop_front();
        delete pData;
        pData = NULL;
    }
}

void CEhDB::freeTblDevice(void)
{
    TagDevice* pData = NULL;
    while (m_tblDevice.size() > 0) {
        pData = *(m_tblDevice.begin());
        m_tblDevice.pop_front();
        delete pData;
        pData = NULL;
    }
}

void CEhDB::freeTblOpPoint(void)
{
    TagOpPoint* pData = NULL;
    while (m_tblOpPoint.size() > 0) {
        pData = *(m_tblOpPoint.begin());
        m_tblOpPoint.pop_front();
        delete pData;
        pData = NULL;
    }
}

int CEhDB::init(void)
{
    Free();
    
    return 0;
}

int CEhDB::init(const char* fileName)
{
    Free();

    //initial region table
    CEhJsonTransfer jsonTransfer;
    if (jsonTransfer.setDB(fileName))
        return -1;

    if (initTblRegion(&jsonTransfer))
        return -1;

    if (initTblDevice(&jsonTransfer))
        return -1;

    if (initTblOpPoint(&jsonTransfer))
        return -1;
    
    return 0;
}

int CEhDB::initTblRegion(void)
{
    for (int i = 0; i < sizeof(s_tblRegion) / sizeof(TagRegion); i++) {
        TagRegion* pData = new TagRegion();
        if (!pData)
            return -1;

        pData->id = s_tblRegion[i].id;
        strcpy(pData->desc, s_tblRegion[i].desc);
        m_tblRegion.push_back(pData);
    }

    return 0;
}

int CEhDB::initTblDevice(void)
{
    for (int i = 0; i < sizeof(s_tblDevice) / sizeof(TagDevice); i++) {
        TagDevice* pData = new TagDevice();
        if (!pData)
            return -1;

        pData->id = s_tblDevice[i].id;
        strcpy(pData->desc, s_tblDevice[i].desc);
        pData->regionId = s_tblDevice[i].regionId;
        m_tblDevice.push_back(pData);
    }

    return 0;
}

int CEhDB::initTblOpPoint(void)
{
    for (int i = 0; i < sizeof(s_tblOpPoint) / sizeof(TagOpPoint); i++) {
        TagOpPoint* pData = new TagOpPoint();
        if (!pData)
            return -1;

        pData->id = s_tblOpPoint[i].id;
        strcpy(pData->desc, s_tblOpPoint[i].desc);
        pData->deviceId = s_tblOpPoint[i].deviceId;
        pData->dataType = s_tblOpPoint[i].dataType;
        strcpy(pData->opAddr, s_tblOpPoint[i].opAddr);
        strcpy(pData->statusAddr, s_tblOpPoint[i].statusAddr);
        m_tblOpPoint.push_back(pData);
    }

    return 0;
}

int CEhDB::initTblRegion(CEhJsonTransfer* p_jsonTrans)
{
    int regionCount = 0;
    regionCount = p_jsonTrans->getDataCount(EH_TBL_REGION);
    for (int i = 0; i < regionCount; i++) {
        TagRegion* pData = new TagRegion();
        if (!pData)
            return -1;

        if (p_jsonTrans->fetchData(EH_TBL_REGION, i, pData)) {
            delete pData;
            return -1;
        }

        m_tblRegion.push_back(pData);
    }

    return 0;
}

int CEhDB::initTblDevice(CEhJsonTransfer* p_jsonTrans)
{
    int deviceCount = 0;
    deviceCount = p_jsonTrans->getDataCount(EH_TBL_DEVICE);
    for (int i = 0; i < deviceCount; i++) {
        TagDevice* pData = new TagDevice();
        if (!pData)
            return -1;

        if (p_jsonTrans->fetchData(EH_TBL_DEVICE, i, pData)) {
            delete pData;
            return -1;
        }

        m_tblDevice.push_back(pData);
    }

    return 0;
}

int CEhDB::initTblOpPoint(CEhJsonTransfer* p_jsonTrans)
{
    int opPointCount = 0;
    opPointCount = p_jsonTrans->getDataCount(EH_TBL_OPPOINT);
    for (int i = 0; i < opPointCount; i++) {
        TagOpPoint* pData = new TagOpPoint();
        if (!pData)
            return -1;

        if (p_jsonTrans->fetchData(EH_TBL_OPPOINT, i, pData)) {
            delete pData;
            return -1;
        }

        m_tblOpPoint.push_back(pData);
    }

    return 0;
}

int CEhDB::save(const char* fileName)
{
    CEhJsonTransfer jsonTransfer;

    //save db data to json data
    if (saveTblRegion(&jsonTransfer))
        return -1;

    if (saveTblDevice(&jsonTransfer))
        return -1;

    if (saveTblOpPoint(&jsonTransfer))
        return -1;
    
    //generate root json data
    jsonTransfer.genRootJsonFromDataJson();

    //save to file
    return jsonTransfer.saveDB(fileName);
}

int CEhDB::saveTblRegion(CEhJsonTransfer* p_jsonTrans)
{
    std::list<TagRegion*>::iterator it;
    TagRegion* p_data = NULL;
    int i = 0;
    for (it = m_tblRegion.begin(); 
        it != m_tblRegion.end(); it++) {
            p_data = *it;
            if (p_jsonTrans->addData(EH_TBL_REGION, p_data))
                return -1;
    }

    return 0;
}

int CEhDB::saveTblDevice(CEhJsonTransfer* p_jsonTrans)
{
    std::list<TagDevice*>::iterator it;
    TagDevice* p_data = NULL;
    int i = 0;
    for (it = m_tblDevice.begin(); 
        it != m_tblDevice.end(); it++) {
            p_data = *it;
            if (p_jsonTrans->addData(EH_TBL_DEVICE, p_data))
                return -1;
    }

    return 0;
}

int CEhDB::saveTblOpPoint(CEhJsonTransfer* p_jsonTrans)
{
    std::list<TagOpPoint*>::iterator it;
    TagOpPoint* p_data = NULL;
    int i = 0;
    for (it = m_tblOpPoint.begin(); 
        it != m_tblOpPoint.end(); it++) {
            p_data = *it;
            if (p_jsonTrans->addData(EH_TBL_OPPOINT, p_data))
                return -1;
    }

    return 0;
}

CEhDB * CEhDB::getInstance(void)
{
    if (!m_instance) {
        m_instance = new CEhDB();
    }

    return m_instance;
}

/*-------------------------------------------------------------------
 Function: getDataCount(pTblName);
           get the record count of the table signed by parameter 
 Parameters: pTblName -- [IN], table name
 Return: 0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int CEhDB::getDataCount(const char* pTblName)
{
    if (strcmp(pTblName, EH_TBL_REGION) == 0)
        return m_tblRegion.size();
    else if (strcmp(pTblName, EH_TBL_DEVICE) == 0)
        return m_tblDevice.size();
    else if (strcmp(pTblName, EH_TBL_OPPOINT) == 0)
        return m_tblOpPoint.size();    
    
    return -1;
}

/*-------------------------------------------------------------------
 Function: fetchData(pTblName, pos, p_data);
           get the record in the table signed by parameter 
 Parameters: pTblName -- [IN], table name
             pos -- [IN], record position
             p_data -- [OUT], record to fetch
 Return: 0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int CEhDB::fetchData(const char* pTblName, int pos, void* p_data)
{
    if (strcmp(pTblName, EH_TBL_REGION) == 0) {
        return fetchRegionData(pos, p_data);
    }
    else if (strcmp(pTblName, EH_TBL_DEVICE) == 0) {
        return fetchDeviceData(pos, p_data);
    }
    else if (strcmp(pTblName, EH_TBL_OPPOINT) == 0) {
        return fetchOpPointData(pos, p_data);
    }

    return -1;
}

int CEhDB::fetchRegionData(int pos, void* p_data)
{
    if (pos >= m_tblRegion.size())
        return -1;

    std::list<TagRegion*>::iterator it;
    int i = 0;
    for (it = m_tblRegion.begin(); 
        it != m_tblRegion.end(); it++) {
            if (i == pos)
                break;

            i++;
    }

    memcpy(p_data, (*it), sizeof(TagRegion));
    return 0;
}

int CEhDB::fetchDeviceData(int pos, void* p_data)
{
    if (pos >= m_tblDevice.size())
        return -1;

    std::list<TagDevice*>::iterator it;
    int i = 0;
    for (it = m_tblDevice.begin(); 
        it != m_tblDevice.end(); it++) {
            if (i == pos)
                break;

            i++;
    }

    memcpy(p_data, (*it), sizeof(TagDevice));
    return 0;
}

int CEhDB::fetchOpPointData(int pos, void* p_data)
{
    if (pos >= m_tblOpPoint.size())
        return -1;

    std::list<TagOpPoint*>::iterator it;
    int i = 0;
    for (it = m_tblOpPoint.begin(); 
        it != m_tblOpPoint.end(); it++) {
            if (i == pos)
                break;

            i++;
    }

    memcpy(p_data, (*it), sizeof(TagOpPoint));
    return 0;
}

/*-------------------------------------------------------------------
 Function: findDataById(pTblName, iId, p_data);
           find data from table by id
 Parameters: pTblName -- [IN], table name
             iId -- [IN], id
             p_data -- [OUT], record to fetch
 Return: 0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int CEhDB::findDataById(const char* pTblName, int iId, void* p_data)
{
    if (strcmp(pTblName, EH_TBL_REGION) == 0) {
        return findRegionDataById(iId, p_data);
    }
    else if (strcmp(pTblName, EH_TBL_DEVICE) == 0) {     
        return findDeviceDataById(iId, p_data);
    }
    else if (strcmp(pTblName, EH_TBL_OPPOINT) == 0) {
        return findOpPointDataById(iId, p_data);
    }    

    return -1;
}

int CEhDB::findRegionDataById(int iId, void* p_data)
{
    std::list<TagRegion*>::iterator it;
    for (it = m_tblRegion.begin(); 
        it != m_tblRegion.end(); it++) {
        if ((*it)->id == iId) {
            memcpy(p_data, *it, sizeof(TagRegion));
            return 0;
        }
    }

    return -1;
}

//0 -- success, -1 -- failed
int CEhDB::findDeviceDataById(int iId, void* p_data)
{
    std::list<TagDevice*>::iterator it;
    for (it = m_tblDevice.begin(); 
        it != m_tblDevice.end(); it++) {
        if ((*it)->id == iId) {
            memcpy(p_data, *it, sizeof(TagDevice));
            return 0;
        }
    }

    return -1;
}

int CEhDB::findOpPointDataById(int iId, void* p_data)
{
    std::list<TagOpPoint*>::iterator it;
    for (it = m_tblOpPoint.begin(); 
        it != m_tblOpPoint.end(); it++) {
        if ((*it)->id == iId) {
            memcpy(p_data, *it, sizeof(TagOpPoint));
            return 0;
        }
    }

    return -1;
}

/*-------------------------------------------------------------------
 Function: findDataByDesc(pTblName, pDesc, p_data);
           find data from table by the field description
 Parameters: pTblName -- [IN], table name
             pDesc -- [IN], description
             p_data -- [OUT], record to fetch
 Return: 0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int CEhDB::findDataByDesc(const char* pTblName, 
                          const char* pDesc, 
                          void* p_data)
{
    if (strcmp(pTblName, EH_TBL_REGION) == 0) {
        return findRegionDataByDesc(pDesc, p_data);
    }

    return -1;
}

int CEhDB::findRegionDataByDesc(const char* pDesc, void* p_data)
{
    std::list<TagRegion*>::iterator it;
    for (it = m_tblRegion.begin(); 
        it != m_tblRegion.end(); it++) {
        if (strcmp((*it)->desc, pDesc) == 0) {
            memcpy(p_data, *it, sizeof(TagRegion));
            return 0;
        }
    }

    return -1;
}

int CEhDB::addRegion(const char* pRegionName)
{
    //region name should not exist in table
    TagRegion regionData = {0};
    if (!findRegionDataByDesc(pRegionName, &regionData))
        return -1;

    TagRegion* pData = NULL;
    pData = new TagRegion();
    pData->id = getNewRegionID();
    strcpy(pData->desc, pRegionName);

    m_tblRegion.push_back(pData);    

    return 0;
}

int CEhDB::addDevice(const char* pDeviceName, int iRegionId)
{
    //check whether can be add
    TagRegion regionData = {0};
    if (findRegionDataById(iRegionId, &regionData))  //region id is not exist in region table
        return -1;
    
    TagDevice deviceData = {0};
    if (!findDeviceDataByDescAndRegionId(pDeviceName,
        iRegionId, &deviceData))
        return -1;

    TagDevice* pData = NULL;
    pData = new TagDevice();
    pData->id = getNewDeviceID();
    strcpy(pData->desc, pDeviceName);
    pData->regionId = iRegionId;

    m_tblDevice.push_back(pData);

    return 0;
}

int CEhDB::addOpPoint(const char* pOpName, int iDeviceId, int iDataType,
                      const char* pOpAddr, const char* pStatusAddr)
{
    //check whether can be added
    if (!isOpPointCanAdd(pOpName, iDeviceId, iDataType, pOpAddr, pStatusAddr))
        return -1;
  
    //get new operation data id
    TagOpPoint* pData = NULL;
    pData = new TagOpPoint();
    if (!pData)
        return -1;

    pData->id = getNewOpPointID();
    strcpy(pData->desc, pOpName);
    pData->deviceId = iDeviceId;
    pData->dataType = iDataType;
    strcpy(pData->opAddr, pOpAddr);
    strcpy(pData->statusAddr, pStatusAddr);
    strcpy(pData->strValue, "0");

    m_tblOpPoint.push_back(pData);

    return 0;
}

int CEhDB::findDeviceDataByDescAndRegionId(const char* pDesc, 
                                           int iRegionId, 
                                           void* p_data)
{
    std::list<TagDevice*>::iterator it;
    for (it = m_tblDevice.begin(); 
        it != m_tblDevice.end(); it++) {
        if (strcmp((*it)->desc, pDesc) == 0 &&
            (*it)->regionId == iRegionId) {
            memcpy(p_data, *it, sizeof(TagDevice));
            return 0;
        }
    }

    return -1; 
}

/*-------------------------------------------------------------------
 Function: isOpPointCanAdd(...);
           check whether operation point can be added
 Parameters: pOpName -- [IN], operation description
             iDeviceId -- [IN], device id which operation belong to
             iDataType -- [IN], operation type
             pOpAddr -- [IN], operation group address
             pStatusAddr -- [IN], status group address
 Return: 1 -- can add
         0 -- can not add
-------------------------------------------------------------------*/
int CEhDB::isOpPointCanAdd(const char* pOpName, int iDeviceId, 
                           int iDataType, const char* pOpAddr, 
                           const char* pStatusAddr)
{
    //check name
    if (!pOpName)
        return 0;

    //check device id in device table
    TagDevice deviceData = {0};
    if (findDeviceDataById(iDeviceId, &deviceData))
        return 0;

    //check desc and id in operation table
    TagOpPoint opPointData = {0};
    if (!findOpPointDataByDescAndDeviceId(pOpName, iDeviceId, &opPointData))
        return 0;

    //check datatype
    if (iDataType < DEV_TYPE_BOOL ||
        iDataType > DEV_TYPE_BYTE)
        return 0;

    //check op address
    char chFirst = 0;
    char chSecond = 0;
    if (parseGroupAddr(pOpAddr, &chFirst, &chSecond))
        return 0;

    //check status address
    if (parseGroupAddr(pStatusAddr, &chFirst, &chSecond))
        return 0;

    return 1;
}

int CEhDB::findOpPointDataByDescAndDeviceId(const char* pDesc, 
                                            int iDeviceId, 
                                            void* p_data)
{
    std::list<TagOpPoint*>::iterator it;
    for (it = m_tblOpPoint.begin(); 
        it != m_tblOpPoint.end(); it++) {
            if ((*it)->deviceId == iDeviceId &&
                strcmp((*it)->desc, pDesc) == 0) {
            memcpy(p_data, *it, sizeof(TagOpPoint));
            return 0;
        }
    }

    return -1;    
}

/*-------------------------------------------------------------------
 Function: deleteDataById(pTblName, iId);
           delete record from table by index
 Parameters: pTblName -- [IN], table name
             iId -- [IN], record index
 Return: 0 -- success
         -1 -- record is not exist
-------------------------------------------------------------------*/
int CEhDB::deleteDataById(const char*  pTblName, int iId)
{
    if (strcmp(pTblName, EH_TBL_REGION) == 0) {
        return deleteRegionAndRefDeviceById(iId);
    }
    else if (strcmp(pTblName, EH_TBL_DEVICE) == 0) {     
        return deleteDeviceAndRefOpPointById(iId);
    }
    else if (strcmp(pTblName, EH_TBL_OPPOINT) == 0) {
        return deleteOpPointById(iId);
    }    

    return -1;
}

int CEhDB::deleteRegionAndRefDeviceById(int iRegionId)
{
    deleteDeviceAndRefOpPointByRegionId(iRegionId);
    return deleteRegionById(iRegionId);
}

int CEhDB::deleteRegionById(int iId)
{
    std::list<TagRegion*>::iterator it;
    TagRegion* p_data = NULL;
    for (it = m_tblRegion.begin(); 
        it != m_tblRegion.end(); it++) {
        p_data = *it;
        if (p_data->id == iId) {
            m_tblRegion.erase(it);
            delete p_data;
            p_data = NULL;
            return 0;
        }
    }

    return -1;  
}

int CEhDB::deleteDeviceAndRefOpPointByRegionId(int iRegionId)
{
    int iRet = -1;
    std::list<TagDevice*>::iterator it;
    TagDevice* p_data = NULL;
    for (it = m_tblDevice.begin(); 
        it != m_tblDevice.end(); ) {
        p_data = *it;
        if (p_data->regionId == iRegionId) {
            //delete op point
            deleteOpPointByDeviceId(p_data->id);

            it = m_tblDevice.erase(it);
            delete p_data;
            p_data = NULL;
            iRet = 0;
        }
        else
            it++;
    }

    return iRet; 
}

int CEhDB::deleteDeviceAndRefOpPointById(int iId)
{
    deleteOpPointByDeviceId(iId);
    return deleteDeviceById(iId);
}

int CEhDB::deleteDeviceById(int iId)
{
    std::list<TagDevice*>::iterator it;
    TagDevice* p_data = NULL;
    for (it = m_tblDevice.begin(); 
        it != m_tblDevice.end(); it++) {
        p_data = *it;
        if (p_data->id == iId) {
            m_tblDevice.erase(it);
            delete p_data;
            p_data = NULL;
            return 0;
        }
    }

    return -1;  
}

int CEhDB::deleteOpPointByDeviceId(int iDeviceId)
{
    int iRet = -1;
    std::list<TagOpPoint*>::iterator it;
    TagOpPoint* p_data = NULL;
    for (it = m_tblOpPoint.begin(); 
        it != m_tblOpPoint.end(); ) {
        p_data = *it;
        if (p_data->deviceId == iDeviceId) {
            it = m_tblOpPoint.erase(it);
            delete p_data;
            p_data = NULL;
            iRet = 0;
        }
        else
            it++;
    }

    return iRet;  
}

int CEhDB::deleteOpPointById(int iId)
{
    std::list<TagOpPoint*>::iterator it;
    TagOpPoint* p_data = NULL;
    for (it = m_tblOpPoint.begin(); 
        it != m_tblOpPoint.end(); it++) {
        p_data = *it;
        if (p_data->id == iId) {
            m_tblOpPoint.erase(it);
            delete p_data;
            p_data = NULL;
            return 0;
        }
    }

    return -1;  
}

//update
int CEhDB::updateTblData(const char* pTblName, void* p_data)
{
    if (strcmp(pTblName, EH_TBL_REGION) == 0) {
        return updateRegion(p_data);
    }
    else if (strcmp(pTblName, EH_TBL_DEVICE) == 0) {     
        return updateDevice(p_data);
    }
    else if (strcmp(pTblName, EH_TBL_OPPOINT) == 0) {
        return updateOpPoint(p_data);
    }    

    return -1;
}

int CEhDB::updateRegion(void* p_data)
{
    //check whether can update
    if (!checkRegionCanUpdate((TagRegion*)p_data))
        return -1;
    
    TagRegion* p_regionData = NULL;
    
    std::list<TagRegion*>::iterator it;
    for (it = m_tblRegion.begin(); 
        it != m_tblRegion.end(); it++) {
        p_regionData = *it;
        if (p_regionData->id == ((TagRegion*)p_data)->id) {
            memcpy(p_regionData, p_data, sizeof(TagRegion));
            return 0;
        }
    }

    return 0;
}

int CEhDB::checkRegionCanUpdate(TagRegion* p_data)
{
    if (!p_data)
        return 0;

    TagRegion regionData = {0};
    if (findDataById(EH_TBL_REGION, p_data->id, &regionData))
        return 0;

    return 1;
}

int CEhDB::updateDevice(void* p_data)
{
    //check whether can update
    if (!checkDeviceCanUpdate((TagDevice*)p_data))
        return -1;
    
    TagDevice* p_deviceData = NULL;
    
    std::list<TagDevice*>::iterator it;
    for (it = m_tblDevice.begin(); 
        it != m_tblDevice.end(); it++) {
        p_deviceData = *it;
        if (p_deviceData->id == ((TagDevice*)p_data)->id) {
            memcpy(p_deviceData, p_data, sizeof(TagDevice));
            return 0;
        }
    }

    return 0;
}

int CEhDB::checkDeviceCanUpdate(TagDevice* p_data)
{
    if (!p_data)
        return 0;

    //check id
    TagDevice deviceData = {0};
    if (findDataById(EH_TBL_DEVICE, p_data->id, &deviceData))
        return 0;

    //check region id
    TagRegion regionData = {0};
    if (findDataById(EH_TBL_REGION, p_data->regionId, &regionData))
        return 0;

    //check desc
    if (!findDeviceDataByDescAndRegionId(p_data->desc, p_data->regionId,
        &deviceData) && deviceData.id != p_data->id)
        return 0;

    return 1;
}

int CEhDB::updateOpPoint(void* p_data)
{
    if (!checkOpPointCanUpdate((TagOpPoint*) p_data))
        return -1;

    TagOpPoint* p_opPointData = NULL;
    
    std::list<TagOpPoint*>::iterator it;
    for (it = m_tblOpPoint.begin(); 
        it != m_tblOpPoint.end(); it++) {
        p_opPointData = *it;
        if (p_opPointData->id == ((TagOpPoint*)p_data)->id) {
            memcpy(p_opPointData, p_data, sizeof(TagOpPoint));
            return 0;
        }
    }

    return 0;
}

/*-------------------------------------------------------------------
 Function: checkOpPointCanUpdate(p_data);
           check whether op point by input can be update to db
 Parameters: p_data -- [IN], data
 Return: 1 -- can update
         0 -- can not update
-------------------------------------------------------------------*/
int CEhDB::checkOpPointCanUpdate(TagOpPoint* p_data)
{
    if (!p_data)
        return 0;

    //check id
    TagOpPoint opPointData = {0};
    if (findDataById(EH_TBL_OPPOINT, p_data->id, &opPointData))
        return 0;

    //check device id
    TagDevice deviceData = {0};
    if (findDataById(EH_TBL_DEVICE, p_data->deviceId, &deviceData))
        return 0;

    //check desc
    if (!findOpPointDataByDescAndDeviceId(p_data->desc, p_data->deviceId,
        &opPointData) && opPointData.id != p_data->id)
        return 0;
    
    //check datatype
    if (p_data->dataType < DEV_TYPE_BOOL ||
        p_data->dataType > DEV_TYPE_BYTE)
        return 0;

    //check op address
    char chFirst = 0;
    char chSecond = 0;
    if (parseGroupAddr(p_data->opAddr, &chFirst, &chSecond))
        return 0;

    //check status address
    if (parseGroupAddr(p_data->statusAddr, &chFirst, &chSecond))
        return 0;

    return 1;
}

int CEhDB::getNewRegionID(void)
{
    int iRet = -1;
    std::list<TagRegion*>::iterator it;
    int iRecordCount = m_tblRegion.size();

    if (iRecordCount == 0)
        return 1;

    int* p_ID = new int[iRecordCount + 1];
    int iIDTemp = 0;
    memset(p_ID, 0, sizeof(int) * (iRecordCount + 1));

    for (it = m_tblRegion.begin(); 
        it != m_tblRegion.end(); it++) {
            iIDTemp = (*it)->id;
            if (iIDTemp < iRecordCount + 1)
                p_ID[iIDTemp] = 1;
    }

    iRet = iRecordCount + 1;
    for (int i = 1; i < iRecordCount + 1; i++) {
        if (p_ID[i] == 0) {
            iRet = i;
            break;
        }
    }

    delete [] p_ID;

    return iRet;
}

int CEhDB::getNewDeviceID(void)
{
    int iRet = -1;
    std::list<TagDevice*>::iterator it;
    int iRecordCount = m_tblDevice.size();

    if (iRecordCount == 0)
        return 1;

    int* p_ID = new int[iRecordCount + 1];
    int iIDTemp = 0;
    memset(p_ID, 0, sizeof(int) * (iRecordCount + 1));

    for (it = m_tblDevice.begin(); 
        it != m_tblDevice.end(); it++) {
            iIDTemp = (*it)->id;
            if (iIDTemp < iRecordCount + 1)
                p_ID[iIDTemp] = 1;
    }

    iRet = iRecordCount + 1;
    for (int i = 1; i < iRecordCount + 1; i++) {
        if (p_ID[i] == 0) {
            iRet = i;
            break;
        }
    }

    delete [] p_ID;

    return iRet;
}

int CEhDB::getNewOpPointID(void)
{
    int iRet = -1;
    std::list<TagOpPoint*>::iterator it;
    int iRecordCount = m_tblOpPoint.size();

    if (iRecordCount == 0)
        return 1;

    int* p_ID = new int[iRecordCount + 1];
    int iIDTemp = 0;
    memset(p_ID, 0, sizeof(int) * (iRecordCount + 1));

    for (it = m_tblOpPoint.begin(); 
        it != m_tblOpPoint.end(); it++) {
            iIDTemp = (*it)->id;
            if (iIDTemp < iRecordCount + 1)
                p_ID[iIDTemp] = 1;
    }

    iRet = iRecordCount + 1;
    for (int i = 1; i < iRecordCount + 1; i++) {
        if (p_ID[i] == 0) {
            iRet = i;
            break;
        }
    }

    delete [] p_ID;

    return iRet;
}

/////////////////////////////////////////////////////////////////////

int init_Db(void)
{
    CEhDB* pInst = CEhDB::getInstance();    
    return pInst->init();
}

int init_Db(const char* fileName)
{
    CEhDB* pInst = CEhDB::getInstance();    
    return pInst->init(fileName);
}

int save_Db(const char* fileName)
{
    CEhDB* pInst = CEhDB::getInstance();    
    return pInst->save(fileName);
}

int getTblRecordCount(const char* pTblName)
{
    CEhDB* pInst = CEhDB::getInstance();    
    return pInst->getDataCount(pTblName);
}

int fetchRecordData(const char* pTblName, int pos, void* p_data)
{
    CEhDB* pInst = CEhDB::getInstance();    
    return pInst->fetchData(pTblName, pos, p_data);
}

/*-------------------------------------------------------------------
 Function: addRegion(pRegionName);
           add a new region
 Parameters: pRegionName -- [IN], new region description
 Return: 0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int addRegion(const char* pRegionName)
{
    CEhDB* pInst = CEhDB::getInstance();    
    return pInst->addRegion(pRegionName);
}

int findDataById(const char* pTblName, int iId, void* p_data)
{
    CEhDB* pInst = CEhDB::getInstance();    
    return pInst->findDataById(pTblName, iId, p_data);
}

int findDataByDesc(const char* pTblName, const char* pDesc, void* p_data)
{
    CEhDB* pInst = CEhDB::getInstance();    
    return pInst->findDataByDesc(pTblName, pDesc, p_data);
}

int findDeviceDataByDescAndRegionId(const char* pDesc, int iRegionId, void* p_data)
{
    CEhDB* pInst = CEhDB::getInstance();    
    return pInst->findDeviceDataByDescAndRegionId(pDesc, iRegionId, p_data);
}

int findOpPointDataByDescAndDeviceId(const char* pDesc, int iDeviceId, void* p_data)
{
    CEhDB* pInst = CEhDB::getInstance();    
    return pInst->findOpPointDataByDescAndDeviceId(pDesc, iDeviceId, p_data);
}

int addDevice(const char* pDeviceName, int iRegionId)
{
    CEhDB* pInst = CEhDB::getInstance();    
    return pInst->addDevice(pDeviceName, iRegionId);
}

int addOpPoint(const char* pOpName, int iDeviceId, int iDataType, 
               const char* pOpAddr, const char* pStatusAddr)
{
    CEhDB* pInst = CEhDB::getInstance();    
    return pInst->addOpPoint(pOpName, iDeviceId, iDataType, 
               pOpAddr, pStatusAddr);
}

/*-------------------------------------------------------------------
 Function: deleteDataById(pTblName, iId);
           delete record from table by index
 Parameters: pTblName -- [IN], table name
             iId -- [IN], record index
 Return: 0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int deleteDataById(const char*  pTblName, int iId)
{
    CEhDB* pInst = CEhDB::getInstance();    
    return pInst->deleteDataById(pTblName, iId);
}

int updateTblData(const char* pTblName, void* p_data)
{
    CEhDB* pInst = CEhDB::getInstance();    
    return pInst->updateTblData(pTblName, p_data);
}