#pragma once
#include <cstdint>
#include "common/Data.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
//菜鸟基地QQ群：964504044
namespace Offset
{
	// 设备句柄:328056
// 进程ID:12144
// 模块地址：7FF713EA0000
// =====================================
// PUBG 一键解密（个人专用版)
// 最全特征码解密，QQ:1342674682
// 菜鸟基地QQ群：964504044
// JD一键机器码：https://pay.611qk.com//links/CF7E5E4F
// 版权所有，请尊重他人劳动成果
// 抄袭盗版者全家进火葬场
// =====================================
// 版本号 = 33.2.2.1
	constexpr uint64_t offset_SpoolBack = 0x208CD05;
	constexpr uint64_t XenuineDecrypt = 0xE75EC28;
	constexpr uint64_t UWorld = 0x10272F38;
	constexpr uint64_t GNames = 0x104D60E0;
	constexpr uint64_t GNamesPtr = 0x8;
	constexpr uint64_t ChunkSize = 0x3E38;
	constexpr uint64_t GObjects = 0x10231108;
	constexpr uint64_t PlayerController = 0x30;
	constexpr uint64_t LocalPlayer = 0xF0;
	constexpr uint64_t GameInstance = 0x160;
	constexpr uint64_t GameState = 0x110;
	constexpr uint64_t CurrentLevel = 0x150;
	constexpr uint64_t Actors = 0x68;
	constexpr uint64_t ActorsForGC = 0x1F0;
	constexpr uint64_t PlayerState = 0x438;
	constexpr uint64_t AcknowledgedPawn = 0x4C0;
	constexpr uint64_t PlayerCameraManager = 0x4E8;

	constexpr uint64_t ObjID = 0x18;
	constexpr uint64_t DecryptNameIndexRor = 0x0;
	constexpr uint64_t DecryptNameIndexRval = 0x7;
	constexpr uint64_t DecryptNameIndexSval = 0x10;
	constexpr uint64_t DecryptNameIndexXorKey1 = 0xA8638239;
	constexpr uint64_t DecryptNameIndexXorKey2 = 0xF97C2D6B;

	constexpr uint64_t ViewTarget = 0xA00;
	constexpr uint64_t CameraCacheLocation = 0x458;
	constexpr uint64_t CameraCacheRotation = 0x448;
	constexpr uint64_t CameraCacheFOV = 0x468;
	constexpr uint64_t LastTeamNum = 0x1B08;
	constexpr uint64_t TeamNumber = 0x878;


	constexpr uint64_t SpectatedCount = 0x2B70;
	constexpr uint64_t Position = 0x220;
	constexpr uint64_t CharacterState = 0x1B78;
	constexpr uint64_t CharacterName = 0x1408;
	constexpr uint64_t PlayerStatistics = 0x6A0;
	constexpr uint64_t TimeSeconds = 0x9A4;
	constexpr uint64_t TimeTillExplosion = 0x7DC;
	constexpr uint64_t DamageDealtOnEnemy = 0x8B4;
	constexpr uint64_t bAlwaysCreatePhysicsState = 0x488;


	constexpr uint64_t Eyes = 0x74C;


	constexpr uint64_t StaticMesh = 0xAC8;
	constexpr uint64_t ComponentToWorld = 0x210;
	constexpr uint64_t Mesh = 0x580;
	constexpr uint64_t FiringAttachPoint = 0x890;
	constexpr uint64_t ScopingAttachPoint = 0xCB8;


	constexpr uint64_t PartnerLevel = 0x88A;
	constexpr uint64_t SurvivalLevel = 0xBBC;
	constexpr uint64_t SurvivalTier = 0xBB8;
	constexpr uint64_t PubgIdData = 0xB98;


