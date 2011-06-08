/*�I�u�W�F�N�g�̐����Ƃ��̊Ǘ����s��factory�N���X�̃w�b�_�[�t�@�C��*/

#include "stdafx.h"

#ifndef _FACTORY_H
#define _FACTORY_H

using namespace std;

typedef size_t OBJID;	// �I�u�W�F�N�gID
class CDrawTask;


/**
* �I�u�W�F�N�g�t�@�N�g����{�C���^�[�t�F�C�X
*/
class IObjectFactory
{
public:
	virtual bool Optimize(void) = 0;
	/**
	* �ێ����Ă���I�u�W�F�N�g�����ׂĉ������
	*/
	virtual void ClearAll(void) = 0;
};



/**
* �t�@�N�g���}�l�[�W����{�N���X
*/
class CFactoryManager
{
protected:
	/**
	* �t�@�N�g���I�u�W�F�N�g�̃|�C���^���X�g
	*�i����͎g�p�����t�@�N�g���N���X����
	*�@�����o�^����Ă����܂��j
	*/
	static list<IObjectFactory*> m_pFactoryList;

protected:
	// �R���X�g���N�^
	// ���̃N���X�͊O���������֎~���܂��̂�
	// �v���e�N�g�錾���܂�
	CFactoryManager(){}

public:
	virtual ~CFactoryManager()
	{
	}

public:
	/**
	* �I�u�W�F�N�g�������\�b�h�i�V���O���g���j
	*/
	static CFactoryManager &Instance(void)
	{
		static CFactoryManager inst;	// static�錾��1�������݂ł��鎖��ۏ�
		return inst;
	}

	/**
	* �Ǘ�����t�@�N�g���I�u�W�F�N�g��o�^
	*/
	bool RegistFactory(IObjectFactory* ptr)
	{
		m_pFactoryList.push_back(ptr);
		return true;
	}

	/**
	* �e�t�@�N�g���ɍœK�����w������
	*/
	virtual bool Optimize(void)
	{
		list<IObjectFactory*>::iterator it;
		for(it=m_pFactoryList.begin(); it!=m_pFactoryList.end();it++)
			(*it)->Optimize();
		return true;
	}

	/**
	* �S�t�@�N�g���ɃN���A���߂��o��
	*/
	virtual void ClearAll(void){
		list<IObjectFactory*>::iterator it;
		for(it=m_pFactoryList.begin(); it!=m_pFactoryList.end();it++)
			(*it)->ClearAll();
	}
};




/**
* �I�u�W�F�N�g�t�@�N�g����{�N���X
* (�Ǝ��g���j
*/
class CMyObjectFactory : public IObjectFactory
{
protected:
	static CDrawTask *p_draw_task;
	
	CMyObjectFactory()
	{
		// ���̊�{�N���X�Ńt�@�N�g���}�l�[�W���Ɏ��g��o�^���܂�
		// ����͔h���N���X���V���O���g���Ŏ������鎖��z�肵�Ă��܂�
		CFactoryManager::Instance().RegistFactory(this);
	}

public:
	static void SetDrawTask(CDrawTask *p){
		p_draw_task = p;
	}
};


/**
* �X�}�[�g�|�C���^�e���v���[�g�V���O���g���t�@�N�g���N���X
* �i�Ǝ��g���j
*/
template <class T>
class CMyFactory : public CMyObjectFactory
{
	typedef Dix::sp<T> OBJ;

	vector<OBJ> v_obj;	// �I�u�W�F�N�g�i�[�x�N�^�[�R���e�i

protected:
	// �V���O���g���Ȃ̂ŃR���X�g���N�^���v���g�^�C�v�錾���܂�
	CMyFactory(){};

public:
	virtual ~CMyFactory(){};

public:
	/**
	* �V���O���g���I�u�W�F�N�g���擾
	*/
	static CMyFactory<T> &Instance(void)
	{
		static CMyFactory<T> inst;
		return inst;
	}

	/**
	* �I�u�W�F�N�g�𐶐�����
	*/
	OBJID Create(OBJID id, OBJ &spOut){
		//�I�u�W�F�N�gID�`�F�b�N
		if(id + 1 > v_obj.size()){

			//�V�����I�u�W�F�N�g���쐬���ăI�u�W�F�N�g��o�^
			OBJ spNewObj( CreateNewObj() );
			p_draw_task->Register(spNewObj);
			v_obj.push_back(spNewObj);
			spOut = spNewObj;
		}else{

			//�����I�u�W�F�N�g�̎Q�Ɠn��
			spOut = v_obj[id];
		}
		
		return id;
	}


	/**
	* �}�b�v���œK������
	*/
	bool Optimize(void){
	// �Q�ƃJ�E���g��1�̃I�u�W�F�N�g�͎�菜��
		v_obj.erase(remove_if(v_obj.begin(), v_obj.end(), boost::mem_fn(&Dix::sp<T>::IsErased)), v_obj.end());
		return true;
	}


	/**
	* ���݂̃I�u�W�F�N�g�ێ������Z�o
	*/
	virtual size_t Size(void)
	{
		return v_obj.size();
	}

	/**
	* �w�肳�ꂽ�v�f�ԍ��̃I�u�W�F�N�g������΁A������A�b�v�L���X�g����
	* ���N���X�̃X�}�[�g�|�C���^�ɔ[�߂ĕԂ�
	*/
	template<class U>
	bool Exists(Dix::sp<U> &SPOut, unsigned Num){
		OBJ sp_obj;

		if(Num < Size()){		//�w�肳�ꂽ�v�f�ԍ��̃I�u�W�F�N�g������ꍇ
			Create(Num, sp_obj);
			if(!sp_obj->IsNotDraw()){	//��ʂɕ`�悳��Ȃ����̂͒e��
				SPOut = sp_obj;
				return true;
			}
		}
		return false;
	}

	/**
	* �I�u�W�F�N�g��ێ����Ă��邩�ǂ�����Ԃ�
	*/
	bool HaveObj(void){
		return(!v_obj.empty());
	}

	/**
	* �S�I�u�W�F�N�g�N���A
	*/
	virtual void ClearAll(void)
	{
		v_obj.clear();
	}

protected:
	/**
	* �V�����I�u�W�F�N�g�𐶐�
	*/
	virtual T* CreateNewObj(void){return new T;}
};

#endif
