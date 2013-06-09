#include "Global.h"


extern struct ROOM Room;

extern DWORD GameState;
int BlockUntil;

bool ToSend = true;

const wchar_t * PlayerColor[12] =
{
	_T("|CFFFF0303"),
	_T("|CFF0042FF"),
	_T("|CFF1CE6B9"),
	_T("|CFF540081"),
	_T("|CFFFFFC01"),
	_T("|CFFFE8A0E"),
	_T("|CFF20C000"),
	_T("|CFFE55BB0"),
	_T("|CFF959697"),
	_T("|CFF7EBFF1"),
	_T("|CFF106246"),
	_T("|CFF4E2A04")
};

CStringW BuildMessage(int i)
{
	CStringW msg;
	msg.Format(_T("%s%d.  %-20s|r %-20s %s"),
		PlayerColor[Room.line[i].color],
		i + 1,
		Room.player[Room.line[i].playerId].name,
		Room.player[Room.line[i].playerId].ip,
		Room.player[Room.line[i].playerId].location
		);
	return msg;
}

int ParseInt(char *str)
{
	if (!isdigit(*str))
	{
		return -1;
	}
	int result = 0;
	bool isInt = false;
	while (*str)
	{
		isInt = true;
		if (isdigit(*str))
		{
			result = result * 10 + (*str - '0');
			++str;
		}
	}
	return result;
}



bool IsRecordRoom()
{
	CStringW str(Room.player[1].ip);
	if (str.CompareNoCase(_T("202.120.58.146")) == 0)
		return true;
	else
		return false;
}

void ShowInfoInRoom(int flag)
{
	if (flag == 0) // all
	{
		CStringW msg;
		msg.Format(_T("------------------------- ȫ����� -------------------------"));
		TextOutInRoom(msg);
		for (int i = 0; i < Room.count; ++i)
		{
			if (Room.line[i].emptyOrClosed == 2 && Room.line[i].humanOrAi == 0)
			{
				TextOutInRoom(BuildMessage(i));
			}
		}
		msg.Format(_T("------------------------------------------------------------"));
		TextOutInRoom(msg);
	}
	else if (flag > 0) // player flag
	{
		int i = flag;
		if (i > 0 && i <= Room.count && Room.line[i - 1].emptyOrClosed == 2 && Room.line[i - 1].humanOrAi == 0)
		{
			TextOutInRoom(BuildMessage(i - 1));
		}
	}
	else // team flag
	{
		int k = -flag;
		CStringW msg;
		if (k == 1)
		{
			msg.Format(_T("------------------------- %s��������|r -------------------------"), PlayerColor[0]);
		}
		else if (k == 2)
		{
			msg.Format(_T("------------------------- %s���־���|r -------------------------"), PlayerColor[6]);
		}
		else
		{
			msg.Format(_T("------------------------- ���� %d -------------------------"), k);
		}
		TextOutInRoom(msg);
		for (int i = 0; i < Room.count; ++i)
		{
			if (Room.line[i].emptyOrClosed == 2 && Room.line[i].humanOrAi == 0 && Room.line[i].team == k - 1)
			{
				TextOutInRoom(BuildMessage(i));
			}
		}
		msg.Format(_T("------------------------------------------------------------"));
		TextOutInRoom(msg);
	}
}

void ShowInfoInGame(int flag)
{
	if (flag == 0) // all
	{
		CStringW msg;
		msg.Format(_T("ȫ�����:"));
		TextOutInGame(msg);
		for (int i = 0; i < Room.count; ++i)
		{
			if (Room.line[i].emptyOrClosed == 2 && Room.line[i].humanOrAi == 0)
			{
				TextOutInGame(BuildMessage(i));
			}
		}
	}
	else if (flag > 0) // player flag
	{
		int i = flag;
		if (i > 0 && i <= Room.count && Room.line[i - 1].emptyOrClosed == 2 && Room.line[i - 1].humanOrAi == 0)
		{
			TextOutInGame(BuildMessage(i - 1));
		}
	}
	else // team flag
	{
		int k = -flag;
		CStringW msg;
		if (k == 1)
		{
			msg.Format(_T("%s��������:"), PlayerColor[0]);
		}
		else if (k == 2)
		{
			msg.Format(_T("%s���־���:"), PlayerColor[6]);
		}
		else
		{
			msg.Format(_T("���� %d:"), k);
		}
		TextOutInGame(msg);
		for (int i = 0; i < Room.count; ++i)
		{
			if (Room.line[i].emptyOrClosed == 2 && Room.line[i].humanOrAi == 0 && Room.line[i].team == k - 1)
			{
				TextOutInGame(BuildMessage(i));
			}
		}
	}
}

