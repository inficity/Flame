// FlameTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace boost;
using namespace coroutines;
using namespace std;

typedef symmetric_coroutine<void>::call_type CoCall;
typedef symmetric_coroutine<void>::yield_type CoYield;


class Component
{
public:
	virtual const char* GetComponentName() = 0;
	virtual ~Component() {}
};

class Message
{
	
};


class OnEnterStage : public Message
{
	
};

typedef list<function<void(Component*, Message*)>> HandlerList;

hash_map<const char*, HandlerList*> GMessageHandlers;


class Actor
{
public:
};

class Entity
{
public:

	vector<Component*> _Components;

	template<typename T>
	T& Get()
	{
		return nullptr;
	}


	void Post(Message* msg)
	{
		for each(auto& com in _Components)
		{
			auto it = GMessageHandlers.find(com->GetComponentName());
			if (it != GMessageHandlers.end())
			{
				for each (auto& msgHandler in *it->second)
				{
					msgHandler(com, msg);
				}
			}
		}
	}
};


template<typename ComTy, typename MsgTy>
void MessageHandlerCallProxy(void (ComTy::*handler)(MsgTy*), Component *pCom, Message *pMsg)
{
	(static_cast<ComTy*>(pCom)->*handler)(static_cast<MsgTy*>(pMsg));
}

template<typename ComTy>
class BaseComponent : public Component
{
protected:
	static const char* ComponentName()
	{
		return typeid(ComTy).raw_name();
	}

	template<typename MsgTy>
	static void RegisterHandler(void (ComTy::*handler)(MsgTy*))
	{
		HandlerList* handlerList = nullptr;
		auto it = GMessageHandlers.find(ComponentName());
		if (it == GMessageHandlers.end())
		{
			handlerList = new HandlerList;
			GMessageHandlers[ComponentName()] = handlerList;
		}
		else
			handlerList = it->second;
		handlerList->push_back(bind(MessageHandlerCallProxy<ComTy, MsgTy>, handler, placeholders::_1, placeholders::_2));
	}
public:
	virtual const char* GetComponentName() override
	{
		return ComponentName();
	}

};

class CharacterCom : public BaseComponent<CharacterCom>
{
public:
	void OnMessage(OnEnterStage *msg)
	{
		cout << "테스트" << endl;
	}

	static void RegisterHandlers()
	{
		RegisterHandler(&OnMessage);
	}
};

Entity* CreateCharacterEntity()
{
	auto obj = new Entity;
	//obj->_Components = new Component*[1];
	obj->_Components.push_back(new CharacterCom);
	return obj;
}

int _tmain(int argc, _TCHAR* argv[])
{
	auto e = CreateCharacterEntity();

	CharacterCom::RegisterHandlers();

	//cout << typeid(CharacterCom).raw_name() << endl;
	//cout << typeid(CharacterCom).name() << endl;

	e->Post(new Message);

	return 0;
}

/*

C++/CLI Lib 버전

using namespace boost;
using namespace coroutines;
using namespace std;

typedef symmetric_coroutine<void>::call_type CoCall;
typedef symmetric_coroutine<void>::yield_type CoYield;


ref class CoContext;
__declspec(thread) gcroot<CoContext^>* CurrentContext = nullptr;
__declspec(thread) CoYield* CurrentYield = nullptr;


static void ProcessCoroutine(gcroot<System::Action^>* ptrAction, CoYield& yield)
{
	System::Action^ action = *ptrAction;
	CurrentYield = &yield;
	action();
	delete ptrAction;
}

public ref class CoContext
{
	CoCall* CallContext = nullptr;
public:
	CoContext(System::Action^ action, int stackSize, bool protectedStack, bool preserveFpu, bool stackUnwind)
	{
		attributes attr(stackSize == 0 ? stack_allocator::traits_type::default_size() : stackSize);
		attr.preserve_fpu = preserveFpu ? fpu_preserved : fpu_not_preserved;
		attr.do_unwind = stackUnwind ? stack_unwind : no_stack_unwind;
		if (protectedStack)
			CallContext = new CoCall(bind(ProcessCoroutine, new gcroot<System::Action^>(action), placeholders::_1), attr, protected_stack_allocator());
		else
			CallContext = new CoCall(bind(ProcessCoroutine, new gcroot<System::Action^>(action), placeholders::_1), attr, stack_allocator());
	}
	CoContext(System::Action^ action)
	{
		CallContext = new CoCall(bind(ProcessCoroutine, new gcroot<System::Action^>(action), placeholders::_1));
	}

	~CoContext()
	{
		delete CallContext;
	}

	bool Next()
	{
		if (!*CallContext)
			return false;
		auto oldContext = CurrentContext;
		auto context = new gcroot<CoContext^>(this);
		CurrentContext = context;
		(*CallContext)();
		delete context;
		CurrentContext = oldContext;
		if (*CallContext)
			return true;
		return false;
	}

	static bool Yield_()
	{
		if (CurrentYield)
		{
			auto yield = CurrentYield;
			(*yield)();
			CurrentYield = yield;
			return true;
		}
		return false;
	}

	static CoContext^ GetCurrent()
	{
		if (CurrentContext)
			return *CurrentContext;
		return nullptr;
	}
};

*/