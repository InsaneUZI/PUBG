// Dear ImGui: standalone example application for DirectX 11

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "Overlay.h"
#include "ESP.h"
#include "Texture.h";
#include <Common/Data.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>
#include <d3d11.h>
#include <tchar.h>
#include <Utils/Throttler.h>
#include <Overlay/Menu/Menu.h>
#include "Style.h"
#include "Map.h"
#include "MThreadRenderer.h"
#include <thread>
#include "imgui/Fonts.h"
#include "imgui/imgui_settings.h"
#include "imgui/images.h"

#include <D3DX11.h>
#pragma comment (lib, "d3dx11.lib")

// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static int int_theme = false;
static bool col_theme = false;
static bool disabled_menu = false;
ID3D11ShaderResourceView* bg = nullptr;
static bool image_loaded = false;

void SetCustomColors()
{
    if (int_theme == 0)
        col_theme = false;
    else
        col_theme = true;

    if (!disabled_menu) {
        text_color[0] = ImLerp(text_color[0], col_theme ? ImColor(222, 225, 230, 255) : ImColor(41, 44, 49, 255), anim_speed);
        text_color[1] = ImLerp(text_color[1], col_theme ? ImColor(52, 53, 54, 255) : ImColor(41, 44, 49, 125), anim_speed);

        winbg_color = ImLerp(winbg_color, col_theme ? ImColor(17, 17, 18, 255) : ImColor(215, 220, 226, 255), anim_speed);

        background_color = ImLerp(background_color, col_theme ? ImColor(32, 32, 34, 255) : ImColor(198, 206, 217, 255), anim_speed);

        bg_color = ImLerp(bg_color, !col_theme ? ImColor(22, 22, 24, 255) : ImColor(230, 233, 238, 255), anim_speed);

        scroll_col = ImLerp(scroll_col, col_theme ? ImColor(222, 225, 230, 255) : ImColor(41, 44, 49, 255), anim_speed);

        child_col[0] = ImLerp(child_col[0], col_theme ? ImColor(22, 22, 24, 255) : ImColor(230, 233, 238, 255), anim_speed);
        child_col[1] = ImLerp(child_col[1], col_theme ? ImColor(27, 27, 29, 255) : ImColor(222, 225, 230, 255), anim_speed);

        tab_color = ImLerp(tab_color, col_theme ? ImColor(27, 27, 29, 255) : ImColor(245, 245, 245, 255), anim_speed);

        main_color = ImLerp(main_color, col_theme ? ImColor(41, 44, 49, 255) : ImColor(41, 44, 49, 255), anim_speed);

        colors_alpha = ImLerp(colors_alpha, 1.f, anim_speed);
    }

    if (disabled_menu) {
        text_color[0].w = ImLerp(text_color[0].w, 0.f, anim_speed);
        text_color[1].w = ImLerp(text_color[1].w, 0.f, anim_speed);

        winbg_color.w = ImLerp(winbg_color.w, 0.f, anim_speed);

        background_color.w = ImLerp(background_color.w, 0.f, anim_speed);
        scroll_col.w = ImLerp(scroll_col.w, 0.f, anim_speed);

        child_col[0].w = ImLerp(child_col[0].w, 0.f, anim_speed);
        child_col[1].w = ImLerp(child_col[1].w, 0.f, anim_speed);

        tab_color.w = ImLerp(tab_color.w, 0.f, anim_speed);
        main_color.w = ImLerp(main_color.w, 0.f, anim_speed);

        colors_alpha = ImLerp(colors_alpha, 0.f, anim_speed);
    }
}


void LoadImages()
{
    D3DX11_IMAGE_LOAD_INFO iInfo;
    ID3DX11ThreadPump* threadPump{ nullptr };

    if (bg == nullptr && !image_loaded) {
        D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, background, sizeof(background), &iInfo, threadPump, &bg, 0);
        image_loaded = true;
    }
}


