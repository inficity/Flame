// FlameTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace boost;
using namespace coroutines;
using namespace std;

typedef symmetric_coroutine<void>::call_type CoCall;
typedef symmetric_coroutine<void>::yield_type CoYield;

int _tmain(int argc, _TCHAR* argv[])
{

	return 0;
}

/*

C++/CLI Lib ¹öÀü

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