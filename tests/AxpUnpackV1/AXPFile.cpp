// AXPFile.cpp: implementation of the CAXPFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AXPUnpack.h"
#include "AXPFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAXPFile::CAXPFile()
:m_FileName(TEXT("")),m_hFile(INVALID_HANDLE_VALUE), m_hFileMap(NULL),\
m_hMapView(NULL), m_dwFileSize(-1), m_lpAxPHeader(NULL), m_lpAxpHashTable(NULL),\
m_lpAxpFileInfo(NULL)
{
	PrepareCryptTable();
}


CAXPFile::~CAXPFile()
{
	Release();
}

BOOL CAXPFile::LoadFile()
{
	if (!MappingFile())
	{
		return FALSE;
	}
	
	m_lpAxPHeader = (AXPHeader*) m_hMapView;
	if (m_lpAxPHeader->signature != AXP_SIGNATURE || m_lpAxPHeader->nUnknow0 != 0x00010001)
	{
		AfxMessageBox(TEXT("文件格式错误!"));
		Release();
		return FALSE;
	}
	
	m_lpAxpHashTable = (AXPHashTable**)((DWORD)m_hMapView + m_lpAxPHeader->nHashTableOffset);
	m_lpAxpFileInfo = (AXPFileInfo**)((DWORD)m_hMapView + m_lpAxPHeader->nIndexTableOffset);
	
	if (IsBadReadPtr((LPVOID)m_lpAxpHashTable, 0x60000) ||\
		IsBadReadPtr((LPVOID)m_lpAxpFileInfo, m_lpAxPHeader->nSizeOfIndexTable))
	{
		Release();
		return FALSE;
	}
	
	if (!GetFileList())
	{
		AfxMessageBox(TEXT("获取文件列表失败！"));
		Release();
		return FALSE;
	}
	
	return TRUE;
}

void CAXPFile::UnLoad()
{
	Release();
}

