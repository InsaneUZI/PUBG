#pragma once
#include <DMALibrary/Memory/Memory.h>
#include <Common/Data.h>
#include <Common/Entitys.h>
#include <Utils/Utils.h>
#include <Utils/Throttler.h>
#include <Hack/GNames.h>
#include <Hack/Decrypt.h>

namespace LineTraceHook
{
	typedef struct _BoneInfo {
		FVector BonePos;
		bool Visibility;
	}BoneInfo, * PBoneInfo;
	typedef struct _ShellCodePrarm {
		uint64_t SPoofcall;
		uint64_t LineTraceSingle;
		uint64_t Uworld;
		uint64_t Actors;
		uint64_t IsloopOk;
		FVector SrartPos;
		BoneInfo Bones[28];
	}ShellCodePrarm, * PShellCodePrarm;
	//TslGame.exe + 1D4BE3A - FF 23 - jmp qword ptr[rbx]

	//uint64_t GameData.Offset["SPOOFCALL"] = GameData.Offset["SPOOFCALL"]; // FF 23 || 5C 24 ?? 44 89 87 ?? ?? ?? ?? 44 3B
	//uint64_t GameData.Offset["LineTraceSingle"] = GameData.Offset["LineTraceSingle"]; // 48 8B C4 55 56 57 41 54  41 55 41 56 41 57 48 81 EC ?? ?? ?? ?? 48 c7 44 24 58 fe ff ff ff 48
	//uint64_t GameData.Offset["HOOK"] = GameData.Offset["HOOK"]; // BF 00 00 00 10 FF 15 ?? ?? ?? ?? B9 00 00 00 08 84 call qword ptr [0BE43740]
	//uint64_t GameData.Offset["HOOK_TWO"] = GameData.Offset["HOOK_TWO"]; // BF 00 00 00 10 FF 15 ?? ?? ?? ?? B9 00 00 00 08 84
	uintptr_t pCurrentFunction;
	uintptr_t pOldFunction;
	uintptr_t pBuffer;
	uintptr_t pParamBuffer;

