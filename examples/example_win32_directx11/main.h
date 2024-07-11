#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <wininet.h>
#include <dwmapi.h>
#include "colors.h"
#include <D3DX11tex.h>
#include "imgui_internal.h"
#pragma comment(lib, "D3DX11.lib")
#include "user_circle.h"
#include "icon.h"
//#include "kogtevran_icon.h"
#include "nohurtcam_icon.h"
#include "velocity_icon.h"
#include "visuals_icon.h"
#include "inter.h"
#include "Dots.h"
#include "arrow_down_icon.h"
#include "segue.h"
#include <string>

#include "resource.h"

#include "RestAPI/Core/Client.hpp"
#include "RestAPI/Errors/ErrorHandler.hpp"
#include "RestAPI/Utils/Utils.hpp"

#include "Configs/ConfigManager.hpp"
#include "Inject/Injector.hpp"

#include "Features/Features.hpp"
#include "Features/License/License.hpp"

#include <md5.h>
#include <md5.cpp>

#include <nlohmann/json.hpp>
#include <StringUtils.h>
#include <thread>

#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "Urlmon.lib")

static RestAPI::Client client("http://api.destructiqn.com:2086");

Features::UnlimitedCPS* unlimitedCPS = new Features::UnlimitedCPS();
Features::Spammer* spammer = new Features::Spammer();
//Features::Kogtevran* kogtevran = new Features::Kogtevran();
Features::NoHurtCam* noHurtCam = new Features::NoHurtCam();
Features::Velocity* velocity = new Features::Velocity();
Features::Visuals* visuals = new Features::Visuals();

static const char* injectWindowName = "VimeWorld";

static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace ImGui;
using namespace std;
using json = nlohmann::json;

#define WIDTH  852 
#define HEIGHT 550

ID3D11ShaderResourceView* lg = nullptr;

POINT mouse;
HWND hwnd;
RECT rc;

int tabs = 0;
bool menu = true;
char key[64] = { "" };

char login[12] = { "" };
char password[64] = { "" };

char email[64] = { "" };
char reg_login[12] = { "" };
char reg_password[64] = { "" };
char reg_password1[64] = { "" };

ImFont* ico;
//ImFont* kogtevran_ico;
ImFont* nohurtcam_ico;
ImFont* velocity_ico;
ImFont* visuals_ico;

ImFont* minimize;
ImFont* minimize2;
ImFont* minimize3;
ImFont* contact_logo;
ImFont* logo;
ImFont* dots;
ImFont* segu;

int logo_positionX = 20, logo_positionY = 480;

bool tabs_4_b = 0;
bool tabs_3_b = 0;
bool tabs_2_b = 0;
bool tabs_1_b = 0;

float tab_alpha = 0, auth_alpha = 0.f;
float tab_add = 0, auth_add = 0;
int active_tab = 0, active_auth = 0;

void initFeatures() {
    Features::License::features[unlimitedCPS->name] = unlimitedCPS;
    Features::License::features[spammer->name] = spammer;
    //Features::License::features[kogtevran->name] = kogtevran;
    Features::License::features[noHurtCam->name] = noHurtCam;
    Features::License::features[velocity->name] = velocity;
    Features::License::features[visuals->name] = visuals;
}

void TextCentered(std::string text, float y) {
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosY(y);
    ImGui::SetCursorPosX((windowWidth - textWidth + 235) * 0.5f);
    ImGui::Text(text.c_str());
}

void CustomStyleColor() // Отрисовка цветов
{
    ImGuiStyle& s = ImGui::GetStyle();

    //s.Colors[ImGuiCol_WindowBg] = ImColor(28, 27, 32, 255);
    s.Colors[ImGuiCol_WindowBg] = ImColor(51, 51, 51, 255);
    s.Colors[ImGuiCol_Border] = ImColor(58, 59, 65, 255);
    s.Colors[ImGuiCol_ChildBg] = ImColor(19, 19, 19, 0);
    s.Colors[ImGuiCol_TextSelectedBg] = ImColor(85, 85, 85, 150);
    s.WindowBorderSize = 0;
    s.WindowPadding = ImVec2(0, 0);
    s.WindowRounding = 11.f;
    s.FrameRounding = 11.f;
    s.FramePadding = ImVec2(10, 0);
}
float input_size = 250.f;



