// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#ifndef __STDAFX_H__
#define __STDAFX_H__

#include <stdio.h>

#include <map>
#include <hash_map>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <assert.h>
#include <set>

#define ASSERT_FAIELD (assert(false))
#define ASSERT(x)     (assert(x))

#include "Platform.h"

extern "C" 
{ 
#include "lua.h"  
#include "lualib.h"  
#include "lauxlib.h"  
}  

#pragma comment(lib, "lua51.lib")

#endif