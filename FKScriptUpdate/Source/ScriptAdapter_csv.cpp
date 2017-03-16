/**
*	created:		2012-7-1   0:10
*	filename: 		ScriptAdapter_csv
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�ű�����-����2ά.csv�ű�.֧��','' '��������ŷָ�
*/
//------------------------------------------------------------------------
#include "../Include/ScriptAdapter_csv.h"
//------------------------------------------------------------------------
CScriptAdaptercsv_row::CScriptAdaptercsv_row() 
: m_pData(0), m_nSize(0), m_pScript(0), m_pLine(0)
{
}	
//------------------------------------------------------------------------
CScriptAdaptercsv_row::~CScriptAdaptercsv_row()
{
	Close();
}
//------------------------------------------------------------------------
void CScriptAdaptercsv_row::Release()
{
	delete this; 
}
//------------------------------------------------------------------------
int CScriptAdaptercsv_row::Size() const
{
	return m_nSize;
}
//------------------------------------------------------------------------
_VARIANT & CScriptAdaptercsv_row::GetData( int nIndex )
{
	if( nIndex < 0 || nIndex >= m_nSize )
		return empty_variant;

	return m_pData[nIndex];
}
//------------------------------------------------------------------------
_VARIANT & CScriptAdaptercsv_row::operator []( int nIndex )
{
	if( nIndex < 0 || nIndex >= m_nSize )
		return empty_variant;

	return m_pData[nIndex];
}
//------------------------------------------------------------------------
_VARIANT & CScriptAdaptercsv_row::GetData( LPCSTR szName )
{
	int nIndex = m_pScript->FindPropName( szName );
	if( nIndex < 0 || nIndex >= m_nSize )
		return empty_variant;

	return m_pData[nIndex];
}
//------------------------------------------------------------------------
_VARIANT & CScriptAdaptercsv_row::operator []( LPCSTR szName )
{
	int nIndex = m_pScript->FindPropName( szName );
	if( nIndex < 0 || nIndex >= m_nSize )
		return empty_variant;

	return m_pData[nIndex];
}
//------------------------------------------------------------------------
void CScriptAdaptercsv_row::Close()
{
	if( m_pData )
	{
		delete []m_pData;
		m_pData = NULL;
	}
	if( m_pLine )
	{
		free( m_pLine );
		m_pLine = NULL;
	}
	m_nSize = 0;
	m_pScript = NULL;
}
//------------------------------------------------------------------------
BOOL CScriptAdaptercsv_row::InitData( CScriptAdapter_csv * pScript ,
									 char * line,
									 char cSeparator/*�ָ���*/ , 
									 BOOL bAllowSeries/*�Ƿ�������������*/ )
{
	if( line == NULL )
		return FALSE;

	int  nLineLen = 0;
	int  nPosTemp[ enMax_Segment_Count ];
	char c = line[nLineLen++];
	BOOL bRecord = TRUE;

	while( c != 0 && c != '\r' && c != '\n' )
	{
		if( c == cSeparator )
		{
			//c = 0;
			line[nLineLen-1]=0;

			if( bAllowSeries ) // �����������֣�����','�ָ�
			{
				if( bRecord )
				{
					nPosTemp[m_nSize++]=nLineLen-1;				
				}
				else
					bRecord = TRUE;
			}
			else			  // �������������֣�����' '�ָ�
				bRecord = TRUE;
		}
		else
		{
			if( bRecord )
			{
				nPosTemp[m_nSize++]=nLineLen-1;
				bRecord = FALSE;
			}
		}

		c = line[nLineLen++];
		if( m_nSize >= enMax_Segment_Count )
			break;
	}

	line[nLineLen-1]=0;

	m_pLine = (char *)malloc( nLineLen );
	memcpy( m_pLine , line , nLineLen );
	m_pData = new _VARIANT[m_nSize];
	for( int i = 0; i < m_nSize ; i ++ )
	{
		m_pData[i].m_Type = _VARIANT::enType_STRING;
		m_pData[i].m_Data.string = &m_pLine[nPosTemp[i]];
	}

	m_pScript = pScript;
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CScriptAdapter_csv::LoadScript( LPCSTR szFile)
{
	FILE *stream = NULL;
	char line[enMax_Line_Len];

	if( (stream = fopen( szFile, "r" )) == NULL )
	{
		TraceLn("Load File %s Failed!" , szFile );
		return FALSE;
	}

	// ��ȡ����
	if( fgets( line, enMax_Line_Len, stream ) == NULL )
	{
		fclose(stream);
		return FALSE;
	}

	// ȷ������ʲô���ŷָ���
	char cSeparator = ' ';
	BOOL bAllowSeries = FALSE;
	for( int i=0;line[i]!=0&&line[i]!='\r';i++)
	{
		if( line[i]==',')
		{
			cSeparator = ',';
			bAllowSeries = TRUE;
			break;
		}
	}

	if( !m_title.InitData(this,line,cSeparator,bAllowSeries) )
	{
		fclose(stream);
		return FALSE;
	}

	while( fgets( line, enMax_Line_Len, stream ) )
	{
		CScriptAdaptercsv_row * pRow = new CScriptAdaptercsv_row;
		if( pRow == NULL
			|| !pRow->InitData(this,line,cSeparator,bAllowSeries) )
		{
			SAFE_RELEASE(pRow);
			fclose(stream);
			return FALSE;
		}

		if( pRow->Size() <= 0 )
			continue;

		_VARIANT data;
		data.m_Type = _VARIANT::enType_ITERATOR;
		data.m_Data.it = pRow;
		m_RowTable.push_back(data);
		m_ObjectNameTable.push_back( pRow->GetData(0).m_Data.string );
	}

	m_nSize = m_RowTable.size();
	fclose( stream );
	stream = NULL;
	return TRUE;
}
//------------------------------------------------------------------------
IScriptIterator * CScriptAdapter_csv::GetScriptIterator()
{
	return static_cast<IScriptIterator *>(this);
}
//------------------------------------------------------------------------
int CScriptAdapter_csv::Size() const
{
	return m_nSize;
}
//------------------------------------------------------------------------
_VARIANT & CScriptAdapter_csv::GetData( int nIndex )
{
	if( nIndex < 0 || nIndex >= m_nSize )
		return empty_variant;

	return m_RowTable[nIndex];	
}
//------------------------------------------------------------------------
_VARIANT & CScriptAdapter_csv::operator []( int nIndex )
{
	if( nIndex < 0 || nIndex >= m_nSize )
		return empty_variant;

	return m_RowTable[nIndex];
}
//------------------------------------------------------------------------
_VARIANT & CScriptAdapter_csv::GetData( LPCSTR szName )
{
	int nIndex = FindObjectName( szName );
	if( nIndex < 0 || nIndex >= m_nSize )
		return empty_variant;

	return m_RowTable[nIndex];
}
//------------------------------------------------------------------------
_VARIANT & CScriptAdapter_csv::operator []( LPCSTR szName )
{
	int nIndex = FindObjectName( szName );
	if( nIndex < 0 || nIndex >= m_nSize )
		return empty_variant;

	return m_RowTable[nIndex];
}
//------------------------------------------------------------------------
int  CScriptAdapter_csv::FindPropName( LPCSTR szName )
{
	for( int i = 0; i < m_title.Size() ; i++ )
	{
		if( stricmp( m_title[i].m_Data.string , szName ) == 0 )
			return i;
	}

	return -1;
}
//------------------------------------------------------------------------
int  CScriptAdapter_csv::FindObjectName( LPCSTR szName )
{
	int nReturn = 0;
	vector< string >::iterator it = m_ObjectNameTable.begin();
	for( ; it != m_ObjectNameTable.end() ; ++ it )
	{
		if( stricmp( (*it).c_str(), szName ) == 0 )
			return nReturn;
		else 
			nReturn ++;
	}

	return -1;
}
//------------------------------------------------------------------------
void CScriptAdapter_csv::Close()
{
	for( int i = 0; i < m_nSize ; i++ )
	{
		if( m_RowTable[i].m_Type == _VARIANT::enType_ITERATOR )
		{
			if (m_RowTable[i].m_Data.it != NULL)
			{
				m_RowTable[i].m_Data.it->Release();
				m_RowTable[i].m_Data.it = NULL;
			}
		}
	}

	m_nSize = 0;
}
//------------------------------------------------------------------------
void CScriptAdapter_csv::Release()
{ 
	delete this; 
}
//------------------------------------------------------------------------
CScriptAdapter_csv::CScriptAdapter_csv():m_nSize(0)
{
}
//------------------------------------------------------------------------
CScriptAdapter_csv::~CScriptAdapter_csv()
{
	Close();
}
//------------------------------------------------------------------------