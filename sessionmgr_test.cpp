#include "gtest/gtest.h"
#include "sessionmgr.h"
#include <string>
#define STRINGSIZE 100
using namespace std;
          
TEST(SessionMgrTest, CreateAndDestroy)
{
    	CSessionMgr *t1 = CSessionMgr::GetInstance();
    	EXPECT_EQ(NULL, t1);
   	CSessionMgr::Create();
    	EXPECT_EQ(0, CSessionMgr::GetInstance()->Init());
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
    	CSessionMgr::GetInstance()->DeleteInstance();
    	CSessionMgr *t5 = CSessionMgr::GetInstance();
    	EXPECT_EQ(NULL, t5);
    	CSessionMgr::GetInstance()->DeleteInstance();
    	CSessionMgr *t6 = CSessionMgr::GetInstance();
    	EXPECT_EQ(NULL, t6);
}


bool existInStrs(string strs[], int strsSize, const string & str)
{
	for(int i=0; i<strsSize; i++)
	{
		if(strs[i].compare(str) == 0)
			return true;
	}
	return false;
}

TEST(SessionMgrTest, GetSessionId)
{
	string strs[STRINGSIZE];
	string temp;
	CSessionMgr::Create();
	strs[0] = "abcdefg";
	for(int i=1; i < STRINGSIZE; i++)
	{
		CSessionMgr::GetInstance()->GetSessionId(temp);
		EXPECT_FALSE(existInStrs(strs, i, temp));
		strs[i] = temp;
	}
	temp = "abcdefg";
	EXPECT_TRUE(existInStrs(strs, STRINGSIZE, temp));
}
