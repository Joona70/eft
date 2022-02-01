#pragma pack(push, 1)
typedef union CR3_
{
	uint64_t value;
	struct
	{
		uint64_t ignored_1 : 3;
		uint64_t write_through : 1;
		uint64_t cache_disable : 1;
		uint64_t ignored_2 : 7;
		uint64_t pml4_p : 40;
		uint64_t reserved : 12;
	};
} CR3;

typedef union VIRT_ADDR_
{
	void* value;
	void* pointer;

	struct {
		uint64_t offset : 12;
		uint64_t pt_index : 9;
		uint64_t pd_index : 9;
		uint64_t pdpt_index : 9;
		uint64_t pml4_index : 9;
		uint64_t reserved : 16;
	} s;
	struct {
		uint64_t offset : 21;
		uint64_t pd_index : 9;
		uint64_t pdpt_index : 9;
		uint64_t pml4_index : 9;
		uint64_t reserved : 16;
	} l;
} VIRT_ADDR;

typedef union PML4E_
{
	uint64_t value;
	struct
	{
		uint64_t present : 1;
		uint64_t rw : 1;
		uint64_t user : 1;
		uint64_t write_through : 1;
		uint64_t cache_disable : 1;
		uint64_t accessed : 1;
		uint64_t ignored_1 : 1;
		uint64_t reserved_1 : 1;
		uint64_t ignored_2 : 4;
		uint64_t pdpt_p : 40;
		uint64_t ignored_3 : 11;
		uint64_t xd : 1;
	};
} PML4E;

typedef union PDPTE_
{
	uint64_t value;
	struct
	{
		uint64_t present : 1;
		uint64_t rw : 1;
		uint64_t user : 1;
		uint64_t write_through : 1;
		uint64_t cache_disable : 1;
		uint64_t accessed : 1;
		uint64_t dirty : 1;
		uint64_t page_size : 1;
		uint64_t ignored_2 : 4;
		uint64_t pd_p : 40;
		uint64_t ignored_3 : 11;
		uint64_t xd : 1;
	};
} PDPTE;

typedef union PDE_
{
	uint64_t value;
	struct
	{
		uint64_t present : 1;
		uint64_t rw : 1;
		uint64_t user : 1;
		uint64_t write_through : 1;
		uint64_t cache_disable : 1;
		uint64_t accessed : 1;
		uint64_t dirty : 1;
		uint64_t page_size : 1;
		uint64_t global : 1;
		uint64_t ignored_2 : 3;
		uint64_t pt_p : 40;
		uint64_t ignored_3 : 11;
		uint64_t xd : 1;
	};
} PDE;

typedef union PTE_
{
	uint64_t value;
	VIRT_ADDR vaddr;
	struct
	{
		uint64_t present : 1;
		uint64_t rw : 1;
		uint64_t user : 1;
		uint64_t write_through : 1;
		uint64_t cache_disable : 1;
		uint64_t accessed : 1;
		uint64_t dirty : 1;
		uint64_t pat : 1;
		uint64_t global : 1;
		uint64_t ignored_1 : 3;
		uint64_t page_frame : 40;
		uint64_t ignored_3 : 11;
		uint64_t xd : 1;
	};
} PTE;
#pragma pack(pop)

auto Flush(void* Pointer) {
	unsigned __int64 v0; // rcx
	__int64 result; // rax

	v0 = __readcr4();
	if ((v0 & 0x20080) != 0) {
		result = v0 ^ 0x80;
		__writecr4(v0 ^ 0x80);
		__writecr4(v0);
	}

	else {
		result = __readcr3();
		__writecr3(result);
	}

	__wbinvd();
	__invlpg(Pointer);
	return result;
}

class PageShift {
private:
	ULONG PML4_Index;
	ULONG PDPT_Index;

	PDE* PPDE;
	PTE* PPTE;

