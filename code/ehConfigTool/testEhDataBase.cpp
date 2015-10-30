
#include "stdafx.h"
#include "testEhDataBase.h"


void CTestEhDataBase::testGetNewDeviceID(void)
{
    CEhDB* pInst = CEhDB::getInstance();    
    int devId = pInst->getNewDeviceID();

    CString strTemp;
    strTemp.Format("new devid is %d", devId);
    MessageBox(NULL, strTemp, "test", 0);    
}

void CTestEhDataBase::testAddDevice(void)
{
    CEhDB* pInst = CEhDB::getInstance();  
    CString strTemp;

    int ret = pInst->addDevice("a", 1);
    if (ret) 
        strTemp.Format("add failed, id = 1, name = a");
    else
        strTemp.Format("add success, id = 1, name = a");
    MessageBox(NULL, strTemp, "test", 0);   

    ret = pInst->addDevice("a", 1);
    if (ret) 
        strTemp.Format("add failed, id = 1, name = a");
    else
        strTemp.Format("add success, id = 1, name = a");
    MessageBox(NULL, strTemp, "test", 0);   

    ret = pInst->addDevice("b", 4);
    if (ret) 
        strTemp.Format("add failed, id = 4, name = b");
    else
        strTemp.Format("add success, id = 4, name = b");
    MessageBox(NULL, strTemp, "test", 0);   
}

void CTestEhDataBase::testFindDeviceDataByID(void)
{
    CString strTemp;
    TagDevice deviceData = {0};
    CEhDB* pInst = CEhDB::getInstance();    
    int devId = 5;
    if (pInst->findDeviceDataById(devId, &deviceData)) {
        strTemp.Format("device not find, devid is %d", devId);
        MessageBox(NULL, strTemp, "test", 0); 
    }
    else {
        strTemp.Format("device find, devid is %d, device name is %s",
            devId, deviceData.desc);
        MessageBox(NULL, strTemp, "test", 0); 
    }

    devId = 15;
    if (pInst->findDeviceDataById(devId, &deviceData)) {
        strTemp.Format("device not find, devid is %d", devId);
        MessageBox(NULL, strTemp, "test", 0); 
    }
    else {
        strTemp.Format("device find, devid is %d, device name is %s",
            devId, deviceData.desc);
        MessageBox(NULL, strTemp, "test", 0); 
    }       
}

void CTestEhDataBase::testFindDeviceDataByDescAndRegionId(void)
{
    CString strTemp;
    TagOpPoint opPointData = {0};
    CEhDB* pInst = CEhDB::getInstance();    
    int devId = 1;
    if (pInst->findOpPointDataByDescAndDeviceId("iec照明开关", devId, &opPointData)) {
        strTemp.Format("oppoint not find, devid is %d, desc is %s",
            devId, "iec照明开关");
        MessageBox(NULL, strTemp, "test", 0); 
    }
    else {
        strTemp.Format("oppoint find, devid is %d, desc is %s, type is %d, "
            " addr is %s, status addr is %s", opPointData.deviceId, opPointData.desc,
            opPointData.dataType, opPointData.opAddr, opPointData.statusAddr);
        MessageBox(NULL, strTemp, "test", 0); 
    }

    devId = 2;
    if (pInst->findOpPointDataByDescAndDeviceId("iec照明开关", devId, &opPointData)) {
        strTemp.Format("oppoint not find, devid is %d, desc is %s",
            devId, "iec照明开关");
        MessageBox(NULL, strTemp, "test", 0); 
    }
    else {
        strTemp.Format("oppoint find, devid is %d, desc is %s, type is %d, "
            " addr is %s, status addr is %s", opPointData.deviceId, opPointData.desc,
            opPointData.dataType, opPointData.opAddr, opPointData.statusAddr);
        MessageBox(NULL, strTemp, "test", 0); 
    }
}

