#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "framecommon/framecommon.h"
#include "mybasecmd.h"
#include "mybasecmd_h_test.h"
using namespace testing;

/**
 *结构体TCmdHead的测试
 *命令的头结构
 *回包和发包中都有
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
 * 该类实现编码命令
 * 发包中使用
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
	EXPECT_EQ(0, encodeCmd.GetCmdBody().GetSize());
	EXPECT_EQ(12, encodeCmd.GetCmdHead().GetBufLen());
}

TEST(CEncodeCmdTest, ProcessCmdTestOne)
{
	MyCEncodeCmd encodeCmd;
	encodeCmd.HeadBodyNoCall();
	EXPECT_EQ(-1, encodeCmd.Process());
}

TEST(CEncodeCmdTest, ProcessCmdTestTwo)
{
	MyCEncodeCmd encodeCmd;
	encodeCmd.HeadBodyCall();
	EXPECT_EQ(-1, encodeCmd.Process());
	
	EXPECT_EQ(0, encodeCmd.Process());
	EXPECT_EQ(10, encodeCmd.GetBufLen());

	EXPECT_EQ(-1, encodeCmd.Process());
	EXPECT_EQ(-1, encodeCmd.Process());
	EXPECT_EQ(-2, encodeCmd.Process());
}

/**
 * 类CDecodeCmd的测试
 * 该类实现解码命令
 * 收包中使用
 */
TEST(CDecodeCmdTest, Create)
{
	char *temp = new char[20];
	CDecodeCmd<MockTCmdHead, MockTCmdBody> decodeCmd(temp, 20);
	EXPECT_EQ(temp, decodeCmd.GetBuf());
	EXPECT_EQ(20, decodeCmd.GetBufLen());
	
	CDecodeCmd<MockTCmdHead, MockTCmdBody> decodeCmd_1(NULL, 0);
	EXPECT_EQ(NULL, decodeCmd_1.GetBuf());
	EXPECT_EQ(0, decodeCmd_1.GetBufLen());
}

TEST(CDecodeCmdTest, BufTest)
{
	CDecodeCmd<MockTCmdHead, MockTCmdBody> decodeCmd(NULL, 0);
	EXPECT_EQ(NULL, decodeCmd.GetBuf());
	EXPECT_EQ(0, decodeCmd.GetBufLen());

	char *temp = new char[20];	
	decodeCmd.AttachBuf(temp, 20);
	EXPECT_EQ(temp, decodeCmd.GetBuf());
	EXPECT_EQ(20, decodeCmd.GetBufLen());
	
	decodeCmd.DetachBuf();
	EXPECT_EQ(NULL, decodeCmd.GetBuf());
	EXPECT_EQ(0, decodeCmd.GetBufLen());
}

TEST(CDecodeCmdTest, GetCmdHeadAndBodyTypeTest)
{
	CDecodeCmd<TCmdHead, MockTCmdBody> decodeCmd(NULL, 0);
	::testing::StaticAssertTypeEq<MockTCmdBody, typeof(decodeCmd.GetCmdBody())>();
	::testing::StaticAssertTypeEq<TCmdHead, typeof(decodeCmd.GetCmdHead())>();
	EXPECT_EQ(0, decodeCmd.GetCmdBody().GetSize());
	EXPECT_EQ(12, decodeCmd.GetCmdHead().GetBufLen());
}

TEST(CDecodeCmdTest, Process)
{
	MyCDecodeCmd decodeCmd(NULL, 0);
	decodeCmd.HeadBodyCall();
	EXPECT_EQ(-1, decodeCmd.Process());
	
	EXPECT_EQ(0, decodeCmd.Process());
	EXPECT_EQ(-1, decodeCmd.Process());
	EXPECT_EQ(-1, decodeCmd.Process());
	EXPECT_EQ(-1, decodeCmd.Process());
}