bool OnSayInRoom(const char *text)
{
	CStringA str(text);
	if (str.CompareNoCase("-all") == 0)
	{
		ShowInfoInRoom(0);
		//if (!IsRecordRoom())
		//{
		//	
		//}
		//else
		//{
		//	//Statsdota(0);
		//	// Pending = 0;
		//	////for (int i = 0; i < Room.count; ++i)
		//	////{
		//	////	if (Room.line[i].emptyOrClosed == 2 && Room.line[i].humanOrAi == 0)
		//	////	{
		//	////		++Pending;
		//	////	}
		//	////}
		//	////for (int i = 0; i < Room.count; ++i)
		//	////{
		//	////	if (Room.line[i].emptyOrClosed == 2 && Room.line[i].humanOrAi == 0)
		//	////	{
		//	////		Statsdota(i);
		//	////	}
		//	////}
		//	//
		//}
		return true;
	}
	else if (str.CompareNoCase("-jw") == 0 || str.CompareNoCase("-jinwei") == 0) // ���� Todo:����Ƿ�DotAͼ?
	{
		ShowInfoInRoom(-1);
		return true;
	}
	else if (str.CompareNoCase("-tz") == 0 || str.CompareNoCase("-tianzai") == 0) // ����
	{
		ShowInfoInRoom(-2);
		return true;
	}
	else if (str.Left(5).CompareNoCase("-team") == 0)
	{
		int k = atoi(str.Mid(5));
		ShowInfoInRoom(-k);
		return true;
	}
	else if (str.GetAt(0) == '-') // ָ��λ��
	{
		int i = atoi(str.Mid(1));
		if (i > 0)
		{
			ShowInfoInRoom(i);
			if (IsRecordRoom())
			{
				Statsdota(i);
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}


bool OnSayInGame(const char *text)
{
	CStringA str(text);
	if (str.CompareNoCase("-all") == 0)
	{
		ShowInfoInGame(0);
		return true;
	}
	else if (str.CompareNoCase("-jw") == 0 || str.CompareNoCase("-jinwei") == 0) // ���� Todo:����Ƿ�DotAͼ?
	{
		ShowInfoInGame(-1);
		return true;
	}
	else if (str.CompareNoCase("-tz") == 0 || str.CompareNoCase("-tianzai") == 0) // ����
	{
		ShowInfoInGame(-2);
		return true;
	}
	else if (str.Left(5).CompareNoCase("-team") == 0)
	{
		int k = atoi(str.Mid(5));
		ShowInfoInGame(-k);
		return true;

	}
	else if (str.CompareNoCase("-woyaodiaoxian") == 0)
	{
		int now = GetTickCount();
		BlockUntil = now + 10000;
		return true;
	}
	else if (str.GetAt(0) == '-') // ָ��λ��
	{
		int i = atoi(str.Mid(1));
		if (i > 0)
		{
			ShowInfoInGame(i);
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}


void OnDisplayInRoom(const char *text)
{
	if (GameState != 1) // ֻ�ڷ����з���
		return;

	if (!ToSend)
	{
		ToSend = true;
		return;
	}
	
	if (XWindowExist())
	{
		CStringW wstr = UTF8ToUTF16(text);

		int len = (wcslen(wstr) + 1) * 2;
		CopyDataSend(1, len, (void*)wstr.GetBuffer(len));
		wstr.ReleaseBuffer();
	}
}