void CTestEhDataBase::testIsOpPointCanAdd(void)
{
    CString strTemp;
    TagOpPoint opPointData = {0};
    char szOpName[256] = {0};
    char szOpAddr[16] = {0};
    char szStatusAddr[16] = {0};
    int iDataType = 0;
    int iDevId = 0;
    CEhDB* pInst = CEhDB::getInstance();    

    iDevId = 1;
    strcpy(szOpName, "iec照明开关");
    strcpy(szOpAddr, "10/1/11");
    strcpy(szStatusAddr, "10/1/11");
    iDataType = 1;

    if (pInst->isOpPointCanAdd(NULL, iDevId, iDataType, szOpAddr, szStatusAddr)) {
        strTemp.Format("point can add, devid is %d, desc is %s, "
            "type is %d, opaddr is %s, status address is %s",
            iDevId, szOpName, iDataType, szOpAddr, szStatusAddr);
        MessageBox(NULL, strTemp, "test", 0); 
    }
    else {
        strTemp.Format("point can not add, devid is %d, desc is %s, "
            "type is %d, opaddr is %s, status address is %s",
            iDevId, szOpName, iDataType, szOpAddr, szStatusAddr);
        MessageBox(NULL, strTemp, "test", 0); 
    }

    if (pInst->isOpPointCanAdd(szOpName, iDevId, iDataType, szOpAddr, szStatusAddr)) {
        strTemp.Format("point can add, devid is %d, desc is %s, "
            "type is %d, opaddr is %s, status address is %s",
            iDevId, szOpName, iDataType, szOpAddr, szStatusAddr);
        MessageBox(NULL, strTemp, "test", 0); 
    }
    else {
        strTemp.Format("point can not add, devid is %d, desc is %s, "
            "type is %d, opaddr is %s, status address is %s",
            iDevId, szOpName, iDataType, szOpAddr, szStatusAddr);
        MessageBox(NULL, strTemp, "test", 0); 
    }

    iDevId = 1;
    strcpy(szOpName, "iec照明开关2");
    strcpy(szOpAddr, "10/1/11");
    strcpy(szStatusAddr, "10/1/11");
    iDataType = 1;
    if (pInst->isOpPointCanAdd(szOpName, iDevId, iDataType, szOpAddr, szStatusAddr)) {
        strTemp.Format("point can add, devid is %d, desc is %s, "
            "type is %d, opaddr is %s, status address is %s",
            iDevId, szOpName, iDataType, szOpAddr, szStatusAddr);
        MessageBox(NULL, strTemp, "test", 0); 
    }
    else {
        strTemp.Format("point can not add, devid is %d, desc is %s, "
            "type is %d, opaddr is %s, status address is %s",
            iDevId, szOpName, iDataType, szOpAddr, szStatusAddr);
        MessageBox(NULL, strTemp, "test", 0); 
    }

    iDevId = 15;
    strcpy(szOpName, "iec照明开关3");
    strcpy(szOpAddr, "10/1/11");
    strcpy(szStatusAddr, "10/1/11");
    iDataType = 1;
    if (pInst->isOpPointCanAdd(szOpName, iDevId, iDataType, szOpAddr, szStatusAddr)) {
        strTemp.Format("point can add, devid is %d, desc is %s, "
            "type is %d, opaddr is %s, status address is %s",
            iDevId, szOpName, iDataType, szOpAddr, szStatusAddr);
        MessageBox(NULL, strTemp, "test", 0); 
    }
    else {
        strTemp.Format("point can not add, devid is %d, desc is %s, "
            "type is %d, opaddr is %s, status address is %s",
            iDevId, szOpName, iDataType, szOpAddr, szStatusAddr);
        MessageBox(NULL, strTemp, "test", 0); 
    }

    iDevId = 1;
    strcpy(szOpName, "iec照明开关3");
    strcpy(szOpAddr, "10/1/11");
    strcpy(szStatusAddr, "10/1/11");
    iDataType = 10;
    if (pInst->isOpPointCanAdd(szOpName, iDevId, iDataType, szOpAddr, szStatusAddr)) {
        strTemp.Format("point can add, devid is %d, desc is %s, "
            "type is %d, opaddr is %s, status address is %s",
            iDevId, szOpName, iDataType, szOpAddr, szStatusAddr);
        MessageBox(NULL, strTemp, "test", 0); 
    }
    else {
        strTemp.Format("point can not add, devid is %d, desc is %s, "
            "type is %d, opaddr is %s, status address is %s",
            iDevId, szOpName, iDataType, szOpAddr, szStatusAddr);
        MessageBox(NULL, strTemp, "test", 0); 
    }

    iDevId = 1;
    strcpy(szOpName, "iec照明开关3");
    strcpy(szOpAddr, "1110/1/11");
    strcpy(szStatusAddr, "10/1/11");
    iDataType = 1;
    if (pInst->isOpPointCanAdd(szOpName, iDevId, iDataType, szOpAddr, szStatusAddr)) {
        strTemp.Format("point can add, devid is %d, desc is %s, "
            "type is %d, opaddr is %s, status address is %s",
            iDevId, szOpName, iDataType, szOpAddr, szStatusAddr);
        MessageBox(NULL, strTemp, "test", 0); 
    }
    else {
        strTemp.Format("point can not add, devid is %d, desc is %s, "
            "type is %d, opaddr is %s, status address is %s",
            iDevId, szOpName, iDataType, szOpAddr, szStatusAddr);
        MessageBox(NULL, strTemp, "test", 0); 
    }
}

