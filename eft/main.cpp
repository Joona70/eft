#include "global.h"

CONTEXT Context;
RenderTexture* Texture;
bool IsPresentHookReset;

GameObjectManager* GameManager = nullptr;
LocalWorld* World = nullptr;
GameObject* FPSCamera = nullptr;
Camera* CurrentCamera = nullptr;
Entity* LocalPlayer = nullptr;
Weapon* CurrentWeapon = nullptr;
Entity* AimbotTarget = nullptr;

uint64_t VisorEffect;
uint64_t ThermalVision;
uint64_t NightVision;

struct SearchedLootTemplate {
	wchar_t Name[128];
};
SearchedLootTemplate SearchedLootCached[30];
int SearchedItemsCount;

struct ExitCached
{
	wchar_t Name[64];
	wchar_t Status[32];
	Vector3 Location;
};
static int ExitCachedCount;
ExitCached ExitCachedArray[25];

void Exists() {
	if (!Visuals::Exits || LocalPlayer == nullptr || LocalPlayer->Health() <= 0)
		return;

	static uint8_t Tick = 0;

	if (!Tick++) {
		int ExitCount = 0;

		ExistsList* ExistsList = World->GetExistsList();
		int ExitsCount = ExistsList->GetExistsCount();

		for (int i = 0; i < ExitsCount; i++)
		{
			Exit* Exit = ExistsList->GetExit(i);

			if (Exit == nullptr)
				continue;

			const wchar_t* Name = Exit->Name();

			if (!IsAddressValid(Name))
				continue;

			if (ExitCount < 24) {
				MemCpy(ExitCachedArray[ExitCount].Name, (PVOID)Name, (StrLen(Name) + 1) * 2);

				int Status = Exit->Status();

				if (Status == NotPresent) {
					auto StatusName = EXor(L"Closed");
					MemCpy(ExitCachedArray[ExitCount].Status, (PVOID)StatusName.GetDecrypted(), (StatusName.Size() + 1) * 2);
				}
				else if (Status == UncompleteRequirements) {
					auto StatusName = EXor(L"Requirement");
					MemCpy(ExitCachedArray[ExitCount].Status, (PVOID)StatusName.GetDecrypted(), (StatusName.Size() + 1) * 2);
				}
				else if (Status == Countdown) {
					auto StatusName = EXor(L"Countdown");
					MemCpy(ExitCachedArray[ExitCount].Status, (PVOID)StatusName.GetDecrypted(), (StatusName.Size() + 1) * 2);
				}
				else if (Status == RegularMode) {
					auto StatusName = EXor(L"Open");
					MemCpy(ExitCachedArray[ExitCount].Status, (PVOID)StatusName.GetDecrypted(), (StatusName.Size() + 1) * 2);
				}
				else if (Status == Pending) {
					auto StatusName = EXor(L"Pending");
					MemCpy(ExitCachedArray[ExitCount].Status, (PVOID)StatusName.GetDecrypted(), (StatusName.Size() + 1) * 2);
				}
				else if (Status == AwaitsManualActivation) {
					auto StatusName = EXor(L"Manual");
					MemCpy(ExitCachedArray[ExitCount].Status, (PVOID)StatusName.GetDecrypted(), (StatusName.Size() + 1) * 2);
				}
				else continue;

				ExitCachedArray[ExitCount++].Location = Exit->GetPosition();
			}
		}
		ExitCachedCount = ExitCount;
	}

	for (int i = 0; i < ExitCachedCount; i++)
	{
		Vector2 ScreenSpace;
		if (World2Screen(ExitCachedArray[i].Location, ScreenSpace)) {
			Texture->String(ScreenSpace, ExitCachedArray[i].Name, RGB(255, 0, 0), true);
			Texture->String({ ScreenSpace.x, ScreenSpace.y + 15.f }, ExitCachedArray[i].Status, RGB(255, 0, 0), true);
			float Distance = Math::Dist3D(LocalPlayer->GetBoneLocation(HumanHead), ExitCachedArray[i].Location);
			wchar_t DinstanceStr[16]; TextFormatW(DinstanceStr, sizeof(DinstanceStr), E(L"%d"), (int)Distance);
			Texture->String({ ScreenSpace.x, ScreenSpace.y + 30.f }, DinstanceStr, RGB(255, 0, 0), true);
		}
	}
}

