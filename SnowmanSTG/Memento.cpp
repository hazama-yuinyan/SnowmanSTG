#include "stdafx.h"
#include "Memento.h"

CMementoKey::CMementoKey(void)
{
	key_state.assign(0);
}

void CMementoKey::SetData(char In[])
{
	for(int i = 0; i < key_state.size(); ++i){
		key_state[i] = *In;
		++In;
	}
}

boost::array<char, 256> &CMementoKey::GetData(void)
{
	return key_state;
}