void CTestEhDataBase::testGetNewOpPointID(void)
{
    CString strTemp;
    CEhDB* pInst = CEhDB::getInstance();  
    int iID = pInst->getNewOpPointID();
    strTemp.Format("new id = %d", iID);

    MessageBox(NULL, strTemp, "test", 0); 

}

void CTestEhDataBase::testAddOpPoint(void)
{
    CString strTemp;
    CEhDB* pInst = CEhDB::getInstance(); 

    int iRecordCount = getTblRecordCount(EH_TBL_OPPOINT);

    strTemp.Format("record count = %d", iRecordCount);
    MessageBox(NULL, strTemp, "test", 0); 

    if (addOpPoint("aaaa", 1, 1, "11/1/11", "11/1/11"))
        MessageBox(NULL, "add op point failed", "test", 0); 
    else
        MessageBox(NULL, "add op point success", "test", 0); 

    iRecordCount = getTblRecordCount(EH_TBL_OPPOINT);

    strTemp.Format("after add record count = %d", iRecordCount);
    MessageBox(NULL, strTemp, "test", 0); 
}

void CTestEhDataBase::testDeleteDataById(void)
{
    CString strTemp;

    int iRecordCount = getTblRecordCount(EH_TBL_OPPOINT);

    strTemp.Format("record count = %d", iRecordCount);
    MessageBox(NULL, strTemp, "test", 0); 


    deleteDataById(EH_TBL_OPPOINT, 1);

    iRecordCount = getTblRecordCount(EH_TBL_OPPOINT);
    strTemp.Format("after delete record1, count = %d", iRecordCount);
    MessageBox(NULL, strTemp, "test", 0); 

    deleteDataById(EH_TBL_OPPOINT, 1);
    iRecordCount = getTblRecordCount(EH_TBL_OPPOINT);
    strTemp.Format("after delete record1, count = %d", iRecordCount);
    MessageBox(NULL, strTemp, "test", 0); 

    deleteDataById(EH_TBL_OPPOINT, 2);
    iRecordCount = getTblRecordCount(EH_TBL_OPPOINT);
    strTemp.Format("after delete record2, count = %d", iRecordCount);
    MessageBox(NULL, strTemp, "test", 0); 
}

void CTestEhDataBase::testDeleteDeviceById(void)
{
    CString strTemp;

    int iDeviceCount = getTblRecordCount(EH_TBL_DEVICE);
    int iOpPointCount = getTblRecordCount(EH_TBL_OPPOINT);

    strTemp.Format("device count = %d, op point count = %d", 
        iDeviceCount, iOpPointCount);
    MessageBox(NULL, strTemp, "test", 0); 


    deleteDataById(EH_TBL_DEVICE, 1);

    MessageBox(NULL, "delete device 1", "test", 0); 

    iDeviceCount = getTblRecordCount(EH_TBL_DEVICE);
    iOpPointCount = getTblRecordCount(EH_TBL_OPPOINT);

    strTemp.Format("device count = %d, op point count = %d", 
        iDeviceCount, iOpPointCount);
    MessageBox(NULL, strTemp, "test", 0); 

    deleteDataById(EH_TBL_DEVICE, 1);

    MessageBox(NULL, "delete device 1", "test", 0); 

    iDeviceCount = getTblRecordCount(EH_TBL_DEVICE);
    iOpPointCount = getTblRecordCount(EH_TBL_OPPOINT);

    strTemp.Format("device count = %d, op point count = %d", 
        iDeviceCount, iOpPointCount);
    MessageBox(NULL, strTemp, "test", 0); 

    deleteDataById(EH_TBL_DEVICE, 2);

    MessageBox(NULL, "delete device 2", "test", 0); 

    iDeviceCount = getTblRecordCount(EH_TBL_DEVICE);
    iOpPointCount = getTblRecordCount(EH_TBL_OPPOINT);

    strTemp.Format("device count = %d, op point count = %d", 
        iDeviceCount, iOpPointCount);
    MessageBox(NULL, strTemp, "test", 0); 
}

void CTestEhDataBase::testDeleteOpPointByDeviceId(void)
{
    CString strTemp;

    int iRecordCount = getTblRecordCount(EH_TBL_OPPOINT);

    strTemp.Format("record count = %d", iRecordCount);
    MessageBox(NULL, strTemp, "test", 0); 


    CEhDB* pInst = CEhDB::getInstance();  
    pInst->deleteOpPointByDeviceId(1);
    
    iRecordCount = getTblRecordCount(EH_TBL_OPPOINT);
    strTemp.Format("after delete record1, count = %d", iRecordCount);
    MessageBox(NULL, strTemp, "test", 0); 

    pInst->deleteOpPointByDeviceId(1);
    
    iRecordCount = getTblRecordCount(EH_TBL_OPPOINT);
    strTemp.Format("after delete record1, count = %d", iRecordCount);
    MessageBox(NULL, strTemp, "test", 0); 

    pInst->deleteOpPointByDeviceId(2);
    
    iRecordCount = getTblRecordCount(EH_TBL_OPPOINT);
    strTemp.Format("after delete record2, count = %d", iRecordCount);
    MessageBox(NULL, strTemp, "test", 0); 

}

void CTestEhDataBase::testFindOpPointDataByDescAndDeviceId(void)
{
    CString strTemp;

    int iRecordCount = getTblRecordCount(EH_TBL_OPPOINT);
    TagOpPoint opPointData = {0};
   
    if (findOpPointDataByDescAndDeviceId("iec照明开关", 1, &opPointData)) {
        strTemp.Format("find opPoint failed");
        MessageBox(NULL, strTemp, "test", 0); 
    }
    else {
        strTemp.Format("point is found, devid is %d, desc is %s, "
            "type is %d, opaddr is %s, status address is %s",
            opPointData.deviceId, opPointData.desc, opPointData.dataType,
            opPointData.opAddr, opPointData.statusAddr);        
        MessageBox(NULL, strTemp, "test", 0); 
    }

    if (findOpPointDataByDescAndDeviceId("iec照明开关", 2, &opPointData)) {
        strTemp.Format("find opPoint failed");
        MessageBox(NULL, strTemp, "test", 0); 
    }
    else {
        strTemp.Format("point is found, devid is %d, desc is %s, "
            "type is %d, opaddr is %s, status address is %s",
            opPointData.deviceId, opPointData.desc, opPointData.dataType,
            opPointData.opAddr, opPointData.statusAddr);        
        MessageBox(NULL, strTemp, "test", 0); 
    }

    if (findOpPointDataByDescAndDeviceId("MSI单排照明开关", 2, &opPointData)) {
        strTemp.Format("find opPoint failed");
        MessageBox(NULL, strTemp, "test", 0); 
    }
    else {
        strTemp.Format("point is found, devid is %d, desc is %s, "
            "type is %d, opaddr is %s, status address is %s",
            opPointData.deviceId, opPointData.desc, opPointData.dataType,
            opPointData.opAddr, opPointData.statusAddr);        
        MessageBox(NULL, strTemp, "test", 0); 
    }

}