	constexpr uint64_t SafetyZonePosition = 0x548;
	constexpr uint64_t SafetyZoneRadius = 0x51C;
	constexpr uint64_t BlueZonePosition = 0x520;
	constexpr uint64_t BlueZoneRadius = 0x678;
	constexpr uint64_t NumAliveTeams = 0x66C;
	constexpr uint64_t HeaFlag = 0x3D0;
	constexpr uint64_t Health1 = 0x988;
	constexpr uint64_t Health2 = 0x9B8;
	constexpr uint64_t Health3 = 0x984;
	constexpr uint64_t Health4 = 0x970;
	constexpr uint64_t Health5 = 0x985;
	constexpr uint64_t Health6 = 0x980;
	constexpr uint64_t Health_keys0 = 0xCEC7A59E;
	constexpr uint64_t Health_keys1 = 0x9B63B256;
	constexpr uint64_t Health_keys2 = 0xCA0332BD;
	constexpr uint64_t Health_keys3 = 0x5638489B;
	constexpr uint64_t Health_keys4 = 0x1C911D0A;
	constexpr uint64_t Health_keys5 = 0x23DDAC74;
	constexpr uint64_t Health_keys6 = 0x9459BC8;
	constexpr uint64_t Health_keys7 = 0xBD39B721;
	constexpr uint64_t Health_keys8 = 0xBAC7A58;
	constexpr uint64_t Health_keys9 = 0xA8EF5687;
	constexpr uint64_t Health_keys10 = 0xE27543B7;
	constexpr uint64_t Health_keys11 = 0x9F8ADB00;
	constexpr uint64_t Health_keys12 = 0xBD1C93D5;
	constexpr uint64_t Health_keys13 = 0x56939B07;
	constexpr uint64_t Health_keys14 = 0x9B099E38;
	constexpr uint64_t Health_keys15 = 0xE3D62A8C;


	constexpr uint64_t bIsReloading_CP = 0x810;
	constexpr uint64_t bIsScoping_CP = 0xC16;
	constexpr uint64_t LeanLeftAlpha_CP = 0xCF4;
	constexpr uint64_t LeanRightAlpha_CP = 0xCF8;
	constexpr uint64_t CurrentAmmoData = 0xAC8;


	constexpr uint64_t DroppedItem = 0x470;
	constexpr uint64_t DroppedItemGroup = 0x308;
	constexpr uint64_t DroppedItemGroupCount = 0x310;
	constexpr uint64_t DroppedItemGroupUItem = 0x850;
	constexpr uint64_t ItemPackageItems = 0x5A8;
	constexpr uint64_t ItemTable = 0xA8;
	constexpr uint64_t ItemID = 0x268;
	constexpr uint64_t AimOffsets = 0x18B8;
	constexpr uint64_t WorldToMap = 0x9B4;


	constexpr uint64_t RootComponent = 0x3C0;
	constexpr uint64_t ComponentLocation = 0x220;
	constexpr uint64_t InputYawScale = 0x664;
	constexpr uint64_t bShowMouseCursor = 0x670;


	constexpr uint64_t WeaponProcessor = 0x990;
	constexpr uint64_t CurrentWeaponIndex = 0x301;
	constexpr uint64_t EquippedWeapons = 0x1F8;
	constexpr uint64_t WeaponTrajectoryData = 0x10D8;
	constexpr uint64_t WeaponAttachmentData = 0x138;
	constexpr uint64_t TrajectoryConfig = 0x100;
	constexpr uint64_t TrajectoryGravityZ = 0x106C;
	constexpr uint64_t BallisticCurve = 0x28;
	constexpr uint64_t FloatCurves = 0x30;
	constexpr uint64_t LastUpdateVelocity = 0x3D0;
	constexpr uint64_t CharacterMovement = 0x598;


	constexpr uint64_t VehicleMovement = 0x488;
	constexpr uint64_t VehicleRiderComponent = 0x2070;
	constexpr uint64_t LastVehiclePawn = 0x260;
	constexpr uint64_t ReplicatedMovement = 0x70;
	constexpr uint64_t SeatIndex = 0x220;


	constexpr uint64_t Wheels = 0x318;
	constexpr uint64_t WheelLocation = 0xF8;
	constexpr uint64_t DampingRate = 0x4C;
	constexpr uint64_t ShapeRadius = 0x40;


