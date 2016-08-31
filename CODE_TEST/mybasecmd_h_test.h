#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mybasecmd.h"
#include "framecommon/framecommon.h"
using namespace testing;
using namespace MYFRAMECOMMON;


class MockTCmdBody
{
	public:
	
	MOCK_METHOD0(GetSize, int());

	MOCK_METHOD1(Encode, int(CBufMgr &bufMgr));
	
	MOCK_METHOD1(Decode, int(CBufMgr &bufMgr));
};

class MockTCmdHead
{
	public:
	
	MOCK_METHOD1(SetBodyLen, void(MockTCmdBody &CmdBody));

	MOCK_METHOD0(GetBufLen, int());
	
	MOCK_METHOD1(Encode, int(CBufMgr& BufMgr));

	MOCK_METHOD1(Decode, int(CBufMgr& BufMgr));
};

class MyCEncodeCmd : public CEncodeCmd<MockTCmdHead, MockTCmdBody>
{
	public:
	MyCEncodeCmd(){	temp = 0;}
	void HeadBodyCall()
	{
		EXPECT_CALL(m_CmdHead, Encode(_))
			.WillOnce(Return(0))	
			.WillOnce(Return(0))	
			.WillRepeatedly(Return(-1));
		EXPECT_CALL(m_CmdHead, SetBodyLen(_))
			.Times(4);
		EXPECT_CALL(m_CmdHead, GetBufLen())
			.WillRepeatedly(Return(10));

		EXPECT_CALL(m_CmdBody, Encode(_))
			.WillOnce(Return(0))	
			.WillOnce(Return(-1))	
			.WillOnce(Return(0))	
			.WillOnce(Return(-1));
	}
	void HeadBodyNoCall()
	{
		EXPECT_CALL(m_CmdHead, Encode(_)).Times(0);
		EXPECT_CALL(m_CmdHead, SetBodyLen(_)).Times(0);
		EXPECT_CALL(m_CmdHead, GetBufLen()).Times(0);
		
		EXPECT_CALL(m_CmdBody, Encode(_)).Times(0);
	}

	protected:
	virtual int ProcessCmd()
	{
		if(0 == temp)
		{
			temp++;
			return -1;	
		}
		return CEncodeCmd<MockTCmdHead, MockTCmdBody>::ProcessCmd();
	}

	private:
	int temp;
};

class MyCDecodeCmd : public CDecodeCmd<MockTCmdHead, MockTCmdBody>
{
	public:
	MyCDecodeCmd(char* pBuf, int nBufLen):CDecodeCmd<MockTCmdHead, MockTCmdBody>(pBuf, nBufLen)
	{
		temp = 0;
	}
	void HeadBodyCall()
	{
		EXPECT_CALL(m_CmdHead, Decode(_))
			.WillOnce(Return(0))	
			.WillOnce(Return(0))	
			.WillOnce(Return(0))	
			.WillRepeatedly(Return(-1));

		EXPECT_CALL(m_CmdBody, Decode(_))
			.WillOnce(Return(0))	
			.WillOnce(Return(0))	
			.WillOnce(Return(-1));
	}	
	protected:
	virtual int ProcessCmd()
	{
		if(0 == temp)
		{
			temp++;
			return -1;
		}
		return CDecodeCmd<MockTCmdHead, MockTCmdBody>::ProcessCmd();
	}

	private:
	int temp;
};
