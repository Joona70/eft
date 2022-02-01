Vector2 ScreenSize;

namespace Offsets
{
	ULONG CharacterController = 0x28;
	ULONG MovementContext = 0x40;
	ULONG PlayerBody = 0xA8;
	ULONG ProceduralWeaponAnimation = 0x190;
	ULONG InteractableObject = 0x490;
	ULONG Profile = 0x4B8;
	ULONG Physical = 0x4C8;
	ULONG HealthController = 0x4F0;
	ULONG HandsController = 0x508;
}

extern class GameObjectManager* GameManager;
extern class Entity* LocalPlayer;
extern class GameObject* FPSCamera;
extern class Camera* CurrentCamera;

enum EMemberCategory : int
{
	// Token: 0x04001F17 RID: 7959
	Default = 0,
	// Token: 0x04001F18 RID: 7960
	Developer = 1,
	// Token: 0x04001F19 RID: 7961
	UniqueId = 2,
	// Token: 0x04001F1A RID: 7962
	Trader = 4,
	// Token: 0x04001F1B RID: 7963
	Group = 8,
	// Token: 0x04001F1C RID: 7964
	System = 16,
	// Token: 0x04001F1D RID: 7965
	ChatModerator = 32,
	// Token: 0x04001F1E RID: 7966
	ChatModeratorWithPermanentBan = 64,
	// Token: 0x04001F1F RID: 7967
	UnitTest = 128,
	// Token: 0x04001F20 RID: 7968
	Sherpa = 256,
	// Token: 0x04001F21 RID: 7969
	Emissary = 512
};
enum Bones : int
{
	HumanBase = 0,
	HumanPelvis = 14,
	HumanLThigh1 = 15,
	HumanLThigh2 = 16,
	HumanLCalf = 17,
	HumanLFoot = 18,
	HumanLToe = 19,
	HumanRThigh1 = 20,
	HumanRThigh2 = 21,
	HumanRCalf = 22,
	HumanRFoot = 23,
	HumanRToe = 24,
	HumanSpine1 = 29,
	HumanSpine2 = 36,
	HumanSpine3 = 37,
	HumanLCollarbone = 89,
	HumanLUpperarm = 90,
	HumanLForearm1 = 91,
	HumanLForearm2 = 92,
	HumanLForearm3 = 93,
	HumanLPalm = 94,
	HumanRCollarbone = 110,
	HumanRUpperarm = 111,
	HumanRForearm1 = 112,
	HumanRForearm2 = 113,
	HumanRForearm3 = 114,
	HumanRPalm = 115,
	HumanNeck = 132,
	HumanHead = 133
};
enum LootItemLabelState : int
{
	Common,
	Uncommon,
	Rare,
	Superrare,
	Epic,
	Quest
};
enum EExfiltrationStatus : int {
	NotPresent = 1,
	UncompleteRequirements,
	Countdown,
	RegularMode,
	Pending,
	AwaitsManualActivation
};
struct TransformAccessReadOnly
{
	uint64_t Data;
	int	Index;
};
struct TransformData
{
	uint64_t Array;
	uint64_t Indices;
};
struct Matrix34
{
	Vector4 Vec0;
	Vector4 Vec1;
	Vector4 Vec2;
};