void CTestEhDataBase::testDeleteRegionById(void)
{
    CString strTemp;
    int iRet = 0;

    int iRecordCount = getTblRecordCount(EH_TBL_REGION);
    strTemp.Format("record count = %d", iRecordCount);
    MessageBox(NULL, strTemp, "test", 0); 

    CEhDB* pInst = CEhDB::getInstance();  
    iRet = pInst->deleteRegionById(1);
    strTemp.Format("delete region 1, ret = %d", iRet);
    MessageBox(NULL, strTemp, "test", 0);
    
    iRecordCount = getTblRecordCount(EH_TBL_REGION);
    strTemp.Format("record count = %d", iRecordCount);
    MessageBox(NULL, strTemp, "test", 0); 

    /////////////////////
    iRet = pInst->deleteRegionById(1);
    strTemp.Format("delete region 1, ret = %d", iRet);
    MessageBox(NULL, strTemp, "test", 0);

    iRecordCount = getTblRecordCount(EH_TBL_REGION);
    strTemp.Format("record count = %d", iRecordCount);
    MessageBox(NULL, strTemp, "test", 0); 


    //////////////////////////////
    iRet = pInst->deleteRegionById(2);
    strTemp.Format("delete region 2, ret = %d", iRet);
    MessageBox(NULL, strTemp, "test", 0);

    iRecordCount = getTblRecordCount(EH_TBL_REGION);
    strTemp.Format("record count = %d", iRecordCount);
    MessageBox(NULL, strTemp, "test", 0); 
}

void CTestEhDataBase::testDeleteDeviceAndRefOpPointByRegionId(void)
{
    CString strTemp;
    int iRet = 0;

    int iDeviceCount = getTblRecordCount(EH_TBL_DEVICE);
    int iOpPointCount = getTblRecordCount(EH_TBL_OPPOINT);

    strTemp.Format("device count = %d, op point count = %d", 
        iDeviceCount, iOpPointCount);
    MessageBox(NULL, strTemp, "test", 0);

    /////////////////////////////////
    CEhDB* pInst = CEhDB::getInstance();  
    iRet = pInst->deleteDeviceAndRefOpPointByRegionId(1);
    strTemp.Format("delete device and ref op point by region id 1, ret = %d",
        iRet);
    MessageBox(NULL, strTemp, "test", 0);

    iDeviceCount = getTblRecordCount(EH_TBL_DEVICE);
    iOpPointCount = getTblRecordCount(EH_TBL_OPPOINT);

    strTemp.Format("device count = %d, op point count = %d", 
        iDeviceCount, iOpPointCount);
    MessageBox(NULL, strTemp, "test", 0);

    /////////////////////////////////
    iRet = pInst->deleteDeviceAndRefOpPointByRegionId(1);
    strTemp.Format("delete device and ref op point by region id 1, ret = %d",
        iRet);
    MessageBox(NULL, strTemp, "test", 0);

    iDeviceCount = getTblRecordCount(EH_TBL_DEVICE);
    iOpPointCount = getTblRecordCount(EH_TBL_OPPOINT);

    strTemp.Format("device count = %d, op point count = %d", 
        iDeviceCount, iOpPointCount);
    MessageBox(NULL, strTemp, "test", 0);

    /////////////////////////////////
    iRet = pInst->deleteDeviceAndRefOpPointByRegionId(2);
    strTemp.Format("delete device and ref op point by region id 2, ret = %d",
        iRet);
    MessageBox(NULL, strTemp, "test", 0);

    iDeviceCount = getTblRecordCount(EH_TBL_DEVICE);
    iOpPointCount = getTblRecordCount(EH_TBL_OPPOINT);

    strTemp.Format("device count = %d, op point count = %d", 
        iDeviceCount, iOpPointCount);
    MessageBox(NULL, strTemp, "test", 0);
}