	constexpr uint64_t AnimScriptInstance = 0xE10;
	constexpr uint64_t PreEvalPawnState = 0x6E8;
	constexpr uint64_t CharacterClanInfo = 0x648;
	constexpr uint64_t AttachedItems = 0x838;
	constexpr uint64_t ExplodeState = 0x5F0;
	constexpr uint64_t ControlRotation_CP = 0xCA0;
	constexpr uint64_t RecoilADSRotation_CP = 0x91C;
	constexpr uint64_t AntiCheatCharacterSyncManager = 0xFA0;
	constexpr uint64_t AttachedStaticComponentMap = 0x1370;
	constexpr uint64_t WeaponConfig_WeaponClass = 0x7A1;
	constexpr uint64_t ElapsedCookingTime = 0xBAC;
	constexpr uint64_t PlayerInput = 0x560;
	constexpr uint64_t InputAxisProperties = 0x130;
	constexpr uint64_t StaticSockets = 0xC0;
	constexpr uint64_t StaticSocketName = 0x28;
	constexpr uint64_t StaticRelativeScale = 0x48;
	constexpr uint64_t StaticRelativeLocation = 0x30;
	constexpr uint64_t StaticRelativeRotation = 0x3C;


	constexpr uint64_t Slot = 0x30;
	constexpr uint64_t MyHUD = 0x4E0;
	constexpr uint64_t Mesh3P = 0x7E0;
	constexpr uint64_t LayoutData = 0x38;
	constexpr uint64_t Alignment = 0x20;
	constexpr uint64_t Keys = 0x60;
	constexpr uint64_t Offsets = 0x0;
	constexpr uint64_t Visibility = 0xA1;
	constexpr uint64_t WidgetStateMap = 0x560;
	constexpr uint64_t TrainingMapGrid = 0x5E8;
	constexpr uint64_t MapGrid = 0x498;
	constexpr uint64_t BlockInputWidgetList = 0x5D8;
	constexpr uint64_t SelectMinimapSizeIndex = 0x5D0;


	constexpr uint64_t Gender = 0xAC0;
	constexpr uint64_t Health = 0x970;
	constexpr uint64_t bEncryptedHealth = 0x985;
	constexpr uint64_t EncryptedHealthOffset = 0x984;
	constexpr uint64_t DecryptedHealthOffset = 0x980;
	constexpr uint64_t PlayerStatusType = 0x458;
	constexpr uint64_t GroggyHealth = 0x1CA0;
	constexpr uint64_t AccountId = 0x8B8;
	constexpr uint64_t SquadMemberIndex = 0x8A4;
	constexpr uint64_t PlayerName = 0x430;
	constexpr uint64_t PlayerArray = 0x428;
	constexpr uint64_t RecentlyRendered = 0xBB0;


	constexpr uint64_t MouseX = 0x41E2;
	constexpr uint64_t MouseY = 0x41E3;


	constexpr uint64_t SPOOFCALL = 0x208CD05;
	constexpr uint64_t LineTraceSingle = 0xCBF60DC;
	constexpr uint64_t HOOK = 0xFBDC648;
	constexpr uint64_t HOOK_TWO = 0x244693;


	constexpr uint64_t InventoryFacade = 0x1358;
	constexpr uint64_t Inventory = 0x418;
	constexpr uint64_t InventoryItems = 0x550;
	constexpr uint64_t InventoryItemCount = 0x558;
	constexpr uint64_t InventoryItemTagItemCount = 0x38;


	constexpr uint64_t VehicleCommonComponent = 0xC30;
	constexpr uint64_t FloatingVehicleCommonComponent = 0x4F8;
	constexpr uint64_t VehicleFuel = 0x2D0;
	constexpr uint64_t VehicleFuelMax = 0x2D4;
	constexpr uint64_t VehicleHealth = 0x2C8;
	constexpr uint64_t VehicleHealthMax = 0x2CC;
	constexpr uint64_t NvOptimusEnablement = 0xFC1F688;




