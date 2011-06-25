#include "stdafx.h"
#include "Memento.h"

MementoKey::MementoKey(void)
{
	key_state.assign(0);
}

void MementoKey::SetData(char In[])
{
	for(unsigned i = 0; i < key_state.size(); ++i){
		key_state[i] = *In;
		++In;
	}
}

boost::array<char, 256> &MementoKey::GetData(void)
{
	return key_state;
}
