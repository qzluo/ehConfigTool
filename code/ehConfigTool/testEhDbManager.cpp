#include "stdafx.h"
#include "testEhDbManager.h"


void CTestEhDbManager::testGenTransJsonFromText(void)
{
    CString strTemp;

    int ret = m_testInst.setDB("knx_json.dat");
    if (ret) {
        strTemp.Format("setDB failed.");
        MessageBox(NULL, strTemp, "test", 0);   
    }
    else {
        char* pRegionData = m_testInst.printRegionData();
        MessageBox(NULL, pRegionData, "test", 0);  

        char* pDeviceData = m_testInst.printDeviceData();
        MessageBox(NULL, pDeviceData, "test", 0); 

        char* pOpPointData = m_testInst.printOpPointData();
        MessageBox(NULL, pOpPointData, "test", 0); 
    }

    return ;
}

void CTestEhDbManager::testGetDataCount(void)
{
    CString strTemp;

    strTemp.Format("regiong count = %d, device count = %d, oppoint count = %d",
            m_testInst.getDataCount(EH_TBL_REGION),
            m_testInst.getDataCount(EH_TBL_DEVICE),
            m_testInst.getDataCount(EH_TBL_OPPOINT));
        MessageBox(NULL, strTemp, "test", 0);  

    int ret = m_testInst.setDB("knx_json.dat");
    if (ret) {
        strTemp.Format("setDB failed.");
        MessageBox(NULL, strTemp, "test", 0);   
    }
    else {
        strTemp.Format("regiong count = %d, device count = %d, oppoint count = %d",
            m_testInst.getDataCount(EH_TBL_REGION),
            m_testInst.getDataCount(EH_TBL_DEVICE),
            m_testInst.getDataCount(EH_TBL_OPPOINT));
        MessageBox(NULL, strTemp, "test", 0);  
    }
}

void CTestEhDbManager::testFetchData(void)
{
    CString strTemp;
    TagRegion regionData = {0};
    TagDevice deviceData = {0};
    TagOpPoint opPointData = {0};

    int ret = m_testInst.setDB("knx_json.dat");
    if (ret) {
        strTemp.Format("setDB failed.");
        MessageBox(NULL, strTemp, "test", 0);   
        return;
    }

    ret = m_testInst.fetchData(EH_TBL_REGION, 1, &regionData);
    if (ret) {
        strTemp.Format("fetch region failed.");
        MessageBox(NULL, strTemp, "test", 0);   
        return;
    }
    else {
        strTemp.Format("id = %d, desc = %s",
            regionData.id, regionData.desc);
        MessageBox(NULL, strTemp, "test", 0);
    }

    ret = m_testInst.fetchData(EH_TBL_DEVICE, 1, &deviceData);
    if (ret) {
        strTemp.Format("fetch device failed.");
        MessageBox(NULL, strTemp, "test", 0);   
        return;
    }
    else {
        strTemp.Format("dev id = %d, dev desc = %s"
            " region id = %d",
            deviceData.id, deviceData.desc, deviceData.regionId);
        MessageBox(NULL, strTemp, "test", 0);
    }

    ret = m_testInst.fetchData(EH_TBL_OPPOINT, 1, &opPointData);
    if (ret) {
        strTemp.Format("fetch oppoint failed.");
        MessageBox(NULL, strTemp, "test", 0);   
        return;
    }
    else {
        strTemp.Format("oppoint id = %d, oppoint desc = %s"
            " oppoint datatype = %d, devid = %d"
            " addr = %s, status addr = %s",
            opPointData.id, opPointData.desc, opPointData.dataType, 
            opPointData.deviceId, opPointData.opAddr,
            opPointData.statusAddr);
        MessageBox(NULL, strTemp, "test", 0);
    }
}

void CTestEhDbManager::testSaveDb(void)
{
    CString strTemp;

    int ret = m_testInst.setDB("knx_json.json");
    if (ret) {
        strTemp.Format("setDB failed.");
        MessageBox(NULL, strTemp, "test", 0);   
        return;
    }

    m_testInst.saveDB("knx_json43.json");
}

void CTestEhDbManager::testAddData(void)
{
    CString strTemp;    
    TagRegion regionData = {0};
    TagDevice deviceData = {0};
    TagOpPoint opPointData = {0};

    regionData.id = 1;
    strcpy(regionData.desc, "test region");

    int ret = m_testInst.addData(EH_TBL_REGION, &regionData);
    if (ret) {
        strTemp.Format("add data failed.");
        MessageBox(NULL, strTemp, "test", 0);   
        return;
    }

    regionData.id = 2;
    strcpy(regionData.desc, "test region2");

    ret = m_testInst.addData(EH_TBL_REGION, &regionData);
    if (ret) {
        strTemp.Format("add data failed.");
        MessageBox(NULL, strTemp, "test", 0);   
        return;
    }
    else {
        ;//MessageBox(NULL, m_testInst.printRegionData(), "test", 0); 
    }

    deviceData.id = 1;
    deviceData.regionId = 1;
    strcpy(deviceData.desc, "test device");
    ret = m_testInst.addData(EH_TBL_DEVICE, &deviceData);
    if (ret) {
        strTemp.Format("add data failed.");
        MessageBox(NULL, strTemp, "test", 0);   
        return;
    }

    deviceData.id = 2;
    deviceData.regionId = 1;
    strcpy(deviceData.desc, "test device2");
    ret = m_testInst.addData(EH_TBL_DEVICE, &deviceData);
    if (ret) {
        strTemp.Format("add data failed.");
        MessageBox(NULL, strTemp, "test", 0);   
        return;
    }
    else {
        ;//MessageBox(NULL, m_testInst.printDeviceData(), "test", 0); 
    }

    //op point add
    opPointData.id = 1;
    opPointData.dataType = 1;
    opPointData.deviceId = 1;
    strcpy(opPointData.desc, "op point 1");
    strcpy(opPointData.opAddr, "11/1/11");
    strcpy(opPointData.statusAddr, "11/1/11");
    strcpy(opPointData.strValue, "1");
    ret = m_testInst.addData(EH_TBL_OPPOINT, &opPointData);
    if (ret) {
        strTemp.Format("add data failed.");
        MessageBox(NULL, strTemp, "test", 0);   
        return;
    }
    else {
        ;//MessageBox(NULL, m_testInst.printOpPointData(), "test", 0); 
    }
}

void CTestEhDbManager::testGenRootJsonFromDataJson(void)
{
    testAddData();
    CString strTemp;    
    
    int ret = m_testInst.genRootJsonFromDataJson();
    if (ret) {
        strTemp.Format("genRootJsonFromDataJson failed.");
        MessageBox(NULL, strTemp, "test", 0);   
        return;
    }
    else {
        MessageBox(NULL, m_testInst.printRootData(), "test", 0); 
    }
}

void CTestEhDbManager::test(void)
{
//    testGenTransJsonFromText();
   // testGetDataCount();
   // testFetchData();
    //testSaveDb();
    //testAddData();
    testGenRootJsonFromDataJson();
    return ;
}
