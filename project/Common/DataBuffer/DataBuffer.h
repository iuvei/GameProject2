﻿#ifndef _DATA_BUFFER_H_
#define _DATA_BUFFER_H_
#include "Utility/CritSec.h"
#include "IBufferHandler.h"

template <int SIZE>
class  CBufferManager;

template <int SIZE = 1024>
class CDataBuffer : public IDataBuffer
{
public:
	CDataBuffer(void)
	{
		m_nDataLen		= 0;
		m_nBufSize		= SIZE;
		m_dwRefCount	= 0;
		m_pPrev			= NULL;
		m_pNext			= NULL;
		m_pManager		= NULL;
		m_dwBufferNo	= 0;
	}

	virtual ~CDataBuffer(void)
	{

	}

	bool AddRef()
	{
		m_dwRefCount++;

		if(m_dwRefCount >= 2)
		{
			ASSERT_FAIELD;
		}

		return true;
	}

	bool Release()
	{
		ASSERT(m_pManager != NULL);
		m_pManager->m_CritSec.Lock();

		m_dwRefCount--;

		if(m_dwRefCount < 0)
		{
			ASSERT_FAIELD;
		}

		if(m_dwRefCount <= 0)
		{
			//首先从己用中删除
			if(m_pManager->m_pUsedList == this)
			{
				//自己是首结点
				m_pManager->m_pUsedList = m_pNext;
				if(m_pManager->m_pUsedList != NULL)
				{
					m_pManager->m_pUsedList->m_pPrev = NULL;
				}
			}
			else
			{
				ASSERT(m_pPrev != NULL);
				m_pPrev->m_pNext = m_pNext;
				if(m_pNext != NULL)
				{
					m_pNext->m_pPrev = m_pPrev;
				}
			}

			//再把自己加到己用中
			m_pNext = m_pManager->m_pFreeList;
			m_pPrev = NULL;
			m_pManager->m_pFreeList = this;

			if(m_pNext != NULL)
			{
				m_pNext->m_pPrev = this;
			}
		}

		m_pManager->m_CritSec.Unlock();

		return true;
	}

	CHAR* GetData()
	{
		return m_Buffer;
	}

	size_t GetDataLenth()
	{
		return m_nDataLen;
	}

	VOID SetDataLenth(size_t nPos)
	{
		m_nDataLen = nPos;
	}

	CHAR* GetBuffer()
	{
		return m_Buffer;
	}

	size_t GetBufferSize()
	{
		return m_nBufSize;
	}

	CHAR*	GetBufferPos(size_t nPos)
	{
		if(nPos >= m_nBufSize)
		{
			return NULL;
		}

		return m_Buffer+nPos;
	}

	size_t  CopyFrom(IDataBuffer *pSrcBuffer)
	{
		memcpy(m_Buffer, pSrcBuffer->GetBuffer(), pSrcBuffer->GetDataLenth());

		m_nDataLen = pSrcBuffer->GetDataLenth();

		return m_nDataLen;
	}

	UINT32  GetBufferNo()
	{
		return m_dwBufferNo;
	}

	CDataBuffer<SIZE> *m_pPrev;

	CDataBuffer<SIZE> *m_pNext;

	CBufferManager<SIZE> *m_pManager;

private:
	INT32		m_dwRefCount;

	size_t		m_nBufSize;

	CHAR		m_Buffer[SIZE];
	
	size_t		m_nDataLen;

public:

	UINT32		m_dwBufferNo;
	
};

template <int SIZE>
class  CBufferManager
{
public:
	CBufferManager()
	{
		m_pUsedList = NULL;
		m_pFreeList = NULL;
	}

	~CBufferManager()
	{

	}

	IDataBuffer* AllocDataBuff()
	{
		m_CritSec.Lock();

		CDataBuffer<SIZE> *pDataBuffer = NULL;

		if(m_pFreeList == NULL)
		{
			pDataBuffer = new CDataBuffer<SIZE>();
			m_dwBufferCount += 1;
			pDataBuffer->m_pManager = this;
			pDataBuffer->m_dwBufferNo = m_dwBufferCount;
		}
		else
		{
			pDataBuffer = m_pFreeList;

			m_pFreeList = m_pFreeList->m_pNext;

			if(m_pFreeList != NULL)
			{
				m_pFreeList->m_pPrev = NULL;
			}

			pDataBuffer->m_pNext = NULL;
			pDataBuffer->m_pPrev = NULL;
		}

		pDataBuffer->AddRef();

		if(m_pUsedList == NULL)
		{
			m_pUsedList = pDataBuffer;
		}
		else
		{
			pDataBuffer->m_pNext = m_pUsedList;
			m_pUsedList->m_pPrev = pDataBuffer;
			pDataBuffer->m_pPrev = NULL;
			m_pUsedList = pDataBuffer;
		}

		m_CritSec.Unlock();

		return pDataBuffer;
	}

	void PrintOutList(CDataBuffer<SIZE> *pList)
	{
		UINT32 dwCount = 0;
		CDataBuffer<SIZE> *pBufferNode = pList;
		if(pBufferNode == NULL)
		{
			printf("O---空列表---O\n");
			return ;
		}

		printf("Begin");
		BOOL bNext = TRUE;
		while(pBufferNode)
		{
			if(bNext)
			{
				dwCount++;
				ASSERT(dwCount<10);
				printf("->%d", pBufferNode->m_dwBufferNo);
				if(pBufferNode->m_pNext != NULL)
				{
					pBufferNode = pBufferNode->m_pNext;
				}
				else
				{
					bNext = FALSE;
					pBufferNode = pBufferNode->m_pPrev;
				}
			}
			else
			{
				dwCount++;
				ASSERT(dwCount<10);
				printf("<-%d", pBufferNode->m_dwBufferNo);
				pBufferNode = pBufferNode->m_pPrev;
			}	
		}

		printf("<-End\n");

		return ;
	}

	CDataBuffer<SIZE> *m_pFreeList;

	CDataBuffer<SIZE> *m_pUsedList;

	CCritSec	m_CritSec;

	UINT32		m_dwBufferCount;
private:
};

class CBufferManagerAll
{
	CBufferManagerAll();
	~CBufferManagerAll();
public:
	static CBufferManagerAll* GetInstancePtr();

public:
	IDataBuffer* AllocDataBuff(int nSize);

	CBufferManager<1024>  g_BufferManager1K;		//管理1k的内存池，需要分配1k以下的内存从这里分配
	CBufferManager<2048>  g_BufferManager2K;		//管理2k的内存池，需要分配2k以下的内存从这里分配
	CBufferManager<4096>  g_BufferManager4K;		//管理4k的内存池，需要分配4k以下的内存从这里分配
	CBufferManager<8192>  g_BufferManager8K;		//管理8k的内存池，需要分配8k以下的内存从这里分配
};

#endif