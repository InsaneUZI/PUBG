#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include <string>

inline ImVec4 main_color;

inline ImVec4 scroll_col;

inline float colors_alpha;

inline ImVec4 child_col[2];

inline ImVec4 check_mark_col = ImColor(255, 255, 255, 255);

inline ImVec4 background_color = ImColor(198, 206, 217, 255);

inline ImVec4 tab_color = ImColor(245, 245, 245, 255);

inline ImVec4 text_color[2];

inline ImVec4 winbg_color;

inline ImVec4 bg_color;

inline ImVec2 frame_size(203, 16);

inline ImFont* heavy_font_big;

inline ImFont* heavy_font;

inline ImFont* bold_font;

inline ImFont* medium_font;

inline ImFont* icon_font;

inline ImFont* icon_font_medium;

inline ImFont* icon_font_small;

inline ImFont* semibold_font;

inline ImFont* ArialFont;
inline ImFont* TitleFont;
inline ImFont* MenuFont;
inline ImFont* PlayersFont;
inline ImFont* MapFont;

inline float anim_speed = 12.f;

inline ImColor GetColorWithAlpha(ImColor color, float alpha)
{
    return ImColor(color.Value.x, color.Value.y, color.Value.z, alpha);
}


inline void AddShadowRectFilled(ImVec2 min, ImVec2 max, ImColor color)
{
    static float offset = 0.f;

    for (int i = 0.f; i < 50; i++)
    {
        if(offset < 5.f)
        offset += 0.0056f;
        ImGui::GetWindowDrawList()->AddRect(min - ImVec2(offset, offset), max + ImVec2(offset, offset), ImColor(0, 0, 0, 130 - i), 10);
    }
}

inline void Pickerbox(std::string label, bool* v, float col[4])
{
    std::string picker_name = "##picker" + label;

    frame_size = ImVec2(160, 16);
    ImGui::Checkbox(label.c_str(), v); ImGui::SetCursorScreenPos(ImGui::GetItemRectMin());
    frame_size = ImVec2(203, 16);
    ImGui::ColorEdit4(picker_name.c_str(), (float*)col, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
}

inline void Keybindbox(std::string label, bool* v, int* key)
{
    std::string picker_name = "##keybind" + label;

    frame_size = ImVec2(160, 16);
    ImGui::Checkbox(label.c_str(), v); ImGui::SetCursorScreenPos(ImGui::GetItemRectMin());
    frame_size = ImVec2(203, 16);
    ImGui::Keybind(picker_name.c_str(), key);
}


inline ImVec2 center_text(ImVec2 min, ImVec2 max, const char* text)
{
    return min + (max - min) / 2 - ImGui::CalcTextSize(text) / 2;
}

inline ImVec4 ImColorToImVec4(const ImColor& color)
{
    return ImVec4(color.Value.x, color.Value.y, color.Value.z, color.Value.w);
}

inline ImGuiColorEditFlags PickerFlags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar;