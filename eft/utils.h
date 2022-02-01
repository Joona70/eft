#ifdef DEBUG
#define wsp(a) 	ImpCall(DbgPrintEx, 0, 0, "\nFACESEX WSTR: %ws\n", (a))
#define sp(a)	ImpCall(DbgPrintEx, 0, 0, "\nFACESEX STR: %s\n", (a))
#define hp(a)	ImpCall(DbgPrintEx, 0, 0, "\nFACESEX HEX: 0x%p\n", (a))
#define dp(a)	ImpCall(DbgPrintEx, 0, 0, "\nFACESEX DEC: %d\n", (a))
#define fp(a)	ImpCall(DbgPrintEx, 0, 0, "\nFACESEX FLOAT: %d.%d\n", (int)(a), (int)(((a) - (int)(a)) * 1000000))
#define v2p(a) { fp(a.x); fp(a.y); }
#define v3p(a) { fp(a.x); fp(a.y); fp(a.z); }
#define v4p(a) { fp(a.x); fp(a.y); fp(a.z); fp(a.w); }
#define rot(a) { fp(a.pitch); fp(a.yaw); fp(a.roll); }
#endif

PVOID PDEBase, PTEBase;

__forceinline PMMPTE GetPTEForVA(PVOID Address)
{
#define PHYSICAL_ADDRESS_BITS 40
#define PTI_SHIFT 12
#define PDI_SHIFT 21
#define PTE_SHIFT 3
	PMMPTE PDE = (PMMPTE)(((((ULONG64)Address >> PDI_SHIFT) << PTE_SHIFT) & 0x3FFFFFF8ull) + EPtr<ULONG64>(PDEBase));

	if (PDE->u.Hard.LargePage)
		return PDE;

	return (PMMPTE)(((((ULONG64)Address >> PTI_SHIFT) << PTE_SHIFT) & 0x7FFFFFFFF8ull) + EPtr<ULONG64>(PTEBase));
}

template <typename A>
__forceinline bool IsAddressValid(A Address) {
	return ImpCall(MmIsAddressValid, (PVOID)Address);
}
__forceinline void MemCpy(PVOID Dst, PVOID Src, ULONG Size) {
	ImpCall(memcpy, Dst, Src, Size);
}
__forceinline void MemZero(PVOID Ptr, SIZE_T Size, UCHAR Filling = 0) {
	ImpCall(memset, Ptr, Filling, Size);
}
__forceinline void Sleep(LONG64 Milliseconds) {
	LARGE_INTEGER Delay;
	Delay.QuadPart = -Milliseconds * 10000;
	ImpCall(KeDelayExecutionThread, KernelMode, false, &Delay);
}

__forceinline PVOID KAlloc(ULONG Size, POOL_TYPE PoolType = NonPagedPoolNx) {
	PVOID Buff = ImpCall(ExAllocatePoolWithTag, PoolType, Size, 'KgxD');
	if (Buff) MemZero(Buff, Size); return Buff;
}
__forceinline void KFree(PVOID Ptr, ULONG Size = 0) {
	if(Size) MemZero(Ptr, Size);
	ImpCall(ExFreePoolWithTag, Ptr, 'KgxD');
}
__forceinline PVOID UAlloc(ULONG Size, ULONG Protect = PAGE_READWRITE) {
	PVOID AllocBase = nullptr; SIZE_T SizeUL = SizeAlign(Size);
	if (!ImpCall(ZwAllocateVirtualMemory, ZwCurrentProcess(), &AllocBase, 0, &SizeUL, MEM_COMMIT, Protect)) {
		MemZero(AllocBase, SizeUL);
	}
	return AllocBase;
}
__forceinline void UFree(PVOID Ptr) {
	SIZE_T SizeUL = 0;
	ImpCall(ZwFreeVirtualMemory, ZwCurrentProcess(), &Ptr, &SizeUL, MEM_RELEASE);
}
__forceinline PEPROCESS KiSwapProcess(PEPROCESS NewProcess) {
	auto CurrentThread = KeGetCurrentThread();
	auto ApcState = *(ULONG64*)((ULONG64)CurrentThread + 0x98);
	auto OldProcess = *(PEPROCESS*)(ApcState + 0x20);
	*(PEPROCESS*)(ApcState + 0x20) = NewProcess;
	auto DirectoryTableBase = *(ULONG64*)((ULONG64)NewProcess + 0x28);
	__writecr3(DirectoryTableBase);
	return OldProcess;
}

#define InRange(x, a, b) (x >= a && x <= b) 
#define GetBits(x) (InRange(x, '0', '9') ? (x - '0') : ((x - 'A') + 0xA))
#define GetByte(x) ((UCHAR)(GetBits(x[0]) << 4 | GetBits(x[1])))

