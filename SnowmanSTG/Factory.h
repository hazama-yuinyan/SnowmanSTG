/*オブジェクトの生成とその管理を行うfactoryクラスのヘッダーファイル*/

#include "stdafx.h"

#ifndef _FACTORY_H
#define _FACTORY_H


typedef size_t OBJID;	// オブジェクトID
class DrawTask;


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
class FactoryManager
{
protected:
	/**
	* ファクトリオブジェクトのポインタリスト
	*（これは使用したファクトリクラス内で
	*　自動登録されていきます）
	*/
	static std::list<IObjectFactory*> m_pFactoryList;

protected:
	// コンストラクタ
	// このクラスは外部生成を禁止しますので
	// プロテクト宣言します
	FactoryManager(){}

public:
	virtual ~FactoryManager()
	{
	}

public:
	/**
	* オブジェクト生成メソッド（シングルトン）
	*/
	static FactoryManager &Instance(void)
	{
		static FactoryManager inst;	// static宣言で1つだけ存在できる事を保証
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
		std::list<IObjectFactory*>::iterator it;
		for(it=m_pFactoryList.begin(); it!=m_pFactoryList.end();it++)
			(*it)->Optimize();
		return true;
	}

	/**
	* 全ファクトリにクリア命令を出す
	*/
	virtual void ClearAll(void){
		std::list<IObjectFactory*>::iterator it;
		for(it=m_pFactoryList.begin(); it!=m_pFactoryList.end();it++)
			(*it)->ClearAll();
	}
};




/**
* オブジェクトファクトリ基本クラス
* (独自拡張）
*/
class MyObjectFactory : public IObjectFactory
{
protected:
	static DrawTask *p_draw_task;
	
	MyObjectFactory()
	{
		// この基本クラスでファクトリマネージャに自身を登録します
		// これは派生クラスをシングルトンで実装する事を想定しています
		FactoryManager::Instance().RegistFactory(this);
	}

public:
	static void SetDrawTask(DrawTask *p){
		p_draw_task = p;
	}
};


/**
* スマートポインタテンプレートシングルトンファクトリクラス
* （独自拡張）
*/
template <class T>
class MyFactory : public MyObjectFactory
{
	typedef Dix::sp<T> OBJ;

	std::vector<OBJ> v_obj;	// オブジェクト格納ベクターコンテナ

protected:
	// シングルトンなのでコンストラクタをプロトタイプ宣言します
	MyFactory(){};

public:
	virtual ~MyFactory(){};

public:
	/**
	* シングルトンオブジェクトを取得
	*/
	static MyFactory<T> &Instance(void)
	{
		static MyFactory<T> inst;
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
		v_obj.erase(std::remove_if(v_obj.begin(), v_obj.end(), boost::bind(&HAZAMA::IsErased<T>, _1)), v_obj.end());
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
			if(!sp_obj->IsNotDrawn()){	//画面に描画されないものは弾く
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
