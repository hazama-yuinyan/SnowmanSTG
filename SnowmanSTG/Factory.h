/*オブジェクトの生成とその管理を行うfactoryクラスのヘッダーファイル*/

#include "stdafx.h"

#ifndef _FACTORY_H
#define _FACTORY_H

using namespace std;

typedef size_t OBJID;	// オブジェクトID
class CDrawTask;


/**
* オブジェクトファクトリ基本インターフェイス
*/
class IObjectFactory
{
public:
	virtual bool Optimize(void) = 0;
	/**
	* 保持しているオブジェクトをすべて解放する
	*/
	virtual void ClearAll(void) = 0;
};



/**
* ファクトリマネージャ基本クラス
*/
class CFactoryManager
{
protected:
	/**
	* ファクトリオブジェクトのポインタリスト
	*（これは使用したファクトリクラス内で
	*　自動登録されていきます）
	*/
	static list<IObjectFactory*> m_pFactoryList;

protected:
	// コンストラクタ
	// このクラスは外部生成を禁止しますので
	// プロテクト宣言します
	CFactoryManager(){}

public:
	virtual ~CFactoryManager()
	{
	}

public:
	/**
	* オブジェクト生成メソッド（シングルトン）
	*/
	static CFactoryManager &Instance(void)
	{
		static CFactoryManager inst;	// static宣言で1つだけ存在できる事を保証
		return inst;
	}

	/**
	* 管理するファクトリオブジェクトを登録
	*/
	bool RegistFactory(IObjectFactory* ptr)
	{
		m_pFactoryList.push_back(ptr);
		return true;
	}

	/**
	* 各ファクトリに最適化を指示する
	*/
	virtual bool Optimize(void)
	{
		list<IObjectFactory*>::iterator it;
		for(it=m_pFactoryList.begin(); it!=m_pFactoryList.end();it++)
			(*it)->Optimize();
		return true;
	}

	/**
	* 全ファクトリにクリア命令を出す
	*/
	virtual void ClearAll(void){
		list<IObjectFactory*>::iterator it;
		for(it=m_pFactoryList.begin(); it!=m_pFactoryList.end();it++)
			(*it)->ClearAll();
	}
};




/**
* オブジェクトファクトリ基本クラス
* (独自拡張）
*/
class CMyObjectFactory : public IObjectFactory
{
protected:
	static CDrawTask *p_draw_task;
	
	CMyObjectFactory()
	{
		// この基本クラスでファクトリマネージャに自身を登録します
		// これは派生クラスをシングルトンで実装する事を想定しています
		CFactoryManager::Instance().RegistFactory(this);
	}

public:
	static void SetDrawTask(CDrawTask *p){
		p_draw_task = p;
	}
};


/**
* スマートポインタテンプレートシングルトンファクトリクラス
* （独自拡張）
*/
template <class T>
class CMyFactory : public CMyObjectFactory
{
	typedef Dix::sp<T> OBJ;

	vector<OBJ> v_obj;	// オブジェクト格納ベクターコンテナ

protected:
	// シングルトンなのでコンストラクタをプロトタイプ宣言します
	CMyFactory(){};

public:
	virtual ~CMyFactory(){};

public:
	/**
	* シングルトンオブジェクトを取得
	*/
	static CMyFactory<T> &Instance(void)
	{
		static CMyFactory<T> inst;
		return inst;
	}

	/**
	* オブジェクトを生成する
	*/
	OBJID Create(OBJID id, OBJ &spOut){
		//オブジェクトIDチェック
		if(id + 1 > v_obj.size()){

			//新しいオブジェクトを作成してオブジェクトを登録
			OBJ spNewObj( CreateNewObj() );
			p_draw_task->Register(spNewObj);
			v_obj.push_back(spNewObj);
			spOut = spNewObj;
		}else{

			//既存オブジェクトの参照渡し
			spOut = v_obj[id];
		}
		
		return id;
	}


	/**
	* マップを最適化する
	*/
	bool Optimize(void){
	// 参照カウントが1のオブジェクトは取り除く
		v_obj.erase(remove_if(v_obj.begin(), v_obj.end(), boost::mem_fn(&Dix::sp<T>::IsErased)), v_obj.end());
		return true;
	}


	/**
	* 現在のオブジェクト保持数を算出
	*/
	virtual size_t Size(void)
	{
		return v_obj.size();
	}

	/**
	* 指定された要素番号のオブジェクトがあれば、それをアップキャストして
	* 基底クラスのスマートポインタに納めて返す
	*/
	template<class U>
	bool Exists(Dix::sp<U> &SPOut, unsigned Num){
		OBJ sp_obj;

		if(Num < Size()){		//指定された要素番号のオブジェクトがある場合
			Create(Num, sp_obj);
			if(!sp_obj->IsNotDraw()){	//画面に描画されないものは弾く
				SPOut = sp_obj;
				return true;
			}
		}
		return false;
	}

	/**
	* オブジェクトを保持しているかどうかを返す
	*/
	bool HaveObj(void){
		return(!v_obj.empty());
	}

	/**
	* 全オブジェクトクリア
	*/
	virtual void ClearAll(void)
	{
		v_obj.clear();
	}

protected:
	/**
	* 新しいオブジェクトを生成
	*/
	virtual T* CreateNewObj(void){return new T;}
};

#endif
