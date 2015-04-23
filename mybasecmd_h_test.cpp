#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "framecommon/framecommon.h"
#include "mybasecmd.h"
using namespace testing;

/**
 *结构体TCmdHead的测试
 */

TEST(TCmdHeadTest, Create)
{
	TCmdHead cmdHead;
	EXPECT_EQ(0, cmdHead.m_nProtocolVer);
	EXPECT_EQ(0, cmdHead.m_nSequence);
	EXPECT_EQ(0, cmdHead.m_nBodyLen);
	EXPECT_EQ(0, cmdHead.m_nCmdId);
}

TEST(TCmdHeadTest, GetCmdId)
{
	TCmdHead cmdHead;
	EXPECT_EQ(-1, cmdHead.GetCmdId(NULL, 20));
	char temp[20];
	EXPECT_EQ(-1, cmdHead.GetCmdId(temp, 12));
	temp[12] = 88;
	EXPECT_EQ(88, cmdHead.GetCmdId(temp, 20));
}

TEST(TCmdHeadTest, GetBodyLen)
{
	TCmdHead cmdHead;
	EXPECT_EQ(-1, cmdHead.GetBodyLen(NULL, 20));
	char temp[20];
	EXPECT_EQ(-1, cmdHead.GetBodyLen(temp, 8));
	EXPECT_EQ(-1, cmdHead.GetBodyLen(NULL, 8));
	int *pBodyLen = (int *)(temp + 8); 
	*pBodyLen = htonl(80);
	EXPECT_EQ(80, cmdHead.GetBodyLen(temp, 20));
	*pBodyLen = htonl(1024);
	EXPECT_EQ(1024, cmdHead.GetBodyLen(temp, 20));
}

TEST(TCmdHeadTest, GetSize)
{
	TCmdHead cmdHead;
	EXPECT_EQ(13, cmdHead.GetSize());
}

TEST(TCmdHeadTest, Encode)
{
	char * temp = new char[20];
	CBufMgr bufMgr(temp ,20);
	TCmdHead cmdHead;
	cmdHead.m_nProtocolVer=900;
	cmdHead.m_nSequence=200;
	cmdHead.m_nBodyLen=20;
	cmdHead.m_nCmdId=99;
	cmdHead.Encode(bufMgr);
	bufMgr.SetOffset(0);

	TCmdHead cmdHead_copy;
	bufMgr.GetInt(cmdHead_copy.m_nProtocolVer);
	bufMgr.GetInt(cmdHead_copy.m_nSequence);
	bufMgr.GetInt(cmdHead_copy.m_nBodyLen);
	bufMgr.GetChar(cmdHead_copy.m_nCmdId);
	EXPECT_EQ(900, cmdHead_copy.m_nProtocolVer); 
	EXPECT_EQ(200, cmdHead_copy.m_nSequence);
	EXPECT_EQ(20, cmdHead_copy.m_nBodyLen);
	EXPECT_EQ(99, cmdHead_copy.m_nCmdId);
}

TEST(TCmdHeadTest, Decode)
{
	char * temp = new char[20];
        CBufMgr bufMgr(temp ,20);
        TCmdHead cmdHead;
        cmdHead.m_nProtocolVer=900;
        cmdHead.m_nSequence=200;
        cmdHead.m_nBodyLen=20;
        cmdHead.m_nCmdId=99;
        cmdHead.Encode(bufMgr);
        bufMgr.SetOffset(0);
        TCmdHead cmdHead_copy;

	cmdHead_copy.Decode(bufMgr);
	EXPECT_EQ(900, cmdHead_copy.m_nProtocolVer);
        EXPECT_EQ(200, cmdHead_copy.m_nSequence);
        EXPECT_EQ(20, cmdHead_copy.m_nBodyLen);
        EXPECT_EQ(99, cmdHead_copy.m_nCmdId);
}

class MockTCmdBody
{
	public:
	
	MOCK_METHOD0(GetSize, int());

	MOCK_METHOD1(Encode, int(CBufMgr &bufMgr));
};

TEST(TCmdHeadTest, SetBodyLen)
{
	TCmdHead cmdHead;
	MockTCmdBody cmdBody;
	EXPECT_CALL(cmdBody, GetSize())
		.WillOnce(Return(0))
		.WillOnce(Return(1024))
		.WillOnce(Return(2047));
	cmdHead.SetBodyLen(cmdBody);
	EXPECT_EQ(1, cmdHead.m_nBodyLen);
	cmdHead.SetBodyLen(cmdBody);
	EXPECT_EQ(1025, cmdHead.m_nBodyLen);
	cmdHead.SetBodyLen(cmdBody);
	EXPECT_EQ(2048, cmdHead.m_nBodyLen);
}

TEST(TCmdHeadTest, GetBufLen)
{
	TCmdHead cmdHead;
	MockTCmdBody cmdBody;
	EXPECT_CALL(cmdBody, GetSize())
		.WillOnce(Return(1024));
	cmdHead.SetBodyLen(cmdBody);
	EXPECT_EQ(12+1025, cmdHead.GetBufLen());
}

/**
 * 类CEncodeCmd的测试
 */

TEST(CEncodeCmdTest, Create)
{
	CEncodeCmd<TCmdHead, MockTCmdBody> encodeCmd;
	EXPECT_EQ(NULL, encodeCmd.GetBuf());
	EXPECT_EQ(0, encodeCmd.GetBufLen());
}

TEST(CEncodeCmdTest, BufTest)
{
	CEncodeCmd<TCmdHead, MockTCmdBody> encodeCmd;
	EXPECT_EQ(NULL, encodeCmd.GetBuf());
	EXPECT_EQ(0, encodeCmd.GetBufLen());

	char *temp = new char[20];	
	encodeCmd.AttachBuf(temp, 20);
	EXPECT_EQ(temp, encodeCmd.GetBuf());
	EXPECT_EQ(20, encodeCmd.GetBufLen());
	
	encodeCmd.DetachBuf();
	EXPECT_EQ(NULL, encodeCmd.GetBuf());
	EXPECT_EQ(0, encodeCmd.GetBufLen());
}

TEST(CEncodeCmdTest, GetCmdHeadAndBodyTypeTest)
{
	CEncodeCmd<TCmdHead, MockTCmdBody> encodeCmd;
	::testing::StaticAssertTypeEq<MockTCmdBody, typeof(encodeCmd.GetCmdBody())>();
	::testing::StaticAssertTypeEq<TCmdHead, typeof(encodeCmd.GetCmdHead())>();
}

/*
class MockTCmdHead : TCmdHead
{
	public:

	MOCK_METHOD1(Encode, int(CBufMgr &bufMgr));
};

class MyCEncodeCmd : public CEncodeCmd<MockTCmdHead, MockTCmdBody>
{
	MyCEncodeCmd()
	{
		EXPECT_CALL(m_CmdHead, Encode(_))
			.WillOnce(Return(0))
			.WillOnce(Return(0))
			.WillRepeatedly(Return(-1));

		EXPECT_CALL(m_CmdBody, Encode(_))
			.WillOnce(Return(0))
			.WillOnce(Return(-1))
			.WillOnce(Return(0))
			.WillOnce(Return(-1));
	}
};

TEST(CEncodeCmdTest, Process)
{
	
}
*/