void Hack() {
	static bool Init;
	if (!Init) {
		PVOID UnityPlayer = GetUserModuleBase(ImpCall(IoGetCurrentProcess), E("UnityPlayer.dll"));
		GameManager = Read<GameObjectManager*>(RVA(FindPatternInProcess(UnityPlayer, E("48 8B 15 ? ? ? ? 66 39")), 7));

		if (!GameManager) return;

		Init = true;
	}

	bool IsInGame = (GameManager->FirstTaggedObject() != GameManager->LastTaggedObject());

	if (!IsInGame) {
		CurrentCamera = nullptr;
		World = nullptr;
		return;
	}

	if (CurrentCamera == nullptr) {
		GameObject* Object = GetObjectByTag(5);

		if (Object == nullptr)
			return;

		CurrentCamera = (Camera*)Object->Entity();

		if (CurrentCamera == nullptr)
			return;

		FPSCamera = Object;

		uint64_t ComponentList = Read<uint64_t>(Object + 0x30);

		for (int i = 0x8; i < 0x300; i += 0x10)
		{
			uint64_t Component = Read<uint64_t>(ComponentList + i);

			if (Component) {
				uint64_t Fields = Read<uint64_t>(Component + 0x28);

				if (Fields) {
					const char* ClassName = ReadChain<const char*>({ Fields + 0x0, 0x0, 0x48 });

					if (IsAddressValid(ClassName)) {
						if (StrStr(ClassName, E("VisorEffect"))) { VisorEffect = Fields; }
						if (StrStr(ClassName, E("NightVision"))) { NightVision = Fields; }
						if (StrStr(ClassName, E("ThermalVision"))) { ThermalVision = Fields; }
					}
				}
			}
		}
	}

	if (World == nullptr) {
		GameObject* Object = GetObjectByName(E("GameWorld"));

		if (Object == nullptr)
			return;

		uint64_t ObjectEntity = Object->Entity();

		if (ObjectEntity) {
			World = Read<LocalWorld*>(ObjectEntity + 0x28);
		}
	}

	if (World == nullptr || CurrentCamera == nullptr)
		return;

	Exists();

	EntityList* EntityList = World->GetEntityList();
	int EntityCount = EntityList->GetEntitiesCount();

	for (int i = 0; i < EntityCount; i++) {
		Entity* Entity = EntityList->GetEntity(i);

		if (Entity == nullptr)
			continue;

		if (Entity->IsLocalPlayer()) { LocalPlayer = Entity; continue; }
	}
}