int Overlay::Init()
{
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 0, 0, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    GameData.Config.Overlay.hWnd = hwnd;


    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    Texture::LoadTextures(g_pd3dDevice, "Assets/Icons");
    Texture::LoadTextures(g_pd3dDevice, "Assets/Insignias");
    Texture::LoadTextures(g_pd3dDevice, "Assets/Icons/Item/All", true);

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    io.ConfigViewportsNoDefaultParent = true;
    io.ConfigViewportsNoAutoMerge = true;
    io.ConfigViewportsNoTaskBarIcon = true;

    ArialFont = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyhbd.ttc", 18.f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    io.Fonts->AddFontFromMemoryTTF(&SFProDisplaySemibold, sizeof SFProDisplaySemibold, 18, NULL, io.Fonts->GetGlyphRangesCyrillic());
    bold_font = io.Fonts->AddFontFromMemoryTTF(&SFProDisplayBold, sizeof SFProDisplayBold, 17, NULL, io.Fonts->GetGlyphRangesCyrillic());
    semibold_font = io.Fonts->AddFontFromMemoryTTF(&SFProDisplaySemibold, sizeof SFProDisplaySemibold, 15, NULL, io.Fonts->GetGlyphRangesCyrillic());
    heavy_font = io.Fonts->AddFontFromMemoryTTF(&SFProDisplayHeavy, sizeof SFProDisplayHeavy, 35, NULL, io.Fonts->GetGlyphRangesCyrillic());
    heavy_font_big = io.Fonts->AddFontFromMemoryTTF(&SFProDisplayHeavy, sizeof SFProDisplayHeavy, 70, NULL, io.Fonts->GetGlyphRangesCyrillic());
    medium_font = io.Fonts->AddFontFromMemoryTTF(&SFProDisplayMedium, sizeof SFProDisplayMedium, 14, NULL, io.Fonts->GetGlyphRangesCyrillic());
    icon_font = io.Fonts->AddFontFromMemoryTTF(&icomoon, sizeof icomoon, 18, NULL, io.Fonts->GetGlyphRangesCyrillic());
    icon_font_small = io.Fonts->AddFontFromMemoryTTF(&icomoon, sizeof icomoon, 10, NULL, io.Fonts->GetGlyphRangesCyrillic());
    icon_font_medium = io.Fonts->AddFontFromMemoryTTF(&icomoon, sizeof icomoon, 14, NULL, io.Fonts->GetGlyphRangesCyrillic());
    TitleFont = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyhbd.ttc", 14.f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    MenuFont = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyhbd.ttc", 15.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    PlayersFont = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyhbd.ttc", 24.f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    MapFont = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyhbd.ttc", 30.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    ImVec4 clear_color = ImVec4(32.f / 255.0f, 32.f / 255.0f, 32.f / 255.0f, 0.00f);

    ImGuiStyle& s = ImGui::GetStyle();

    s.FramePadding = ImVec2(10, 10);
    s.ItemSpacing = ImVec2(10, 12);
    s.FrameRounding = 4.f;
    s.WindowRounding = 10.f;
    s.WindowBorderSize = 0.f;
    s.FrameBorderSize = 0.f;
    s.PopupBorderSize = 0.f;
    s.WindowPadding = ImVec2(10, 10);
    s.ChildBorderSize = 1.f;
    // s.Colors[ImGuiCol_WindowBg] = winbg_color;
    s.Colors[ImGuiCol_Border] = ImVec4(0.f, 0.f, 0.f, 0.f);
    s.PopupRounding = 5.f;
    s.PopupBorderSize = 1.3f;
    s.ScrollbarSize = 5.f;
    s.ScrollbarRounding = 10.f;

    bool done = false;

    CameraData Camera;
    float TimeSeconds = 0.f;
    auto hScatter = mem.CreateScatterHandle();

    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            SetCustomColors();

            if (GameData.Config.Window.IsLogin)
            {
                if (!GameData.Config.Overlay.UseThread)
                {
                    {
                        mem.AddScatterRead(hScatter, GameData.PlayerCameraManager + GameData.Offset["CameraCacheLocation"], &Camera.Location);
                        mem.AddScatterRead(hScatter, GameData.PlayerCameraManager + GameData.Offset["CameraCacheRotation"], &Camera.Rotation);
                        mem.AddScatterRead(hScatter, GameData.PlayerCameraManager + GameData.Offset["CameraCacheFOV"], &Camera.FOV);

                    }
                    mem.AddScatterRead(hScatter, GameData.UWorld + GameData.Offset["TimeSeconds"], &TimeSeconds);
                    mem.ExecuteReadScatter(hScatter);

                    GameData.Camera = Camera;
                    GameData.WorldTimeSeconds = TimeSeconds;

                }

                DrawOverlayHandler();
                Menu::DrawMain();
            }
        }

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        g_pSwapChain->Present((!GameData.Config.Overlay.VSync) ? 0 : 1, 0);

    }
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    GameData.Config.Window.Main = false;
    mem.CloseScatterHandle(hScatter);


    return 0;
}

void EnableMouseTransparency()
{
    HWND hWnd = (HWND)ImGui::GetCurrentWindow()->Viewport->PlatformHandle;
    LONG_PTR exStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
    exStyle |= WS_EX_TRANSPARENT | WS_EX_LAYERED;
    SetWindowLongPtr(hWnd, GWL_EXSTYLE, exStyle);
}

void Overlay::DrawOverlayHandler()
{
    if (GameData.Config.Overlay.Enable)
    {
        auto selectedMonitor = ImGui::GetPlatformIO().Monitors[GameData.Config.Overlay.MonitorCurrentIdx];
        ImGui::SetNextWindowSize(ImVec2(selectedMonitor.MainSize.x, selectedMonitor.MainSize.y));
        ImGui::SetNextWindowPos((ImVec2(selectedMonitor.MainPos.x, selectedMonitor.MainPos.y)));

        ImGuiWindowFlags WindowFlags = 0;

        WindowFlags |= ImGuiWindowFlags_NoTitleBar;
        WindowFlags |= ImGuiWindowFlags_NoResize;
        WindowFlags |= ImGuiWindowFlags_NoMove;;
        WindowFlags |= ImGuiWindowFlags_NoDocking;
        WindowFlags |= ImGuiWindowFlags_NoInputs;

        if (!GameData.Config.Overlay.FusionMode)
        {
            WindowFlags |= ImGuiWindowFlags_NoBackground;
        }

        ImGuiStyle& style = ImGui::GetStyle();
        float OriginalBorderSize = style.WindowBorderSize;
        ImVec4 OriginalWindowBgColor = style.Colors[ImGuiCol_WindowBg];
        ImVec2 OriginalWindowPadding = style.WindowPadding;

        style.WindowBorderSize = 0.0f;
        style.WindowPadding = { 0.f, 0.f };
        style.Colors[ImGuiCol_WindowBg] = GameData.Config.Overlay.FusionColor;
        ImGui::Begin("ESP Overlay", nullptr, WindowFlags);
        {
            if (!GameData.Config.Overlay.FusionMode) EnableMouseTransparency();
            ImGuiIO& io = ImGui::GetIO(); (void)io;

            ESP::DrawFPS(io);
            ESP::DrawESP();
        }
        ImGui::End();

        style.WindowBorderSize = OriginalBorderSize;
        style.WindowPadding = OriginalWindowPadding;
        style.Colors[ImGuiCol_WindowBg] = OriginalWindowBgColor;
    }
}

// Helper functions
bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
