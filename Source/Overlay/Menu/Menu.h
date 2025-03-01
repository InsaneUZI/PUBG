#pragma once
#include <DMALibrary/Memory/Memory.h>
#include <Common/Data.h>
#include <Common/Entitys.h>
#include <Common/Config.h>
#include <Utils/Utils.h>
#include <Utils/Throttler.h>
#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui/imgui.h>
#include <imgui/imgui_settings.h>
#include <Utils/KmBox.h>
#include <Utils/KmBoxNet.h>
#include <Utils/Lurker.h>
#include <Utils/MoBox.h>

#include "Overlay/Menu/MenuPlayerLists.h"

static int iTabs = 0;
static int iSubTab = 0;

inline std::vector<std::string> MenuItemTypes = {
        U8("全部"),
        U8("步枪"),
        U8("连狙"),
        U8("栓狙"),
        U8("散弹"),
        U8("冲锋"),
        U8("机枪"),
        U8("手枪"),
        U8("药品"),
        U8("装备"),
        U8("枪口"),
        U8("握把"),
        U8("弹匣"),
        U8("枪托"),
        U8("瞄具"),
        U8("子弹"),
        U8("投掷"),
        U8("其它")
};

inline std::vector<WeaponType> MenuItemTypeIndexs = {
       WeaponType::AR,
       WeaponType::AR,
       WeaponType::DMR,
       WeaponType::SR,
       WeaponType::SG,
       WeaponType::SMG,
       WeaponType::LMG,
       WeaponType::HG,
       WeaponType::Drug,
       WeaponType::Armor,
       WeaponType::Muzzle,
       WeaponType::Grip,
       WeaponType::Magazine,
       WeaponType::GunButt,
       WeaponType::Sight,
       WeaponType::Bullet,
       WeaponType::Grenade,
       WeaponType::Other
};

namespace ImGui
{
    int rotation_start_index;
    void ImRotateStart()
    {
        rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
    }

    ImVec2 ImRotationCenter()
    {
        ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

        const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
        for (int i = rotation_start_index; i < buf.Size; i++)
            l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

        return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
    }


    void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
    {
        float s = sin(rad), c = cos(rad);
        center = ImRotate(center, s, c) - center;

        auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
        for (int i = rotation_start_index; i < buf.Size; i++)
            buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
    }
}


class Menu
{
public:

    static void Clipboard(const char* data) {
        const TCHAR szOperation[] = _T("open");
        //const TCHAR szAddress[] = data;
        HINSTANCE hRslt = ShellExecute(NULL, szOperation,
            data, NULL, NULL, SW_SHOWNORMAL);
        assert(hRslt > (HINSTANCE)HINSTANCE_ERROR);
    }