void Present() {
	PVOID CurrentThread = KeGetCurrentThread();
	PVOID KBaseA = EPtr(KBase);

	InterruptedThreadArray[1] = EPtr<PKTHREAD>(CurrentThread);
	PreservedStackArray[1] = EPtr(Context.Rsp);

	if (IsPresentHookReset) goto Ret;

	static ULONG ProcessFlagsOffset = 0;
	if (!ProcessFlagsOffset) ProcessFlagsOffset = *(ULONG*)(FindPattern(KBaseA, E(".text"), E("F0 83 8F ? ? ? ? ? 8A ? ? ? ? ? 80 E2")) + 3);

	if ((*(ULONG*)((ULONG64)ImpCall(IoGetCurrentProcess) + ProcessFlagsOffset) & (1UL << 3))) {
		KFree(Texture);
		IsPresentHookReset = true;
		goto Ret;
	}

	{
		POINT CursorPosition = *(POINT*)(EPtr<ULONG64>(Win32::gpsi) + 0x1360);

		auto WindowInstance = CallPtr(EPtr(Win32::ValidateHwnd), DirectX.GetSwapChainHandle());
		if (WindowInstance) ScreenToClient(WindowInstance, CursorPosition); else goto Ret;

		ScreenSize = { FLOAT(Texture->Width), FLOAT(Texture->Height) };

		//Hack();

		if (Visuals::Crosshair) {
			Texture->FillRectangle({ ScreenSize.x / 2.f - 1.f, ScreenSize.y / 2.f + 2.f }, { 2.f, 8.f }, RGB(196, 196, 196));
			Texture->FillRectangle({ ScreenSize.x / 2.f + 2.f, ScreenSize.y / 2.f - 1.f }, { 8.f, 2.f }, RGB(196, 196, 196));
			Texture->FillRectangle({ ScreenSize.x / 2.f - 1.f, ScreenSize.y / 2.f - 10.f }, { 2.f, 8.f }, RGB(196, 196, 196));
			Texture->FillRectangle({ ScreenSize.x / 2.f - 10.f, ScreenSize.y / 2.f - 1.f }, { 8.f, 2.f }, RGB(196, 196, 196));
		}
		if (Visuals::FOV) {
			Texture->Circle(ScreenSize / 2.f, Aimbot::FOV, RGB(196, 196, 196));
		}
		
		if (GetAsyncKeyState(0x71) & 0x1) { Loot::Active = !Loot::Active; }
		if (GetAsyncKeyState(0x72) & 0x1) { Container::Active = !Container::Active; }
		if (GetAsyncKeyState(0x72) & 0x1) { Container::Active = !Container::Active; }

		if (GetAsyncKeyState(0x73) & 0x1 && Loot::Price > 0) { Loot::Price < 10000 ? Loot::Price = 0 : Loot::Price -= 10000; }
		if (GetAsyncKeyState(0x74) & 0x1 && Loot::Price < 200000) { Loot::Price > 190000 ? Loot::Price = 200000 : Loot::Price += 10000; }

		if (GetAsyncKeyState(0x75) & 0x1 && Container::Price > 0) { Container::Price < 10000 ? Container::Price = 0 : Container::Price -= 10000; }
		if (GetAsyncKeyState(0x76) & 0x1 && Container::Price < 200000) { Container::Price > 190000 ? Container::Price = 200000 : Container::Price += 10000; }

		if (GUI::Begin({ FLOAT(CursorPosition.x), FLOAT(CursorPosition.y) }, ScreenSize)) {
			if (GUI::Tab(E(L"Visuals"))) {
				GUI::Checkbox(E(L"ESP"), Visuals::ESP);
				GUI::Checkbox(E(L"Health"), Visuals::Health);
				GUI::Checkbox(E(L"Nickname"), Visuals::Name);
				GUI::Checkbox(E(L"Kill/Death"), Visuals::KD);
				GUI::Checkbox(E(L"Distance"), Visuals::Distance);
				GUI::Checkbox(E(L"Weapon"), Visuals::Weapon);
				GUI::Checkbox(E(L"Skeleton"), Visuals::Skeleton);
				GUI::Checkbox(E(L"Bots"), Visuals::Bots);
				GUI::Checkbox(E(L"Grenades"), Visuals::Grenade);
				GUI::Checkbox(E(L"Loot Amount"), Visuals::LootAmount);
				GUI::Checkbox(E(L"Entity Loot"), Visuals::EntityLoot);
				GUI::Checkbox(E(L"Corpses"), Visuals::Corpse);
				GUI::Checkbox(E(L"Corpses Loot"), Visuals::CorpseLoot);
				GUI::InputNumber(E(L"Minimum Corpses Price"), Visuals::CorpsePrice);
				GUI::Slider(E(L"Visibility Distance"), Visuals::VisibilityDistance, 100.f, 3000.f);
				GUI::Checkbox(E(L"Exits"), Visuals::Exits);
				GUI::Checkbox(E(L"Ammo Counter"), Visuals::AmmoCounter);
				GUI::Checkbox(E(L"FOV"), Visuals::FOV);
				GUI::Checkbox(E(L"Crosshair"), Visuals::Crosshair);
			}
			if (GUI::Tab(E(L"Loot"))) {
				GUI::Checkbox(E(L"Active"), Loot::Active, E(L"F2"), RGB(118, 38, 198));
				GUI::Checkbox(E(L"Ammo"), Loot::Ammo);
				GUI::Checkbox(E(L"Ammo Boxes"), Loot::AmmoBoxes);
				GUI::Checkbox(E(L"Barter"), Loot::Barter);
				GUI::Checkbox(E(L"Containers"), Loot::Containers);
				GUI::Checkbox(E(L"Currency"), Loot::Currency);
				GUI::Checkbox(E(L"Epic"), Loot::Epic);
				GUI::Checkbox(E(L"Gear"), Loot::Gear);
				GUI::Checkbox(E(L"Keys"), Loot::Keys);
				GUI::Checkbox(E(L"Magazines"), Loot::Magazines);
				GUI::Checkbox(E(L"Maps"), Loot::Maps);
				GUI::Checkbox(E(L"Meds"), Loot::Meds);
				GUI::Checkbox(E(L"Provisions"), Loot::Provisions);
				GUI::Checkbox(E(L"Sights"), Loot::Sights);
				GUI::Checkbox(E(L"Special Equipment"), Loot::SpecialEquipment);
				GUI::Checkbox(E(L"Suppressors"), Loot::Suppressors);
				GUI::Checkbox(E(L"Tactical Devices"), Loot::TacticalDevices);
				GUI::Checkbox(E(L"Weapons"), Loot::Weapons);
				GUI::Checkbox(E(L"Weapon Parts"), Loot::WeaponParts);
				GUI::Checkbox(E(L"Quest"), Loot::Quest);
				GUI::Checkbox(E(L"Searched"), Loot::Searched);
				GUI::InputNumber(E(L"Minimum Loot Price"), Loot::Price, E(L"F4 / F5"), RGB(118, 38, 198));
				GUI::Slider(E(L"Visibility Distance"), Loot::Distance, 50.f, 3000.f);
			}
			if (GUI::Tab(E(L"Containers"))) {
				GUI::Checkbox(E(L"Active"), Container::Active, E(L"F3"), RGB(118, 38, 198));
				GUI::Checkbox(E(L"Ammo"), Container::Ammo);
				GUI::Checkbox(E(L"Ammo Boxes"), Container::AmmoBoxes);
				GUI::Checkbox(E(L"Barter"), Container::Barter);
				GUI::Checkbox(E(L"Containers"), Container::Containers);
				GUI::Checkbox(E(L"Currency"), Container::Currency);
				GUI::Checkbox(E(L"Epic"), Container::Epic);
				GUI::Checkbox(E(L"Gear"), Container::Gear);
				GUI::Checkbox(E(L"Keys"), Container::Keys);
				GUI::Checkbox(E(L"Magazines"), Container::Magazines);
				GUI::Checkbox(E(L"Maps"), Container::Maps);
				GUI::Checkbox(E(L"Meds"), Container::Meds);
				GUI::Checkbox(E(L"Provisions"), Container::Provisions);
				GUI::Checkbox(E(L"Sights"), Container::Sights);
				GUI::Checkbox(E(L"Special Equipment"), Container::SpecialEquipment);
				GUI::Checkbox(E(L"Suppressors"), Container::Suppressors);
				GUI::Checkbox(E(L"Tactical Devices"), Container::TacticalDevices);
				GUI::Checkbox(E(L"Weapons"), Container::Weapons);
				GUI::Checkbox(E(L"Weapon Parts"), Container::WeaponParts);
				GUI::Checkbox(E(L"Searched"), Container::Searched);
				GUI::InputNumber(E(L"Minimum Loot Price"), Container::Price, E(L"F6 / F7"), RGB(118, 38, 198));
				GUI::Slider(E(L"Loot Distance"), Container::Distance, 50.f, 3000.f);
			}
			if (GUI::Tab(E(L"Aimbot"))) {
				GUI::Checkbox(E(L"Active"), Aimbot::Active);
				GUI::Checkbox(E(L"Silent"), Aimbot::Silent);
				GUI::BindKey(E(L"Key"), Aimbot::Key);
				GUI::Select(E(L"Bone"), Aimbot::BonesLabel(Aimbot::BonesState), 3, Aimbot::BonesState);
				GUI::Slider(E(L"Smooth"), Aimbot::Smooth, 1.f, 50.f);
				GUI::Slider(E(L"FOV"), Aimbot::FOV, 5.f, 700.f);
				GUI::Slider(E(L"Aimbot Distance"), Aimbot::Distance, 10.f, 1000.f);
				GUI::Checkbox(E(L"Prediction"), Aimbot::Prediction);
				GUI::Checkbox(E(L"No Recoil & No Sway"), Aimbot::FixWeapon);
				GUI::Checkbox(E(L"No Spread"), Aimbot::NoSpread);
				GUI::Checkbox(E(L"Insta Scope"), Aimbot::InstantADS);
			}
			if (GUI::Tab(E(L"Misc"))) {
				GUI::Checkbox(E(L"Infinity Stamina"), Misc::InfinityStamina);
				GUI::Checkbox(E(L"Infinity Oxygen"), Misc::InfinityOxygen);
				GUI::Checkbox(E(L"No Visor Effect"), Misc::VisorEffect);
				GUI::Checkbox(E(L"Thermal Vision"), Misc::ThermalVision);
				GUI::Checkbox(E(L"Night Vision"), Misc::NightVision);
			}
			if (GUI::Tab(E(L"Search"))) {
				GUI::List(E(L"Maximum 30 items"), E(L"Search..."), SearchedLootCached, SearchedItemsCount);
			}
			if (GUI::Tab(E(L"Colors"))) {
				GUI::ColorPicker(E(L"ESP"), Colors::ESP);
				GUI::ColorPicker(E(L"Name"), Colors::Name);
				GUI::ColorPicker(E(L"Kill/Death"), Colors::KD);
			}
			GUI::End();
		}

		DirectX.Tick(Texture->PixelsBGRA, Texture->Width, Texture->Height);

		Texture->ReleaseTexture();
	}

Ret:

	CONTEXT_RETURN Ctx{};
	Ctx.Stack = (PVOID)Context.Rsp;
	Ctx.Function = (ULONG64)GetProcAddress(KBaseA, E("PsGetCurrentThreadPreviousMode")) + 0x10; 
	Ctx.Ret = *(CHAR*)((ULONG64)CurrentThread + 0x232);

	Ctx.R12 = Context.R12;
	Ctx.R13 = Context.R13;
	Ctx.R14 = Context.R14;
	Ctx.R15 = Context.R15;

	Ctx.RDI = Context.Rdi;
	Ctx.RSI = Context.Rsi;
	Ctx.RBX = Context.Rbx;
	Ctx.RBP = Context.Rbp;

	CalloutReturn(&Ctx);
}

