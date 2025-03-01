#pragma once
#include <Winsock2.h>
#include <DMALibrary/Memory/Memory.h>
#include <Common/Data.h>
#include <Utils/KmBox.h>
#include <Utils/KmBoxNet.h>
#include <Hack/Players.h>

namespace KeyState
{
	void Init()
	{
		GameData.Keyboard = mem.GetKeyboard();

		if (!GameData.Keyboard.InitKeyboard_win11())
		{
			Utils::Log(2, U8("键鼠设备初始化失败"));
		}
		else {
			Utils::Log(1, U8("键鼠设备初始化成功"));
		}
	}
	void Update() {
		while (true)
		{
			GameData.Keyboard.UpdateKeys();

			std::unordered_map<int, std::vector<std::string>> Keys;
			Keys[GameData.Config.Menu.ShowKey].push_back("Menu");
			Keys[GameData.Config.AimBot.Configs[0].Key].push_back("AimBotConfig0");
			Keys[GameData.Config.AimBot.Configs[1].Key].push_back("AimBotConfig1");
			Keys[VK_END].push_back("RecoverOverlay");
			Keys[GameData.Config.Function.ClearKey].push_back("Clear");
			Keys[GameData.Config.Item.GroupKey].push_back("GroupKey");
			Keys[GameData.Config.Item.GroupAKey].push_back("GroupAKey");
			Keys[GameData.Config.Item.GroupBKey].push_back("GroupBKey");
			Keys[GameData.Config.Item.GroupCKey].push_back("GroupCKey");
			Keys[GameData.Config.Item.GroupDKey].push_back("GroupDKey");
			Keys[GameData.Config.Vehicle.EnableKey].push_back("VehicleEnable");
			Keys[GameData.Config.PlayerList.MarkKey].push_back("PlayerListMarkType");
			Keys[GameData.Config.ESP.FocusModeKey].push_back("FocusModeKey");
			Keys[GameData.Config.AirDrop.EnableKey].push_back("AirDropEnableKey");
			Keys[GameData.Config.DeadBox.EnableKey].push_back("DeadBoxEnableKey");

			for (auto Key : Keys)
			{
				if (GameData.Keyboard.WasKeyPressed(Key.first))
				{
					for (auto KeyName : Key.second)
					{
						

						if (KeyName == "AirDropEnableKey")
						{
							GameData.Config.AirDrop.Enable = !GameData.Config.AirDrop.Enable;
						}

						if (KeyName == "DeadBoxEnableKey")
						{
							GameData.Config.DeadBox.Enable = !GameData.Config.DeadBox.Enable;
						}

						if (KeyName == "FocusModeKey")
						{
							GameData.Config.ESP.FocusMode = !GameData.Config.ESP.FocusMode;
						}

						if (KeyName == "Clear")
						{
							if (GameData.Config.AimBot.Connected)
							{
								if (GameData.Config.AimBot.Controller == 0)
								{
									KmBox::Clear();
									Utils::Log(1, "KMBOX Clear Success");
								}

								if (GameData.Config.AimBot.Controller == 1)
								{
									KmBoxNet::Clear();
									Utils::Log(1, "KMBOXNET Clear Success");
								}
							}

							Data::SetCacheEntitys({});
							Data::SetCachePlayers({});
							Data::SetPlayers({});
							Data::SetPlayersData({});
							Data::SetCacheVehicles({});
							Data::SetVehicles({});
							Data::SetVehiclWheels({});
							Data::SetItems({});
							Data::SetCacheDroppedItems({});
							Data::SetCacheDroppedItemGroups({});
							GameData.AimBot.Target = 0;
							GameData.AimBot.Lock = false;
							mem.RefreshAll();
						}

						

						if (KeyName == "VehicleEnable")
						{
							GameData.Config.Vehicle.Enable = !GameData.Config.Vehicle.Enable;
						}
						if (KeyName == "Menu")
						{
							GameData.Config.Menu.Show = !GameData.Config.Menu.Show;
							if (GameData.Config.Menu.Show)
							{
								::ShowWindow(GameData.Config.Overlay.hWnd, SW_SHOWDEFAULT);
								//SetForegroundWindow(GameData.Config.Overlay.hWnd);
							}
							else {
								::ShowWindow(GameData.Config.Overlay.hWnd, SW_HIDE);
								//SetForegroundWindow(GetDesktopWindow());
							}
						}
						if (KeyName == "PlayerListShow")
						{
							GameData.Config.Window.Players = !GameData.Config.Window.Players;
						}
						if (KeyName == "SettingShow")
						{
							GameData.Config.Window.Setting = !GameData.Config.Window.Setting;
						}
						if (KeyName == "AimBotConfig0")
						{
							GameData.Config.AimBot.ConfigIndex = 0;
						}
						if (KeyName == "AimBotConfig1")
						{
							GameData.Config.AimBot.ConfigIndex = 1;
						}

					}
				}
			}

			Sleep(100);
		}
	}
}