void CTestEhDataBase::testDeleteRegionAndRefDeviceById(void)
{
    CString strTemp;
    int iRet = 0;

    int iRegionCount = getTblRecordCount(EH_TBL_REGION);
    int iDeviceCount = getTblRecordCount(EH_TBL_DEVICE);
    int iOpPointCount = getTblRecordCount(EH_TBL_OPPOINT);

    strTemp.Format("region count = %d, device count = %d, op point count = %d", 
        iRegionCount, iDeviceCount, iOpPointCount);
    MessageBox(NULL, strTemp, "test", 0);

    /////////////////////////////////
    iRet = deleteDataById(EH_TBL_REGION, 1);
    strTemp.Format("delete region and ref device by region id 1, ret = %d",
        iRet);
    MessageBox(NULL, strTemp, "test", 0);

    iRegionCount = getTblRecordCount(EH_TBL_REGION);
    iDeviceCount = getTblRecordCount(EH_TBL_DEVICE);
    iOpPointCount = getTblRecordCount(EH_TBL_OPPOINT);

    strTemp.Format("region count = %d, device count = %d, op point count = %d", 
        iRegionCount, iDeviceCount, iOpPointCount);
    MessageBox(NULL, strTemp, "test", 0);

    /////////////////////////////////
    iRet = deleteDataById(EH_TBL_REGION, 1);
    strTemp.Format("delete region and ref device by region id 1, ret = %d",
        iRet);
    MessageBox(NULL, strTemp, "test", 0);

    iRegionCount = getTblRecordCount(EH_TBL_REGION);
    iDeviceCount = getTblRecordCount(EH_TBL_DEVICE);
    iOpPointCount = getTblRecordCount(EH_TBL_OPPOINT);

    strTemp.Format("region count = %d, device count = %d, op point count = %d", 
        iRegionCount, iDeviceCount, iOpPointCount);
    MessageBox(NULL, strTemp, "test", 0);

    /////////////////////////////////
    iRet = deleteDataById(EH_TBL_REGION, 1);
    strTemp.Format("delete region and ref device by region id 1, ret = %d",
        iRet);
    MessageBox(NULL, strTemp, "test", 0);

    iRegionCount = getTblRecordCount(EH_TBL_REGION);
    iDeviceCount = getTblRecordCount(EH_TBL_DEVICE);
    iOpPointCount = getTblRecordCount(EH_TBL_OPPOINT);

    strTemp.Format("region count = %d, device count = %d, op point count = %d", 
        iRegionCount, iDeviceCount, iOpPointCount);
    MessageBox(NULL, strTemp, "test", 0);

}