PVOID MmAllocateIndependentPages(PVOID KBase, ULONG PageCount)
{
	auto MiSystemPartition = (PVOID)RVA(FindPattern(KBase, E(".text"), E("0F 85 ? ? ? ? 48 8D 05 ? ? ? ? 4C 3B D0")), 13);
	auto MiGetPage = (PVOID)RVA(FindPattern(KBase, E(".text"), E("48 8B 0C C8 E8 ? ? ? ? 48 83 F8 FF")), 9);
	auto MiRemovePhysicalMemory = (PVOID)RVA(FindPattern(KBase, E(".text"), E("E8 ? ? ? ? 48 83 3D ? ? ? ? ? 75 E9 48 8D ?")), 5);
	auto MiSystemPteInfo = (PVOID)RVA(FindPattern(KBase, E(".text"), E("4C 2B D1 48 8D 0D ? ? ? ?")), 10);
	auto MiReservePtes = (PVOID)RVA(FindPattern(KBase, E(".text"), E("48 8B 80 ? ? ? ? 48 89 45 ? E8 ? ? ? ?")), 16);

	MMPTE* PTE = CallPtr<MMPTE*>(MiReservePtes, MiSystemPteInfo, PageCount);

	if (!PTE) return nullptr;

#define PTE_SHIFT 3
#define VA_SHIFT (63 - 47)
#define MiGetVirtualAddressMappedByPte(PTE) ((PVOID)((LONG_PTR)(((LONG_PTR)(PTE) - EPtr<ULONG64>(PTEBase)) << (PAGE_SHIFT + VA_SHIFT - PTE_SHIFT)) >> VA_SHIFT))

	auto VA = MiGetVirtualAddressMappedByPte(PTE);

	for (SIZE_T i = 0; i < PageCount; i++)
	{
	NewTry:
		auto PFN = CallPtr<ULONG64>(MiGetPage, MiSystemPartition, 0ull, 8ull);

		if (PFN == -1) goto NewTry;

		ULONG64 PfnSize = 0x1000; PfnSize = PfnSize >> 12;
		CallPtr<void>(MiRemovePhysicalMemory, PFN, PfnSize);

		PTE->u.Hard.Valid = 1;
		PTE->u.Hard.Owner = 0;
		PTE->u.Hard.Write = 1;
		PTE->u.Hard.NoExecute = 0;
		PTE->u.Hard.PageFrameNumber = PFN;

		++PTE;
	}

	return VA;
}