PVOID FindSection(PVOID ModBase, const char* Name, PULONG SectSize)
{
	PIMAGE_NT_HEADERS NT_Header = NT_HEADER(ModBase);
	PIMAGE_SECTION_HEADER Sect = IMAGE_FIRST_SECTION(NT_Header);
	for (PIMAGE_SECTION_HEADER pSect = Sect; pSect < Sect + NT_Header->FileHeader.NumberOfSections; pSect++)
	{
		char SectName[9]; SectName[8] = 0;
		MemCpy(SectName, pSect->Name, 8);
		if (StrICmp(pSect->Name, Name, true))
		{
			if (SectSize) {
				ULONG SSize = SizeAlign(max(pSect->Misc.VirtualSize, pSect->SizeOfRawData));
				*SectSize = SSize;
			}
			return (PVOID)((ULONG64)ModBase + pSect->VirtualAddress);
		}
	}
	return nullptr;
}
PUCHAR FindPattern(PVOID ModBase, const char* SectName, const char* Pattern, ULONG AddressOffset = 0)
{
	ULONG SectSize = 0; ULONG Offset = 0;
	PUCHAR SectStart = (PUCHAR)FindSection(ModBase, SectName, &SectSize);

	PUCHAR ModBuff = (PUCHAR)KAlloc(SectSize); MemCpy(ModBuff, SectStart, SectSize);
	PUCHAR ModuleStart = ModBuff; PUCHAR ModuleEnd = ModBuff + SectSize;

	PUCHAR FirstMatch = nullptr;
	const char* CurPatt = Pattern;
	for (; ModuleStart < ModuleEnd; ++ModuleStart)
	{
		bool SkipByte = (*CurPatt == '\?');
		if (SkipByte || *ModuleStart == GetByte(CurPatt)) {
			if (!FirstMatch) FirstMatch = ModuleStart;
			SkipByte ? CurPatt += 2 : CurPatt += 3;

			if (CurPatt[-1] == 0 && Offset++ == AddressOffset)
				break;
		}
		else if (FirstMatch) {
			ModuleStart = FirstMatch;
			FirstMatch = nullptr;
			CurPatt = Pattern;
		}
	}

	KFree(ModBuff, SectSize);

	return FirstMatch ? (PUCHAR)(((ULONG64)FirstMatch - (ULONG64)ModBuff) + (ULONG64)SectStart) : nullptr;
}

PVOID NQSI(SYSTEM_INFORMATION_CLASS Class, ULONG* Size = nullptr)
{
NewTry: ULONG ReqSize = 0;
	ImpCall(ZwQuerySystemInformation, Class, nullptr, ReqSize, &ReqSize);
	if (!ReqSize) goto NewTry;

	PVOID pInfo = KAlloc(ReqSize);
	if (!NT_SUCCESS(ImpCall(ZwQuerySystemInformation, Class, pInfo, ReqSize, &ReqSize))) {
		KFree(pInfo, ReqSize); goto NewTry;
	}

	if (Size) *Size = ReqSize;

	return pInfo;
}
PVOID GetKernelAddress(const char* ModName, ULONG* Size = nullptr)
{
	ULONG ReqSize;
	PSYSTEM_MODULE_INFORMATION ModuleList = (PSYSTEM_MODULE_INFORMATION)NQSI(SystemModuleInformation, &ReqSize);

	PVOID ModuleBase = 0;
	for (ULONG64 i = 0; i < ModuleList->ulModuleCount; i++)
	{
		SYSTEM_MODULE Module = ModuleList->Modules[i];
		if (StrICmp(&Module.ImageName[Module.ModuleNameOffset], ModName, true)) {
			ModuleBase = Module.Base;

			if (Size) *Size = Module.Size;

			break;
		}
	}

	KFree(ModuleList, ReqSize);
	return ModuleBase;
}
PEPROCESS GetProcessByName(const char* ProcName) {
	PEPROCESS EProc = nullptr; ULONG ReqSize;
	PSYSTEM_PROCESS_INFO pInfo = (PSYSTEM_PROCESS_INFO)NQSI(SystemProcessInformation, &ReqSize), pInfoCur = pInfo;

	while (true)
	{
		const wchar_t* ProcessName = pInfoCur->ImageName.Buffer;
		if (IsAddressValid(ProcessName))
		{
			if (StrICmp(ProcName, ProcessName, true))
			{
				if (!ImpCall(PsLookupProcessByProcessId, pInfoCur->UniqueProcessId, &EProc))
					break;
			}
		}

		if (!pInfoCur->NextEntryOffset) break;
		pInfoCur = (PSYSTEM_PROCESS_INFO)((ULONG64)pInfoCur + pInfoCur->NextEntryOffset);
	}

	KFree(pInfo, ReqSize);
	return EProc;
}
PVOID GetUserModuleBase(PEPROCESS Process, const char* ModName, ULONG* ModSize = nullptr)
{
	PPEB PPEB = ImpCall(PsGetProcessPeb, Process);

	if (IsAddressValid(PPEB)) {
		PEB PEB_Data;
		MemCpy(&PEB_Data, PPEB, sizeof(PEB));

		if (IsAddressValid(PEB_Data.Ldr)) {
			PEB_LDR_DATA Ldr;
			MemCpy(&Ldr, PEB_Data.Ldr, sizeof(PEB_LDR_DATA));

			PLIST_ENTRY LdrListHead = Ldr.InLoadOrderModuleList.Flink;
			PLIST_ENTRY LdrCurrentNode = Ldr.InLoadOrderModuleList.Flink;

			if (IsAddressValid(LdrListHead)) {
				do
				{
					LDR_DATA_TABLE_ENTRY ListEntry;
					MemCpy(&ListEntry, LdrCurrentNode, sizeof(LDR_DATA_TABLE_ENTRY));
				
					if (ListEntry.BaseDllName.Length > 0 && StrICmp(ModName, ListEntry.BaseDllName.Buffer, true)) {
						if (ModSize) *ModSize = ListEntry.SizeOfImage;
						return ListEntry.DllBase;
					}
				
					LdrCurrentNode = ListEntry.InLoadOrderLinks.Flink;
				} while (LdrListHead != LdrCurrentNode);
			}
		}
	}

	return nullptr;
}

