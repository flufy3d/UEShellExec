#include "UEShellExecPrivatePCH.h"

#if PLATFORM_WINDOWS
#include "WindowsSpec.h"
#else
#include "UnixSpec.h"
#endif



USystemDoneObject::USystemDoneObject(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}
UPopenDoneObject::UPopenDoneObject(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}


UShellExec::UShellExec(const class FObjectInitializer& PCIP)
: Super(PCIP)
{

}


class CallSystemTask : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<CallSystemTask>;

	FString _cmd;
	USystemDoneObject* Obj;

	CallSystemTask(const FString& cmd, USystemDoneObject* obj)
		: _cmd(cmd),
		Obj(obj)
	{
	}

	void DoWork()
	{
		int ret = 0;
		ret = SystemWrap(*_cmd);
		Obj->SystemDoneEvent.Broadcast(_cmd, ret);
		UE_LOG(LogUEShellExec, Log, TEXT("CallSystemTask..."));

	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(ShellExecAutoDeleteAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};

class CallPopenTask : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<CallPopenTask>;

	FString _cmd;
	UPopenDoneObject* Obj;

	CallPopenTask(const FString& cmd, UPopenDoneObject* obj)
		: _cmd(cmd),
		Obj(obj)
	{
	}

	void DoWork()
	{
		FString ret;
		ret = PopenWrap(*_cmd);
		Obj->PopenDoneEvent.Broadcast(_cmd, ret);
		UE_LOG(LogUEShellExec, Log, TEXT("CallPopenTask..."));
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(ShellExecAutoDeleteAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};

USystemDoneObject* UShellExec::System_Background(UObject* WorldContextObject,const FString& cmd)
{
	
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
	USystemDoneObject* tempObject = Cast<USystemDoneObject>(StaticConstructObject_Internal(USystemDoneObject::StaticClass()));

	
	(new FAutoDeleteAsyncTask<CallSystemTask>(cmd, tempObject))->StartBackgroundTask();

	return tempObject;


}


int UShellExec::System(const FString& cmd)
{
	int ret = 0;
	ret = SystemWrap(*cmd);
	UE_LOG(LogUEShellExec, Log, TEXT("UShellExec::System Process returned %d."), ret);
	return ret;

}


UPopenDoneObject* UShellExec::Popen_Background(UObject* WorldContextObject, const FString& cmd)
{

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
	UPopenDoneObject* tempObject = Cast<UPopenDoneObject>(StaticConstructObject_Internal(UPopenDoneObject::StaticClass()));


	(new FAutoDeleteAsyncTask<CallPopenTask>(cmd, tempObject))->StartBackgroundTask();

	return tempObject;


}
    
FString UShellExec::Popen(const FString& cmd)
{
	FString ret;
	ret = PopenWrap(*cmd);
	return ret;

}




