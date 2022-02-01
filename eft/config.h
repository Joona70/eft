namespace Aimbot
{
	bool Active = true;
	bool Silent = false;
	float Distance = 200.f;
	float FOV = 100.f;
	float Smooth = 8.f;
	int Key = 0x1;

	int BonesState = 0;
	int Bones[] = { 0, 133, 14 };

	bool Prediction = false;
	bool FixWeapon = false;
	bool NoSpread = false;
	bool InstaBullet = false;
	bool InstantADS = false;

	__forceinline const wchar_t* BonesLabel(int State) {
		switch (State)
		{
		case 0: return E(L"Nearest");
		case 1: return E(L"Head");
		case 2: return E(L"Pelvis");
		default: return nullptr;
		}
	}
}

namespace Visuals
{
	bool ESP = true;
	bool Health = true;
	bool Name = true;
	bool Distance = true;
	bool Weapon = true;
	bool Skeleton = true;
	bool Bots = true;
	bool LootAmount = true;
	bool EntityLoot = true;
	bool KD = true;
	bool Corpse = true;
	bool CorpseLoot = true;
	int CorpsePrice = 0;
	bool Exits = true;
	bool Grenade = true;
	bool AmmoCounter = true;
	bool FOV = true;
	bool Crosshair = true;
	float VisibilityDistance = 500.f;
}

namespace Loot {
	bool Active = true;
	bool Ammo = true;
	bool AmmoBoxes = true;
	bool Barter = true;
	bool Containers = true;
	bool Currency = true;
	bool Epic = true;
	bool Gear = true;
	bool Keys = true;
	bool Magazines = true;
	bool Maps = true;
	bool Meds = true;
	bool Provisions = true;
	bool Sights = true;
	bool SpecialEquipment = true;
	bool Suppressors = true;
	bool TacticalDevices = true;
	bool Weapons = true;
	bool WeaponParts = true;
	bool Quest = true;
	bool Searched = true;
	float Distance = 350.f;
	int Price = 0;
}

namespace Container {
	bool Active = true;
	bool Ammo = true;
	bool AmmoBoxes = true;
	bool Barter = true;
	bool Containers = true;
	bool Currency = true;
	bool Epic = true;
	bool Gear = true;
	bool Keys = true;
	bool Magazines = true;
	bool Maps = true;
	bool Meds = true;
	bool Provisions = true;
	bool Sights = true;
	bool SpecialEquipment = true;
	bool Suppressors = true;
	bool TacticalDevices = true;
	bool Weapons = true;
	bool WeaponParts = true;
	bool Searched = true;
	float Distance = 350.f;
	int Price = 0;
}

namespace Misc
{
	bool InfinityStamina = false;
	bool InfinityOxygen = false;
	bool VisorEffect = false;
	bool ThermalVision = false;
	bool NightVision = false;
	bool Fly = false;
	bool Speedhack = false;
	bool MultiSearch = false;
	bool LootThroughWalls = false;
}

namespace Colors {
	ULONG ESP = 0xFFFFFF;
	ULONG Name = 0xFFFFFF;
	ULONG KD = 0xFFFFFF;
}