void CTestEhDataBase::testUpdateOpPoint(void)
{
    CString strTemp;
    int iRet = 0;

    TagRegion regionData = {0};
    TagDevice deviceData = {0};
    TagOpPoint opPointData = {0};

    ////////////////////////////
    iRet = findDataById(EH_TBL_OPPOINT, 9, &opPointData);
    if (iRet == 0) {
        strTemp.Format("find oppoint data1 success\n"
            "id = %d, desc = %s, dataType = %d, deviceId = %d"
            " opaddr = %s, statusAddr = %s", 
            opPointData.id, opPointData.desc, 
            opPointData.dataType, opPointData.deviceId,
            opPointData.opAddr, opPointData.statusAddr);
    }
    else {
        strTemp.Format("find device data1 failed\n");
    }

    MessageBox(NULL, strTemp, "test", 0); 

    //update
    opPointData.id = 9;
    strcpy(opPointData.desc, "new desc");

    opPointData.deviceId = 1;
    opPointData.dataType = 2;
    strcpy(opPointData.opAddr, "1/1/1");
    strcpy(opPointData.statusAddr, "1/1/2");
    iRet = updateTblData(EH_TBL_OPPOINT, &opPointData);
    if (iRet == 0) {
        strTemp.Format("update oppoint data success\n"
            "id = %d, desc = %s, dataType = %d, deviceId = %d"
            " opaddr = %s, statusAddr = %s", 
            opPointData.id, opPointData.desc, 
            opPointData.dataType, opPointData.deviceId,
            opPointData.opAddr, opPointData.statusAddr);
    }
    else {
        strTemp.Format("update oppoint data failed\n"
            "id = %d, desc = %s, dataType = %d, deviceId = %d"
            " opaddr = %s, statusAddr = %s", 
            opPointData.id, opPointData.desc, 
            opPointData.dataType, opPointData.deviceId,
            opPointData.opAddr, opPointData.statusAddr);
    }
    MessageBox(NULL, strTemp, "test", 0); 

    ////////////////////////////
    iRet = findDataById(EH_TBL_OPPOINT, 9, &opPointData);
    if (iRet == 0) {
        strTemp.Format("find oppoint data1 success\n"
            "id = %d, desc = %s, dataType = %d, deviceId = %d"
            " opaddr = %s, statusAddr = %s", 
            opPointData.id, opPointData.desc, 
            opPointData.dataType, opPointData.deviceId,
            opPointData.opAddr, opPointData.statusAddr);
    }
    else {
        strTemp.Format("find device data1 failed\n");
    }

    MessageBox(NULL, strTemp, "test", 0); 
}

void CTestEhDataBase::testFindDataById(void)
{
    CString strTemp;
    int iRet = 0;

    TagRegion regionData = {0};
    TagDevice deviceData = {0};
    TagOpPoint opPointData = {0};

    ////////////////////////////
    iRet = findDataById(EH_TBL_REGION, 1, &regionData);
    if (iRet == 0) {
        strTemp.Format("find region data1 success\n"
            "id = %d, desc = %s", regionData.id, regionData.desc);
    }
    else {
        strTemp.Format("find region data1 failed\n");
    }

    MessageBox(NULL, strTemp, "test", 0);

    ////////////////////////////
    iRet = findDataById(EH_TBL_DEVICE, 1, &deviceData);
    if (iRet == 0) {
        strTemp.Format("find device data1 success\n"
            "id = %d, desc = %s, region id = %d", 
            deviceData.id, deviceData.desc, deviceData.regionId);
    }
    else {
        strTemp.Format("find device data1 failed\n");
    }

    MessageBox(NULL, strTemp, "test", 0);

    ////////////////////////////
    iRet = findDataById(EH_TBL_OPPOINT, 9, &opPointData);
    if (iRet == 0) {
        strTemp.Format("find oppoint data1 success\n"
            "id = %d, desc = %s, dataType = %d, deviceId = %d"
            " opaddr = %s, statusAddr = %s", 
            opPointData.id, opPointData.desc, 
            opPointData.dataType, opPointData.deviceId,
            opPointData.opAddr, opPointData.statusAddr);
    }
    else {
        strTemp.Format("find device data1 failed\n");
    }

    MessageBox(NULL, strTemp, "test", 0);   

}

