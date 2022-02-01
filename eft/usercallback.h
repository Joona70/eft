class UsermodeCallback {
private: 
	CONTEXT* ContextUser;
	PVOID NtContinue;

	PVOID MmCreateKernelStack;
	PVOID MmDeleteKernelStack;
	PVOID KiCallUserMode;
	PVOID PspGetContextThreadInternal;

	KTRAP_FRAME* PsGetTrapFrame(PETHREAD Thread)
	{
		return *(KTRAP_FRAME**)((ULONG64)Thread + 0x90);
	}
public:
	void Init(PVOID KBase) {
		ContextUser = (CONTEXT*)UAlloc(sizeof(CONTEXT));
		auto CurrentProcess = ImpCall(IoGetCurrentProcess);

		ULONG NtdllSize; auto NtdllBase = GetUserModuleBase(CurrentProcess, E("ntdll.dll"), &NtdllSize);
		NtContinue = GetProcAddress(NtdllBase, E("NtContinue"));

		CFG_CALL_TARGET_INFO TargetInfo[1];
		TargetInfo[0].Flags = 0x00000001;
		TargetInfo[0].Offset = (ULONG_PTR)((ULONG64)NtContinue - (ULONG64)NtdllBase);

		MEMORY_RANGE_ENTRY RangeEntry;
		RangeEntry.VirtualAddress = NtdllBase;
		RangeEntry.NumberOfBytes = NtdllSize;

		VM_INFORMATION VMInfo;
		VMInfo.NumberOfOffsets = 1;
		VMInfo.MustBeZero = 0;
		VMInfo.TargetsProcessed = &VMInfo.ExpectedFileOffset;

		VMInfo.CallTargets = &TargetInfo[0];
		VMInfo.Section.Section = 0;
		VMInfo.Section.Data = 0;
		VMInfo.ExpectedFileOffset = 0;

		ImpCall(ZwSetInformationVirtualMemory, (HANDLE)-1, (VIRTUAL_MEMORY_INFORMATION_CLASS)2, 1, &RangeEntry, (PVOID)&VMInfo, 0x28);

		MmCreateKernelStack = EPtr(RVA(FindPattern(KBase, E(".text"), E("E8 ? ? ? ? 41 83 CF 04")), 5));

		MmDeleteKernelStack = EPtr(RVA(FindPattern(KBase, E("PAGE"), E("8B D5 E8 ? ? ? ? 48 8B 05 ? ? ? ? 48 05 ? ? ? ?")), 7));

		KiCallUserMode = EPtr(RVA(FindPattern(KBase, E("PAGE"), E("4D 8D ? ? 48 8B 94 24 ? ? ? ? 48 8B 8C 24 ? ? ? ? E8 ? ? ? ?")), 25));
	}
	template<typename Ret = void*, typename A1 = void*, typename A2 = void*, typename A3 = void*, typename A4 = void*, typename A5 = void*, typename A6 = void*>
	void Call(PVOID Ptr, A1 a1 = 0, A2 a2 = 0, A3 a3 = 0, A4 a4 = 0, A5 a5 = 0, A6 a6 = 0)
	{
		ContextUser->Rcx = (ULONG64)a1;
		ContextUser->Rdx = (ULONG64)a2;
		ContextUser->R8 = (ULONG64)a3;
		ContextUser->R9 = (ULONG64)a4;
	
		auto TrapFrame = PsGetTrapFrame(KeGetCurrentThread());
		ContextUser->Rsp = TrapFrame->Rsp - 0xF0;
		
		ContextUser->Rip = (ULONG64)Ptr;
		ContextUser->ContextFlags = CONTEXT_CONTROL | CONTEXT_INTEGER;
		*(ULONG64*)(ContextUser->Rsp + 0x30) = (ULONG64)a6;

		struct KEUSER_CALLBACK {
			ULONG64 Arg1; 
			ULONG64 Arg2;
			ULONG64 Arg3;
			ULONG64 Arg4; 
			ULONG64 Arg5;
			PVOID   Func;
		};

		KEUSER_CALLBACK UserData;
		UserData.Arg1 = (ULONG64)ContextUser;
		UserData.Arg5 = (ULONG64)a5;
		UserData.Arg2 = 0;
		UserData.Func = NtContinue;

		ImpCall(KeUserModeCallback, 2, &UserData, sizeof(UserData), &UserData, &UserData.Arg2);
	}
} UserCallback;

