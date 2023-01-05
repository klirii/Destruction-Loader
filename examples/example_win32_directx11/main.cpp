#include "main.h"

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    WNDCLASSEXW wc;
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = nullptr;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = L"ImGui";
    wc.lpszClassName = L"Example";
    wc.hIconSm = LoadIcon(0, IDI_APPLICATION);

    RegisterClassExW(&wc);
    hwnd = CreateWindowExW(NULL, wc.lpszClassName, L"Example", WS_POPUP, (GetSystemMetrics(SM_CXSCREEN) / 2) - (WIDTH / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (HEIGHT / 2), WIDTH, HEIGHT, 0, 0, 0, 0);

    SetWindowLongA(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    mouse;
    rc = { 0 };    
    GetWindowRect(hwnd, &rc);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.Fonts->AddFontFromMemoryTTF(&inter, sizeof inter, 23, NULL, io.Fonts->GetGlyphRangesCyrillic());
    minimize = io.Fonts->AddFontFromMemoryTTF(&inter, sizeof inter, 17, NULL, io.Fonts->GetGlyphRangesCyrillic());
    minimize2 = io.Fonts->AddFontFromMemoryTTF(&inter, sizeof inter, 21, NULL, io.Fonts->GetGlyphRangesCyrillic());
    minimize3 = io.Fonts->AddFontFromMemoryTTF(&inter, sizeof inter, 23, NULL, io.Fonts->GetGlyphRangesCyrillic());
    segu = io.Fonts->AddFontFromMemoryTTF(&segue, sizeof segue, 23, NULL, io.Fonts->GetGlyphRangesCyrillic());
    ico = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 30, NULL, io.Fonts->GetGlyphRangesCyrillic());
    contact_logo = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 38, NULL, io.Fonts->GetGlyphRangesCyrillic());
    logo = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 40, NULL, io.Fonts->GetGlyphRangesCyrillic());
    dots = io.Fonts->AddFontFromMemoryTTF(&ZenDots, sizeof ZenDots, 28, NULL, io.Fonts->GetGlyphRangesCyrillic());
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool done = false;
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

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            CustomStyleColor();

            ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));

            ImGui::Begin("Main window", &menu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse);
            {
                mouse_window(); // Движение 2 окон, совмещенных в одно
                const auto& p = ImGui::GetWindowPos();
                ImGuiStyle& s = ImGui::GetStyle();
                D3DX11_IMAGE_LOAD_INFO info;
                ID3DX11ThreadPump* pump{ nullptr };

                static bool anim_hide = false;
                static int opticaly = 255;

                ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(20.000f + p.x, 20.000f + p.y), 6, ImColor(255, 81, 84, 255), 100);
                ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(40.000f + p.x, 20.000f + p.y), 6, ImColor(254, 191, 67, 255), 100);
                ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(60.000f + p.x, 20.000f + p.y), 6, ImColor(25, 204, 74, 255), 100);

                ImGui::SetCursorPos(ImVec2(15, 15));
                if (ImGui::InvisibleButton("close", ImVec2(15, 15))) SendMessage(hwnd, WM_CLOSE, 0, 0);
                ImGui::SameLine(0, 3);
                if (ImGui::InvisibleButton("hide", ImVec2(15, 15))) anim_hide = true;

                tab_alpha = ImClamp(tab_alpha + (6.f * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);
                if (tab_alpha == 0.f && tab_add == 0.f) { active_tab = tabs; }

                // HIDE ANIMATION

                if (opticaly > 0 && anim_hide) opticaly -= 1 / ImGui::GetIO().Framerate * 540.f;

                if (opticaly <= 0) {
                    ShowWindow(hwnd, SW_MINIMIZE);
                    opticaly = 255;
                    anim_hide = false;
                }

                SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), opticaly, LWA_ALPHA);

                // TABS HANDLE

                if (tabs > 1) {

                    if (lg == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, user_circle, sizeof(user_circle), &info, pump, &lg, 0);

                    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0.000f + p.x, 0.000f + p.y), ImVec2(235 + p.x, HEIGHT + p.y), ImGui::GetColorU32(color::border), s.WindowRounding, ImDrawCornerFlags_Left); // Background
                    ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(234 + p.x, 0.000f + p.y), ImVec2(235 + p.x, HEIGHT + p.y), ImGui::GetColorU32(color::rainbow_first), ImGui::GetColorU32(color::rainbow_first), ImGui::GetColorU32(color::rainbow_last), ImGui::GetColorU32(color::rainbow_last)); // Background
                    ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(0 + p.x, 100.000f + p.y), ImVec2(235 + p.x, 101 + p.y), ImGui::GetColorU32(color::rainbow_first), ImGui::GetColorU32(color::rainbow_last), ImGui::GetColorU32(color::rainbow_last), ImGui::GetColorU32(color::rainbow_first)); // Background

                    ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(0 + p.x, 220.000f + p.y), ImVec2(230 + p.x, 221 + p.y), ImGui::GetColorU32(color::rainbow_first), ImGui::GetColorU32(color::rainbow_last), ImGui::GetColorU32(color::rainbow_last), ImGui::GetColorU32(color::rainbow_first)); // Background
                    ImGui::GetWindowDrawList()->AddImage(lg, ImVec2(logo_positionX + p.x, logo_positionY + p.y), ImVec2(logo_positionX + 45 + p.x, logo_positionY + 45 + p.y), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));

                    // USER CIRCLE  AND  USER NAME, LICENSE TIME

                    ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(0 + p.x, 455.000f + p.y), ImVec2(230 + p.x, 456 + p.y), ImGui::GetColorU32(color::rainbow_first), ImGui::GetColorU32(color::rainbow_last), ImGui::GetColorU32(color::rainbow_last), ImGui::GetColorU32(color::rainbow_first)); // Background
                    ImGui::GetWindowDrawList()->AddText(minimize, 17, ImVec2(75 + p.x, 483 + p.y), ImGui::GetColorU32(color::text_circle), "User: kliri");
                    ImGui::GetWindowDrawList()->AddText(minimize, 17, ImVec2(75 + p.x, 503 + p.y), ImGui::GetColorU32(color::text_circle), "License: lifetime");

                    // LOGO COMPANY  AND  NAME COMPANY

                    ImGui::GetWindowDrawList()->AddText(logo, 40, ImVec2(18 + p.x, 44 + p.y), ImColor(255, 164, 0, 255), "D");
                    ImGui::GetWindowDrawList()->AddText(dots, 25, ImVec2(59 + p.x, 50 + p.y), ImColor(255, 255, 255, 255), "Destruction");

                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.f));
                    ImGui::PushFont(contact_logo);

                    ImGui::SetCursorPos(ImVec2(695, 10));

                    if (ImGui::CButton("##vk", "J", false, ImVec2(80, 50))) ShellExecute(NULL, L"open", L"https://vk.com/destructiqn", NULL, NULL, SW_SHOW);

                    ImGui::SetCursorPos(ImVec2(745, 10));

                    if (ImGui::CButton("##discord", "K", false, ImVec2(80, 50))) ShellExecute(NULL, L"open", L"https://discord.gg/esWZAA3cBC", NULL, NULL, SW_SHOW);

                    ImGui::PopFont();
                    ImGui::PopStyleColor();

                    ImGui::BeginGroupPos(ImVec2(0, 120));
                    {

                        if (ImGui::Tab("A", "UnlimitedCPS", 2 == tabs || 5 == tabs, ImVec2(240, 40)))
                            tabs = 2;

                        if (ImGui::Tab("B", "Spammer", 3 == tabs || 4 == tabs, ImVec2(240, 40)))
                            tabs = 3;

                    }
                    ImGui::EndGroupPos();

                }


                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * s.Alpha);

                switch (active_tab) {

                case 0:

                    AlignForWidth(300, 200);

                    //ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(200 + p.x, 100 + p.y), ImVec2(650 + p.x, 470 + p.y), ImColor(38, 37, 43, 100), s.WindowRounding, ImDrawFlags_None);

                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 13));
                    ImGui::BeginGroup();
                    {
                        ImGui::PushFont(minimize2);
                        ImGui::InputTextEx("##0", u8"Введите логин", login, 64, ImVec2(300, 41), ImGuiInputTextFlags_None);

                        ImGui::InputTextEx("##1", u8"Введите пароль", password, 64, ImVec2(300, 41), ImGuiInputTextFlags_None);

                        if (ImGui::Button(u8"Войти", ImVec2(300, 40))) tabs = 2;

                        ImGui::PopFont();

                        ImGui::PushFont(minimize);
                        ImGui::SetCursorPos(ImVec2(314, 360));
                        ImGui::TextColored(ImColor(255, 255, 255, 255), u8"Ещё нет аккаунта?");
                        ImGui::SameLine(0, 5);
                        if (ImGui::CBAutton(u8"##1", u8"Регистрация", false, ImVec2(90, 20))) tabs = 1;

                        ImGui::PopFont();
                    }
                    ImGui::EndGroup();
                    ImGui::PopStyleVar();

                    break;

                case 1:

                    AlignForWidth(300, 150);

                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 13));
                    ImGui::BeginGroup();
                    {
                        //ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(200 + p.x, 100 + p.y), ImVec2(650 + p.x, 470 + p.y), ImColor(38, 37, 43, 100), s.WindowRounding, ImDrawFlags_None);

                        ImGui::PushFont(minimize2);
                        ImGui::InputTextEx("##0", u8"Введите логин", reg_login, 64, ImVec2(300, 41), ImGuiInputTextFlags_None);

                        ImGui::InputTextEx("##2", u8"Введите пароль", reg_password, 64, ImVec2(300, 41), ImGuiInputTextFlags_None);

                        ImGui::InputTextEx("##3", u8"Подтвердите пароль", reg_password1, 64, ImVec2(300, 41), ImGuiInputTextFlags_None);

                        ImGui::InputTextEx("##4", u8"Введите почту", email, 64, ImVec2(300, 41), ImGuiInputTextFlags_None);

                        if (ImGui::Button(u8"Создать аккаунт", ImVec2(300, 40))) tabs = 2;
                        ImGui::PopFont();

                        ImGui::PushFont(minimize);
                        ImGui::SetCursorPos(ImVec2(338, 417));
                        ImGui::TextColored(ImColor(255, 255, 255, 255), u8"Уже есть аккаунт?");
                        ImGui::SameLine(0, 5);
                        if (ImGui::CBAutton(u8"##0", u8"Войти", false, ImVec2(45, 20))) tabs = 0;
                        ImGui::PopFont();

                    }
                    ImGui::EndGroup();
                    ImGui::PopStyleVar();

                    break;

                case 2:

                    if (tabs_2_b) tabs = 5;

                    ImGui::PushFont(dots);
                    TextCentered(u8"UnlimitedCPS", 95);
                    ImGui::PopFont();

                    ImGui::PushFont(minimize2);
                    TextCentered(u8"Убирает ограничение по количеству кликов в секунду", 140);
                    TextCentered(u8"как на левую кнопку мыши, так и на правую,", 170);
                    TextCentered(u8"улучшает удары, и уменьшает отдачу", 200);
                    ImGui::PopFont();

                    ImGui::PushFont(minimize2);
                    ImGui::SetCursorPos(ImVec2(655, 485));
                    if (ImGui::CButton("H", u8"Внедрить", true, ImVec2(170, 40))) tabs = 5;
                    ImGui::PopFont();

                    break;

                case 3:

                    if (tabs_1_b) tabs = 4;

                    ImGui::PushFont(dots);
                    TextCentered(u8"Spammer", 95);
                    ImGui::PopFont();

                    PushFont(minimize2);
                    TextCentered(u8"Спамит в чат любыми сообщениями, даже не открывая его", 140);
                    TextCentered(u8"лучшее решение для игроков \"Prison\" и \"SkyBlock\"", 170);
                    TextCentered(u8"обходит анти-флуд систему", 200);
                    PopFont();

                    PushFont(minimize2);
                    ImGui::SetCursorPos(ImVec2(655, 485));
                    if (ImGui::CButton("H", u8"Внедрить", true, ImVec2(170, 40))) tabs = 4;
                    PopFont();


                    break;

                case 4:

                    ImGui::SetCursorPos(ImVec2(255, 20));

                    ImGui::BeginChild("child_widget", ImVec2(250, 400));
                    {
                        //ImGui::PushFont(dots);
                        //Text(u8"Spammer", 20);
                        //ImGui::PopFont();
                        static int key0;
                        static bool enable = false;
                        ImGui::HotKeybox("Enable", "0", &enable, &key0);

                        static int delay_millisecond = 60;
                        static int delay_ssecond = 0;
                        static int delay_minute = 0;
                        static int delay_hour = 0;

                        ImGui::SliderInt("millisecond", &delay_millisecond, 1, 100);
                        ImGui::SliderInt("second", &delay_ssecond, 0, 60);
                        ImGui::SliderInt("minute", &delay_minute, 0, 60);
                        ImGui::SliderInt("hour", &delay_hour, 0, 24);


                        static char message[200] = { "" };
                        ImGui::InputTextEx("##message", "", message, 200, ImVec2(202, 40), ImGuiInputTextFlags_None);

                        tabs_1_b = true;
                    }
                    ImGui::EndChild();

                    break;

                case 5:

                    ImGui::SetCursorPos(ImVec2(255, 20));

                    ImGui::BeginChild("child_widget", ImVec2(250, 400));
                    {
                        //ImGui::PushFont(dots);
                        //Text(u8"UnlimitedCPS", 20);
                        //ImGui::PopFont();

                        static int key0;
                        static bool enable_unlimited = false;
                        ImGui::HotKeybox("Enable", "0", &enable_unlimited, &key0);

                        tabs_2_b = true;

                    }
                    ImGui::EndChild();

                    break;
                }
                ImGui::PopStyleVar(1);
            }

            ImGui::End();

        }

        ImGui::Render();

        const float clear_color_with_alpha[4] = { 0 };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
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

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