void CTestEhDataBase::testUpdateDevice(void)
{
    CString strTemp;
    int iRet = 0;

    TagRegion regionData = {0};
    TagDevice deviceData = {0};
    TagOpPoint opPointData = {0};

    ////////////////////////////
    iRet = findDataById(EH_TBL_DEVICE, 1, &deviceData);
    if (iRet == 0) {
        strTemp.Format("find device data1 success\n"
            "id = %d, desc = %s, regionId = %d", 
            deviceData.id, deviceData.desc, 
            deviceData.regionId);
    }
    else {
        strTemp.Format("find device data1 failed\n");
    }

    MessageBox(NULL, strTemp, "test", 0); 

    //update
    deviceData.id = 1;
    strcpy(deviceData.desc, "筒灯");

    deviceData.regionId = 2;
    iRet = updateTblData(EH_TBL_DEVICE, &deviceData);
    if (iRet == 0) {
        strTemp.Format("update device data1 success\n"
            "id = %d, desc = %s, regionId = %d", 
            deviceData.id, deviceData.desc, 
            deviceData.regionId);
    }
    else {
        strTemp.Format("update device data1 failed\n"
            "id = %d, desc = %s, regionId = %d", 
            deviceData.id, deviceData.desc, 
            deviceData.regionId);
    }
    MessageBox(NULL, strTemp, "test", 0); 

    ////////////////////////////
    iRet = findDataById(EH_TBL_DEVICE, 1, &deviceData);
    if (iRet == 0) {
        strTemp.Format("find device data1 success\n"
            "id = %d, desc = %s, regionId = %d", 
            deviceData.id, deviceData.desc, 
            deviceData.regionId);
    }
    else {
        strTemp.Format("find device data1 failed\n");
    }

    MessageBox(NULL, strTemp, "test", 0); 
}

void CTestEhDataBase::testUpdateRegion(void)
{
    CString strTemp;
    int iRet = 0;

    TagRegion regionData = {0};
    TagDevice deviceData = {0};
    TagOpPoint opPointData = {0};

    ////////////////////////////
    iRet = findDataById(EH_TBL_REGION, 1, &regionData);
    if (iRet == 0) {
        strTemp.Format("find region data1 success\n"
            "id = %d, desc = %s", 
            regionData.id, regionData.desc);
    }
    else {
        strTemp.Format("find region data1 failed\n");
    }

    MessageBox(NULL, strTemp, "test", 0); 

    //update
    regionData.id = 11;
    strcpy(regionData.desc, "筒灯区");

    iRet = updateTblData(EH_TBL_REGION, &regionData);
    if (iRet == 0) {
        strTemp.Format("update region data1 success\n"
            "id = %d, desc = %s", 
            regionData.id, regionData.desc);
    }
    else {
        strTemp.Format("update region data1 failed\n"
            "id = %d, desc = %s", 
            regionData.id, regionData.desc);
    }
    MessageBox(NULL, strTemp, "test", 0); 

    ////////////////////////////
    iRet = findDataById(EH_TBL_REGION, 1, &regionData);
    if (iRet == 0) {
        strTemp.Format("find region data1 success\n"
            "id = %d, desc = %s", 
            regionData.id, regionData.desc);
    }
    else {
        strTemp.Format("find region data1 failed\n");
    }

    MessageBox(NULL, strTemp, "test", 0); 
}

void CTestEhDataBase::testSaveDb(void)
{
    if (init_Db("knx_json.json")) {
        MessageBox(NULL, "init failed", "test", 0); 
        return ;
    }
    else
        MessageBox(NULL, "init success", "test", 0); 


    if (save_Db("knx_jsonggg.json")) {
        MessageBox(NULL, "save failed", "test", 0); 
        return ;
    }
    else
        MessageBox(NULL, "save success", "test", 0); 
}

void CTestEhDataBase::test(void)
{
    //testGetNewDeviceID();
    //testAddDevice();
    //testFindDeviceDataByID();
    //testFindDeviceDataByDescAndRegionId();
    ///testIsOpPointCanAdd();
    //testGetNewOpPointID();
    //testAddOpPoint();
    //testDeleteDataById();
    //testFindOpPointDataByDescAndDeviceId();
    //testDeleteDeviceById();
    //testDeleteOpPointByDeviceId();
    //testDeleteRegionById();
    //testDeleteDeviceAndRefOpPointByRegionId();
    //testDeleteRegionAndRefDeviceById();
    //testFindDataById();
    //testUpdateOpPoint();
    //testUpdateDevice();
    //testUpdateRegion();
    testSaveDb();
}
