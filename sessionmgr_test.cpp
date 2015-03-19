#include "gtest/gtest.h"
#include "sessionmgr.h"
#include <string>
using namespace std;
          
TEST(SessionMgrTest, CreateAndDestroy)
{
    CSessionMgr *t1 = CSessionMgr::GetInstance();
    EXPECT_EQ(NULL, t1);
    CSessionMgr::Create();
    CSessionMgr *t2 = CSessionMgr::GetInstance();
    EXPECT_NE(t1, t2);
    CSessionMgr::Create();
    EXPECT_EQ(t2, CSessionMgr::GetInstance());

    CSessionMgr::Destroy();
    CSessionMgr *t3 = CSessionMgr::GetInstance();
    EXPECT_EQ(NULL, t3);
    
    CSessionMgr::Destroy();
    CSessionMgr *t4 = CSessionMgr::GetInstance();
    EXPECT_EQ(NULL, t4);    
    
    CSessionMgr::Create();
    string str;
    CSessionMgr::GetInstance()->GetSessionId(str);

}