void WaitGame(PVOID StartContext, PVOID StackPreserve) {
	PVOID KBaseA = EPtr(KBase);
	PETHREAD CurrentThread = KeGetCurrentThread();

	InterruptedThreadArray[0] = EPtr<PKTHREAD>(CurrentThread);
	PreservedStackArray[0] = EPtr(StackPreserve);

	PVOID StartThreadAddress = (PVOID)RVA(FindPattern(KBaseA, E("PAGE"), E("48 89 44 24 ? 48 8D 05 ? ? ? ? 48 89 54 24 ?")), 12);

	ULONG StartAddressOffset = *(ULONG*)(FindPattern(KBaseA, E(".text"), E("75 19 48 8B 89 ? ? ? ?")) + 5);
	ULONG Win32StartAddressOffset = *(ULONG*)(FindPattern(KBaseA, E(".text"), E("48 8B 81 ? ? ? ? 48 89 84 24 ? ? ? ? 4C 8B 99 ? ? ? ?")) + 3);

	*(PVOID*)((ULONG64)CurrentThread + StartAddressOffset) = StartThreadAddress;
	*(PVOID*)((ULONG64)CurrentThread + Win32StartAddressOffset) = StartThreadAddress;

WaitProcess:
	auto TargetProcess = GetProcessByName(E("TestEnvironment.exe"));

	if (!TargetProcess) { Sleep(100); goto WaitProcess; }

	auto CurrentProcess = KiSwapProcess(TargetProcess);

	auto DXGIBase = GetUserModuleBase(TargetProcess, E("dxgi.dll"));

	if (!DXGIBase) { Sleep(100); goto WaitProcess; }

	UserCallback.Init(KBaseA);
	PageShift.Init(KBaseA);

	auto KdpTrap = (PVOID)RVA(FindPattern(KBaseA, E(".text"), E("48 8B ? 88 44 24 20 E8 ? ? ? ? 90 E9 ? ? ? ?")), 12);
	auto PopPepTriggerComponentActiveActivity = FindPattern(KBaseA, E(".text"), E("48 85 D2 74 0E ? ? ? ? ? ? ? ? C7 00 ? ? ? ? C3"));
	auto PsGetCurrentThreadPreviousMode = (PUCHAR)GetProcAddress(KBaseA, E("PsGetCurrentThreadPreviousMode")) + 0xF;

	auto DxgBase = GetKernelAddress(E("dxgkrnl.sys")); 

	auto DxgkPresent = FindPattern(DxgBase, E("PAGE"), E("C7 84 24 ? ? ? ? ? ? ? ? 48 FF 15 ? ? ? ? 0F 1F 44 00 ? 3C 01")) + 0x12;

	if (!IsAddressValid(DxgkPresent)) {
		DxgkPresent = FindPattern(DxgBase, E("PAGE"), E("C7 84 24 ? ? ? ? ? ? ? ? 4C 8B 15 ? ? ? ? E8 ? ? ? ? 3C 01 0F 85 ? ? ? ? 44 0F B6 ?")) + 0x17;
	}

	BYTE ExceptionCode[] = { 0xCC, 0xC3 };
	BYTE ReturnCode[] = {
		0xFA,
		0x48, 0x8B, 0xE5,
		0x48, 0x8B, 0xAD, 0xD8, 0x00, 0x00, 0x00,
		0x48, 0x81, 0xC4, 0xE8, 0x00, 0x00, 0x00,
		0x48, 0xCF
	};
	BYTE KdpStub[] = {
		0x41, 0x81, 0x38, 0x03, 0x00, 0x00, 0x80, 0x0F, 0x85, 0xFB, 0x00, 0x00,
		0x00, 0x49, 0x8B, 0x81, 0x98, 0x00, 0x00, 0x00, 0x48, 0xB9, 0xA1, 0xDE,
		0xCE, 0xFA, 0xAD, 0xDE, 0xCE, 0xFA, 0x48, 0x39, 0x08, 0x0F, 0x85, 0xCD,
		0x00, 0x00, 0x00, 0x81, 0x60, 0xE0, 0xFF, 0xFD, 0xFF, 0xFF, 0x49, 0x8B,
		0x81, 0x98, 0x00, 0x00, 0x00, 0x41, 0x81, 0x61, 0x44, 0xFF, 0xFD, 0xFF,
		0xFF, 0x48, 0xA3, 0xA2, 0xDE, 0xCE, 0xFA, 0xAD, 0xDE, 0xCE, 0xFA, 0x49,
		0x8B, 0x81, 0xD8, 0x00, 0x00, 0x00, 0x48, 0xA3, 0xA3, 0xDE, 0xCE, 0xFA,
		0xAD, 0xDE, 0xCE, 0xFA, 0x49, 0x8B, 0x81, 0xE0, 0x00, 0x00, 0x00, 0x48,
		0xA3, 0xA4, 0xDE, 0xCE, 0xFA, 0xAD, 0xDE, 0xCE, 0xFA, 0x49, 0x8B, 0x81,
		0xE8, 0x00, 0x00, 0x00, 0x48, 0xA3, 0xA5, 0xDE, 0xCE, 0xFA, 0xAD, 0xDE,
		0xCE, 0xFA, 0x49, 0x8B, 0x81, 0xF0, 0x00, 0x00, 0x00, 0x48, 0xA3, 0xA6,
		0xDE, 0xCE, 0xFA, 0xAD, 0xDE, 0xCE, 0xFA, 0x49, 0x8B, 0x81, 0xB0, 0x00,
		0x00, 0x00, 0x48, 0xA3, 0xA7, 0xDE, 0xCE, 0xFA, 0xAD, 0xDE, 0xCE, 0xFA,
		0x49, 0x8B, 0x81, 0xA8, 0x00, 0x00, 0x00, 0x48, 0xA3, 0xA8, 0xDE, 0xCE,
		0xFA, 0xAD, 0xDE, 0xCE, 0xFA, 0x49, 0x8B, 0x81, 0x90, 0x00, 0x00, 0x00,
		0x48, 0xA3, 0xA9, 0xDE, 0xCE, 0xFA, 0xAD, 0xDE, 0xCE, 0xFA, 0x49, 0x8B,
		0x81, 0xA0, 0x00, 0x00, 0x00, 0x48, 0xA3, 0x10, 0xDE, 0xCE, 0xFA, 0xAD,
		0xDE, 0xCE, 0xFA, 0x48, 0xB8, 0x11, 0xDE, 0xCE, 0xFA, 0xAD, 0xDE, 0xCE,
		0xFA, 0x49, 0x89, 0x81, 0x98, 0x00, 0x00, 0x00, 0x48, 0xB8, 0x12, 0xDE,
		0xCE, 0xFA, 0xAD, 0xDE, 0xCE, 0xFA, 0x49, 0x89, 0x81, 0xF8, 0x00, 0x00,
		0x00, 0xB0, 0x01, 0xC3, 0x48, 0xB8, 0x13, 0xDE, 0xCE, 0xFA, 0xAD, 0xDE,
		0xCE, 0xFA, 0x49, 0x89, 0x81, 0xF8, 0x00, 0x00, 0x00, 0xB0, 0x01, 0xC3,
		0x32, 0xC0, 0xC3
	};
	BYTE KdpDebugRoutineCode[] = { 0x1 };
	
	size_t StackSize = 0x1000 * 16;
	auto RealStack = MmAllocateIndependentPages(KBaseA, 16);
	MemZero(RealStack, StackSize);

	*(PVOID*)&KdpStub[0x16] = DxgkPresent;

	*(ULONG64*)&KdpStub[0x3F] = (ULONG64)&Context + FIELD_OFFSET(CONTEXT, Rsp);

	*(ULONG64*)&KdpStub[0x50] = (ULONG64)&Context + FIELD_OFFSET(CONTEXT, R12);
	*(ULONG64*)&KdpStub[0x61] = (ULONG64)&Context + FIELD_OFFSET(CONTEXT, R13);
	*(ULONG64*)&KdpStub[0x72] = (ULONG64)&Context + FIELD_OFFSET(CONTEXT, R14);
	*(ULONG64*)&KdpStub[0x83] = (ULONG64)&Context + FIELD_OFFSET(CONTEXT, R15);

	*(ULONG64*)&KdpStub[0x94] = (ULONG64)&Context + FIELD_OFFSET(CONTEXT, Rdi);
	*(ULONG64*)&KdpStub[0xA5] = (ULONG64)&Context + FIELD_OFFSET(CONTEXT, Rsi);
	*(ULONG64*)&KdpStub[0xB6] = (ULONG64)&Context + FIELD_OFFSET(CONTEXT, Rbx);
	*(ULONG64*)&KdpStub[0xC7] = (ULONG64)&Context + FIELD_OFFSET(CONTEXT, Rbp);

	*(ULONG64*)&KdpStub[0xD1] = (ULONG64)RealStack + StackSize - 0x28;
	*(ULONG64*)&KdpStub[0xE2] = (ULONG64)Present;

	*(ULONG64*)&KdpStub[0xF6] = (ULONG64)PsGetCurrentThreadPreviousMode + 0x1;

	auto KdpDebugRoutineSelect = (PVOID)(RVA(FindPattern(KBaseA, E(".text"), E("83 3D ? ? ? ? ? 8A 44 24 68")), 6) + 0x1);

	RopGadgetAddressArray[1] = RopGadgetAddressArray[0];//EPtr(PopPepTriggerComponentActiveActivity);

	PageShift.Patch(KdpTrap, KdpStub, sizeof(KdpStub));
	PageShift.Patch(PsGetCurrentThreadPreviousMode, ExceptionCode, sizeof(ExceptionCode));
	PageShift.Patch(KdpDebugRoutineSelect, KdpDebugRoutineCode, sizeof(KdpDebugRoutineCode));
	PageShift.Patch(PopPepTriggerComponentActiveActivity, ReturnCode, sizeof(ReturnCode));
	PageShift.Enable();

	hp(PsGetCurrentThreadPreviousMode);

	KiSwapProcess(CurrentProcess);

	ImpCall(PsTerminateSystemThread, 0);
}

