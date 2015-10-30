#ifndef __TESTEHDBMANAGER_H__
#define __TESTEHDBMANAGER_H__

#include "ehDbManager.h"

class CTestEhDbManager
{
private:
    CEhJsonTransfer m_testInst;

    void testGenTransJsonFromText(void);
    void testGetDataCount(void);
    void testFetchData(void);
    void testSaveDb(void);
    void testAddData(void);
    void testGenRootJsonFromDataJson(void);

public:
    void test(void);
};

#endif  //__TESTEHDBMANAGER_H__