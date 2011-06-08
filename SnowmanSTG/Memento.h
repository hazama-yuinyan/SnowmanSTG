#pragma once

#include "stdafx.h"

class CMementoKey
{
	friend class CInputDeviceKeyBoard;

private:
	CMementoKey(void);
	boost::array<char, 256> key_state;
	void SetData(char In[]);
	boost::array<char, 256> &GetData(void);

public:
	~CMementoKey(void){};
};
