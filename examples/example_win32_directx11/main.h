#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <dwmapi.h>
#include "colors.h"
#include <D3DX11tex.h>
#include "imgui_internal.h"
#pragma comment(lib, "D3DX11.lib")
#include "user_circle.h"
#include "icon.h"
#include "inter.h"
#include "Dots.h"
#include "segue.h"
#include <string>
#include "resource.h"

#include "rest/client.hpp"
#include "rest/utils/errorhandler.hpp"
#include "rest/utils/utils.hpp"

#include <md5.h>
#include <md5.cpp>

#include <nlohmann/json.hpp>
#include <StringUtils.h>

static Destruction::RestAPI::Client client("https://destructiqn.com:9990");

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
ImFont* minimize;
ImFont* minimize2;
ImFont* minimize3;
ImFont* contact_logo;
ImFont* logo;
ImFont* dots;
ImFont* segu;

int logo_positionX = 20, logo_positionY = 480;
bool tabs_2_b = 0;
bool tabs_1_b = 0;

float tab_alpha = 0, auth_alpha = 0.f;
float tab_add = 0, auth_add = 0;
int active_tab = 0, active_auth = 0;

__forceinline bool isLicenseExist(string featureName, uint8_t tabNumber) {
    json features;
    if (client.GetFeatures(client.user.name, client.user.password, client.user.session, features)) {
        if (features.contains(featureName)) {
            if (features[featureName].get<int>()) {
                tabs = tabNumber;
                return true;
            }
            else {
                MessageBoxA(Destruction::RestAPI::ErrorHandler::hWindow, "Лицензия истекла :(\nКупить новую можно здесь -> https://vk.com/destructiqn", "Destruction Loader", MB_ICONERROR);
                return false;
            }
        }
        else {
            MessageBoxA(Destruction::RestAPI::ErrorHandler::hWindow, "У Вас нет лицензии :(\nПриобрести её можно здесь -> https://vk.com/destructiqn", "Destruction Loader", MB_ICONERROR);
            return false;
        }
    }
    return false;
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
