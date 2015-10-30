#ifndef __TESTEHDATABASE_H__
#define __TESTEHDATABASE_H__

#include "ehDataBase.h"

class CTestEhDataBase
{
private:
    void testGetNewDeviceID(void);
    void testAddDevice(void);

    void testFindDeviceDataByID(void);
    void testFindDeviceDataByDescAndRegionId(void);
    void testIsOpPointCanAdd(void);
    void testGetNewOpPointID(void);
    void testAddOpPoint(void);
    void testDeleteDataById(void);
    void testFindOpPointDataByDescAndDeviceId(void);
    void testDeleteDeviceById(void);
    void testDeleteOpPointByDeviceId(void);
    void testDeleteRegionById(void);
    void testDeleteDeviceAndRefOpPointByRegionId(void);
    void testDeleteRegionAndRefDeviceById(void);
    void testUpdateOpPoint(void);
    void testFindDataById(void);
    void testUpdateDevice(void);
    void testUpdateRegion(void);
    void testSaveDb(void);

public:    
    void test(void);

};

#endif  //__TESTEHDATABASE_H__