	inline void Init()
	{

		/*GameData.Offset["DecryptedHealthOffsets"] = DecryptedHealthOffsets;
		for (size_t i = 0; i < 16; i++)
		{
			GameData.Offset["DecryptedHealthOffsets" + std::to_string(i)] = DecryptedHealthOffsets[i];
		}*/
		
			GameData.Offset["GNamesPtr"] = GNamesPtr;
		GameData.Offset["Health"] = Health;
		GameData.Offset["HeaFlag"] = HeaFlag;
		GameData.Offset["Health1"] = Health1;
		GameData.Offset["Health2"] = Health2;
		GameData.Offset["Health3"] = Health3;
		GameData.Offset["Health4"] = Health4;
		GameData.Offset["Health5"] = Health5;
		GameData.Offset["Health6"] = Health6;
		GameData.Offset["DecryptedHealthOffsets0"] = Health_keys0;
		GameData.Offset["DecryptedHealthOffsets1"] = Health_keys1;
		GameData.Offset["DecryptedHealthOffsets2"] = Health_keys2;
		GameData.Offset["DecryptedHealthOffsets3"] = Health_keys3;
		GameData.Offset["DecryptedHealthOffsets4"] = Health_keys4;
		GameData.Offset["DecryptedHealthOffsets5"] = Health_keys5;
		GameData.Offset["DecryptedHealthOffsets6"] = Health_keys6;
		GameData.Offset["DecryptedHealthOffsets7"] = Health_keys7;
		GameData.Offset["DecryptedHealthOffsets8"] = Health_keys8;
		GameData.Offset["DecryptedHealthOffsets9"] = Health_keys9;
		GameData.Offset["DecryptedHealthOffsets10"] = Health_keys10;
		GameData.Offset["DecryptedHealthOffsets11"] = Health_keys11;
		GameData.Offset["DecryptedHealthOffsets12"] = Health_keys12;
		GameData.Offset["DecryptedHealthOffsets13"] = Health_keys13;
		GameData.Offset["DecryptedHealthOffsets14"] = Health_keys14;
		GameData.Offset["DecryptedHealthOffsets15"] = Health_keys15;
		GameData.Offset["bEncryptedHealth"] = bEncryptedHealth;
		GameData.Offset["EncryptedHealthOffset"] = EncryptedHealthOffset;
		GameData.Offset["DecryptedHealthOffset"] = DecryptedHealthOffset;


		GameData.Offset["MouseX"] = MouseX;
		GameData.Offset["MouseY"] = MouseY;

		GameData.Offset["bAlwaysCreatePhysicsState"] = bAlwaysCreatePhysicsState;


		GameData.Offset["XenuineDecrypt"] = XenuineDecrypt;
		GameData.Offset["UWorld"] = UWorld;

		GameData.Offset["GNames"] = GNames;
		GameData.Offset["ChunkSize"] = ChunkSize;



		GameData.Offset["DecryptNameIndexRor"] = DecryptNameIndexRor;
		GameData.Offset["DecryptNameIndexXorKey1"] = DecryptNameIndexXorKey1;
		GameData.Offset["DecryptNameIndexXorKey2"] = DecryptNameIndexXorKey2;
		GameData.Offset["DecryptNameIndexRval"] = DecryptNameIndexRval;
		GameData.Offset["DecryptNameIndexSval"] = DecryptNameIndexSval;




























		GameData.Offset["ObjID"] = ObjID;
		GameData.Offset["GameInstance"] = GameInstance;
		GameData.Offset["LocalPlayer"] = LocalPlayer;
		GameData.Offset["Actors"] = Actors;
		//GameData.Offset["ActorsForGC"] = ActorsForGC;
		GameData.Offset["ComponentToWorld"] = ComponentToWorld;
		GameData.Offset["ComponentLocation"] = ComponentLocation;

		GameData.Offset["TimeSeconds"] = TimeSeconds;
		GameData.Offset["WorldToMap"] = WorldToMap;

		GameData.Offset["CurrentLevel"] = CurrentLevel;
		GameData.Offset["Mesh"] = Mesh;
		GameData.Offset["CharacterMovement"] = CharacterMovement;
		GameData.Offset["LastUpdateVelocity"] = LastUpdateVelocity;
		GameData.Offset["RootComponent"] = RootComponent;
		GameData.Offset["PlayerController"] = PlayerController;
		GameData.Offset["PlayerInput"] = PlayerInput;
		GameData.Offset["InputYawScale"] = InputYawScale;
		GameData.Offset["bShowMouseCursor"] = bShowMouseCursor;
		GameData.Offset["AcknowledgedPawn"] = AcknowledgedPawn;

		GameData.Offset["MyHUD"] = MyHUD;
		GameData.Offset["InputAxisProperties"] = InputAxisProperties;
		GameData.Offset["AntiCtCharacterSyncManager"] = AntiCheatCharacterSyncManager;
		//GameData.Offset["DormantCharacterClientList"] = DormantCharacterClientList;





		GameData.Offset["GameState"] = GameState;
		GameData.Offset["SafetyZoneRadius"] = SafetyZoneRadius;
		GameData.Offset["SafetyZonePosition"] = SafetyZonePosition;
		GameData.Offset["BlueZoneRadius"] = BlueZoneRadius;
		GameData.Offset["BlueZonePosition"] = BlueZonePosition;
		GameData.Offset["PlayerArray"] = PlayerArray;

		GameData.Offset["PlayerCameraManager"] = PlayerCameraManager;
		GameData.Offset["ViewTarget"] = ViewTarget;
		GameData.Offset["CameraCacheFOV"] = CameraCacheFOV;
		GameData.Offset["CameraCacheRotation"] = CameraCacheRotation;
		GameData.Offset["CameraCacheLocation"] = CameraCacheLocation;
		//GameData.Offset["LastFrameCameraCacheFOV"] = LastFrameCameraCacheFOV;
		//GameData.Offset["LastFrameCameraCacheRotation"] = LastFrameCameraCacheRotation;
		//GameData.Offset["LastFrameCameraCacheLocation"] = LastFrameCameraCacheLocation;
		GameData.Offset["Gender"] = Gender;











		GameData.Offset["GroggyHealth"] = GroggyHealth;

		GameData.Offset["CharacterName"] = CharacterName;
		GameData.Offset["LastTeamNum"] = LastTeamNum;

		GameData.Offset["SpectatedCount"] = SpectatedCount;
		GameData.Offset["PlayerState"] = PlayerState;
		GameData.Offset["CharacterState"] = CharacterState;





		GameData.Offset["AnimScriptInstance"] = AnimScriptInstance;
		GameData.Offset["PreEvalPawnState"] = PreEvalPawnState;
		GameData.Offset["StaticMesh"] = StaticMesh;
		GameData.Offset["Eyes"] = Eyes;

		GameData.Offset["AimOffsets"] = AimOffsets;













		GameData.Offset["PlayerStatistics"] = PlayerStatistics;
		GameData.Offset["DamageDealtOnEnemy"] = DamageDealtOnEnemy;
		GameData.Offset["PartnerLevel"] = PartnerLevel;
		GameData.Offset["TeamNumber"] = TeamNumber;
		GameData.Offset["PlayerName"] = PlayerName;
		GameData.Offset["PlayerStatusType"] = PlayerStatusType;
		GameData.Offset["SquadMemberIndex"] = SquadMemberIndex;
		GameData.Offset["AccountId"] = AccountId;
		GameData.Offset["PubgIdData"] = PubgIdData;
		GameData.Offset["SurvivalTier"] = SurvivalTier;
		GameData.Offset["SurvivalLevel"] = SurvivalLevel;


		//GameData.Offset["MedalOne"] = MedalOne;




		GameData.Offset["CharacterClanInfo"] = CharacterClanInfo;





		GameData.Offset["VehicleRiderComponent"] = VehicleRiderComponent;
		GameData.Offset["ReplicatedMovement"] = ReplicatedMovement;
		GameData.Offset["SeatIndex"] = SeatIndex;
		GameData.Offset["LastVehiclePawn"] = LastVehiclePawn;
		GameData.Offset["VehicleMovement"] = VehicleMovement;
		GameData.Offset["Wheels"] = Wheels;
		GameData.Offset["WheelLocation"] = WheelLocation;


		GameData.Offset["DampingRate"] = DampingRate;
		GameData.Offset["ShapeRadius"] = ShapeRadius;

		GameData.Offset["WidgetStateMap"] = WidgetStateMap;
		GameData.Offset["TrainingMapGrid"] = TrainingMapGrid;
		GameData.Offset["LayoutData"] = LayoutData;
		GameData.Offset["Visibility"] = Visibility;
		GameData.Offset["Slot"] = Slot;
		GameData.Offset["Offsets"] = Offsets;
		GameData.Offset["Alignment"] = Alignment;

		GameData.Offset["BlockInputWidgetList"] = BlockInputWidgetList;
		GameData.Offset["SelectMinimapSizeIndex"] = SelectMinimapSizeIndex;

		GameData.Offset["ItemID"] = ItemID;
		GameData.Offset["ItemTable"] = ItemTable;
		GameData.Offset["ItemPackageItems"] = ItemPackageItems;
		GameData.Offset["DroppedItemGroup"] = DroppedItemGroup;

		GameData.Offset["DroppedItem"] = DroppedItem;
		GameData.Offset["DroppedItemGroupUItem"] = DroppedItemGroupUItem;

		GameData.Offset["TimeTillExplosion"] = TimeTillExplosion;


		GameData.Offset["ExplodeState"] = ExplodeState;

		GameData.Offset["WeaponProcessor"] = WeaponProcessor;
		GameData.Offset["CurrentAmmoData"] = CurrentAmmoData;
		GameData.Offset["CurrentWeaponIndex"] = CurrentWeaponIndex;
		GameData.Offset["EquippedWeapons"] = EquippedWeapons;
		GameData.Offset["WeaponTrajectoryData"] = WeaponTrajectoryData;
		GameData.Offset["TrajectoryGravityZ"] = TrajectoryGravityZ;
		GameData.Offset["TrajectoryConfig"] = TrajectoryConfig;
		GameData.Offset["BallisticCurve"] = BallisticCurve;
		GameData.Offset["FloatCurves"] = FloatCurves;
		GameData.Offset["Keys"] = Keys;

		//GameData.Offset["WeaponConfig"] = WeaponConfig;
		//GameData.Offset["WeaponClass"] = WeaponClass;

		GameData.Offset["WeaponConfig_WeaponClass"] = WeaponConfig_WeaponClass;
		//GameData.Offset["WeaponConfig"] = WeaponConfig;
		//GameData.Offset["WeaponClass"] = WeaponClass;
		GameData.Offset["Mesh3P"] = Mesh3P;
		GameData.Offset["FiringAttachPoint"] = FiringAttachPoint;
		GameData.Offset["AttachedStaticComponentMap"] = AttachedStaticComponentMap;
		GameData.Offset["AttachedItems"] = AttachedItems;
		GameData.Offset["WeaponAttachmentData"] = WeaponAttachmentData;





		GameData.Offset["ScopingAttachPoint"] = ScopingAttachPoint;
		GameData.Offset["ElapsedCookingTime"] = ElapsedCookingTime;


		GameData.Offset["ControlRotation_CP"] = ControlRotation_CP;
		GameData.Offset["RecoilADSRotation_CP"] = RecoilADSRotation_CP;
		GameData.Offset["LeanLeftAlpha_CP"] = LeanLeftAlpha_CP;
		GameData.Offset["LeanRightAlpha_CP"] = LeanRightAlpha_CP;
		GameData.Offset["bIsScoping_CP"] = bIsScoping_CP;
		GameData.Offset["bIsReloading_CP"] = bIsReloading_CP;

		GameData.Offset["StaticSockets"] = StaticSockets;
		GameData.Offset["StaticSocketName"] = StaticSocketName;
		GameData.Offset["StaticRelativeLocation"] = StaticRelativeLocation;
		GameData.Offset["StaticRelativeRotation"] = StaticRelativeRotation;
		GameData.Offset["StaticRelativeScale"] = StaticRelativeScale;

	}
}