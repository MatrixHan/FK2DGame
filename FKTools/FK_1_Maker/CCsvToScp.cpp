/**
*	created:		2012-7-4   20:32
*	filename: 		CCsvToScp
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		CSVToSCP�����ڰ�*.csv��ʽ���ļ�ת��Ϊ*.scp��ʽ���ļ�
*/
//------------------------------------------------------------------------
#include "StdAfx.h"
#include "CCSVToSCP.h"
//------------------------------------------------------------------------
TCHAR wchBuf[MAX_STRING_LEN];
TCHAR wchObjectName[MAX_STRING_LEN];
//------------------------------------------------------------------------
CCSVToSCP::CCSVToSCP()
{
}
//------------------------------------------------------------------------
CCSVToSCP::~CCSVToSCP()
{
}
//------------------------------------------------------------------------
BOOL CCSVToSCP::MakeCSVToSCP(LPCTSTR szCSVFileName, LPCTSTR szSCPFileName, LPTSTR szErrMsg, BOOL bPlus2Comma)
{
	ifstream inCsvFile(szCSVFileName, ios::in);
	if ( !inCsvFile )
	{
		sprintf(szErrMsg, "ת��ʧ�ܣ�\n��CSV�ļ�ʧ�ܣ���ʾ��CSV�ļ�����Ϊ���ģ�");
		return FALSE;
	}

	ofstream outSCPFile(szSCPFileName, ios::out);
	if ( !outSCPFile )
	{
		sprintf(szErrMsg, "ת��ʧ�ܣ�\n����FK1�ļ�ʧ�ܣ�");
		inCsvFile.close();
		return FALSE;
	}

	inCsvFile.getline(wchBuf, sizeof(wchBuf));
	if ( strlen(wchBuf) == 0 )
	{
		sprintf(szErrMsg, "ת��ʧ�ܣ�\nCSV�ļ���һ��Ϊ�գ�");
		outSCPFile.close();
		inCsvFile.close();
		return FALSE;
	}

	CString cstrFirstRow[256];
	int nQueue = SaveFirstRow(wchBuf, strlen(wchBuf), cstrFirstRow);
	int nLen = PrintConstStr(outSCPFile);				// ���ͷ��ʾ

	inCsvFile.getline(wchBuf, sizeof(wchBuf));

	int nResObjectLevel[MAX_OBJECT_LEVEL]= {0};
	int nCurResObject = 0;
	int nMaxID = 1;
	int nCurID = 1;
	vector<int> nVectorID;

	BOOL bFirst = TRUE;
	int nOldLevel = 0;
	
	while(1)
	{
		memset(wchBuf, 0, sizeof(wchBuf));
		inCsvFile.get(wchBuf, 4, ',');
		int nNewLevel = 0;
		nNewLevel = atoi(wchBuf);
		if ( 0 == nNewLevel)
		{
			if(( inCsvFile.eof() != 0 ) || ( inCsvFile.bad() != 0 ) || ( inCsvFile.fail() != 0 ))
			{
				break;
			}
			
			inCsvFile.getline(wchBuf, sizeof(wchBuf));
			continue;
		}

		nNewLevel = (nNewLevel > 0) ? nNewLevel : 1;

		if ( TRUE == bFirst)
		{
			nCurResObject++;
			bFirst = FALSE;
		}
		else
			if (nNewLevel > nOldLevel)
			{
				nCurResObject++;
				nResObjectLevel[nCurResObject] = nResObjectLevel[nCurResObject - 1] + (nNewLevel - nOldLevel);
				outSCPFile << "\n";
			}
			else
				if (nNewLevel < nOldLevel)
				{
					for (int i = nOldLevel; i >= nNewLevel && nCurResObject >= 1; i--)
					{
						PrintTab(outSCPFile, nResObjectLevel[nCurResObject]);
						outSCPFile << "}\n";
						nCurResObject--;
					}

					outSCPFile << "\n";
					nCurResObject++;
				}
				else
				{
					PrintTab(outSCPFile, nResObjectLevel[nCurResObject]);
					outSCPFile << "}\n\n";
				}
		nOldLevel = nNewLevel;

		PrintTab(outSCPFile, nResObjectLevel[nCurResObject]);
		outSCPFile << "Object ";
		inCsvFile.getline(wchBuf, sizeof(wchBuf), ',');
		inCsvFile.get(wchBuf, sizeof(wchBuf), ',');
		outSCPFile << wchBuf;
		strcpy(wchObjectName, wchBuf);
		outSCPFile << " = ";
		inCsvFile.get();
		inCsvFile.get(wchBuf, sizeof(wchBuf), ',');
		if ( strlen(wchBuf) > 0)
		{
			nCurID = (atoi(wchBuf) > 0) ? atoi(wchBuf) : nCurID;
			//�ж�ID�Ƿ����ù�
			if ( !IsIDUsed(nVectorID, nCurID) )
			{
				sprintf(szErrMsg, "ת��ʧ�ܣ�\n���ظ���ID��%d", nCurID);
				outSCPFile.close();
				inCsvFile.close();

				ofstream outFileEmpty(szSCPFileName, ios::out);
				outFileEmpty.close();
				return FALSE;
			}

			vector<int>::iterator it = nVectorID.end();
			nVectorID.insert(it, nCurID);
		}
		nMaxID = (nCurID > nMaxID) ? nCurID : nMaxID;
		outSCPFile << nCurID++;		
		outSCPFile << "\n";

		PrintTab(outSCPFile, nResObjectLevel[nCurResObject]);
		outSCPFile << "{\n";

		for (int i = 3; i < nQueue; i++)
		{	
			inCsvFile.get();	//��ȡ','
			
			if ( i == (nQueue - 1) )
			{
				inCsvFile.get(wchBuf, sizeof(wchBuf), '\n');
			}
			else
			{
				inCsvFile.get(wchBuf, sizeof(wchBuf), ',');
				if ( strrchr(wchBuf, '\n') )
				{
					sprintf(szErrMsg, "ת��ʧ�ܣ�\n����Object���ڵ���ȱ��һЩ��:\nObject �� %s ", wchObjectName);
					outSCPFile.close();
					inCsvFile.close();

					ofstream outFileEmpty(szSCPFileName, ios::out);
					outFileEmpty.close();
					return FALSE;
				}				
			}

			if ( strlen(wchBuf) > 0)
			{
				//��+תΪ,
				if (TRUE == bPlus2Comma)
				{
					PlusToComma(wchBuf, strlen(wchBuf));
				}

				PrintTab(outSCPFile, nResObjectLevel[nCurResObject]);
				outSCPFile << "\t";
				outSCPFile << cstrFirstRow[i];
				outSCPFile << " = ";
				outSCPFile << wchBuf;
				outSCPFile << "\n";
			}						
		}		
	}

	nVectorID.clear();

	while (nCurResObject >= 1)
	{
		PrintTab(outSCPFile, nResObjectLevel[nCurResObject]);
		outSCPFile << "}\n";
		nCurResObject--;
	}

	outSCPFile.seekp(nLen + 5, ios::beg);
	outSCPFile << nMaxID;

	outSCPFile.close();
	inCsvFile.close();

	return TRUE;
}
//------------------------------------------------------------------------
int CCSVToSCP::SaveFirstRow(TCHAR *pwchBuf, int nBufLen, CString *pcstrFirstRow)
{
	CString cstrTemp;
	int nQueue = 0;
	for (int i = 0; i < nBufLen; i++)
	{
		if (pwchBuf[i] != ',')
		{
			if (pwchBuf[i] != ' ')
			{
				cstrTemp += pwchBuf[i];
			}			
		}
		else
		{
			pcstrFirstRow[nQueue++] = cstrTemp;
			cstrTemp.Empty();
		}
	}
	pcstrFirstRow[nQueue++] = cstrTemp;
	return nQueue;
}
//------------------------------------------------------------------------
BOOL CCSVToSCP::IsIDUsed(vector<int> &nVectorID, int nCurID)
{
	for (vector<int>::iterator it = nVectorID.begin(); it != nVectorID.end(); it++)
	{
		if (nCurID == *it)
		{
			return FALSE;
		}
	}
	return TRUE;
}
//------------------------------------------------------------------------
int CCSVToSCP::PrintConstStr(ofstream &outSCPFile)
{
	int ntemp = 0;
	TCHAR tchConstStr[] = {"//--------------- FK1��Դ�ű� From CSV ---------------\n//--------------- ÿ����󳤶�2048�ֽ� ---------------\n// ���IDΪ = "};
	outSCPFile << tchConstStr;
	outSCPFile << "        \n";

	return sizeof(tchConstStr);
}
//------------------------------------------------------------------------
void CCSVToSCP::PrintTab(ofstream &outSCPFile, int nNum)
{
	for ( int i = 1; i <= nNum; i++)
	{
		outSCPFile << '\t';
	}
}
//------------------------------------------------------------------------
void CCSVToSCP::PlusToComma(TCHAR *pwchBuf, int nBufLen)
{
	for (int i = 0; i < nBufLen; i++)
	{
		if ( '+' == pwchBuf[i] )
		{
			wchBuf[i] = ',';
		}
	}
}
//------------------------------------------------------------------------