__declspec(noinline) Vector3 GetPosByTransform(uint64_t Transform)
{
	__m128 result;

	uint64_t internalTransform = Read<uint64_t>(Transform + 0x10);

	if (internalTransform) {
		const __m128 mulVec0 = { -2.f, 2.f, -2.f, 0.f };
		const __m128 mulVec1 = { 2.f, -2.f, -2.f, 0.f };
		const __m128 mulVec2 = { -2.f, -2.f, 2.f, 0.f };

		TransformAccessReadOnly transformAccessReadOnly = Read<TransformAccessReadOnly>(internalTransform + 0x38);

		if (transformAccessReadOnly.Data) {
			TransformData transformData = Read<TransformData>(transformAccessReadOnly.Data + 0x18);

			if (transformData.Array && transformData.Indices) {
				result = Read<__m128>(transformData.Array + 0x30 * transformAccessReadOnly.Index);
				int transformIndex = Read<int>(transformData.Indices + 0x4 * transformAccessReadOnly.Index);

				int counterIndex = 0;
				while (transformIndex >= 0 && counterIndex++ < 1000)
				{
					Matrix34 matrix34 = Read<Matrix34>(transformData.Array + 0x30 * transformIndex);

					__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.Vec1), 0x00));	// xxxx
					__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.Vec1), 0x55));	// yyyy
					__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.Vec1), 0x8E));	// zwxy
					__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.Vec1), 0xDB));	// wzyw
					__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.Vec1), 0xAA));	// zzzz
					__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.Vec1), 0x71));	// yxwy
					__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.Vec2), result);

					result = _mm_add_ps(
						_mm_add_ps(
							_mm_add_ps(
								_mm_mul_ps(
									_mm_sub_ps(
										_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
										_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
									_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
								_mm_mul_ps(
									_mm_sub_ps(
										_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
										_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
									_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
							_mm_add_ps(
								_mm_mul_ps(
									_mm_sub_ps(
										_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
										_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
									_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
								tmp7)), *(__m128*)(&matrix34.Vec0));

					transformIndex = Read<int>(transformData.Indices + 4 * transformIndex);
				}
			}
		}
	}

	return Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
}

class Camera {
public:
	__forceinline Matrix* ViewMatrix() {
		return (Matrix*)(this + 0xD8);
	}

	__forceinline float FovX() {
		return Read<float>(this + 0xAC);
	}

	__forceinline float FovY() {
		return Read<float>(this + 0x98);
	}

	__forceinline float MysteryX() {
		return Read<float>(this + 0x12C);
	}

	__forceinline float MysteryY() {
		return Read<float>(this + 0x118);
	}

	__forceinline float FOV() {
		return Read<float>(this + 0x158);
	}

	__forceinline float AspectRatio() {
		return Read<float>(this + 0x4C8);
	}
};

class Weapon {
public:
	__forceinline const wchar_t* ID()
	{
		uint64_t ID = Read<uint64_t>(this + 0x50);
		return (const wchar_t*)(ID ? ID + 0x14 : 0);
	}
};

class Entity
{
public:
	float Health() {
		uint64_t HealthArray = ReadChain<uint64_t>({ (uint64_t)this + Offsets::HealthController, 0x50, 0x18 });
		float Health = ReadChain<float>({ HealthArray, 0x30, 0x10, 0x10 }, false); //head
		Health += ReadChain<float>({ HealthArray, 0x48, 0x10, 0x10 }, false); //thorax
		Health += ReadChain<float>({ HealthArray, 0x60, 0x10, 0x10 }, false); //stomach
		Health += ReadChain<float>({ HealthArray, 0x78, 0x10, 0x10 }, false); //left arm
		Health += ReadChain<float>({ HealthArray, 0x90, 0x10, 0x10 }, false); //right arm
		Health += ReadChain<float>({ HealthArray, 0xA8, 0x10, 0x10 }, false); //left leg
		Health += ReadChain<float>({ HealthArray, 0xC0, 0x10, 0x10 }, false); //right leg
		return Health;
	}
	float MaxHealth() {
		uint64_t HealthArray = ReadChain<uint64_t>({ (uint64_t)this + Offsets::HealthController, 0x50, 0x18 });
		float Health = ReadChain<float>({ HealthArray, 0x30, 0x10, 0x14 }, false); //head
		Health += ReadChain<float>({ HealthArray, 0x48, 0x10, 0x14 }, false); //thorax
		Health += ReadChain<float>({ HealthArray, 0x60, 0x10, 0x14 }, false); //stomach
		Health += ReadChain<float>({ HealthArray, 0x78, 0x10, 0x14 }, false); //left arm
		Health += ReadChain<float>({ HealthArray, 0x90, 0x10, 0x14 }, false); //right arm
		Health += ReadChain<float>({ HealthArray, 0xA8, 0x10, 0x14 }, false); //left leg
		Health += ReadChain<float>({ HealthArray, 0xC0, 0x10, 0x14 }, false); //right leg
		return Health;
	}

	__declspec(noinline) Vector3 GetBoneLocation(int Bone)
	{
		uint64_t BoneTransform = ReadChain<uint64_t>({ (uint64_t)this + Offsets::PlayerBody, 0x28, 0x28, 0x10, (uint64_t)0x20 + Bone * 0x8 });
		return BoneTransform ? GetPosByTransform(BoneTransform) : Vector3();
	}

	__forceinline bool IsLocalPlayer() {
		return Read<uint64_t>(this + 0x18) != 0;
	}

	__forceinline bool IsPlayer() {
		return (ReadChain<uint64_t>({ (uint64_t)this + Offsets::Profile, 0x28, 0x5C }) != 0);
	}

	__forceinline const wchar_t* GroupID() {
		uint64_t NamePtr = ReadChain<uint64_t>({ (uint64_t)this + Offsets::Profile, 0x28, 0x20 });
		return (const wchar_t*)(NamePtr ? NamePtr + 0x14 : 0);
	}

	__forceinline const wchar_t* Name() {
		uint64_t NamePtr = ReadChain<uint64_t>({ (uint64_t)this + Offsets::Profile, 0x28, 0x18 });
		return (const wchar_t*)(NamePtr ? NamePtr + 0x14 : 0);
	}

	__forceinline int MemberCategory() {
		return ReadChain<int>({ (uint64_t)this + Offsets::Profile, 0x28, 0x70 });
	}

	__forceinline const wchar_t* GetBotName(bool& IsBoss) {
		ULONG Role = ReadChain<ULONG>({ (uint64_t)this + Offsets::Profile, 0x28, 0x40, 0x10 });

		switch (Role) {
		case 8: { IsBoss = true; return E(L"BOT: BULLY"); }
		case 32: return E(L"BOT: F. BULLY");
		case 64: { IsBoss = true; return E(L"BOT: KILLA"); }
		case 128: { IsBoss = true;  return E(L"BOT: KOJANIY"); }
		case 256: return E(L"BOT: F. KOJANIY");
		case 2048: { IsBoss = true; return E(L"BOT: GLUHAR"); }
		case 4096: return E(L"BOT: F. GLUHAR");
		case 8192: return E(L"BOT: F. GLUHAR");
		case 16384: return E(L"BOT: F. GLUHAR");
		case 32768: return E(L"BOT: F. GLUHAR");
		case 65536: return E(L"BOT: F. SANITAR");
		case 131072: { IsBoss = true; return E(L"BOT: SANITAR"); }
		case 1048576: return E(L"BOT: S. WARRIOR");
		case 2097152: return E(L"BOT: S. PRIEST");
		case 4194304: { IsBoss = true; return E(L"BOT: TAGILLA"); }
		case 8388608: return E(L"BOT: F. TAGILLA");
		default: return E(L"BOT");
		}
	}

	__forceinline Vector3 GetVelocity() {
		return ReadChain<Vector3>({ (uint64_t)this, Offsets::CharacterController, 0x48 }, false);
	}

	__forceinline Weapon* GetWeaponTemplate() {
		return ReadChain<Weapon*>({ (uint64_t)this + Offsets::HandsController, 0x58, 0x38 });
	}

	__forceinline Weapon* GetCurrentWeapon() {
		return ReadChain<Weapon*>({ (uint64_t)this + Offsets::ProceduralWeaponAnimation, 0x80, 0x58, 0x90, 0x20 });
	}

	__forceinline float GetBulletSpeed() {
		auto ActiveWeapon = ReadChain<uint64_t>({ (uint64_t)this, Offsets::HandsController, 0x58 }, false);

		if (IsAddressValid(ActiveWeapon)) {
			// EFT.Player->HandsController (as FirearmController)->Item (as Weapon)->Chambers (Slot[])->Slot[0] (Slot)->ContainedItem (as GClassXXXX)->ItemTemplate (as AmmoTemplate)->InitialSpeed (float)
			auto AmmoTemplate = ReadChain<uint64_t>({ ActiveWeapon, 0x90, 0x20, 0x38, 0x38 }, false);

			if (!IsAddressValid(AmmoTemplate)) {
				//EFT.Player->HandsController(as FirearmController)->Item(as Weapon)->ItemTemplate(as WeaponTemplate)->_defAmmoTemplate(AmmoTemplate)->InitialSpeed(float)
				auto AmmoTemplate = ReadChain<uint64_t>({ ActiveWeapon, 0x38, 0x160 }, false);
			}

			return IsAddressValid(AmmoTemplate) ? Read<float>(AmmoTemplate + 0x17C) : 0.f; // InitialSpeed
		}

		return 0.f;
	}

	__forceinline Vector3 Fireport() {
		auto Transform = ReadChain<uint64_t>({ (uint64_t)this + Offsets::PlayerBody, 0x20, 0x140, 0x10 });
		return Transform ? GetPosByTransform(Transform) : Vector3();
	}

	__forceinline PVOID FireportNative() {
		return ReadChain<PVOID>({ (uint64_t)this + Offsets::PlayerBody, 0x20, 0x140, 0x10, 0x10 });
	}

	__forceinline Vector2 GetViewAngles() {
		return ReadChain<Vector2>({ (uint64_t)this, Offsets::MovementContext, 0x20C }, false);
	}

	__forceinline void SetViewAngles(Vector2 ViewAngles) {
		WriteChain<Vector2>({ (uint64_t)this, Offsets::MovementContext, 0x20C }, ViewAngles, false);
	}

	__forceinline void SetShotDirection(Vector3 Direction) {
		WriteChain<Vector3>({ (uint64_t)this, Offsets::ProceduralWeaponAnimation, 0x1DC }, Direction, false);
	}

	__forceinline void FixWeapon() {
		uint64_t ProceduralWeaponAnimation = Read<uint64_t>(this + Offsets::ProceduralWeaponAnimation);

		if (ProceduralWeaponAnimation) {
			uint64_t Breath = Read<uint64_t>(ProceduralWeaponAnimation + 0x28);
			uint64_t Walk = Read<uint64_t>(ProceduralWeaponAnimation + 0x30);

			if (IsAddressValid(Breath)) { *(float*)(Breath + 0xA4) *= 0.01f; }
			if (IsAddressValid(Walk)) { *(float*)(Walk + 0x44) *= 0.01f; }

			uint64_t MotionReac = Read<uint64_t>(ProceduralWeaponAnimation + 0x38 /*MotionReact*/);

			if (IsAddressValid(MotionReac)) {
				*(Vector3*)(MotionReac + 0xA4) *= 0.01f; /*SwayFactors*/
				*(float*)(MotionReac + 0xD0) *= 0.01f; /*Intensity*/
			}

			uint64_t ForceRect = Read<uint64_t>(ProceduralWeaponAnimation + 0x40);

			if (IsAddressValid(ForceRect)) { *(float*)(ForceRect + 0x30) *= 0.01f; } /*Intensity*/

			uint64_t Shooting = Read<uint64_t>(ProceduralWeaponAnimation + 0x48 /*Shootingg*/);

			if (IsAddressValid(Shooting)) {
				*(Vector2*)(Shooting + 0x38) *= 0.01f; /*RecoilStrengthXy*/
				*(Vector2*)(Shooting + 0x40) *= 0.01f; /*RecoilStrengthZ*/
			}

			Write<bool>(ProceduralWeaponAnimation + 0x177, 0);
		}
	}

	__forceinline void InstantADS() {
		uint64_t ProceduralWeaponAnimation = Read<uint64_t>(this + Offsets::ProceduralWeaponAnimation);
		if (ProceduralWeaponAnimation) {
			Write<float>(ProceduralWeaponAnimation + 0x194, 10.f);
		}
	}

	__forceinline void NoSpread() {
		auto FirearmController = ReadChain<uint64_t>({ (uint64_t)this, Offsets::ProceduralWeaponAnimation, 0x80 }, false);

		if (IsAddressValid(FirearmController)) {
			*(float*)(FirearmController + 0x15C) *= 0.01f;
			*(float*)(FirearmController + 0x164) *= 0.01f;
		}
	}

	__forceinline void InstaBullet() {
		auto FirearmController = ReadChain<uint64_t>({ (uint64_t)this, Offsets::ProceduralWeaponAnimation, 0x80 }, false);

		if (IsAddressValid(FirearmController)) {
			float SpeedFactor = Read<float>(FirearmController + 0x16C);

			if (SpeedFactor < 0.f || SpeedFactor >= 5.f)
				return;

			Write<float>(FirearmController + 0x16C, 5.f);
		}
	}

	__forceinline bool IsAiming() {
		return ReadChain<bool>({ (uint64_t)this + Offsets::ProceduralWeaponAnimation, 0x28, 0xA0 });
	}

	__forceinline void InfinityStamina() {
		uint64_t Physical = Read<uint64_t>(this + Offsets::Physical);

		if (Physical) {
			float Stamina = ReadChain<float>({ Physical, 0x28, 0x48 }, false);
			float HandsStamina = ReadChain<float>({ Physical, 0x30, 0x48 }, false);

			if (Stamina <= 80.f) {
				WriteChain<float>({ Physical, 0x28, 0x48 }, 80.f, false);
				WriteChain<int>({ Physical, 0x28, 0x58 }, 80, false);
			}

			if (HandsStamina <= 80.f) {
				WriteChain<float>({ Physical, 0x30, 0x48 }, 80.f, false);
				WriteChain<int>({ Physical, 0x30, 0x58 }, 80, false);
			}
		}
	}

	__forceinline void InfinityOxygen() {
		uint64_t Physical = Read<uint64_t>(this + Offsets::Physical);
		if (Physical) {
			float Oxygen = ReadChain<float>({ Physical, 0x38, 0x48 }, false);

			if (Oxygen <= 80.f) {
				WriteChain<float>({ Physical, 0x38, 0x48 }, 80.f, false);
				WriteChain<int>({ Physical, 0x38, 0x58 }, 80, false);
			}
		}
	}

	__forceinline void SpeedHack(float Value) {
		uint64_t Animators = Read<uint64_t>(this + 0x288);
		if (Animators) {
			uint64_t AnimatorArray = Read<uint64_t>(Animators + 0x20);
			if (AnimatorArray) {
				uint64_t Animator = Read<uint64_t>(AnimatorArray + 0x10);
				if (Animator) {
					Animator = Read<uint64_t>(Animator + 0x10);
					if (Animator) {
						Write<float>(Animator + 0x36C, Value);
					}
				}
			}
		}
	}
};

class EntityList {
public:
	__forceinline int GetEntitiesCount() {
		return Read<int>(this + 0x18);
	}
	__forceinline class Entity* GetEntity(int ID) {
		auto PlayerList = Read<uint64_t>(this + 0x10);
		return PlayerList ? Read<Entity*>(PlayerList + 0x20 + (ID * 8)) : nullptr;
	}
};

class Item {
public:
	__forceinline const wchar_t* ID()
	{
		uint64_t Template = Read<uint64_t>(this + 0x50);
		return (const wchar_t*)(Template ? Template + 0x14 : 0);
	}
	__forceinline int Rarity() {
		return Read<int>(this + 0xC4);
	}
};

class ItemProfile {
public:
	__forceinline class Item* GetItem() {
		return ReadChain<Item*>({ (uint64_t)this + 0x28, 0x50, 0x38 });
	}
	__forceinline uint64_t GetInteractiveClass() {
		return Read<uint64_t>(this + 0x28);
	}
	__forceinline Vector3 GetPosition() {
		uint64_t TransformArray = ReadChain<uint64_t>({ (uint64_t)this + 0x30, 0x30, 0x8, 0x28 });
		return TransformArray ? GetPosByTransform(TransformArray) : Vector3();
	}
	__forceinline bool IsLootable() {
		const char* Name = ReadChain<const char*>({ (uint64_t)this, 0x30, 0x60 }, false);

		if (IsAddressValid(Name)) {
			if (StrStr(Name, E("cap"))) return true;
			else if (StrStr(Name, E("Ammo_crate_Cap"))) return true;
			else if (StrStr(Name, E("Grenade_box_Door")))	return true;
			else if (StrStr(Name, E("Medical_Door"))) return true;
			else if (StrStr(Name, E("POS_Money"))) return true;
			else if (StrStr(Name, E("Toolbox_Door"))) return true;
			else if (StrStr(Name, E("card_file_box")))	return true;
			else if (StrStr(Name, E("cover_"))) return true;
			else if (StrStr(Name, E("lootable"))) return true;
			else if (StrStr(Name, E("scontainer_Blue_Barrel_Base_Cap"))) return true;
			else if (StrStr(Name, E("scontainer_wood_CAP"))) return true;
			else if (StrStr(Name, E("suitcase_plastic_lootable_open"))) return true;
			else if (StrStr(Name, E("weapon_box_cover"))) return true;
			else if (StrStr(Name, E("item_static_container_moneyBag"))) return true;
			else if (StrStr(Name, E("boor_safe"))) return true;
		}
		return false;
	}
	__forceinline bool IsCorpse() {
		uint64_t Interactive = Read<uint64_t>(this + 0x28);
		if (Interactive) {
			int PlayerSide = Read<int>(Interactive + 0x140);
			return PlayerSide == 1 || PlayerSide == 2 || PlayerSide == 4;
		}
		return false;
	}
};

class ItemList {
public:
	__forceinline uint64_t GetItemBase(int ID) {
		uint64_t ItemsList = Read<uint64_t>(this + 0x10);
		return ItemsList ? Read<uint64_t>(ItemsList + 0x20 + (ID * 8)) : 0;
	}
	__forceinline int GetItemCount() {
		return Read<int>(this + 0x18);
	}
};

class Exit
{
public:
	const wchar_t* Name() {
		uint64_t NamePtr = ReadChain<uint64_t>({ (uint64_t)this, 0x58, 0x10 }, false);
		return (const wchar_t*)(NamePtr ? NamePtr + 0x14 : 0);
	}
	int Status() {
		return Read<int>(this + 0xA8);
	}
	Vector3 GetPosition() {
		uint64_t TransformArray = ReadChain<uint64_t>({ (uint64_t)this + 0x10, 0x30, 0x30, 0x8, 0x28 });
		return TransformArray ? GetPosByTransform(TransformArray) : Vector3();
	}
};

class ExistsList
{
public:
	__forceinline int GetExistsCount() {
		return  ReadChain<int>({ (uint64_t)this, 0x20, 0x18 }, false);
	}
	__forceinline class Exit* GetExit(int Index) {
		uint64_t ExistsList = Read<uint64_t>(this + 0x20);
		return ExistsList ? Read<Exit*>(ExistsList + 0x20 + (Index * 8)) : nullptr;
	}
};

class Grenade
{
public:
	Vector3 GetPosition() {
		uint64_t TransformArray = ReadChain<uint64_t>({ (uint64_t)this + 0x10, 0x30, 0x30, 0x8, 0x28 });
		return TransformArray ? GetPosByTransform(TransformArray) : Vector3();
	}
};

class GrenadesList
{
public:
	__forceinline int GetGrenadesCount() {
		return  ReadChain<int>({ (uint64_t)this, 0x18, 0x18 }, false);
	}
	__forceinline class Grenade* GetGrenade(int Index) {
		uint64_t GrenadesList = ReadChain<uint64_t>({ (uint64_t)this, 0x18, 0x10 }, false);
		return GrenadesList ? Read<Grenade*>(GrenadesList + 0x20 + (Index * 8)) : nullptr;
	}
};

class LocalWorld
{
public:
	__forceinline ExistsList* GetExistsList() {
		return Read<ExistsList*>(this + 0x18);
	}
	__forceinline ItemList* GetItemList() {
		return Read<ItemList*>(this + 0x60);
	}
	__forceinline EntityList* GetEntityList() {
		return Read<EntityList*>(this + 0x80);
	}
	__forceinline GrenadesList* GetGrenadesList() {
		return Read<GrenadesList*>(this + 0xD8);
	}
};

class GameObject
{
public:
	__forceinline uint64_t Entity() {
		return ReadChain<uint64_t>({ (uint64_t)this, 0x30, 0x18 }, false);
	}

	__forceinline const char* Name() {
		uint64_t NamePtr = Read<uint64_t>(this + 0x60);
		return (const char*)(NamePtr ? NamePtr : 0);
	}

	__forceinline uint16_t Tag() {
		return Read<uint16_t>(this + 0x54);
	}
};

class Objects
{
public:
	__forceinline Objects* NextObject() {
		return Read<Objects*>(this + 0x8);
	}
	__forceinline GameObject* Object() {
		return Read<GameObject*>(this + 0x10);
	}
};

class GameObjectManager
{
public:
	__forceinline Objects* LastTaggedObject() {
		return (Objects*)this;
	}
	__forceinline Objects* FirstTaggedObject() {
		return Read<Objects*>(this + 0x8);
	}
	__forceinline Objects* LastActiveObject() {
		return Read<Objects*>(this + 0x10);
	}
	__forceinline Objects* FirstActiveObject() {
		return Read<Objects*>(this + 0x18);
	}
};

GameObject* GetObjectByTag(int Tag) {
	Objects* CurrentObject = GameManager->FirstTaggedObject();
	while (CurrentObject && (CurrentObject != GameManager->LastTaggedObject()))
	{
		if (CurrentObject == nullptr)
			continue;

		GameObject* Object = CurrentObject->Object();

		if (Object == nullptr)
			continue;

		if (Object->Tag() == Tag) {
			return Object;
		}

		CurrentObject = CurrentObject->NextObject();
	}

	return nullptr;
}
GameObject* GetObjectByName(const char* Name) {
	Objects* CurrentObject = GameManager->FirstActiveObject();
	while (CurrentObject && (CurrentObject != GameManager->LastActiveObject()))
	{
		if (CurrentObject == nullptr)
			continue;

		GameObject* Object = CurrentObject->Object();

		if (Object == nullptr)
			continue;

		const char* ObjectName = Object->Name();

		if (IsAddressValid(ObjectName) && StrICmp(Name, ObjectName, true)) {
			return Object;
		}

		CurrentObject = CurrentObject->NextObject();
	}

	return nullptr;
}

bool World2Screen(const Vector3& EntityPos, Vector2& ScreenPos)
{
	if (!IsAddressValid(CurrentCamera) || !IsAddressValid(LocalPlayer))
		return false;

	Matrix* CurrentMatrix = CurrentCamera->ViewMatrix();

	if (!IsAddressValid(CurrentMatrix))
		return false;

	Vector3 TransVec = Vector3(CurrentMatrix->_14, CurrentMatrix->_24, CurrentMatrix->_34);
	Vector3 RightVec = Vector3(CurrentMatrix->_11, CurrentMatrix->_21, CurrentMatrix->_31);
	Vector3 UpVec = Vector3(CurrentMatrix->_12, CurrentMatrix->_22, CurrentMatrix->_32);
	float w = Math::Dot(TransVec, EntityPos) + CurrentMatrix->_44;

	if (w < 0.098f) return false;

	float y = Math::Dot(UpVec, EntityPos) + CurrentMatrix->_42;
	float x = Math::Dot(RightVec, EntityPos) + CurrentMatrix->_41;

	ScreenPos = Vector2((ScreenSize.x / 2) * (1.f + x / w), (ScreenSize.y / 2) * (1.f - y / w));
	return true;
}