﻿#ifndef __IBUFFER_HANDLER_H__
#define __IBUFFER_HANDLER_H__
class CConnection;
class CBufferHelper;
struct NetPacket;

class IDataBuffer
{
public:
	virtual bool	AddRef() = 0;

	virtual bool	Release() = 0;

	virtual CHAR*	GetData() = 0;

	virtual size_t	GetDataLenth() = 0;

	virtual VOID	SetDataLenth(size_t nPos) = 0;

	virtual CHAR*	GetBuffer() = 0;

	virtual size_t	GetBufferSize() = 0;

	virtual CHAR*	GetBufferPos(size_t nIndex) = 0;

	virtual size_t  CopyFrom(IDataBuffer *pSrcBuffer) = 0;

	virtual UINT32  GetBufferNo() = 0;

};

struct IDataHandler
{
	virtual BOOL OnDataHandle( IDataBuffer *pDataBuffer, CConnection *pConnection) = 0;
	virtual BOOL OnCloseConnect(CConnection *pConnection) = 0;
	virtual BOOL OnNewConnect(CConnection *pConnection) = 0;
};

struct IPacketDispatcher
{
	virtual BOOL DispatchPacket( NetPacket *pNetPacket) = 0;
	virtual BOOL OnCloseConnect(CConnection *pConnection) = 0;
	virtual BOOL OnNewConnect(CConnection *pConnection) = 0;
};

struct  IThreadCommandHandler
{
	virtual BOOL OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper) = 0;
	virtual BOOL OnUpdate(UINT32 dwTick) = 0;
};






#endif /* __IBUFFER_HANDLER_H__ */
