#define _CRT_SECURE_NO_WARNINGS
#include "main.h"

std::string unicode2ansi(const std::wstring& wstr);
std::wstring ansi2unicode(const std::string& str);
std::string utf8_encode(const std::wstring& wstr);
std::wstring utf8_decode(const std::string& str);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    setlocale(LC_ALL, "ru");

    Features::License::client = &client;
    initFeatures();

    if (!client.GetVersion()) return FALSE;
    if (!client.GetState()) return FALSE;

    WNDCLASSEXW wc;
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = nullptr;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = L"Loader";
    wc.lpszClassName = L"Main";
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

    RegisterClassExW(&wc);
    hwnd = CreateWindowExW(NULL, wc.lpszClassName, L"Destruction Loader", WS_POPUP, (GetSystemMetrics(SM_CXSCREEN) / 2) - (WIDTH / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (HEIGHT / 2), WIDTH, HEIGHT, 0, 0, 0, 0);

    if (client.version != "1.1.0")
        if (MessageBoxA(hwnd, "�������� ������!", "Destruction Loader", MB_ICONINFORMATION))
            return FALSE;

    if (client.state != "available")
        if (MessageBoxA(hwnd, "� ������ ������ ���������� ���.������", "Destruction Loader", MB_ICONINFORMATION))
            return FALSE;

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
    while (!done) {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
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
                mouse_window(); // �������� 2 ����, ����������� � ����
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
                    ImGui::GetWindowDrawList()->AddText(minimize, 17, ImVec2(75 + p.x, 483 + p.y), ImGui::GetColorU32(color::text_circle), (string("User: ") + client.user.name).c_str());
                    ImGui::GetWindowDrawList()->AddText(minimize, 17, ImVec2(75 + p.x, 503 + p.y), ImGui::GetColorU32(color::text_circle), Features::License::GetLicenseTime(&tabs).c_str());

                    if (!Features::License::updaterIsInitialized) {
                        CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Features::License::UpdateLicenseTimes), nullptr, NULL, nullptr);
                        Features::License::updaterIsInitialized = true;
                    }

                    Features::License::SetLicenseTimes(&tabs);

                    // LOGO COMPANY  AND  NAME COMPANY

                    ImGui::GetWindowDrawList()->AddText(logo, 40, ImVec2(18 + p.x, 44 + p.y), ImColor(255, 164, 0, 255), "D");
                    ImGui::GetWindowDrawList()->AddText(dots, 25, ImVec2(59 + p.x, 50 + p.y), ImColor(255, 255, 255, 255), "Destruction");

                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.f));
                    ImGui::PushFont(contact_logo);

                    ImGui::SetCursorPos(ImVec2(745, 9));
                    if (ImGui::CBAAutton("##vk", "J", false, ImVec2(40, 40)))
                        ShellExecute(NULL, L"open", L"https://vk.com/destructiqn", NULL, NULL, SW_SHOW);

                    ImGui::SetCursorPos(ImVec2(795, 10));
                    if (ImGui::CBAAutton("##discord", "K", false, ImVec2(40, 40)))
                        ShellExecute(NULL, L"open", L"https://discord.gg/nhtg", NULL, NULL, SW_SHOW);

                    ImGui::PopFont();
                    ImGui::PopStyleColor();

                    ImGui::BeginGroupPos(ImVec2(0, 120));
                    {
                        if (ImGui::Tab("A", "UnlimitedCPS", 2 == tabs || 5 == tabs, ImVec2(240, 40))) tabs = 2;
                        if (ImGui::Tab("B", "Spammer", 3 == tabs || 4 == tabs, ImVec2(240, 40))) tabs = 3;
                    }
                    ImGui::EndGroupPos();

                }

                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * s.Alpha);

                string savedLogin;
                string savedPassword;

                switch (active_tab) {
                case 0:
                    AlignForWidth(300, 200);

                    //ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(200 + p.x, 100 + p.y), ImVec2(650 + p.x, 470 + p.y), ImColor(38, 37, 43, 100), s.WindowRounding, ImDrawFlags_None);

                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 13));
                    ImGui::BeginGroup();
                    {
                        ImGui::PushFont(minimize2);
                        ImGui::InputTextEx("##0", u8"������� �����", login, 12, ImVec2(300, 41), ImGuiInputTextFlags_None);

                        ImGui::InputTextEx("##1", u8"������� ������", password, 64, ImVec2(300, 41), ImGuiInputTextFlags_None);

                        if (ImGui::Button(u8"�����", ImVec2(300, 40))) {
                            string pass = md5(md5(password));
                            if (client.Login(login, pass.c_str())) {
                                RestAPI::UserData::save(login, pass);
                                tabs = 2;
                            }
                        }

                        ImGui::PopFont();

                        ImGui::PushFont(minimize);
                        ImGui::SetCursorPos(ImVec2(314, 360));
                        ImGui::TextColored(ImColor(255, 255, 255, 255), u8"��� ��� ��������?");
                        ImGui::SameLine(0, 5);
                        if (ImGui::CBAutton(u8"##1", u8"�����������", false, ImVec2(90, 20))) tabs = 1;

                        ImGui::PopFont();
                    }
                    ImGui::EndGroup();
                    ImGui::PopStyleVar();

                    if (tabs == 0) {
                        RestAPI::UserData::get(savedLogin, savedPassword);
                        if (!savedLogin.empty() && !savedPassword.empty()) {
                            if (client.Login(savedLogin.c_str(), savedPassword.c_str())) tabs = 2;
                            else RestAPI::UserData::del();
                        }
                    }

                    break;

                case 1:
                    AlignForWidth(300, 150);

                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 13));
                    ImGui::BeginGroup();
                    {
                        //ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(200 + p.x, 100 + p.y), ImVec2(650 + p.x, 470 + p.y), ImColor(38, 37, 43, 100), s.WindowRounding, ImDrawFlags_None);

                        ImGui::PushFont(minimize2);
                        ImGui::InputTextEx("##0", u8"������� �����", reg_login, 12, ImVec2(300, 41), ImGuiInputTextFlags_None);

                        ImGui::InputTextEx("##2", u8"������� ������", reg_password, 64, ImVec2(300, 41), ImGuiInputTextFlags_None);

                        ImGui::InputTextEx("##3", u8"����������� ������", reg_password1, 64, ImVec2(300, 41), ImGuiInputTextFlags_None);

                        ImGui::InputTextEx("##4", u8"������� �����", email, 64, ImVec2(300, 41), ImGuiInputTextFlags_None);

                        if (ImGui::Button(u8"������� �������", ImVec2(300, 40))) {
                            if (strcmp(reg_password, reg_password1) == 0) {
                                string pass = md5(md5(reg_password));
                                string unHash = RestAPI::Utils::GetUnHash();

                                if (client.Register(reg_login, pass.c_str(), StringUtils::toLower(email).c_str(), unHash.c_str())) {
                                    RestAPI::UserData::save(reg_login, pass);
                                    if (client.Login(reg_login, pass.c_str())) tabs = 2;
                                }
                            }
                            else {
                                MessageBoxA(RestAPI::ErrorHandler::hWindow, "������ �� ���������!", "Destruction Loader", MB_ICONERROR);
                            }
                        }

                        ImGui::PopFont();

                        ImGui::PushFont(minimize);
                        ImGui::SetCursorPos(ImVec2(338, 417));
                        ImGui::TextColored(ImColor(255, 255, 255, 255), u8"��� ���� �������?");
                        ImGui::SameLine(0, 5);
                        if (ImGui::CBAutton(u8"##0", u8"�����", false, ImVec2(45, 20))) tabs = 0;
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
                    TextCentered(u8"������� ����������� �� ���������� ������ � �������", 140);
                    TextCentered(u8"��� �� ����� ������ ����, ��� � �� ������,", 170);
                    TextCentered(u8"�������� �����, � ��������� ������", 200);
                    ImGui::PopFont();

                    ImGui::PushFont(minimize2);
                    ImGui::SetCursorPos(ImVec2(655, 485));
                    if (ImGui::CButton("H", u8"��������", true, ImVec2(170, 40))) {
                        if (Features::License::ToggleTabIfLicenseExists("unlimitedcps", 5, &tabs)) {
                            vector<std::uint8_t> dll;
                            if (client.GetSessionHash(md5("unlimitedcps"), client.user.name, client.user.password, client.user.session, dll)) {
                                HANDLE hProcess = GetProcessHandleFromHwnd(FindWindowA(nullptr, injectWindowName));
                                if (hProcess) {
                                    LPVOID lpReserved = VirtualAllocEx(hProcess, nullptr, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
                                    if (lpReserved) {
                                        WriteProcessMemory(hProcess, lpReserved, client.user.session.c_str(), client.user.session.length(), nullptr);
                                        ManualMapDll(hProcess, dll.data(), dll.size(), true, true, true, true, DLL_VIMEWORLD_ATTACH, lpReserved);
                                    }
                                }
                                else {
                                    tabs = 2;
                                }
                            }
                            else {
                                tabs = 2;
                            }
                        }
                    }
                    ImGui::PopFont();

                    break;

                case 3:
                    if (tabs_1_b) tabs = 4;

                    ImGui::PushFont(dots);
                    TextCentered(u8"Spammer", 95);
                    ImGui::PopFont();

                    PushFont(minimize2);
                    TextCentered(u8"������ � ��� ������ �����������, ���� �� �������� ���", 140);
                    TextCentered(u8"������ ������� ��� ������� \"Prison\" � \"SkyBlock\"", 170);
                    TextCentered(u8"������� ����-���� �������", 200);
                    PopFont();

                    PushFont(minimize2);
                    ImGui::SetCursorPos(ImVec2(655, 485));
                    if (ImGui::CButton("H", u8"��������", true, ImVec2(170, 40))) {
                        if (Features::License::ToggleTabIfLicenseExists("spammer", 4, &tabs)) {
                            vector<std::uint8_t> dll;
                            if (client.GetSessionHash(md5("spammer"), client.user.name, client.user.password, client.user.session, dll)) {
                                HANDLE hProcess = GetProcessHandleFromHwnd(FindWindowA(nullptr, injectWindowName));
                                if (hProcess) {
                                    LPVOID lpReserved = VirtualAllocEx(hProcess, nullptr, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
                                    if (lpReserved) {
                                        WriteProcessMemory(hProcess, lpReserved, client.user.session.c_str(), client.user.session.length(), nullptr);
                                        ManualMapDll(hProcess, dll.data(), dll.size(), true, true, true, true, DLL_VIMEWORLD_ATTACH, lpReserved);
                                    }
                                }
                                else {
                                    tabs = 3;
                                }
                            }
                            else {
                                tabs = 3;
                            }
                        }
                    }

                    PopFont();
                    break;
                case 4:
                    ImGui::SetCursorPos(ImVec2(255, 20));
                    ImGui::BeginChild("child_widget", ImVec2(250, 400));
                    {
                        if (ImGui::Checkbox("AntiMute", &spammer->antiMute)) Configs::ConfigManager::WriteFeatureSettings(spammer);
                        SetCursorPos(ImVec2(GetCursorPos().x + 242, GetCursorPos().y + -47));

                        if (ImGui::Keybind1("0", &spammer->keyCode)) Configs::ConfigManager::WriteFeatureSettings(spammer);
                        SetCursorPos(ImVec2(GetCursorPos().x + 0, GetCursorPos().y + 8));

                        static int delay_ms = 0;
                        static int delay_s = 0;
                        static int delay_m = 0;
                        static int delay_h = 0;

                        static char message[100] = { "" };

                        if (ImGui::SliderInt("millisecond", &delay_ms, 0, 1000))
                            Configs::Spammer::SaveDelay(spammer, delay_ms, delay_s, delay_m, delay_h);
                        if (ImGui::SliderInt("second", &delay_s, 0, 60))
                            Configs::Spammer::SaveDelay(spammer, delay_ms, delay_s, delay_m, delay_h);
                        if (ImGui::SliderInt("minute", &delay_m, 0, 60))
                            Configs::Spammer::SaveDelay(spammer, delay_ms, delay_s, delay_m, delay_h);
                        if (ImGui::SliderInt("hour", &delay_h, 0, 24))
                            Configs::Spammer::SaveDelay(spammer, delay_ms, delay_s, delay_m, delay_h);

                        // �������� �������
                        if (!Configs::Spammer::configIsLoaded) {
                            int delayCount;
                            string delayUnit;
                            string msg;

                            // ���� ������� ������� �� ���������� - ������ ���
                            struct _stat fiBuf;
                            if (_stat(Configs::ConfigManager::Spammer.c_str(), &fiBuf) == -1)
                                Configs::ConfigManager::WriteFeatureSettings(spammer);

                            // ������ ������
                            if (Configs::Spammer::Parse(spammer->antiMute, spammer->keyCode, delayCount, delayUnit, msg)) {
                                // ������ ��������
                                Configs::Spammer::ParseDelayInSliders(delayCount, delayUnit, delay_ms, delay_s, delay_m, delay_h);

                                // ������ ���������
                                msg = utf8_encode(ansi2unicode(msg)); // convert ANSI to unicode, then unicode to UTF-8
                                strcpy(message, msg.c_str());
                            }

                            Configs::Spammer::configIsLoaded = true;
                        }

                        if (ImGui::InputTextEx("##message", "", message, 100, ImVec2(202, 40), ImGuiInputTextFlags_None)) {
                            spammer->message = unicode2ansi(utf8_decode(message)); // convert UTF-8 to unicode, then unicode to ANSI
                            spammer->message = spammer->message.substr(0, spammer->message.size() - 1);
                            Configs::ConfigManager::WriteFeatureSettings(spammer);
                        }

                        tabs_1_b = true;
                    }

                    ImGui::EndChild();
                    break;
                case 5:
                    ImGui::SetCursorPos(ImVec2(255, 20));
                    ImGui::BeginChild("child_widget", ImVec2(250, 400));
                    {
                        if (!Configs::UnlimitedCPS::configIsLoaded) {
                            // ���� ������� �� ���������� - ������ ���
                            struct _stat fiBuf;
                            if (_stat(Configs::ConfigManager::UnlimitedCPS.c_str(), &fiBuf) == -1)
                                Configs::ConfigManager::WriteFeatureSettings(unlimitedCPS);

                            Configs::UnlimitedCPS::Parse(unlimitedCPS->isEnabled, unlimitedCPS->keyCode);
                            Configs::UnlimitedCPS::configIsLoaded = true;
                        }

                        if (ImGui::Checkbox("Enable", &unlimitedCPS->isEnabled)) {
                            unlimitedCPS->isEnabled = !unlimitedCPS->isEnabled;
                            Configs::ConfigManager::WriteFeatureSettings(unlimitedCPS);
                        }
                        SetCursorPos(ImVec2(GetCursorPos().x + 242, GetCursorPos().y + -47));

                        if (ImGui::Keybind1("0", &unlimitedCPS->keyCode))
                            Configs::ConfigManager::WriteFeatureSettings(unlimitedCPS);
                        SetCursorPos(ImVec2(GetCursorPos().x + 0, GetCursorPos().y + 8));

                        Configs::UnlimitedCPS::Parse(unlimitedCPS->isEnabled, unlimitedCPS->keyCode);
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

    unlimitedCPS->~UnlimitedCPS();
    spammer->~Spammer();

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