PUCHAR FindPatternInProcess(PVOID ModBase, const char* Pattern)
{
	PVOID HeaderPage = KAlloc(PAGE_SIZE); MemCpy(HeaderPage, ModBase, PAGE_SIZE);
	PIMAGE_NT_HEADERS NtHeader = ((PIMAGE_NT_HEADERS)((ULONG64)HeaderPage + ((PIMAGE_DOS_HEADER)(ULONG64)HeaderPage)->e_lfanew));
	ULONG SizeOfImage = NtHeader->OptionalHeader.SizeOfImage;
	PVOID ModBuff = KAlloc(SizeOfImage); MemCpy(ModBuff, ModBase, SizeOfImage);

	PUCHAR ModuleStart = (PUCHAR)ModBuff; if (!ModuleStart) return nullptr;
	PUCHAR ModuleEnd = (PUCHAR)(ModuleStart + SizeOfImage - 0x1000); ModuleStart += 0x1000;

	PUCHAR FirstMatch = nullptr;
	const char* CurPatt = Pattern;
	for (; ModuleStart < ModuleEnd; ++ModuleStart)
	{
		bool SkipByte = (*CurPatt == '\?');
		if (SkipByte || *ModuleStart == GetByte(CurPatt)) {
			if (!FirstMatch) FirstMatch = ModuleStart;
			SkipByte ? CurPatt += 2 : CurPatt += 3;
			if (CurPatt[-1] == 0) break;
		}

		else if (FirstMatch) {
			ModuleStart = FirstMatch;
			FirstMatch = nullptr;
			CurPatt = Pattern;
		}
	}

	KFree(HeaderPage, PAGE_SIZE);
	KFree(ModBuff, SizeOfImage);

	return FirstMatch ? (PUCHAR)(((ULONG64)FirstMatch - (ULONG64)ModBuff) + (ULONG64)ModBase) : nullptr;
}
PUCHAR FindPatternInRange(PVOID AllocationBase, ULONG Size, const char* Pattern)
{
	if (!AllocationBase) return nullptr;
	PVOID ModBuff = KAlloc(Size); MemCpy(ModBuff, AllocationBase, Size);

	PUCHAR ModuleStart = (PUCHAR)ModBuff; if (!ModuleStart) return nullptr;
	PUCHAR ModuleEnd = (PUCHAR)(ModuleStart + Size);

	PUCHAR FirstMatch = nullptr;
	const char* CurPatt = Pattern;
	for (; ModuleStart < ModuleEnd; ++ModuleStart)
	{
		bool SkipByte = (*CurPatt == '\?');
		if (SkipByte || *ModuleStart == GetByte(CurPatt)) {
			if (!FirstMatch) FirstMatch = ModuleStart;
			SkipByte ? CurPatt += 2 : CurPatt += 3;
			if (CurPatt[-1] == 0) break;
		}

		else if (FirstMatch) {
			ModuleStart = FirstMatch;
			FirstMatch = nullptr;
			CurPatt = Pattern;
		}
	}

	KFree(ModBuff, Size);

	return FirstMatch ? (PUCHAR)(((ULONG64)FirstMatch - (ULONG64)ModBuff) + (ULONG64)AllocationBase) : nullptr;
}