	const char buf[] = { 0x48,0x8B,0x04,0x24,0x48,0x3B,0x05,0x2C,0x00,0x00,0x00,0x75,0x20,0x83,0x3D,0x1F,0x00,0x00,0x00,0x01,0xC7,0x05,0x15,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x75,0x0D,0x48,0x83,0xEC,0x28,0xE8,
0x13,0x01,0x00,0x00,0x48,0x83,0xC4,0x28,0xFF,0x25,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x4C,0x8B,0xDC,0x48,0x81,0xEC,0x68,0x01,0x00,0x00,0x49,0x8B,0x00,0x0F,0x57,0xC0,0x4D,0x8B,0x48,0x18,0x0F,0x57,0xC9,0x4D,0x8B,0x50,0x10,0x49,0x89,0x83,0x28,0xFF,
0xFF,0xFF,0x49,0x8B,0x40,0x08,0x4C,0x8D,0x44,0x24,0x70,0x49,0x89,0x83,0x30,0xFF,0xFF,0xFF,0x33,0xC0,0x49,0x89,0x83,0x38,0xFF,0xFF,0xFF,0x8B,0x42,0x08,0x89,0x44,0x24,0x78,0x8B,
0x41,0x08,0x89,0x84,0x24,0x88,0x00,0x00,0x00,0x49,0x8D,0x83,0x48,0xFF,0xFF,0xFF,0x0F,0x29,0x84,0x24,0xB0,0x00,0x00,0x00,0xF2,0x0F,0x10,0x02,0x49,0x8D,0x93,0x18,0xFF,0xFF,0xFF,
0xF2,0x0F,0x11,0x44,0x24,0x70,0xF2,0x0F,0x10,0x01,0x49,0x8B,0xCA,0xF2,0x0F,0x11,0x84,0x24,0x80,0x00,0x00,0x00,0x0F,0x57,0xC0,0xF3,0x0F,0x11,0x44,0x24,0x68,0x48,0x89,0x44,0x24,
0x60,0x49,0x8D,0x83,0x58,0xFF,0xFF,0xFF,0x48,0x89,0x44,0x24,0x58,0x49,0x8D,0x83,0x68,0xFF,0xFF,0xFF,0xC6,0x44,0x24,0x50,0x01,0x48,0x89,0x44,0x24,0x48,0x33,0xC0,0x89,0x44,0x24,
0x40,0x4C,0x89,0x4C,0x24,0x38,0x41,0xB9,0x01,0x00,0x00,0x00,0xC6,0x44,0x24,0x30,0x01,0x48,0x89,0x44,0x24,0x28,0x49,0x8D,0x83,0x28,0xFF,0xFF,0xFF,0x48,0x89,0x44,0x24,0x20,0x0F,0x29,
0x8C,0x24,0xC0,0x00,0x00,0x00,0xE8,0x00,0x01,0x00,0x00,0x84,0xC0,0x0F,0x94,0xC0,0x48,0x81,0xC4,0x68,0x01,0x00,0x00,0xC3,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0x40,0x56,
0x57,0x48,0x83,0xEC,0x58,0x48,0xB8,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x48,0x89,0x44,0x24,0x28,0x48,0x8B,0x44,0x24,0x28,0x48,0x83,0x38,0x00,0x0F,0x86,0xBE,0x00,0x00,0x00,
0x48,0x8B,0x44,0x24,0x28,0x48,0x83,0x78,0x08,0x00,0x0F,0x86,0xAE,0x00,0x00,0x00,0xC7,0x44,0x24,0x20,0x00,0x00,0x00,0x00,0xEB,0x0A,0x8B,0x44,0x24,0x20,0xFF,0xC0,0x89,0x44,0x24,
0x20,0x83,0x7C,0x24,0x20,0x1C,0x0F,0x8D,0x82,0x00,0x00,0x00,0x48,0x63,0x44,0x24,0x20,0x48,0x6B,0xC0,0x10,0x48,0x8D,0x4C,0x24,0x30,0x48,0x8B,0x54,0x24,0x28,0x48,0x8B,0xF9,0x48,
0x8D,0x74,0x02,0x34,0xB9,0x0C,0x00,0x00,0x00,0xF3,0xA4,0x48,0x8D,0x44,0x24,0x40,0x48,0x8B,0x4C,0x24,0x28,0x48,0x8B,0xF8,0x48,0x8D,0x71,0x28,0xB9,0x0C,0x00,0x00,0x00,0xF3,0xA4,
0x4C,0x8B,0x44,0x24,0x28,0x48,0x8D,0x54,0x24,0x30,0x48,0x8D,0x4C,0x24,0x40,0xE8,0x6D,0xFE,0xFF,0xFF,0x0F,0xB6,0xC0,0x85,0xC0,0x74,0x15,0x48,0x63,0x44,0x24,0x20,0x48,0x6B,0xC0,
0x10,0x48,0x8B,0x4C,0x24,0x28,0xC6,0x44,0x01,0x40,0x01,0xEB,0x13,0x48,0x63,0x44,0x24,0x20,0x48,0x6B,0xC0,0x10,0x48,0x8B,0x4C,0x24,0x28,0xC6,0x44,0x01,0x40,0x00,0xE9,0x69,0xFF,
0xFF,0xFF,0x48,0x8B,0x44,0x24,0x28,0x48,0xC7,0x40,0x20,0x01,0x00,0x00,0x00,0x48,0x83,0xC4,0x58,0x5F,0x5E,0xC3,0x41,0x5B,0x48,0x83,0xC4,0x08,0x48,0x8B,0x44,0x24,0x18,0x4C,0x8B,
0x10,0x4C,0x89,0x14,0x24,0x4C,0x8B,0x50,0x08,0x4C,0x89,0x58,0x08,0x48,0x89,0x58,0x10,0x48,0x8D,0x1D,0x09,0x00,0x00,0x00,0x48,0x89,0x18,0x48,0x8B,0xD8,0x41,0xFF,0xE2,0x48,0x83,
0xEC,0x10,0x48,0x8B,0xCB,0x48,0x8B,0x59,0x10,0xFF,0x61,0x08,0x48,0xC7,0xC0,0x00,0x00,0x00,0x00,0xC3,0xB8,0x12,0x00,0x00,0x00,0xC3 };

	void StartHook()
	{
		/*if (Utils::ValidPtr(GameData.ActorArrayEmpty))
		{
			return;
		}*/

		auto hScatter = mem.CreateScatterHandle();

		pCurrentFunction = mem.Read<uintptr_t>(GameData.GameBase + GameData.Offset["HOOK"]);
		pBuffer = GameData.HookBase + 0x301;
		//Utils::Log(1, "pBuffer %p", pBuffer);
		pParamBuffer = pBuffer + sizeof(buf) + 0x40;

		if (pCurrentFunction == pBuffer)
			return;
		pOldFunction = pCurrentFunction;

		char ShellCode[sizeof(buf)];
		memcpy(ShellCode, buf, sizeof(buf));
		*(uint32_t*)(ShellCode + 0x33) = 0;
		*(uintptr_t*)(ShellCode + 0x37) = GameData.GameBase + GameData.Offset["HOOK_TWO"];
		*(uintptr_t*)(ShellCode + 0x3F) = pOldFunction;
		*(uintptr_t*)(ShellCode + 0x145) = pParamBuffer;

		ShellCodePrarm scp;
		ZeroMemory(&scp, sizeof(scp));
		scp.SPoofcall = GameData.GameBase + GameData.Offset["SPOOFCALL"];
		scp.LineTraceSingle = GameData.GameBase + GameData.Offset["LineTraceSingle"];
		scp.Uworld = GameData.UWorld;
		scp.Actors = GameData.ActorArray;
		//scp.Actors = pParamBuffer + sizeof(ShellCodePrarm);
		//std::cout << "DLL: " << std::hex << pBuffer << std::endl;
		//std::cout << "PARAM: " << std::hex << pParamBuffer << std::endl;

		mem.AddScatterWrite(hScatter, pParamBuffer, scp);
		mem.AddScatterWrite(hScatter, pBuffer, ShellCode);
		//mem.ExecuteWriteScatter(hScatter);

		mem.AddScatterWrite(hScatter, GameData.GameBase + GameData.Offset["HOOK"], pBuffer);
		mem.ExecuteWriteScatter(hScatter);

		mem.CloseScatterHandle(hScatter);
	}

