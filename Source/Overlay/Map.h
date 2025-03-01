#pragma once

#include <winsock2.h>
#include <windows.h>
#include <Common/Data.h>
#include "DMARender.h"
#include "Texture.h"
#include <d3d11.h>


constexpr float DegreesToRadians(float degrees)
{
    return degrees * (3.14159265358979323846f / 180.0f);
}

class Map
{
public:
    static float ScaleConversion(float Location)
    {
        float Scale = 1.f;
        if (GameData.MapName == "Baltic_Main") Scale = 0.99609375f;
        return Location / GameData.Radar.MapSize.ImageSize * GameData.Radar.ImageMapHeight * (GameData.Radar.MapSize.ImageSize == 816000.f ? Scale : 1);
    }

    static ImVec2 WorldToRadar(const RECT& Rect, const DMARender::MapTransform& mTransform, FVector WorldLocation)
    {
        WorldLocation.X = ScaleConversion(GameData.Radar.WorldOriginLocation.X + WorldLocation.X);
        WorldLocation.Y = ScaleConversion(GameData.Radar.WorldOriginLocation.Y + WorldLocation.Y);

        ImVec2 MapLocation = { WorldLocation.X, WorldLocation.Y };

        MapLocation.x *= mTransform.mapZoom;
        MapLocation.y *= mTransform.mapZoom;

        MapLocation.x += mTransform.dragOffsetX;
        MapLocation.y += mTransform.dragOffsetY;

        MapLocation.x += (float)Rect.left;
        MapLocation.y += (float)Rect.top;

        return MapLocation;
    }

    static void DrawFPS(const RECT& Rect)
    {
        ImGuiIO& io = ImGui::GetIO();
        int fps = GameData.Config.Overlay.VSync ? static_cast<int>(io.Framerate) : (io.Framerate * 2.0f);
        ImVec2 TextPosition = ImVec2(Rect.left + 20, Rect.top + 40);
        RenderHelper::Text(std::format("FPS: {}\nBeta Version", static_cast<int>(fps)).c_str(), TextPosition, IM_COL32(255, 255, 255, 255), 16, false, false, false);
    }

    static void DrawButton(const RECT& Rect)
    {
        ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
        ImGui::SetNextWindowPos({ (float)Rect.right - 105, (float)Rect.top + 35 });
        ImGui::Begin("ButtonLists", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        std::string SettingIcon = "Assets/Icons/Radar/Setting.png";
        std::string PlayersIcon = "Assets/Icons/Radar/Players.png";

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(18.f / 255.f, 18.f / 255.f, 18.f / 255.f, 200.f / 255.f));
        if (GameData.Config.Window.Players) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(149.0f / 255.0f, 1.0f / 255.0f, 247.f / 255.0f, 200.f / 255.f));
        if (ImGui::ImageButton((ImTextureID)GImGuiTextureMap[PlayersIcon].Texture, { 25, 25 }, ImVec2(0, 0), ImVec2(1, 1), 5, ImColor(0, 0, 0, 0), ImColor(255, 255, 255, 255)))
        {
            GameData.Config.Window.Players = !GameData.Config.Window.Players;
            ImGui::PopStyleColor();
        }
        if (GameData.Config.Window.Players) ImGui::PopStyleColor();

        ImGui::SameLine();

