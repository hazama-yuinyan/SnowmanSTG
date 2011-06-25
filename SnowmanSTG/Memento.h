#pragma once

#include "stdafx.h"

class MementoKey
{
	friend class InputDeviceKeyBoard;

private:
	MementoKey(void);
	boost::array<char, 256> key_state;
	void SetData(char In[]);
	boost::array<char, 256> &GetData(void);

public:
	~MementoKey(void){};
};