	void EndHook()
	{
		if (pOldFunction) {
			auto hScatter = mem.CreateScatterHandle();

			char Zero[sizeof(buf)];
			char Zero2[sizeof(ShellCodePrarm)];
			memset(Zero, 0, sizeof(buf));
			memset(Zero2, 0, sizeof(ShellCodePrarm));

			mem.AddScatterWrite(hScatter, pBuffer, Zero);
			mem.AddScatterWrite(hScatter, pParamBuffer, Zero2);
			mem.AddScatterWrite(hScatter, GameData.GameBase + GameData.Offset["HOOK"], pOldFunction);
			mem.ExecuteWriteScatter(hScatter);

			pOldFunction = 0;

			//mem.AddScatterWrite(hScatter, pBuffer, Zero);
			//mem.AddScatterWrite(hScatter, pParamBuffer, Zero2);
			//mem.ExecuteWriteScatter(hScatter);

			mem.CloseScatterHandle(hScatter);
		}
			
	}

	bool GetLocation(const Player& TargetCharacter, const EBoneIndex& FirstBoneIndex, EBoneIndex* VisibilityBoneIndex)
	{
		auto hScatter = mem.CreateScatterHandle();

		ShellCodePrarm scp;
		ZeroMemory(&scp, sizeof(scp));
		scp.SPoofcall = GameData.GameBase + GameData.Offset["SPOOFCALL"];
		scp.LineTraceSingle = GameData.GameBase + GameData.Offset["LineTraceSingle"];
		scp.Uworld = GameData.UWorld;
		scp.Actors = GameData.ActorArray;
		//scp.SrartPos = GameData.LocalPlayerInfo.Skeleton.LocationBones.at(EBoneIndex::ForeHead);
		scp.SrartPos = GameData.Camera.Location;

		mem.Write(pParamBuffer + 0x28, scp.SrartPos);

		std::unordered_map<int, int> MapBoneIndex;
		int i = 0;

		for (auto a : SkeletonLists::Skeleton)
		{
			for (int bone : a)
			{
				if (MapBoneIndex.count(bone))
					continue;
				MapBoneIndex[bone] = i;
				scp.Bones[i].BonePos = TargetCharacter.Skeleton.LocationBones.at(bone);
				//AimScatter->write(pParamBuffer + 0x10 * i + 0x34, &scp.Bones[i].BonePos);
				mem.AddScatterWrite(hScatter, pParamBuffer + 0x10 * i + 0x34, scp.Bones[i].BonePos);
				//mem.ExecuteWriteScatter(hScatter);
				i++;
			}
		}

		mem.AddScatterWrite(hScatter, pBuffer + 0x33, 1);
		mem.ExecuteWriteScatter(hScatter);

		mem.AddScatterRead(hScatter, pParamBuffer, &scp);
		mem.ExecuteReadScatter(hScatter);

		mem.CloseScatterHandle(hScatter);

		if (scp.Bones[MapBoneIndex[(int)FirstBoneIndex]].Visibility)
		{
			//*oLocation = scp.Bones[Settings::AimBoneIndex].BonePos;
			//*oLocation = GetBoneWithRotation(TargetCharacter, Settings::AimBoneIndex);
			*VisibilityBoneIndex = FirstBoneIndex;
			return true;
		}
		else
		{
			for (auto a : SkeletonLists::Skeleton)
			{
				for (int bone : a)
				{
					//Utils::Log(1, "%d - %f %f %f", bone, scp.Bones[MapBoneIndex[bone]].BonePos.X, scp.Bones[MapBoneIndex[bone]].BonePos.Y, scp.Bones[MapBoneIndex[bone]].BonePos.Z);
					if (scp.Bones[MapBoneIndex[bone]].Visibility)
					{
						//*oLocation = scp.Bones[MapBoneIndex[bone]].BonePos;
						*VisibilityBoneIndex = (EBoneIndex)bone;
						//std::cout << "bone: " << VisibilityBoneIndex << std::endl;
						//*oLocation = GetBoneWithRotation(TargetCharacter, bone);
						return true;
					}
				}
			}
		}

		return false;
	}
}