    static void DrawEspSetting()
    {
        ImGui::SetCursorPos(ImVec2(220, 60));
        auto pIO = ImGui::GetPlatformIO();
        ImGuiStyle* ImStyle = &ImGui::GetStyle();

        ImGui::BeginChild(U8("覆盖"), ImVec2(300, 260));
        {
            ImGui::Text(U8("垂直同步"));
            ImGui::SameLine(197);
            ImGui::Checkbox(U8(""), &GameData.Config.Overlay.VSync);

            ImGui::Text(U8("融合模式"));
            ImGui::SameLine(197);
            ImGui::Checkbox(U8(" "), &GameData.Config.Overlay.FusionMode);

            ImGui::ColorEdit4(U8("融合背景颜色"), (float*)&GameData.Config.Overlay.FusionColor, PickerFlags);


            ImGui::Text(U8("独立线程"));
            ImGui::SameLine(197);
            ImGui::Checkbox(U8("  "), &GameData.Config.Overlay.UseThread);

            GameData.Config.Overlay.ScreenWidth = pIO.Monitors[GameData.Config.Overlay.MonitorCurrentIdx].MainSize.x;
            GameData.Config.Overlay.ScreenHeight = pIO.Monitors[GameData.Config.Overlay.MonitorCurrentIdx].MainSize.y;

            std::string comboPreview = Utils::StringToUTF8(std::format("{}x{} (屏幕{})", GameData.Config.Overlay.ScreenWidth, GameData.Config.Overlay.ScreenHeight, GameData.Config.Overlay.MonitorCurrentIdx));
            if (ImGui::NewBeginCombo(U8("屏幕列表"), comboPreview.c_str()))
            {
                for (int i = 0; i < pIO.Monitors.size(); i++)
                {
                    auto pMonitor = pIO.Monitors[i];
                    const bool isSelected = (GameData.Config.Overlay.MonitorCurrentIdx == i);
                    auto monitorName = Utils::StringToUTF8(std::format("{}x{} (屏幕{})", pMonitor.MainSize.x, pMonitor.MainSize.y, i));
                    if (ImGui::Selectable(monitorName.c_str(), isSelected))
                    {
                        GameData.Config.Overlay.MonitorCurrentIdx = i;
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if (GameData.Config.Overlay.Enable)
            {
                if (ImGui::Button(U8("关闭透视"), ImVec2(ImGui::GetContentRegionMax().x - ImStyle->WindowPadding.x, 50)))
                    GameData.Config.Overlay.Enable = false;

            }
            else
            {
                if (ImGui::Button(U8("启用透视"), ImVec2(ImGui::GetContentRegionMax().x - ImStyle->WindowPadding.x, 50)))
                {
                    GameData.Config.Overlay.Enable = true;
                }
            }
        }
        ImGui::EndChild();
    }

    static void DrawEspPlayer()
    {
        ImGui::SetCursorPos(ImVec2(220, 60));

        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("显示"), ImVec2(229, 350));
            {
                ImGui::Checkbox(U8("玩家透视"), &GameData.Config.ESP.Enable);

                static const char* ShowMultiItems[17] = {
                                U8("骨骼"), // 0
                                U8("战队"), // 1
                                U8("名称"), // 2
                                U8("血条"), // 3
                                U8("距离"), // 4
                                U8("队标"), // 5
                                U8("击杀"), // 6
                                U8("观战"), // 7
                                U8("伤害"), // 8
                                U8("掉线"), // 9
                                U8("合作者"), // 10
                                U8("段位"), // 11
                                U8("段位分"), // 12
                                U8("KD"), // 13
                                U8("手持图标"), // 14
                                U8("手持武器"), // 15
                                U8("等级"), // 16
                };

                ImGui::NewMultiCombo(U8("显示信息"), GameData.Config.ESP.ShowInfos, ShowMultiItems, 17);
                static const char* DistanceStyleItems[2]{ U8("顶部"), U8("底部") };
                ImGui::NewCombo(U8("距离位置"), &GameData.Config.ESP.DistanceStyle, DistanceStyleItems, IM_ARRAYSIZE(DistanceStyleItems), 2);
                static const char* HealthBarStyleItems[2]{ U8("顶部"), U8("左侧") };
                ImGui::NewCombo(U8("血条位置"), &GameData.Config.ESP.HealthBarStyle, HealthBarStyleItems, IM_ARRAYSIZE(HealthBarStyleItems), 2);

                ImGui::Checkbox(U8("可视检查"), &GameData.Config.ESP.VisibleCheck);
                ImGui::Checkbox(U8("瞄准信息"), &GameData.Config.ESP.AimExpandInfo);
                ImGui::Checkbox(U8("渐变血条"), &GameData.Config.ESP.DynamicHealthColor);
                Pickerbox(U8("被瞄射线"), &GameData.Config.ESP.TargetedRay, GameData.Config.ESP.Color.Ray.Line);

                ImGui::Checkbox(U8("可视射线"), &GameData.Config.ESP.VisibleCheckRay);
                ImGui::Checkbox(U8("隐藏骨骼 (锁定时)"), &GameData.Config.ESP.LockedHiddenBones);

            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();

        ImGui::SameLine(0, 30);
        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("调节"), ImVec2(229, 220));
            {
                ImGui::SliderInt(U8("显示距离"), &GameData.Config.ESP.DistanceMax, 0, 1000);
                ImGui::SliderInt(U8("信息距离"), &GameData.Config.ESP.InfoDistanceMax, 0, 1000);
                ImGui::SliderInt(U8("手持距离"), &GameData.Config.ESP.WeaponDistanceMax, 0, 1000);
                ImGui::SliderInt(U8("骨骼粗细"), &GameData.Config.ESP.SkeletonWidth, 1, 3);
                ImGui::SliderInt(U8("射线粗细"), &GameData.Config.ESP.RayWidth, 1, 3);
                ImGui::SliderInt(U8("字体比例"), &GameData.Config.ESP.FontSize, 10, 20);
            }
            ImGui::EndChild();

        }
        ImGui::EndGroup();

        ImGui::SameLine(0, 30);
        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("颜色"), ImVec2(229, 530));
            {
                ImGui::ColorEdit4(U8("可视信息颜色"), (float*)&GameData.Config.ESP.Color.Visible.Info, PickerFlags);
                ImGui::ColorEdit4(U8("可视骨骼颜色"), (float*)&GameData.Config.ESP.Color.Visible.Skeleton, PickerFlags);

                ImGui::ColorEdit4(U8("不可视信息"), (float*)&GameData.Config.ESP.Color.Default.Info, PickerFlags);
                ImGui::ColorEdit4(U8("不可视骨骼"), (float*)&GameData.Config.ESP.Color.Default.Skeleton, PickerFlags);

                ImGui::ColorEdit4(U8("5杀以上玩家信息颜色"), (float*)&GameData.Config.ESP.Color.Dangerous.Info, PickerFlags);
                ImGui::ColorEdit4(U8("5杀以上玩家骨骼颜色"), (float*)&GameData.Config.ESP.Color.Dangerous.Skeleton, PickerFlags);

                ImGui::ColorEdit4(U8("官方的走狗信息颜色"), (float*)&GameData.Config.ESP.Color.Partner.Info, PickerFlags);
                ImGui::ColorEdit4(U8("官方的走狗骨骼颜色"), (float*)&GameData.Config.ESP.Color.Partner.Skeleton, PickerFlags);

                ImGui::ColorEdit4(U8("黑名单信息颜色"), (float*)&GameData.Config.ESP.Color.Blacklist.Info, PickerFlags);
                ImGui::ColorEdit4(U8("黑名单骨骼颜色"), (float*)&GameData.Config.ESP.Color.Blacklist.Skeleton, PickerFlags);

                ImGui::ColorEdit4(U8("白名单信息颜色"), (float*)&GameData.Config.ESP.Color.Whitelist.Info, PickerFlags);
                ImGui::ColorEdit4(U8("白名单骨骼颜色"), (float*)&GameData.Config.ESP.Color.Whitelist.Skeleton, PickerFlags);

                ImGui::ColorEdit4(U8("倒地信息颜色"), (float*)&GameData.Config.ESP.Color.Groggy.Info, PickerFlags);
                ImGui::ColorEdit4(U8("倒地骨骼颜色"), (float*)&GameData.Config.ESP.Color.Groggy.Skeleton, PickerFlags);

                ImGui::ColorEdit4(U8("AI信息颜色"), (float*)&GameData.Config.ESP.Color.AI.Info, PickerFlags);
                ImGui::ColorEdit4(U8("AI骨骼颜色"), (float*)&GameData.Config.ESP.Color.AI.Skeleton, PickerFlags);
            }
            ImGui::EndChild();


        }
        ImGui::EndGroup();
    }

    static void DrawEspItems()
    {
        ImGui::SetCursorPos(ImVec2(220, 60));

        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("显示"), ImVec2(229, 220));
            {
                ImGui::Checkbox(U8("物品透视"), &GameData.Config.Item.Enable);
                ImGui::Keybind(U8("切换热键"), &GameData.Config.Item.GroupKey);
                std::cout << std::format("GameData.Config.Item.GroupKey:{}\n", GameData.Config.Item.GroupKey);
                ImGui::Checkbox(U8("分组显示"), &GameData.Config.Item.Combination);
                ImGui::Checkbox(U8("显示图标"), &GameData.Config.Item.ShowIcon);
                ImGui::Checkbox(U8("显示距离"), &GameData.Config.Item.ShowDistance);
                ImGui::Checkbox(U8("配件过滤"), &GameData.Config.Item.AccessoriesFilter);
            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();

        ImGui::SameLine(0, 30);
        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("调节"), ImVec2(229, 190));
            {
                ImGui::SliderInt(U8("显示距离"), &GameData.Config.Item.DistanceMax, 0, 150);
                ImGui::SliderInt(U8("分组X轴"), &GameData.Config.Item.ThresholdX, 1, 100);
                ImGui::SliderInt(U8("分组Y轴"), &GameData.Config.Item.ThresholdY, 1, 100);
                ImGui::SliderInt(U8("字体比例"), &GameData.Config.Item.FontSize, 10, 20);
                ImGui::ColorEdit4(U8("物资颜色"), GameData.Config.Item.GroupAColor, PickerFlags);

            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();
    }

    static void DrawEspVehicle()
    {
        ImGui::SetCursorPos(ImVec2(220, 60));

        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("显示"), ImVec2(229, 160));
            {
                ImGui::Checkbox(U8("显示载具"), &GameData.Config.Vehicle.Enable);
                ImGui::Keybind(U8("快捷热键"), &GameData.Config.Vehicle.EnableKey);
                ImGui::ColorEdit4(U8("文字颜色"), (float*)&GameData.Config.Vehicle.Color, PickerFlags);
                ImGui::SliderInt(U8("文字大小"), &GameData.Config.Vehicle.FontSize, 10, 20);
            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();
    }

    static void DrawEspAirDrop()
    {
        ImGui::SetCursorPos(ImVec2(220, 60));

        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("显示"), ImVec2(229, 230));
            {
                ImGui::Checkbox(U8("显示空投"), &GameData.Config.AirDrop.Enable);
                ImGui::Keybind(U8("快捷热键"), &GameData.Config.AirDrop.EnableKey);
                ImGui::Checkbox(U8("显示物品"), &GameData.Config.AirDrop.ShowItems);
                ImGui::SliderInt(U8("最大距离"), &GameData.Config.AirDrop.DistanceMax, 10, 1500);
                ImGui::ColorEdit4(U8("文字颜色"), (float*)&GameData.Config.AirDrop.Color, PickerFlags);
                ImGui::SliderInt(U8("比例大小"), &GameData.Config.AirDrop.FontSize, 10, 20);
            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();
    }

    static void DrawEspDeadBox()
    {
        ImGui::SetCursorPos(ImVec2(220, 60));

        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("显示"), ImVec2(229, 230));
            {
                ImGui::Checkbox(U8("显示死亡盒"), &GameData.Config.DeadBox.Enable);
                ImGui::Keybind(U8("快捷热键"), &GameData.Config.DeadBox.EnableKey);
                ImGui::Checkbox(U8("显示物品"), &GameData.Config.DeadBox.ShowItems);
                ImGui::SliderInt(U8("最大距离"), &GameData.Config.DeadBox.DistanceMax, 10, 1500);
                ImGui::ColorEdit4(U8("文字颜色"), (float*)&GameData.Config.DeadBox.Color, PickerFlags);
                ImGui::SliderInt(U8("比例大小"), &GameData.Config.DeadBox.FontSize, 10, 20);
            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();
    }

    static void DrawEspRadar()
    {
        ImGui::SetCursorPos(ImVec2(220, 60));

        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("大地图"), ImVec2(229, 200));
            {
                ImGui::Checkbox(U8("显示玩家"), &GameData.Config.Radar.Main.ShowPlayer);
                ImGui::Checkbox(U8("显示载具"), &GameData.Config.Radar.Main.ShowVehicle);
                ImGui::Checkbox(U8("显示空投"), &GameData.Config.Radar.Main.ShowAirDrop);
                ImGui::Checkbox(U8("显示死亡"), &GameData.Config.Radar.Main.ShowDeadBox);
                ImGui::SliderInt(U8("图标比例"), &GameData.Config.Radar.Main.FontSize, 5, 20);
            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();

        ImGui::SameLine(0, 30);

        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("小地图"), ImVec2(229, 200));
            {
                ImGui::Checkbox(U8("显示玩家"), &GameData.Config.Radar.Mini.ShowPlayer);
                ImGui::Checkbox(U8("显示载具"), &GameData.Config.Radar.Mini.ShowVehicle);
                ImGui::Checkbox(U8("显示空投"), &GameData.Config.Radar.Mini.ShowAirDrop);
                ImGui::Checkbox(U8("显示死亡"), &GameData.Config.Radar.Mini.ShowDeadBox);
                ImGui::SliderInt(U8("图标比例"), &GameData.Config.Radar.Mini.FontSize, 5, 20);
            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();
    }

    static void DrawEspProject()
    {
        ImGui::SetCursorPos(ImVec2(220, 60));

        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("显示"), ImVec2(229, 200));
            {
                ImGui::Checkbox(U8("显示投掷物"), &GameData.Config.Project.Enable);
                ImGui::SliderInt(U8("最大距离"), &GameData.Config.Project.DistanceMax, 10, 1000);
                ImGui::ColorEdit4(U8("显示颜色"), (float*)&GameData.Config.Project.Color, PickerFlags);
                ImGui::SliderInt(U8("比例大小"), &GameData.Config.Project.FontSize, 10, 20);
            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();
    }

    static void DrawEspOther()
    {
        ImGui::SetCursorPos(ImVec2(220, 60));

        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("恰雷"), ImVec2(229, 150));
            {
                ImGui::Checkbox(U8("显示预警"), &GameData.Config.Project.ShowChareTime);
                ImGui::ColorEdit4(U8("显示颜色"), (float*)&GameData.Config.Project.ChareColor, PickerFlags);
                ImGui::SliderInt(U8("比例大小"), &GameData.Config.Project.ChareFontSize, 5, 20);
            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();

        ImGui::SameLine(0, 30);
        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("预警雷达"), ImVec2(229, 160));
            {
                ImGui::Checkbox(U8("显示预警"), &GameData.Config.Early.Enable);
                ImGui::Checkbox(U8("显示距离"), &GameData.Config.Early.ShowDistance);
                ImGui::SliderInt(U8("最大距离"), &GameData.Config.Early.DistanceMax, 10, 1000);
                ImGui::SliderInt(U8("图标比例"), &GameData.Config.Early.FontSize, 5, 20);
            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();
    }

    static void DrawAimbotParam(std::string type)
    {
        auto& Config = GameData.Config.AimBot.Configs[GameData.Config.AimBot.ConfigIndex].Weapon[type];

        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("热键"), ImVec2(230, 445));
            {
                ImGui::Keybind(U8("锁甲热键"), &Config.First.Key);
                ImGui::Keybind(U8("锁头热键"), &Config.Second.Key);
                ImGui::Checkbox(U8("热键合并"), &Config.HotkeyMerge);
                ImGui::Keybind(U8("倒热键地"), &Config.Groggy.Key);
                const char* aim_mode[] = { U8("范围优先"),U8("距离优先") };

                const char* MultiItems[17] = {
                    U8("头顶"),
                    U8("头部"),
                    U8("脖子"),
                    U8("胸部"),
                    U8("裆部"),
                    U8("左肩"),
                    U8("左肘"),
                    U8("右肩"),
                    U8("右肘"),
                    U8("左手"),
                    U8("右手"),
                    U8("左骨盆"),
                    U8("左腿骨"),
                    U8("右骨盆"),
                    U8("右腿骨"),
                    U8("左脚"),
                    U8("右脚")
                };

                ImGui::NewMultiCombo(U8("主要部位"), Config.First.Bones, MultiItems, 17);
                ImGui::NewMultiCombo(U8("次要部位"), Config.Second.Bones, MultiItems, 17);
                ImGui::NewMultiCombo(U8("倒地部位"), Config.Groggy.Bones, MultiItems, 17);

                ImGui::Checkbox(U8("启用锁胎"), &Config.AimWheel);
                ImGui::Keybind(U8("锁胎热键"), &Config.Wheel.Key);
                const char* configItems[5]{ U8("顶部"), U8("偏上"), U8("中间"), U8("偏下"), U8("底部") };
                ImGui::NewCombo(U8("锁胎部位"), &Config.AimWheelBone, configItems, IM_ARRAYSIZE(configItems), 5);
                ImGui::SliderInt(U8("锁胎FOV"), &Config.WheelFOV, 1, 360);
                ImGui::SliderFloat(U8("锁胎速度"), &Config.AimWheelSpeed, 0.1, 100);

            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();

        ImGui::SameLine();
        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("调节"), ImVec2(230, 300));
            {
                ImGui::SliderInt(U8("最大距离"), &Config.AimDistance, 0, 1000);
                ImGui::SliderInt(U8("FOV"), &Config.FOV, 1, 500);
                ImGui::SliderFloat(U8("Y轴速度"), &Config.YSpeed, 0.1, 100);
                ImGui::SliderFloat(U8("X轴速度"), &Config.XSpeed, 0.1, 100);
                ImGui::SliderInt(U8("平滑比例"), &Config.InitialValue, 100, 1500);
                ImGui::SliderFloat(U8("自瞄强度"), &Config.AimSpeedMaxFactor, 0.1, 1);
                ImGui::SliderInt(U8("保持压枪"), &Config.RecoilTime, 0, 20);
                ImGui::SliderInt(U8("FPS限制"), &Config.FPS, 30, 1500);
                ImGui::SliderInt(U8("切换延迟"), &Config.SwitchingDelay, 0, 10);
            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();

        ImGui::SameLine();
        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("选项"), ImVec2(230, 390));
            {
                ImGui::Checkbox(U8("算法漏打"), &Config.LineTraceSingle);
                ImGui::Checkbox(U8("掩体不瞄"), &Config.VisibleCheck);
                ImGui::Checkbox(U8("后座抑制"), &Config.NoRecoil);
                if (!Config.NoRecoil)
                {
                    ImGui::Checkbox(U8("原始回弹"), &Config.OriginalRecoil);
                }
                ImGui::Checkbox(U8("弹道预判"), &Config.Prediction);
                if (Config.Prediction)
                {
                    const char* PredictionItems[2]{ U8("演员自瞄"), U8("精准自瞄") };
                    ImGui::NewCombo(U8("预判算法"), &Config.PredictionMode, PredictionItems, IM_ARRAYSIZE(PredictionItems), 2);
                }
                ImGui::Checkbox(U8("动态FOV"), &Config.DynamicFov);
                ImGui::Checkbox(U8("跳过倒地"), &Config.IgnoreGroggy);
                ImGui::Checkbox(U8("击杀切换"), &Config.AutoSwitch);
                ImGui::Checkbox(U8("空弹不锁"), &Config.NoBulletNotAim);
            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();
    }


    static void DrawRadarWeb()
    {
        ImGui::SetCursorPos(ImVec2(220, 60));

        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("网页雷达"), ImVec2(229, 400));
            {
                ImGui::MyInputTextEx("1", U8("服务器"), GameData.Config.WebRadar.IP, IM_ARRAYSIZE(GameData.Config.WebRadar.IP), ImVec2{0,0}, 0, false);

                ImGuiStyle* ImStyle = &ImGui::GetStyle();

                if (ImGui::Button(GameData.Config.WebRadar.isWebRadarConnect ? U8("断开连接") : U8("连接"), ImVec2(ImGui::GetContentRegionMax().x - ImStyle->WindowPadding.x, 40)))
                {
                    GameData.Config.WebRadar.isWebRadarEnable = !GameData.Config.WebRadar.isWebRadarEnable;
                    GameData.Config.WebRadar.isWebRadarConnect = !GameData.Config.WebRadar.isWebRadarConnect;
                }

                if (GameData.Config.WebRadar.isWebRadarConnect)
                {
                    char buffer[512];
                    snprintf(buffer, sizeof(buffer), "http://%s:7891", GameData.Config.WebRadar.IP);
                    if (ImGui::Button(U8("打开网页地址"), ImVec2(ImGui::GetContentRegionMax().x - ImStyle->WindowPadding.x, 25)))
                    {
                        Clipboard(buffer);
                    }
                }
            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();
    }

    static void DrawGlobalSetting()
    {
        ImGui::SetCursorPos(ImVec2(220, 60));

        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("设置"), ImVec2(229, 400));
            {
                ImGui::Keybind(U8("战斗模式"), &GameData.Config.ESP.FocusModeKey);
                ImGui::Keybind(U8("隐藏所有窗口"), &GameData.Config.Menu.ShowKey);
                ImGui::Keybind(U8("刷新缓存"), &GameData.Config.Function.ClearKey);
                ImGui::Checkbox(U8("文字阴影"), &GameData.Config.ESP.Stroke);

            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();

        ImGui::SameLine(0, 30);
        ImGui::BeginGroup();
        {
            ImGui::BeginChild(U8("配置"), ImVec2(229, 400));
            {
                ImGuiStyle* ImStyle = &ImGui::GetStyle();
                if (ImGui::Button(U8("载入配置"), ImVec2(ImGui::GetContentRegionMax().x - ImStyle->WindowPadding.x, 40)))
                {
                    Config::Load();
                }
                if (ImGui::Button(U8("保存配置"), ImVec2(ImGui::GetContentRegionMax().x - ImStyle->WindowPadding.x, 40)))
                {
                    Config::Save();
                }
            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();
    }


    static void DrawTabs()
    {
        ImGuiStyle& s = ImGui::GetStyle();
        ImGui::SetCursorPos(ImVec2(15, 100));
        ImGui::BeginGroup();
        {
            s.ItemSpacing = ImVec2(3, 3);

            ImGui::Tab(U8("视觉\n效果"), "6", &iTabs, 0);
            ImGui::Dummy({ 0, 30 });


            ImGui::Tab(U8("自动\n瞄准"), "7", &iTabs, 1);
            ImGui::Dummy({ 0, 30 });

            ImGui::Tab(U8("物资\n设置"), "8", &iTabs, 2);
            ImGui::Dummy({ 0, 30 });

            ImGui::Tab(U8("雷达\n设置"), "0", &iTabs, 3);
            ImGui::Dummy({ 0, 30 });


            ImGui::Tab(U8("辅助\n压枪"), "2", &iTabs, 4);
            ImGui::Dummy({ 0, 30 });

            ImGui::Tab(U8("全局\n设置"), "1", &iTabs, 5);

            s.ItemSpacing = ImVec2(10, 12);
        }
        ImGui::EndGroup();

        ImGui::SetCursorPos(ImVec2(224, 19));
        switch (iTabs)
        {
        case 0:
        {
            ImGui::SubTab(U8("设置"), &iSubTab, 0); ImGui::SameLine();
            ImGui::SubTab(U8("玩家"), &iSubTab, 1); ImGui::SameLine();
            ImGui::SubTab(U8("物品"), &iSubTab, 2); ImGui::SameLine();
            ImGui::SubTab(U8("载具"), &iSubTab, 3); ImGui::SameLine();
            ImGui::SubTab(U8("空投"), &iSubTab, 4); ImGui::SameLine();
            ImGui::SubTab(U8("死亡盒"), &iSubTab, 5); ImGui::SameLine();
            ImGui::SubTab(U8("雷达"), &iSubTab, 6); ImGui::SameLine();
            ImGui::SubTab(U8("投掷物"), &iSubTab, 7); ImGui::SameLine();
            ImGui::SubTab(U8("其他"), &iSubTab, 8);

            switch (iSubTab)
            {
                case 0: DrawEspSetting(); break;
                case 1: DrawEspPlayer(); break;
                case 2: DrawEspItems(); break;
                case 3: DrawEspVehicle(); break;
                case 4: DrawEspAirDrop(); break;
                case 5: DrawEspDeadBox(); break;
                case 6: DrawEspRadar(); break;
                case 7: DrawEspProject(); break;
                case 8: DrawEspOther(); break;
            }
        }
        break;
        case 1:
        {
            ImGui::SubTab(U8("设置"), &iSubTab, 0); ImGui::SameLine();
            ImGui::SubTab(U8("步枪"), &iSubTab, 1); ImGui::SameLine();
            ImGui::SubTab(U8("连狙"), &iSubTab, 2); ImGui::SameLine();
            ImGui::SubTab(U8("栓狙"), &iSubTab, 3); ImGui::SameLine();
            ImGui::SubTab(U8("机枪"), &iSubTab, 4); ImGui::SameLine();
            ImGui::SubTab(U8("手枪"), &iSubTab, 5); ImGui::SameLine();
            ImGui::SubTab(U8("霰弹枪"), &iSubTab, 6); ImGui::SameLine();
            ImGui::SubTab(U8("冲锋枪"), &iSubTab, 7);

            if (iSubTab == 0)
            {
                ImGui::SetCursorPos(ImVec2(220, 60));

                ImGui::BeginGroup();
                {
                    ImGui::BeginChild(U8("自瞄配置"), ImVec2(229, 400));
                    {
                        ImGui::Checkbox(U8("启用自瞄"), &GameData.Config.AimBot.Enable);
                        Pickerbox(U8("显示FOV"), &GameData.Config.AimBot.ShowFOV, (float*)&GameData.Config.AimBot.FOVColor);
                        Pickerbox(U8("锁胎FOV"), &GameData.Config.AimBot.ShowWheelFOV, (float*)&GameData.Config.AimBot.WheelFOVColor);
                        Pickerbox(U8("显示预瞄"), &GameData.Config.AimBot.ShowPoint, (float*)&GameData.Config.AimBot.PointColor);

                        ImGui::SliderInt(U8("预瞄比例"), &GameData.Config.AimBot.PointSize, 1, 20);
                        ImGui::Keybind(U8("自瞄配置 一"), &GameData.Config.AimBot.Configs[0].Key);
                        ImGui::Keybind(U8("自瞄配置 二"), &GameData.Config.AimBot.Configs[1].Key);
                    }
                    ImGui::EndChild();

                }
                ImGui::EndGroup();

                ImGui::SameLine(0, 30);
                ImGui::BeginGroup();
                {
                    if (GameData.Config.AimBot.Mode == 0)
                    {
                        ImGui::BeginChild(U8("硬件设备"), ImVec2(276, 400));
                        {
                            const char* items[] = { U8("KmBox"), U8("KmBox网络版"), U8("双头"), U8("魔盒") };
                            ImGui::NewCombo(U8("控制器"), &GameData.Config.AimBot.Controller, items, IM_ARRAYSIZE(items));

                            if (GameData.Config.AimBot.Controller == 1)
                            {
                                ImGui::MyInputTextEx("a", "IP", GameData.Config.AimBot.IP, IM_ARRAYSIZE(GameData.Config.AimBot.IP), ImVec2(200, 40), 0, false);
                                ImGui::MyInputTextEx("b", U8("端口"), GameData.Config.AimBot.Port, IM_ARRAYSIZE(GameData.Config.AimBot.Port), ImVec2(200, 40), 0, false);
                                ImGui::MyInputTextEx("c", "UUID", GameData.Config.AimBot.UUID, IM_ARRAYSIZE(GameData.Config.AimBot.UUID), ImVec2(200, 40), 0, false);
                            }

                            static std::vector<std::string> ports = Utils::GetCOMPorts();

                            if (GameData.Config.AimBot.Controller != 1)
                            {
                                std::vector<std::pair<std::string, std::string>> portVector;
                                for (const auto& port : ports)
                                {
                                    portVector.push_back({ port, Utils::StringToUTF8(port) });
                                }
                                std::vector<const char*> items;
                                for (const auto& port : portVector) {
                                    items.push_back(port.second.c_str());
                                }

                                if (items.size() > 0)
                                {
                                    ImGui::NewCombo(U8("COM端口"), &GameData.Config.AimBot.COM, &items[0], items.size());
                                }
                            }

                            if (GameData.Config.AimBot.Controller == 0)
                            {
                                ImGui::SliderInt(U8("执行延迟"), &GameData.Config.AimBot.Delay, 0, 20);
                            }

                            bool NeedPushStyleColor = false;
                            bool ConnectedNeedPushStyleColor = false;
                            ImGuiStyle* ImStyle = &ImGui::GetStyle();

                            if (ImGui::Button(GameData.Config.AimBot.Connected ? U8("断开连接") : U8("连接"), ImVec2(ImGui::GetContentRegionMax().x - ImStyle->WindowPadding.x, 40))) {
                                bool Connected = false;
                                std::string extractedStr;
                                if (ports.size() > 0)
                                {
                                    extractedStr = Utils::ExtractSubstring(ports[GameData.Config.AimBot.COM], R"(COM(\d+))");
                                }
                                int COM = 0;
                                int number = 0;
                                if (extractedStr != "")
                                {
                                    COM = std::stoi(extractedStr);
                                    number = std::stoi(extractedStr);
                                }

                                switch (GameData.Config.AimBot.Controller)
                                {
                                case 0:
                                    if (!GameData.Config.AimBot.Connected)
                                    {
                                        Connected = KmBox::Init(COM);
                                        if (!Connected)
                                        {
                                            ImGui::OpenPopup(U8("连接失败"));
                                        }
                                    }
                                    else {
                                        KmBox::Close();
                                        ConnectedNeedPushStyleColor = true;
                                        Connected = false;
                                    }
                                    break;
                                case 1:
                                    if (!GameData.Config.AimBot.Connected)
                                    {
                                        Connected = KmBoxNet::Init(GameData.Config.AimBot.IP, GameData.Config.AimBot.Port, GameData.Config.AimBot.UUID);
                                        if (!Connected)
                                        {
                                            ImGui::OpenPopup(U8("连接失败"));
                                        }
                                    }
                                    else {
                                        KmBoxNet::Close();
                                        ConnectedNeedPushStyleColor = true;
                                        Connected = false;
                                    }
                                    break;
                                case 2:
                                    if (!GameData.Config.AimBot.Connected)
                                    {
                                        Connected = Lurker::Init(COM);
                                        if (!Connected)
                                        {
                                            ImGui::OpenPopup(U8("连接失败"));
                                        }
                                    }
                                    else {
                                        Lurker::Close();
                                        ConnectedNeedPushStyleColor = true;
                                        Connected = false;
                                    }
                                    break;
                                case 3:
                                    if (!GameData.Config.AimBot.Connected)
                                    {
                                        Connected = MoBox::Init(COM);
                                        if (!Connected)
                                        {
                                            ImGui::OpenPopup(U8("连接失败"));
                                        }
                                    }
                                    else {
                                        MoBox::Close();
                                        ConnectedNeedPushStyleColor = true;
                                        Connected = false;
                                    }
                                    break;
                                default:
                                    break;
                                }

                                GameData.Config.AimBot.Connected = Connected;
                            }

                            if (ImGui::BeginPopupModal(U8("连接失败"), NULL, ImGuiWindowFlags_AlwaysAutoResize))
                            {
                                ImGui::Text(U8("请检查COM端口或控制器状态"));

                                if (ImGui::Button(U8("关闭")))
                                {
                                    ImGui::CloseCurrentPopup();
                                }

                                ImGui::EndPopup();
                            }

                        }
                        ImGui::EndChild();
                    }
                }
                ImGui::EndGroup();
            }
            else
            {
                ImGui::SetCursorPos(ImVec2(220, 60));
                const std::string WeapType[7] = { "AR", "DMR", "SR", "LMG", "HG", "SG", "SMG" };
                if (iSubTab > 7) iSubTab = 1;
                DrawAimbotParam(WeapType[iSubTab - 1]);
            }
        }
        break;
        case 2:
        {
            ImGui::BeginGroup();
            for (int i = 0; i < MenuItemTypes.size(); ++i)
            {
                ImGui::SubTab(MenuItemTypes[i].c_str(), &iSubTab, i);
                if (i != 8 && i < MenuItemTypes.size() - 1)
                {
                    ImGui::SameLine();
                }
            }
            ImGui::EndGroup();

            ImGui::SetCursorPos(ImVec2(224, 100));
            ImGui::BeginGroup();
            {

                static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
                if (ImGui::BeginTable(U8("物品列表"), 3, flags, ImVec2(760, 520)))
                {
                    ImGui::TableSetupScrollFreeze(0, 1);
                    ImGui::TableSetupColumn(U8("   图片"), ImGuiTableColumnFlags_WidthFixed, 50.f);
                    ImGui::TableSetupColumn(U8("名称"), ImGuiTableColumnFlags_WidthFixed, 100.f);
                    ImGui::TableSetupColumn(U8("分组"), ImGuiTableColumnFlags_WidthFixed, 500.f);
                    ImGui::TableHeadersRow();

                    for (auto& Item : GameData.Config.Item.Lists)
                    {
                        if (iSubTab > 0 && Item.second.Type != MenuItemTypeIndexs[iSubTab])
                        {
                            continue;
                        }

                        ImGui::TableNextRow();

                        ImGui::TableSetColumnIndex(0);
                        {
                            std::string Space = "";
                            ImGui::TextUnformatted(Space.c_str());
                            ImGui::SameLine();
                            std::string ItemName = Item.first;
                            std::string IconUrl= "Assets/Icons/Item/All/" + ItemName + ".png";

                            if (GImGuiTextureMap[IconUrl].Width > 0) {
                                ImGui::Image((ImTextureID)GImGuiTextureMap[IconUrl].Texture, ImVec2(20, 20));
                            }
                        }

                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextUnformatted(Utils::StringToUTF8(Item.second.DisplayName).c_str());

                        ImGui::TableSetColumnIndex(2);
                        {
                            static const char* items[] = { U8("隐藏"), U8("显示") };
                            if (Item.second.Group > 1) Item.second.Group = 1;
                            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 4));

                            ImGui::NewCombo(Item.second.Name.c_str(), &Item.second.Group, items, IM_ARRAYSIZE(items), 2, true);
                        }
                    }

                    ImGui::EndTable();

                }
            }
            ImGui::EndGroup();

        }
        break;

        case 3:
        {
            ImGui::SubTab(U8("网页雷达"), &iSubTab, 0); ImGui::SameLine();

            if (iSubTab == 0)
            {
                DrawRadarWeb();
            }
        }
        break;

        case 5: DrawGlobalSetting(); break;
        }
    }



    static void Trinage_background()
    {

        ImVec2 screen_size = { (float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN) };

        static ImVec2 partile_pos[100];
        static ImVec2 partile_target_pos[100];
        static float partile_speed[100];
        static float partile_size[100];
        static float partile_radius[100];
        static float partile_rotate[100];

        for (int i = 1; i < 50; i++)
        {
            if (partile_pos[i].x == 0 || partile_pos[i].y == 0)
            {
                partile_pos[i].x = rand() % (int)screen_size.x + 1;
                partile_pos[i].y = 15.f;
                partile_speed[i] = 1 + rand() % 25;
                partile_radius[i] = rand() % 4;
                partile_size[i] = rand() % 8;

                partile_target_pos[i].x = rand() % (int)screen_size.x;
                partile_target_pos[i].y = screen_size.y * 2;
            }

            partile_pos[i] = ImLerp(partile_pos[i], partile_target_pos[i], ImGui::GetIO().DeltaTime * (partile_speed[i] / 60));
            partile_rotate[i] += ImGui::GetIO().DeltaTime;

            if (partile_pos[i].y > screen_size.y)
            {
                partile_pos[i].x = 0;
                partile_pos[i].y = 0;
                partile_rotate[i] = 0;
            }

            ImGui::ImRotateStart();
            ImGui::GetWindowDrawList()->AddCircleFilled(partile_pos[i], partile_size[i], ImGui::GetColorU32(main_color), 1);
            ImGui::ImRotateEnd(partile_rotate[i]);
        }
    }


    static void DrawMain()
    {

        MenuPlayerLists::Render();


        if (GameData.Config.Window.Setting)
        {
            const ImVec2 Spacing = ImGui::GetStyle().ItemSpacing;
            ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
            ImGui::SetNextWindowSize({ 990, 650 });
            ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos + ImVec2{ 80, 50 });

            ImGui::Begin("MENU", &GameData.Config.Window.Setting, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize);
            {
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, colors_alpha);
                anim_speed = ImGui::GetIO().DeltaTime * 10.f;

                auto draw = ImGui::GetWindowDrawList();
                const auto& p = ImGui::GetWindowPos();

                draw->AddRectFilled(p, p + ImVec2(990, 650), ImGui::GetColorU32(winbg_color), 10.f);

                if (iTabs != 2)
                {
                    draw->AddRectFilled(p + ImVec2(214, 14), p + ImVec2(980, 54), ImGui::GetColorU32(child_col[0]), 10.f);
                }
                else
                {
                    draw->AddRectFilled(p + ImVec2(214, 14), p + ImVec2(980, 100), ImGui::GetColorU32(child_col[0]), 10.f);
                }

                draw->AddRectFilled(p, p + ImVec2(200, 650), ImGui::GetColorU32(child_col[0]), 10.f, ImDrawFlags_RoundCornersLeft);
                               

                ImGui::PushFont(heavy_font_big);
                draw->AddText(center_text(p, p + ImVec2(200, 80), "SP"), ImColor(text_color[0].x, text_color[0].y, text_color[0].z, colors_alpha / 7), "SP");
                ImGui::PopFont();

                ImGui::PushFont(heavy_font);
                draw->AddText(center_text(p, p + ImVec2(200, 80), "S1mple"), ImGui::GetColorU32(text_color[0]), "S1mple");
                ImGui::PopFont();

                ImGui::Dummy({ 0, 40 });

                DrawTabs();



                ImGui::PopStyleVar();

            }

            ImGui::End();
        }
    }
};