        if (GameData.Config.Window.Setting) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(149.0f / 255.0f, 1.0f / 255.0f, 247.f / 255.0f, 200.f / 255.f));
        if (ImGui::ImageButton((ImTextureID)GImGuiTextureMap[SettingIcon].Texture, { 25, 25 }, ImVec2(0, 0), ImVec2(1, 1), 5, ImColor(0, 0, 0, 0), ImColor(255, 255, 255, 255)))
        {
            GameData.Config.Window.Setting = !GameData.Config.Window.Setting;
            ImGui::PopStyleColor();
        }
        if (GameData.Config.Window.Setting) ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::End();
    }

    static void DrawStates(const RECT& Rect)
    {
        ImGui::PushFont(MapFont);
        ImVec2 CenterPosition = { float(Rect.left + Rect.right) / 2, float(Rect.top + Rect.bottom) / 2 };
        std::string Text = U8("等待游戏启动");
        switch (GameData.Scene)
        {
        case Scene::Lobby:
            Text = U8("等待进入游戏");
            break;
        case Scene::Gaming:
            Text = Utils::StringToUTF8(std::format("游戏中 ({})", Utils::FormatMinutes(GameData.WorldTimeSeconds)));
            break;
        default:
            break;
        }

        //RenderHelper::Text(U8("PUBG HELPER"), CenterPosition, IM_COL32(255, 255, 255, 255), 60, true, true, false);
        std::string IconUrl = "Assets/Icons/Radar/T_TitleScreen_Logo.png";
        CenterPosition.y -= 20;
        if (GImGuiTextureMap[IconUrl].Width > 0) {
            float Zoom = 0.4f;
            ImVec2 IconSize = { GImGuiTextureMap[IconUrl].Width * Zoom, GImGuiTextureMap[IconUrl].Height * Zoom };
            RenderHelper::AddImageRotated((ImTextureID)GImGuiTextureMap[IconUrl].Texture, CenterPosition, IconSize, 0, false);
        }
        CenterPosition.y += 100;
        RenderHelper::Text(Text.c_str(), CenterPosition, IM_COL32(255, 255, 255, 180), 30, true, true, false);
        ImGui::PopFont();
    }

    // 定义一个名为 DrawOverlay 的静态函数，该函数用于在指定的矩形区域内绘制覆盖层。
    static void DrawOverlay(const RECT& Rect, DMARender::MapTransform& mTransform)
    {
        // 获取ImGui的背景绘图列表，用于后续绘制操作。
        ImDrawList* fgDrawList = ImGui::GetBackgroundDrawList();

        // 从数据源中获取所有玩家的信息。
        auto Players = Data::GetPlayers();
        for (auto Item : Players)
        {
            // 对每个玩家进行处理。
            auto Player = Item.second;

            // 如果玩家总是创建物理状态为4或处于死亡状态，则跳过此玩家。
            if (Player.bAlwaysCreatePhysicsState == 4 || (Player.State == CharacterState::Dead))
                continue;

            // 将玩家的世界坐标转换为雷达（屏幕）坐标。
            auto MapLocation = WorldToRadar(Rect, mTransform, Player.Location);

            // 如果是当前玩家自己，则更新中心位置并调整拖拽偏移量以保持玩家在屏幕中央。
            if (Player.IsMe)
            {
                Player.Name = ""; // 清空当前玩家的名字，可能为了不显示自己的名字。

                // 计算视口中心点的位置。
                float viewportCenterX = (Rect.right - Rect.left) / 2.0f + Rect.left;
                float viewportCenterY = (Rect.bottom - Rect.top) / 2.0f + Rect.top;

                // 更新拖拽偏移量，使地图跟随玩家移动。
                mTransform.dragOffsetX += viewportCenterX - MapLocation.x;
                mTransform.dragOffsetY += viewportCenterY - MapLocation.y;
            }

            // 再次更新MapLocation以反映新的偏移。
            MapLocation = WorldToRadar(Rect, mTransform, Player.Location);

            // 根据玩家信息获取要使用的颜色。
            RenderHelper::PlayerColor PlayerColors = RenderHelper::GetPlayerColor(Player);

            // 如果玩家处于昏昏欲睡的状态，设置颜色为红色。
            if (Player.State == CharacterState::Groggy)
            {
                PlayerColors.teamNumberColor = ImColor(255, 0, 0, 255);
            }

            // 如果玩家是我的队友，设置颜色为绿色，并更新队伍ID。
            if (Player.IsMyTeam)
            {
                //PlayerColors.teamNumberColor = ImColor(0, 255, 0, 255);
               // Player.TeamID = Player.SquadMemberIndex + 1;
                Player.Name = "";
            }

            // 计算玩家瞄准方向的角度。
            FVector AimDirection = FRotator(0.0f, Player.AimOffsets.Yaw, 0.0f).GetUnitVector();
            FVector2D Direction = FVector2D{ AimDirection.X, AimDirection.Y };
            float AngleRadians = atan2(Direction.Y, Direction.X);
            float AngleDegrees = AngleRadians; // 角度值未被转换成度数，这可能是错误的。

           
            // 绘制玩家的图标和相关信息。
            RenderHelper::Draw2DRadarPlayerCircleWithText(Player.Type == EntityType::AI ? "AI" : std::to_string(Player.TeamID).c_str(), PlayerColors.teamNumberColor, 10, MapLocation, AngleDegrees, fgDrawList, Player.IsMe, Player.AimOffsets.Yaw, Player.IsAimMe);
        }

        
        /*// 绘制白色的安全区圆圈。
        {
            // 将安全区的世界坐标转换为雷达（屏幕）坐标。
            auto MapBlueZonePosition = WorldToRadar(Rect, mTransform, { Data::GetBlueZonePosition().X - GameData.Radar.WorldOriginLocation.X, Data::GetBlueZonePosition().Y - GameData.Radar.WorldOriginLocation.Y, 0 });
            // 计算缩放后的安全区半径。
            auto MapBlueZoneRadius = Data::GetBlueZoneRadius() * mTransform.mapZoom / 100.f;
            // 设置颜色为白色。
            ImU32 color = IM_COL32(255, 255, 255, 255);
            int num_segments = 200; // 圆的分段数，越高越平滑。
            // 使用背景绘图列表绘制圆圈。
            fgDrawList->AddCircle(MapBlueZonePosition, MapBlueZoneRadius, color, num_segments);
        }

        // 绘制蓝色的安全区圆圈。
        {
            // 将安全区的世界坐标转换为雷达（屏幕）坐标。
            auto MapSafetyZonePosition = WorldToRadar(Rect, mTransform, { Data::GetSafetyZonePosition().X - GameData.Radar.WorldOriginLocation.X, Data::GetSafetyZonePosition().Y - GameData.Radar.WorldOriginLocation.Y, 0 });
            // 计算缩放后的安全区半径。
            auto MapSafetyZoneRadius = Data::GetSafetyZoneRadius() * mTransform.mapZoom / 100.f;
            // 设置颜色为蓝色。
            ImU32 color = IM_COL32(0, 0, 255, 255);
            int num_segments = 200;
            // 使用背景绘图列表绘制圆圈。
            fgDrawList->AddCircle(MapSafetyZonePosition, MapSafetyZoneRadius, color, num_segments);
        }*/
    }

    //static void DrawOverlayYaw(const RECT& Rect, DMARender::MapTransform& mTransform)
    //{
    //    ImDrawList* fgDrawList = ImGui::GetBackgroundDrawList();
    //    // 假设你的位置和朝向为中心点
    //    FVector2D myPosition = { (Rect.right - Rect.left) / 2.0f, (Rect.bottom - Rect.top) / 2.0f };
    //    float myAngleRadians = 0.0f; // 自己的朝向保持不变，始终为0度，即朝向上方
    //    auto Players = Data::GetPlayers();
    //    for (auto Item : Players)
    //    {
    //        auto Player = Item.second;
    //        // 获取玩家在地图上的位置
    //        auto MapLocation = WorldToRadar(Rect, mTransform, Player.Location);
    //        if (!Player.IsMe) // 如果不是本地玩家
    //        {
    //            // 计算相对于你的旋转角度
    //            float playerAngleRadians = DegreesToRadians(Player.AimOffsets.Yaw);
    //            // 计算其他玩家的位置相对于你的旋转
    //            FVector2D relativePosition = { MapLocation.x - myPosition.X, MapLocation.y - myPosition.Y };
    //            float cosAngle = cos(playerAngleRadians);
    //            float sinAngle = sin(playerAngleRadians);
    //            float rotatedX = relativePosition.X * cosAngle - relativePosition.Y * sinAngle;
    //            float rotatedY = relativePosition.X * sinAngle + relativePosition.Y * cosAngle;
    //            // 更新其他玩家的位置
    //            MapLocation.x = myPosition.X + rotatedX;
    //            MapLocation.y = myPosition.X + rotatedY;
    //        }
    //        else
    //        {
    //            // 如果是本地玩家，保持位置不变，并在中心显示
    //            MapLocation = { myPosition.X,myPosition.Y };
    //        }
    //        // 绘制玩家名称和图标
    //        RenderHelper::PlayerColor PlayerColors = RenderHelper::GetPlayerColor(Player);
    //        RenderHelper::Text(std::format("{}", Player.Name).c_str(), { MapLocation.x ,MapLocation.y + 20.0f }, IM_COL32(255, 255, 255, 255), 12, true, true, false);
    //        FVector AimDirection = FRotator(0.0f, Player.AimOffsets.Yaw, 0.0f).GetUnitVector();
    //        FVector2D Direction = FVector2D{ AimDirection.X, AimDirection.Y };
    //        float AngleRadians = atan2(Direction.Y, Direction.X);
    //        float AngleDegrees = AngleRadians;
    //        // 只有其他玩家的图标相对于你旋转
    //        float angleForIcon = Player.IsMe ? 0.0f : AngleDegrees;
    //        RenderHelper::Draw2DRadarPlayerCircleWithText(Player.Type == EntityType::AI ? "AI" : std::to_string(Player.TeamID).c_str(), PlayerColors.teamNumberColor, 10, MapLocation, angleForIcon, fgDrawList, Player.IsAimMe, Player.AimOffsets.Yaw);
    //    }
    //}


    static void Render(ID3D11Device* g_pd3dDevice)
    {
        ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
        ImGui::Begin("Map", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

        RECT Rect;
        GetWindowRect(GameData.Config.Overlay.hWnd, &Rect);

        DrawStates(Rect);
  

    }



};