	uint64_t AllocatePFN(PVOID PageVA, PVOID Data, ULONG Size, ULONG Offset)
	{
		auto AllocBase = CallPtr<PVOID>(EPtr(MmAllocateContiguousMemoryFn), PAGE_SIZE, -1);
			
		MemCpy(AllocBase, PageVA, PAGE_SIZE);

		if (Data) MemCpy((PVOID)((ULONG64)AllocBase + Offset), Data, Size);
		
		ULONG64 PhysAddress = CallPtr<ULONG64>(EPtr(MmGetPhysicalAddressFn), AllocBase);
		ImpCall(MmFreeContiguousMemory, AllocBase);

		ULONG64 AllocSize = PAGE_SIZE;

		if (CallPtr<NTSTATUS>(EPtr(MmRemovePhysicalMemoryFn), &PhysAddress, &AllocSize) == STATUS_SUCCESS)
			return PhysAddress >> PAGE_SHIFT;

		return 0;
	}
	uint64_t AllocatePFN(PVOID PageVA, PDE* PDE_Array)
	{
		auto AllocBase = CallPtr<PVOID>(EPtr(MmAllocateContiguousMemoryFn), PAGE_SIZE, -1);

		MemCpy(AllocBase, PageVA, PAGE_SIZE);

		for (int i = 0; i < 512; i++) {
			auto NewPDE = &PDE_Array[i];
			if (NewPDE->pt_p) MemCpy((PVOID)((ULONG64)AllocBase + (i * sizeof(PDE))), NewPDE, sizeof(PDE));
		}

		ULONG64 PhysAddress = CallPtr<ULONG64>(EPtr(MmGetPhysicalAddressFn), AllocBase);
		ImpCall(MmFreeContiguousMemory, AllocBase);

		ULONG64 AllocSize = PAGE_SIZE;
		if (CallPtr<NTSTATUS>(EPtr(MmRemovePhysicalMemoryFn), &PhysAddress, &AllocSize) == STATUS_SUCCESS)
			return PhysAddress >> PAGE_SHIFT;
		
		return 0;
	}
	void* PfnToVA(uint64_t Pfn)
	{
		return CallPtr<PVOID>(EPtr(MmGetVirtualForPhysicalFn), Pfn << PAGE_SHIFT);
	}
public:
	void Init(PVOID BaseAddress) {
		auto Address = VIRT_ADDR{ BaseAddress };

		PML4_Index = Address.s.pml4_index;
		PDPT_Index = Address.s.pdpt_index;

		PPDE = (PDE*)KAlloc(PAGE_SIZE);
		PPTE = (PTE*)KAlloc(0x200000);
	}
	bool Patch(PVOID Address, PVOID Data, ULONG Size) {
		auto DirectoryTableBase = *(CR3*)((ULONG64)ImpCall(IoGetCurrentProcess) + 0x28);
		auto PatchAddress = VIRT_ADDR{ Address };

		auto PML4 = (PML4E*)PfnToVA(DirectoryTableBase.pml4_p);
		auto PML4E_Ptr = &PML4[PML4_Index];
		if (!PML4E_Ptr->present) return false;

		auto PDPT = (PDPTE*)PfnToVA(PML4E_Ptr->pdpt_p);
		auto PDPTE_Ptr = &PDPT[PDPT_Index];
		if (!PDPTE_Ptr->present) return false;

		auto PD = (PDE*)PfnToVA(PDPTE_Ptr->pd_p);
		auto PDE_Ptr = &PD[PatchAddress.s.pd_index];
		if (!PDE_Ptr->present) return false;

		auto NewPDE = &PPDE[PatchAddress.s.pd_index];

		auto PageFramePTE = NewPDE->pt_p;
		if (!PageFramePTE) PageFramePTE = PDE_Ptr->pt_p;

		PTE* PTE_Array = (PTE*)KAlloc(PAGE_SIZE);

		for (int i = 0; i < 512; i++) {
			PTE_Array[i].value = 0;
			PTE_Array[i].present = true;
			PTE_Array[i].rw = PDE_Ptr->rw;
			PTE_Array[i].user = PDE_Ptr->user;
			PTE_Array[i].xd = PDE_Ptr->xd;
			PTE_Array[i].global = PDE_Ptr->global;
			auto NewPTE = &PPTE[PatchAddress.s.pd_index * i];
			PTE_Array[i].page_frame = NewPTE->page_frame ? NewPTE->page_frame : PDE_Ptr->pt_p + i;
		}

		auto PTE_Ptr = &PTE_Array[PatchAddress.s.pt_index];
		auto NewPTE = &PPTE[PatchAddress.s.pd_index * PatchAddress.s.pt_index];

		auto PageFrame = NewPTE->page_frame;
		if (!PageFrame) PageFrame = PTE_Ptr->page_frame;		

		auto PageVA = PfnToVA(PageFrame);

		if (!PageVA) return false;

		auto PageNew_4K_Pfn = AllocatePFN(PageVA, Data, Size, PatchAddress.s.offset);
	
		if (!PageNew_4K_Pfn) return false;

		PTE_Ptr->global = false;
		PTE_Ptr->page_frame = PageNew_4K_Pfn;
		NewPTE->page_frame = PageNew_4K_Pfn;

		auto PTE_New_Pfn = AllocatePFN(PTE_Array, 0, 0, 0);
		if (!PTE_New_Pfn) return false;

		*NewPDE = *PDE_Ptr;
		PDE_Ptr->global = false;
		NewPDE->page_size = false;
		NewPDE->pt_p = PTE_New_Pfn;

		KFree(PTE_Array, PAGE_SIZE);

		return true;
	}
	bool Enable() {
		auto DirectoryTableBase = *(CR3*)((ULONG64)ImpCall(IoGetCurrentProcess) + 0x28);

		auto PML4 = (PML4E*)PfnToVA(DirectoryTableBase.pml4_p);
		auto PML4E_Ptr = &PML4[PML4_Index];
		if (!PML4E_Ptr->present) return false;

		auto PDPT = (PDPTE*)PfnToVA(PML4E_Ptr->pdpt_p);
		auto PDPTE_Ptr = &PDPT[PDPT_Index];
		auto PDPTE_Val = *PDPTE_Ptr;
		if (!PDPTE_Ptr->present) return false;

		auto PD = (PDE*)PfnToVA(PDPTE_Ptr->pd_p);

		auto PDE_New_Pfn = AllocatePFN(PD, PPDE);
		if (!PDE_New_Pfn) return false;

		PDPTE_Val.pd_p = PDE_New_Pfn;

		auto PDPTE_New_Pfn = AllocatePFN(PDPT, &PDPTE_Val, sizeof(PDPTE), PDPT_Index * sizeof(PDPTE));
		if (!PDPTE_New_Pfn) return false;

		PML4E_Ptr->pdpt_p = PDPTE_New_Pfn;

		KFree(PPDE, PAGE_SIZE);
		KFree(PPTE, 0x200000);

		ImpCall(KeIpiGenericCall, (PKIPI_BROADCAST_WORKER)Flush, 0);
		ImpCall(KeInvalidateAllCaches);

		return true;
	}
} PageShift;