void AlignForWidth(float width, float y = 0.f, float alignment = 0.5f) // Center widgets :)
{
    ImGuiStyle& style = ImGui::GetStyle();
    float avail = ImGui::GetContentRegionAvail().x;
    float off = (avail - width) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosY(y);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
}

void mouse_window() {

    GetWindowRect(hwnd, &rc);

    if (ImGui::GetWindowPos().x != 0 || ImGui::GetWindowPos().y != 0)
    {
        MoveWindow(hwnd, rc.left + ImGui::GetWindowPos().x, rc.top + ImGui::GetWindowPos().y, WIDTH, HEIGHT, TRUE);
        ImGui::SetWindowPos(ImVec2(0.f, 0.f));
    }

    GetCursorPos(&mouse);

}

inline bool fileExists(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

bool DropdownList(const char* label, const char** items, int items_length, const char** current_item) {
    bool widget_used = false;

    ImGuiComboFlags flags = ImGuiComboFlags_None;

    ImGuiStyle& style = ImGui::GetStyle();
    float w = ImGui::CalcItemWidth();
    float spacing = style.ItemInnerSpacing.x;
    float button_sz = ImGui::GetFrameHeight();
    ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
    if (ImGui::BeginCombo(label, *current_item, flags)) {
        for (int n = 0; n < items_length; n++)
        {
            bool is_selected = (*current_item == items[n]);
            if (ImGui::Selectable(items[n], is_selected)) {
                *current_item = items[n];
                widget_used = true;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    ImGui::SameLine(0, style.ItemInnerSpacing.x);

    return widget_used;
}

void hsv_to_rgb(float* hsv, toml::value::array_type& rgb) {
    for (int i = 0; i < rgb.size(); i++)
        rgb[i].as_integer() = (int64_t)round(hsv[i] * 255.0F);
}


void SelectEspColor(
    ImVec2& window_pos, ImVec2& start_cursor_pos, float esp_color[3],
    bool& color_picker_enabled, bool& outline_color_picker_enabled, bool& filling_color_picker_enabled, const Features::Visuals::ESP& esp_module
) {
    const char* color_button_text = u8"Color ";
    ImVec2 color_button_size = CalcTextSize(color_button_text);

    color_button_size.x += 5;
    color_button_size.y += 5;

    SetCursorPos(ImVec2(start_cursor_pos.x, start_cursor_pos.y + 35));
    ImGui::Text(color_button_text);

    SetCursorPos(ImVec2(start_cursor_pos.x, start_cursor_pos.y + 35));
    RenderArrow(
        GetWindowDrawList(),
        ImVec2(window_pos.x + GetCursorPosX() + color_button_size.x - 12, window_pos.y + GetCursorPosY() + 8 - GetScrollY()),
        0xffffffff,
        color_picker_enabled ? ImGuiDir_Up : ImGuiDir_Down,
        0.5f
    );

    SetCursorPos(ImVec2(start_cursor_pos.x, start_cursor_pos.y + 35));
    if (ImGui::InvisibleButton("player_esp_color_button", ImVec2(color_button_size.x + 5, color_button_size.y))
        && (!outline_color_picker_enabled && !filling_color_picker_enabled))
        color_picker_enabled = !color_picker_enabled;

    if (color_picker_enabled) {
        SetCursorPos(ImVec2(start_cursor_pos.x, start_cursor_pos.y + 75));
        if (ImGui::ColorPicker3f(esp_color))
            esp_module.update_box_color();
    }
}