BOOL CAXPFile::MappingFile()
{
	BOOL bResult = FALSE;
	
	Release();
	
	if (m_FileName.IsEmpty())
	{
		return bResult;
	}
	
	__try 
	{
		m_hFile = CreateFile(m_FileName, GENERIC_READ, FILE_SHARE_READ,\
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		
		if (INVALID_HANDLE_VALUE == m_hFile)
		{
			ShowErrorMessage();
			__leave;
		}
		
		m_dwFileSize = GetFileSize(m_hFile, NULL);
		
		if (m_dwFileSize == -1 || m_dwFileSize == 0)
		{
			CloseHandle(m_hFile);
			m_hFile = INVALID_HANDLE_VALUE;
			__leave;
		}
		m_hFileMap = CreateFileMapping(m_hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		
		if (NULL == m_hFileMap)
		{
			ShowErrorMessage();
			Release();
			__leave;
		}
		
		m_hMapView = MapViewOfFile(m_hFileMap, FILE_MAP_READ, 0, 0, 0);
		
		if (NULL == m_hMapView)
		{
			ShowErrorMessage();
			Release();
			__leave;
		}
		
		//CloseHandle(m_hFileMap);
		//CloseHandle(m_hFile);
		bResult = TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		ShowErrorMessage();
		Release();
	}
	
	return bResult;
}

void CAXPFile::SetFileName(LPCTSTR lpFileName)
{
	m_FileName = lpFileName;
}

void CAXPFile::ShowErrorMessage()
{
	LPVOID lpMsgBuf;
	DWORD dw = GetLastError(); 
	
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );
	
	AfxMessageBox((LPCTSTR)lpMsgBuf, MB_OK | MB_ICONERROR);
	
	LocalFree(lpMsgBuf);
}

void CAXPFile::Release()
{
	if (NULL != m_hMapView)
	{
		UnmapViewOfFile(m_hMapView);
		m_hMapView = NULL;
	}
	
	if (NULL != m_hFileMap)
	{
		CloseHandle(m_hFileMap);
		m_hFileMap = NULL;
	}
	
	if (INVALID_HANDLE_VALUE != m_hFile)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
	
	m_lpAxpFileInfo = NULL;
	m_lpAxpHashTable = NULL;
	m_lpAxpFileInfo = NULL;
	m_FileList.clear();
}

void CAXPFile::PrepareCryptTable()
{
/*
DWORD var_C, var_8, var_4; 
LPVOID lpCryptTable = (LPVOID)&m_CryptTable;
__asm
{
	mov		eax,lpCryptTable
	mov		var_C, eax	
	add		eax,400h
	mov     var_8, eax	
	add		eax,400h
	mov     var_4, eax	
	push    ebx
	push    esi
	mov     ebx, ecx
	push    edi
	mov     ecx, 400h
	xor     eax, eax
	mov     edi, var_C
	rep		stosd
	mov     edx, 100001h
	mov     ecx, var_8
	lea     ecx, [ecx]
$iter:                            
	mov     eax, edx
	imul    eax, 7Dh
	add     eax, 3
	xor     edx, edx
	mov     esi, 2AAAABh
	div     esi
	mov     edi, 2AAAABh
	add     ecx, 4
	mov     eax, edx
	mov     esi, eax
	imul    eax, 7Dh
	add     eax, 3
	xor     edx, edx
	div     edi
	shl     esi, 10h
	mov     eax, edx
	imul    eax, 7Dh
	add     eax, 3
	and     edx, 0FFFFh
	or      edx, esi
	mov     [ecx-404h], edx
	xor     edx, edx
	mov     esi, edi
	div     esi
	mov     eax, edx
	mov     esi, eax
	imul    eax, 7Dh
	add     eax, 3
	xor     edx, edx
	div     edi
	shl     esi, 10h
	mov     eax, edx
	imul    eax, 7Dh
	and     edx, 0FFFFh
	or      edx, esi
	add     eax, 3
	mov     [ecx-4], edx
	xor     edx, edx
	mov     esi, edi
	div     esi
	mov     eax, edx
	mov     esi, eax
	imul    eax, 7Dh
	add     eax, 3
	xor     edx, edx
	div     edi
	shl     esi, 10h
	mov     eax, edx
	imul    eax, 7Dh
	and     edx, 0FFFFh
	or      edx, esi
	add     eax, 3
	mov     [ecx+3FCh], edx
	xor     edx, edx
	mov     esi, edi
	div     esi
	mov     eax, edx
	mov     esi, eax
	imul    eax, 7Dh
	add     eax, 3
	xor     edx, edx
	div     edi
	shl     esi, 10h
	mov     eax, edx
	and     eax, 0FFFFh
	or      eax, esi
	cmp     ecx, var_4
	mov     [ecx+7FCh], eax
	jl      $iter
	mov		eax,lpCryptTable
}
	*/
	
	DWORD seed = 0x00100001;
	memset(m_CryptTable, 0, 0x500 * sizeof(DWORD));

	DWORD temp1, temp2;
	for (DWORD i = 0; i < 0x100 ; i++)
	{
		seed = (125 * seed + 3) % 0x2AAAAB;
		temp1 = seed << 0x10;
		seed = (125 * seed + 3) % 0x2AAAAB;
		temp2 = 0xFFFF & seed;
		m_CryptTable[i] = temp1 | temp2;
	
		seed = (125 * seed + 3) % 0x2AAAAB;
		temp1 = seed << 0x10;
		seed = (125 * seed + 3) % 0x2AAAAB;
		temp2 = 0xFFFF & seed;
		m_CryptTable[i + 0x100] = temp1 | temp2;
	
		seed = (125 * seed + 3) % 0x2AAAAB;
		temp1 = seed << 0x10;
		seed = (125 * seed + 3) % 0x2AAAAB;
		temp2 = 0xFFFF & seed;
		m_CryptTable[i + 0x200] = temp1 | temp2;
	
		seed = (125 * seed + 3) % 0x2AAAAB;
		temp1 = seed << 0x10;
		seed = (125 * seed + 3) % 0x2AAAAB;
		temp2 = 0xFFFF & seed;
		m_CryptTable[i + 0x300] = temp1 | temp2;
		
		/*
		for (DWORD j = 0; j < 5 ; j++)
		{ 
			seed = (125 * seed + 3) % 0x2AAAAB;
			temp1 = seed << 0x10;
			seed = (125 * seed + 3) % 0x2AAAAB;
			temp2 = 0xFFFF & seed;
			m_CryptTable[(j * 0x100 + i] = (temp1 | temp2);
		}
		*/
	}
	
	/*
	DWORD dwHih, dwLow,seed = 0x00100001,index1 = 0,index2 = 0, i;
	for(index1 = 0; index1 < 0x100; index1++)
	{ 
		for(index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
		{ 
			seed = (seed * 125 + 3) % 0x2AAAAB;
			//dwHih = (seed & 0xFFFF) << 0x10;
			dwHih = seed << 0x10;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			dwLow = (seed & 0xFFFF);
			m_CryptTable[index2] = (dwHih | dwLow); 
		} 
	}
	*/
}


DWORD CAXPFile::HashString(const char *lpszFileName, DWORD dwCryptIndex)
{
/*
	__asm
	{
		mov     esi, lpszFileName
		mov     dl, [esi]
		test    dl, dl
		mov     eax, 7FED7FEDh
		mov     ecx, 0EEEEEEEEh
		jz      short $ret

		mov     edi, dwCryptIndex
		shl     edi, 8
  
	$iter:                             
		add		eax, ecx
		imul    ecx, 21h
		movsx   edx, dl
		lea     ebx, [edi+edx]
		mov     ebp, m_CryptTable[ebx*4]
		inc     esi
		add     ecx, edx
		mov     dl, [esi]
		xor     eax, ebp
		test    dl, dl
		lea     ecx, [ecx+eax+3]
		jnz     short $iter
	$ret:
	}
*/

	signed char *key = (signed char *)lpszFileName;
	DWORD seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
	signed int ch;
	
	while(*key != 0)
	{ 
		ch = *key++;
		seed1 = m_CryptTable[(dwCryptIndex<< 8) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3; 
	}
	
	return seed1; 

}

BOOL CAXPFile::GetHashTablePos(const char *lpszString, int *lpFileOffset, int *lpFileSize)
{
	const int HASH_A = 1, HASH_B = 2, HASH_OFFSET = 3;  
	
	int nHashA = HashString(lpszString, HASH_A);
	int nHashB = HashString(lpszString, HASH_B);
	
	int nHash = HashString(lpszString, HASH_OFFSET);
	
	nHash &= 0x7FFF;
	int nHashStart = nHash, nHashPos;

	try
	{
		while (!((nHashPos = ((int*)m_lpAxpHashTable)[nHashStart * 3 + 2]) & 0x80000000) ||\
			nHashA != ((int*)m_lpAxpHashTable)[nHashStart * 3] ||\
			nHashB != ((int*)m_lpAxpHashTable)[nHashStart * 3 + 1])
		{
			nHashStart++;
			nHashStart &= 0x7FFF;
			if (nHashStart == nHash)
			{
				return FALSE;
			}
		}
	}
	catch (...)
	{
		return FALSE;
	}

	nHashPos &= 0x3FFFFFFF;
	*lpFileOffset = ((int*)m_lpAxpFileInfo)[nHashPos * 3];
	*lpFileSize = ((int*)m_lpAxpFileInfo)[nHashPos * 3 + 1];
	
	return TRUE;  
}

BOOL CAXPFile::GetFileList()
{
	m_FileList.clear();
	BOOL bResult = FALSE;
	int nFileOffset, nFileSize;
	try
	{
		if (GetHashTablePos("(list)", &nFileOffset, &nFileSize))
		{
			char* lpBuffer = (char*)((DWORD)m_hMapView + nFileOffset);
			
			string liststr;   
			vector<string> v;   
			
			for(int i = 0; i < nFileSize; i++)   
			{   
				if (lpBuffer[i] != 0xD && lpBuffer[i + 1] != 0xA)   
				{   
					liststr += tolower(lpBuffer[i]);   
				}
				else
				{   
					string::size_type loc = liststr.find("|"); 
					if (loc != string::npos)
					{
						m_FileList.push_back(liststr.substr(0, loc));   
					}
					liststr = "" ;   
					i++;
				}   
			}   
			
			bResult = TRUE;
		}
	}
	catch (...)
	{
		bResult = FALSE;
	}
	
	return bResult;
}


BOOL CAXPFile::GenerateFile(int i, CString &strFileName)
{
	BOOL bResult = FALSE;
	int nFileOffset, nFileSize;
	CString	strMsg;
	HANDLE	hFile;
	DWORD	dwBytesWritten;
	try
	{
		if (GetHashTablePos(m_FileList[i].c_str()/*m_FileList[i].c_str()*/, &nFileOffset, &nFileSize))
		{
			char* lpBuffer = (char*)((DWORD)m_hMapView + nFileOffset);
			
			hFile = CreateFile(strFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, \
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			
			if (hFile == INVALID_HANDLE_VALUE)
			{
				strMsg.Format(TEXT("创建文件失败: %s"), strFileName);
				AfxMessageBox(strMsg);
				return FALSE;
			}
			WriteFile(hFile, (LPVOID)lpBuffer, nFileSize, &dwBytesWritten, NULL);
			
			if (dwBytesWritten != nFileSize)
			{
				CloseHandle(hFile);
				strMsg.Format(TEXT("写入文件数据不完整: %s"), strFileName);
				AfxMessageBox(strMsg);
				return FALSE;
			}

			bResult = TRUE;
		}
		else
		{
			AfxMessageBox(TEXT("计算文件索引信息出错！"));
		}
		CloseHandle(hFile);
	}
	catch (...)
	{
		if (hFile != INVALID_HANDLE_VALUE)
		{	
			CloseHandle(hFile);
		}
	
		AfxMessageBox(TEXT("提取文件数据出现错误！"));
		bResult = FALSE;
	}
	
	return bResult;
}