unsigned __int16 GetAsyncKeyState(unsigned int a1)
{
	__int16 result;

	if (a1 >= 0x100) { result = 0; }
	else
	{
		unsigned __int64 v1 = (unsigned __int8)a1;
		unsigned __int64 v2 = (unsigned __int64)(unsigned __int8)a1 >> 3;

		__int16 v4 = 0;
		unsigned int v3 = v1 & 7;

		int v5 = *(unsigned __int8*)(v2 + EPtr<ULONG64>(Win32::gafAsyncKeyStateRecentDown));

		if (_bittest((LONG*)&v5, v3)) {
			v4 = 1;
			*(UCHAR*)(v2 + EPtr<ULONG64>(Win32::gafAsyncKeyStateRecentDown)) = v5 & ~(1 << v3);
		}

		result = v4 | 0x8000;
		if (!(*((UCHAR*)EPtr(Win32::gafAsyncKeyState) + (v1 >> 2)) & (unsigned __int8)(1 << (2 * (v1 & 3)))))
			result = v4;
	}

	return result;
}
void ScreenToClient(PVOID a1, POINT& a2)
{
	__int64 v3; // rdx
	__int64 result; // rax

	v3 = *(ULONG64*)((ULONG64)a1 + 0x28);
	result = *(WORD*)(v3 + 0x2A) & 0x3FFF;
	if ((ULONG)result != 0x29D)
	{
		if ((*(BYTE*)(v3 + 26) & 0x40) != 0)
			a2.x = *(ULONG*)(v3 + 0x70) - a2.x;
		else
			a2.x -= *(ULONG*)(v3 + 0x68);
		result = *(ULONG64*)((ULONG64)a1 + 0x28);
		a2.y -= *(ULONG*)(result + 0x6C);
	}
}

template <typename T, typename A>
__forceinline T Read(A Address) {
	if (IsAddressValid(Address)) { return *(T*)Address; }
	return T{};
}
template <typename T>
__forceinline void ReadArray(T Address, PVOID Data, ULONG Size)
{
	if (IsAddressValid(Address)) { ImpCall(memcpy, Data, (PVOID)Address, Size); }
}
template <typename T>
__forceinline T ReadChain(std::initializer_list<uint64_t> Offsets, bool ReadFirstOffset = true) {
	auto Start = Offsets.begin(); size_t ReadSize = Offsets.size();
	uint64_t LastPtr = Read<uint64_t>((ReadFirstOffset ? Read<uint64_t>(Start[0]) : Start[0]) + Start[1]);
	for (size_t i = 2; i < ReadSize - 1; i++) if (!LastPtr || !(LastPtr = Read<uint64_t>(LastPtr + Start[i]))) return T{};
	return Read<T>(LastPtr + Start[ReadSize - 1]);
}
template <typename T, typename A>
__forceinline void Write(A Address, T Data) {
	if (IsAddressValid(Address)) { *(T*)Address = Data; }
}
template <typename T>
__forceinline void WriteArray(T Address, PVOID Data, ULONG Size) {
	if (IsAddressValid(Address)) { ImpCall(memcpy, (PVOID)Address, Data, Size); }
}
template <typename T>
__forceinline void WriteChain(std::initializer_list<uint64_t> Offsets, T Value, bool ReadFirstOffset = true) {
	auto Start = Offsets.begin(); size_t ReadSize = Offsets.size();
	uint64_t LastPtr = Read<uint64_t>((ReadFirstOffset ? Read<uint64_t>(Start[0]) : Start[0]) + Start[1]);
	for (size_t i = 2; i < ReadSize - 1; i++) if (!LastPtr || !(LastPtr = Read<uint64_t>(LastPtr + Start[i]))) return;
	Write(LastPtr + Start[ReadSize - 1], Value);
}
template <typename T>
__forceinline void WriteProtected(T Address, PVOID Data, ULONG Size)
{
	auto onWP = __readcr0(),
		offWP = __readcr0() & ~(1UL << 16);

	_disable();

	__writecr0(offWP);
	__movsb((PUCHAR)Address, (PUCHAR)Data, Size);
	__writecr0(onWP);

	_enable();
}
void TextFormatA(char* Dest, size_t Size, const char* Format, ...)
{
	size_t sizeDest = Size / sizeof(char);
	va_list argList;
	va_start(argList, Format);
	ImpCall(_vsnprintf, Dest, sizeDest - 1, Format, argList);
	va_end(argList);
}
void TextFormatW(wchar_t* Dest, size_t Size, const wchar_t* Format, ...)
{
	size_t sizeDest = Size / sizeof(wchar_t);
	va_list argList;
	va_start(argList, Format);
	ImpCall(_vsnwprintf, Dest, sizeDest - 1, Format, argList);
	va_end(argList);
}