#pragma code_seg(push)
#pragma code_seg("INIT")
void InitializeThread(PVOID KBase) {
	UCHAR ThreadStartShellcode[] = { 0xFA, 0x48, 0x89, 0xE2, 0x48, 0xBC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0 };
	UCHAR* ShellcodeBase = (UCHAR*)KAlloc(sizeof(ThreadStartShellcode), NonPagedPool);
	MemCpy(ShellcodeBase, &ThreadStartShellcode[0], sizeof(ThreadStartShellcode));

	SIZE_T StackSize = 0x1000 * 16;
	auto RealStack = MmAllocateIndependentPages(KBase, 16);
	
	MemZero(RealStack, StackSize);

	*(ULONG64*)(&ShellcodeBase[6]) = (ULONG64)RealStack + StackSize - 0x28;
	*(ULONG64*)(&ShellcodeBase[0x10]) = (ULONG64)WaitGame;

	HANDLE ThreadHandle; OBJECT_ATTRIBUTES ObjectAttributes; CLIENT_ID ClientID{ };
	InitializeObjectAttributes(&ObjectAttributes, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
	ImpCall(PsCreateSystemThread, &ThreadHandle, THREAD_ALL_ACCESS, &ObjectAttributes, 0, &ClientID, (PKSTART_ROUTINE)ShellcodeBase, 0);
	while (!InterruptedThreadArray[0] && !PreservedStackArray[0]) { Sleep(100); }
	ImpCall(ZwClose, ThreadHandle);
	KFree(ShellcodeBase, sizeof(ThreadStartShellcode));
}
NTSTATUS DriverEntry(PVOID ImageBase, PVOID KBaseA)
{
	KBase = EPtr(KBaseA);

	ImpSet(ExAllocatePoolWithTag);
	ImpSet(ExFreePoolWithTag);
	ImpSet(PsCreateSystemThread);
	ImpSet(PsTerminateSystemThread);
	ImpSet(ZwQuerySystemInformation);
	ImpSet(MmIsAddressValid);
	ImpSet(PsLookupProcessByProcessId);
	ImpSet(ZwClose);
	ImpSet(KeDelayExecutionThread);
	ImpSet(IoGetCurrentProcess);
	ImpSet(memcpy);
	ImpSet(memset);
	ImpSet(ZwAllocateVirtualMemory);
	ImpSet(ZwFreeVirtualMemory);
	ImpSet(PsGetProcessPeb);
	ImpSet(ZwSetInformationVirtualMemory);
	ImpSet(memmove);
	ImpSet(MmGetVirtualForPhysical);
	ImpSet(MmAllocateContiguousMemory);
	ImpSet(MmGetPhysicalAddress);
	ImpSet(MmFreeContiguousMemory);
	ImpSet(MmRemovePhysicalMemory);
	ImpSet(ZwQueryVirtualMemory);
	ImpSet(_vsnprintf);
	ImpSet(_vsnwprintf);
	ImpSet(_wtoi);
	ImpSet(KeIpiGenericCall);
	ImpSet(KeInvalidateAllCaches);
	ImpSet(PsGetProcessImageFileName);
	ImpSet(ZwCreateFile);
	ImpSet(ZwWriteFile);
	ImpSet(KeUserModeCallback);

#ifdef DEBUG
	ImpSet(DbgPrintEx);
#endif

	RetInstruction = FindPattern(KBaseA, E(".text"), E("C3"));
	RopGadgetAddressArray[0] = EPtr(FindPattern(KBaseA, E(".text"), E("48 8B E5 48 8B AD ? ? ? ? 48 81 C4 ? ? ? ? 48 CF"), Random(2, 20)));
	
	ULONG64 PTE = (ULONG64)FindPattern(KBaseA, E(".text"), E("48 23 C8 48 B8 ? ? ? ? ? ? ? ? 48 03 C1 C3"));
	PTE = *(ULONG64*)(PTE + 5);
	ULONGLONG Mask = (1ll << (PHYSICAL_ADDRESS_BITS - 1)) - 1;
	PDEBase = EPtr((PTE & ~Mask) | ((PTE >> 9) & Mask));
	PTEBase = EPtr(PTE);

	auto Win32KBase = GetKernelAddress(E("win32kbase.sys"));
	auto Win32FBase = GetKernelAddress(E("win32kbase.sys"));

	Win32::ValidateHwnd = EPtr(GetProcAddress(Win32KBase, E("ValidateHwnd")));

	Win32::gafAsyncKeyState = EPtr(GetProcAddress(Win32KBase, E("gafAsyncKeyState")));

	Win32::gafAsyncKeyStateRecentDown = EPtr(GetProcAddress(Win32KBase, E("gafAsyncKeyStateRecentDown")));

	Win32::gpsi = EPtr(*(PVOID*)GetProcAddress(Win32KBase, E("gpsi")));

	Texture = (RenderTexture*)KAlloc(16384008);

	InitializeThread(KBaseA);

	return STATUS_SUCCESS;
}
#pragma code_seg(pop)
