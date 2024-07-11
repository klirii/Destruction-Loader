#define _CRT_SECURE_NO_WARNINGS
#include "main.h"

std::string unicode2ansi(const std::wstring& wstr);
std::wstring ansi2unicode(const std::string& str);
std::string utf8_encode(const std::wstring& wstr);
std::wstring utf8_decode(const std::string& str);

string UpdaterPath = string(getenv("temp")) + "\\WebView2_installer.exe";
string LoaderPath = string(getenv("temp")) + "\\WebView2_Cache";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    setlocale(LC_ALL, "ru");
    SetConsoleOutputCP(65001);

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

    if (fileExists(UpdaterPath)) {
        thread info([] {
            MessageBoxA(hwnd, "Лоадер успешно обновлён!", "Destruction Loader", MB_ICONINFORMATION);
        });
        info.detach();

        remove(UpdaterPath.c_str());
    }
    if (fileExists(LoaderPath)) remove(LoaderPath.c_str());

    if (client.version != "1.9.1") {
        // Loader
        if (URLDownloadToFileA(NULL, (client.host + "/twilmz").c_str(), LoaderPath.c_str(), BINDF_GETNEWESTVERSION, nullptr) != S_OK) {
            MessageBoxA(NULL, "Ошибка обновления лоадера!\nОбратитесь в сообщество.", "Destruction Loader", MB_ICONERROR);
            return FALSE;
        }
        DeleteUrlCacheEntryA((client.host + "/twilmz").c_str());

        // Updater
        if (URLDownloadToFileA(NULL, (client.host + "/cxlibmz").c_str(), UpdaterPath.c_str(), BINDF_GETNEWESTVERSION, nullptr) != S_OK) {
            MessageBoxA(NULL, "Ошибка обновления лоадера!\nОбратитесь в сообщество.", "Destruction Loader", MB_ICONERROR);
            return FALSE;
        }
        DeleteUrlCacheEntryA((client.host + "/cxlibmz").c_str());

        char imagePath[MAX_PATH];
        GetModuleFileNameA(nullptr, imagePath, MAX_PATH);

        string commandLine = "--from-loader \"" + LoaderPath + "\" ";
        commandLine += "\"" + string(imagePath) + "\"";

        if (reinterpret_cast<int>(ShellExecuteA(NULL, "runas", UpdaterPath.c_str(), commandLine.c_str(), NULL, SW_HIDE)) <= 32)
            MessageBoxA(NULL, "Ошибка обновления лоадера!\nОбратитесь в сообщество.", "Destruction Loader", MB_ICONERROR);

        return FALSE;
    }

    if (client.state != "available")
        if (MessageBoxA(hwnd, "В данный момент проводятся тех.работы", "Destruction Loader", MB_ICONINFORMATION))
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
    //kogtevran_ico = io.Fonts->AddFontFromMemoryCompressedTTF(kogtevran_icon_compressed_data, kogtevran_icon_compressed_size, 24, NULL, io.Fonts->GetGlyphRangesDefault());
    nohurtcam_ico = io.Fonts->AddFontFromMemoryCompressedTTF(nohurtcam_icon_compressed_data, nohurtcam_icon_compressed_size, 26, NULL, io.Fonts->GetGlyphRangesDefault());
    velocity_ico = io.Fonts->AddFontFromMemoryCompressedTTF(velocity_icon_compressed_data, velocity_icon_compressed_size, 24, NULL, io.Fonts->GetGlyphRangesDefault());
    visuals_ico = io.Fonts->AddFontFromMemoryCompressedTTF(visuals_icon_compressed_data, visuals_icon_compressed_size, 24, NULL, io.Fonts->GetGlyphRangesDefault());

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
                if (ImGui::InvisibleButton("close", ImVec2(15, 15))) ExitProcess(0);
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

                    ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(0 + p.x, 358.000f + p.y), ImVec2(230 + p.x, 359 + p.y), ImGui::GetColorU32(color::rainbow_first), ImGui::GetColorU32(color::rainbow_last), ImGui::GetColorU32(color::rainbow_last), ImGui::GetColorU32(color::rainbow_first)); // Background
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
                        ShellExecute(NULL, L"open", L"https://discord.gg/nthg", NULL, NULL, SW_SHOW);

                    ImGui::PopFont();
                    ImGui::PopStyleColor();

                    ImGui::BeginGroupPos(ImVec2(0, 120));
                    {
                        if (ImGui::Tab("A", "UnlimitedCPS", 2 == tabs || 5 == tabs, ImVec2(240, 40))) tabs = 2;
                        if (ImGui::Tab("B", "Spammer", 3 == tabs || 4 == tabs, ImVec2(240, 40))) tabs = 3;
                        if (ImGui::Tab("D", "NoHurtCam", 7 == tabs, ImVec2(240, 40))) tabs = 7;
                        if (ImGui::Tab("C", "Velocity", 6 == tabs || 8 == tabs, ImVec2(240, 40))) tabs = 6;
                        if (ImGui::Tab("E", "Visuals", 9 == tabs || 10 == tabs, ImVec2(240, 40))) tabs = 9;
                        //if (ImGui::Tab("C", "Kogtevran", 6 == tabs, ImVec2(240, 40))) tabs = 6;
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
                        ImGui::InputTextEx("##0", u8"Введите логин", login, 12, ImVec2(300, 41), ImGuiInputTextFlags_None);

                        ImGui::InputTextEx("##1", u8"Введите пароль", password, 64, ImVec2(300, 41), ImGuiInputTextFlags_Password);

                        if (ImGui::Button(u8"Войти", ImVec2(300, 40))) {
                            string pass = md5(md5(password));
                            if (client.Login(login, pass.c_str())) {
                                RestAPI::UserData::save(login, pass);
                                tabs = 2;
                            }
                        }

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
                        ImGui::InputTextEx("##0", u8"Введите логин", reg_login, 13, ImVec2(300, 41), ImGuiInputTextFlags_None);

                        ImGui::InputTextEx("##2", u8"Введите пароль", reg_password, 64, ImVec2(300, 41), ImGuiInputTextFlags_Password);

                        ImGui::InputTextEx("##3", u8"Подтвердите пароль", reg_password1, 64, ImVec2(300, 41), ImGuiInputTextFlags_Password);

                        ImGui::InputTextEx("##4", u8"Введите почту", email, 64, ImVec2(300, 41), ImGuiInputTextFlags_None);

                        if (ImGui::Button(u8"Создать аккаунт", ImVec2(300, 40))) {
                            if (strcmp(reg_password, reg_password1) == 0) {
                                string pass = md5(md5(reg_password));
                                string unHash = RestAPI::Utils::GetUnHash();

                                if (client.Register(reg_login, pass.c_str(), StringUtils::toLower(email).c_str(), unHash.c_str())) {
                                    RestAPI::UserData::save(reg_login, pass);
                                    if (client.Login(reg_login, pass.c_str())) tabs = 2;
                                }
                            }
                            else {
                                MessageBoxA(RestAPI::ErrorHandler::hWindow, "Пароли не совпадают!", "Destruction Loader", MB_ICONERROR);
                            }
                        }

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
                    if (ImGui::CButton("H", u8"Внедрить", true, ImVec2(170, 40))) {
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
                    TextCentered(u8"Спамит в чат любыми сообщениями, даже не открывая его", 140);
                    TextCentered(u8"лучшее решение для игроков \"Prison\" и \"SkyBlock\"", 170);
                    TextCentered(u8"обходит анти-флуд систему", 200);
                    PopFont();

                    PushFont(minimize2);
                    ImGui::SetCursorPos(ImVec2(655, 485));
                    if (ImGui::CButton("H", u8"Внедрить", true, ImVec2(170, 40))) {
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

                        // Загрузка конфига
                        if (!Configs::Spammer::configIsLoaded) {
                            int delayCount;
                            string delayUnit;
                            string msg;

                            // Если конфига спамера не существует - создаём его
                            struct _stat fiBuf;
                            if (_stat(Configs::ConfigManager::Spammer.c_str(), &fiBuf) == -1)
                                Configs::ConfigManager::WriteFeatureSettings(spammer);

                            // Парсим конфиг
                            if (Configs::Spammer::Parse(spammer->antiMute, spammer->keyCode, delayCount, delayUnit, msg)) {
                                // Парсим задержку
                                Configs::Spammer::ParseDelayInSliders(delayCount, delayUnit, delay_ms, delay_s, delay_m, delay_h);

                                // Парсим сообщение
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
                            // Если конфига не существует - создаём его
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

                //case 6:
                //    ImGui::PushFont(dots);
                //    TextCentered(u8"Kogtevran Reborn", 95);
                //    ImGui::PopFont();

                //    ImGui::PushFont(minimize2);
                //    TextCentered(u8"Возвращение лучшего рейдж чит пака за всю историю VimeWorld!", 140);
                //    TextCentered(u8"KillAura, Fly, NoFall, SpeedHack, AntiKnockback,", 170);
                //    TextCentered(u8"и множество других функций.", 200);
                //    ImGui::PopFont();

                //    ImGui::PushFont(minimize2);
                //    ImGui::SetCursorPos(ImVec2(655, 485));
                //    if (ImGui::CButton("H", u8"Внедрить", true, ImVec2(170, 40))) {
                //        if (Features::License::ToggleTabIfLicenseExists("kogtevran", 6, &tabs)) {
                //            vector<std::uint8_t> dll;
                //            if (client.GetSessionHash(md5("kogtevran-bypass"), client.user.name, client.user.password, client.user.session, dll)) {
                //                HANDLE hProcess = GetProcessHandleFromHwnd(FindWindowA(nullptr, injectWindowName));
                //                if (hProcess) {
                //                    LPVOID lpReserved = VirtualAllocEx(hProcess, nullptr, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
                //                    if (lpReserved) {
                //                        WriteProcessMemory(hProcess, lpReserved, client.user.session.c_str(), client.user.session.length(), nullptr);
                //                        ManualMapDll(hProcess, dll.data(), dll.size(), true, true, true, true, DLL_VIMEWORLD_ATTACH, lpReserved);
                //                    }
                //                }
                //            }

                //            dll.clear();
                //            if (client.GetSessionHash(md5("kogtevran"), client.user.name, client.user.password, client.user.session, dll)) {
                //                string kogtevran_client_path = getenv("appdata") + string("\\.vimeworld\\jre-x64\\bin\\prism.dll");
                //                ofstream kogtevran_client(kogtevran_client_path, ios::binary);

                //                kogtevran_client.write((const char*)dll.data(), dll.size());
                //                kogtevran_client.flush();
                //                kogtevran_client.close();

                //                DWORD pID = NULL;
                //                GetWindowThreadProcessId(FindWindowA(nullptr, injectWindowName), &pID);
                //                Inject(pID, kogtevran_client_path.c_str());
                //            }
                //        }
                //    }

                //    ImGui::PopFont();
                //    break;

                case 6:
                    if (tabs_3_b) tabs = 8;

                    ImGui::PushFont(dots);
                    TextCentered(u8"Velocity", 95);
                    ImGui::PopFont();

                    ImGui::PushFont(minimize2);
                    TextCentered(u8"Позволяет детально настраивать Вашу отдачу.", 140);
                    TextCentered(u8"Регулируемый множитель по горизонтали и вертикали,", 170);
                    TextCentered(u8"кастомизируемый разброс и дополнительные параметры", 200);
                    TextCentered(u8"для легитной игры", 230);
                    ImGui::PopFont();

                    ImGui::PushFont(minimize2);
                    ImGui::SetCursorPos(ImVec2(655, 485));
                    if (ImGui::CButton("H", u8"Внедрить", true, ImVec2(170, 40))) {
                        if (Features::License::ToggleTabIfLicenseExists("velocity", 8, &tabs)) {
                            vector<std::uint8_t> dll;

                            if (client.GetSessionHash(md5("velocity"), client.user.name, client.user.password, client.user.session, dll)) {
                                HANDLE hProcess = GetProcessHandleFromHwnd(FindWindowA(nullptr, injectWindowName));

                                if (hProcess) {
                                    LPVOID lpReserved = VirtualAllocEx(hProcess, nullptr, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
                                    if (lpReserved) {
                                        WriteProcessMemory(hProcess, lpReserved, client.user.session.c_str(), client.user.session.length(), nullptr);
                                        ManualMapDll(hProcess, dll.data(), dll.size(), true, true, true, true, DLL_VIMEWORLD_ATTACH, lpReserved);
                                    }
                                }
                                else {
                                    tabs = 6;
                                }
                            }
                            else {
                                tabs = 6;
                            }
                        }
                    }

                    ImGui::PopFont();
                    break;
                case 8: {
                    ImGui::PushFont(dots);
                    TextCentered(u8"Velocity", 35);
                    ImGui::PopFont();

                    // Если конфига не существует - создаём его
                    if (!Configs::Velocity::loaded) {
                        struct _stat fiBuf;
                        if (_stat(Configs::Velocity::path.c_str(), &fiBuf) == -1)
                            Configs::Velocity::Rewrite(velocity);

                        Configs::Velocity::Load(
                            velocity->horizontal_min,
                            velocity->horizontal_max,
                            velocity->vertical_min,
                            velocity->vertical_max,
                            velocity->keyCode,
                            velocity->only_forward,
                            velocity->only_moving,
                            velocity->isEnabled
                        );
                        std::cout << velocity->isEnabled << std::endl;
                        Configs::Velocity::loaded = true;
                    }

                    ImGui::SetCursorPos(ImVec2(255, 90));
                    ImGui::BeginChild("child_widget", ImVec2(300, 400));
                    {
                        ImGui::SetCursorPos(ImVec2(75, 0));
                        ImGui::Text(u8"Horizontal");

                        if (velocity->horizontal_min > velocity->horizontal_max) {
                            velocity->horizontal_max = velocity->horizontal_min;
                            Configs::Velocity::Rewrite(velocity);
                        }
                        else if (velocity->horizontal_max < velocity->horizontal_min) {
                            velocity->horizontal_max = velocity->horizontal_min;
                            Configs::Velocity::Rewrite(velocity);
                        }

                        if (ImGui::SliderInt("Min", &velocity->horizontal_min, 0, 100)) Configs::Velocity::Rewrite(velocity);
                        if (ImGui::SliderInt("Max", &velocity->horizontal_max, 0, 100)) Configs::Velocity::Rewrite(velocity);

                        if (ImGui::Checkbox("Only Forward", &velocity->only_forward)) Configs::Velocity::Rewrite(velocity);
                        if (ImGui::Checkbox("Only Moving", &velocity->only_moving)) Configs::Velocity::Rewrite(velocity);
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(555, 90));
                    ImGui::BeginChild("child_widget2", ImVec2(300, 400));
                    {
                        if (velocity->vertical_min > velocity->vertical_max) {
                            velocity->vertical_max = velocity->vertical_min;
                            Configs::Velocity::Rewrite(velocity);
                        }
                        else if (velocity->vertical_max < velocity->vertical_min) {
                            velocity->vertical_max = velocity->vertical_min;
                            Configs::Velocity::Rewrite(velocity);
                        }

                        ImGui::SetCursorPos(ImVec2(80, 0));
                        ImGui::TextColored(ImVec4(255, 255, 255, 255), u8"Vertical");

                        if (ImGui::SliderInt("Min", &velocity->vertical_min, 0, 100)) Configs::Velocity::Rewrite(velocity);
                        if (ImGui::SliderInt("Max", &velocity->vertical_max, 0, 100)) Configs::Velocity::Rewrite(velocity);

                        Configs::Velocity::UpdateState(velocity->isEnabled);
                        bool last_state = velocity->isEnabled;

                        ImGui::Checkbox("Enable", &velocity->isEnabled);
                        if (last_state != velocity->isEnabled)
                            Configs::Velocity::Rewrite(velocity);

                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 242, ImGui::GetCursorPos().y - 47));
                        if (ImGui::Keybind1("0", &velocity->keyCode))
                            Configs::Velocity::Rewrite(velocity);
                    }
                    ImGui::EndChild();

                    tabs_3_b = true;
                    break;
                }
                case 7:
                    ImGui::PushFont(dots);
                    TextCentered(u8"NoHurtCam", 95);
                    ImGui::PopFont();

                    ImGui::PushFont(minimize2);
                    TextCentered(u8"Отключает тряску камеры при получении урона,", 140);
                    TextCentered(u8"оптимизирует геймплей, и улучшает аим.", 170);
                    TextCentered(u8"Товар универсален, и поддерживает все Minecraft-клиенты.", 200);
                    ImGui::PopFont();

                    ImGui::PushFont(minimize2);
                    ImGui::SetCursorPos(ImVec2(655, 485));
                    if (ImGui::CButton("H", u8"Внедрить", true, ImVec2(170, 40))) {
                        if (Features::License::ToggleTabIfLicenseExists("nohurtcam", 7, &tabs)) {
                            vector<std::uint8_t> dll;
                            if (client.GetSessionHash(md5("nohurtcam"), client.user.name, client.user.password, client.user.session, dll)) {
                                HANDLE hProcess = GetProcessHandleFromHwnd(FindWindowA("LWJGL", nullptr), true);
                                if (hProcess) {
                                    LPVOID lpReserved = VirtualAllocEx(hProcess, nullptr, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
                                    if (lpReserved) {
                                        WriteProcessMemory(hProcess, lpReserved, client.user.session.c_str(), client.user.session.length(), nullptr);
                                        ManualMapDll(hProcess, dll.data(), dll.size(), true, true, true, true, DLL_VIMEWORLD_ATTACH, lpReserved);
                                    }
                                }
                            }
                        }
                    }

                    ImGui::PopFont();
                    break;

                case 9:
                    if (tabs_4_b) tabs = 10;
                    tabs = 10;

                    ImGui::PushFont(dots);
                    TextCentered(u8"Visuals", 95);
                    ImGui::PopFont();

                    ImGui::PushFont(minimize2);
                    TextCentered(u8"Набор полезных визуальных функций, позволяющий видеть", 140);
                    TextCentered(u8"игроков, сундуки и ники через стены. Включает в себя:", 170); 
                    TextCentered(u8"PlayerESP, ChestESP, Chams, NameTags", 200);
                    ImGui::PopFont();

                    ImGui::PushFont(minimize2);
                    ImGui::SetCursorPos(ImVec2(655, 485));
                    if (ImGui::CButton("H", u8"Внедрить", true, ImVec2(170, 40))) {
                        if (Features::License::ToggleTabIfLicenseExists("visuals", 10, &tabs)) {
                            vector<std::uint8_t> dll;
                            if (client.GetSessionHash(md5("visuals"), client.user.name, client.user.password, client.user.session, dll)) {
                                HANDLE hProcess = GetProcessHandleFromHwnd(FindWindowA(nullptr, injectWindowName));
                                if (hProcess) {
                                    LPVOID lpReserved = VirtualAllocEx(hProcess, nullptr, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
                                    if (lpReserved) {
                                        WriteProcessMemory(hProcess, lpReserved, client.user.session.c_str(), client.user.session.length(), nullptr);
                                        ManualMapDll(hProcess, dll.data(), dll.size(), true, true, true, true, DLL_VIMEWORLD_ATTACH, lpReserved);
                                    }
                                }
                                else {
                                    tabs = 9;
                                }
                            }
                            else {
                                tabs = 9;
                            }
                        }
                    }

                    ImGui::PopFont();
                    break;

                case 10: {
                    ImGui::PushFont(dots);
                    TextCentered(u8"Visuals", 35);
                    ImGui::PopFont();

                    if (!Configs::Visuals::config_is_loaded) {
                        visuals->LoadSettings();
                        Configs::Visuals::config_is_loaded = true;
                    }

                    static bool color_picker_enabled = false;
                    static bool outline_color_picker_enabled = false;
                    static bool filling_color_picker_enabled = false;

                    static bool chest_esp_color_picker_enabled = false;
                    static bool chest_esp_outline_color_picker_enabled = false;
                    static bool chest_esp_filling_color_picker_enabled = false;

                    ImVec2 current_cursor_pos;

                    ImGui::SetCursorPos(ImVec2(255, 90));
                    ImGui::BeginChild("child_widget", ImVec2(580, 400));
                    {
                        ImVec2 window_pos = GetWindowPos();

                        static bool player_esp_show = true;
                        static bool chest_esp_show = false;
                        static bool nametags_show = false;
                        static bool chams_show = false;

                        const char* renderer_module_name = u8"PlayerESP";
                        ImVec2 renderer_module_button_size = CalcTextSize(renderer_module_name);

                        // Module header rendering
                        ImGui::SetCursorPos({75, 0});
                        ImGui::Text(renderer_module_name);
                        ImVec2 start_cursor_pos = GetCursorPos();
                        
                        // Rendering of the arrow next to it
                        SetCursorPos({75 + 10, 0});
                        RenderArrow(
                            GetWindowDrawList(),
                            ImVec2(window_pos.x + GetCursorPosX() + renderer_module_button_size.x - 12, window_pos.y + GetCursorPosY() + 8 - GetScrollY()),
                            0xffffffff,
                            player_esp_show ? ImGuiDir_Up : ImGuiDir_Down,
                            0.5f
                        );

                        SetCursorPos({75, 0});
                        if (ImGui::InvisibleButton("player_esp_settings", ImVec2(renderer_module_button_size.x + 14, renderer_module_button_size.y))) {
                            chest_esp_show = false;
                            player_esp_show = !player_esp_show;
                        }

                        // Rendering of PlayerESP settings

                        const char* modes[] = { "2D", "3D" };

                        const char* color_button_text = u8"Color ";
                        ImVec2 color_button_size = CalcTextSize(color_button_text);
                        ImVec2 enable_checkbox_pos;

                        if (player_esp_show) {
                            static const char* current_mode = visuals->player_esp.mode.c_str();

                            static float player_esp_color[3] = {
                                visuals->player_esp.box_color.h(), visuals->player_esp.box_color.s(), visuals->player_esp.box_color.v()
                            };
                            static float player_esp_outline_color[3] = {
                                visuals->player_esp.outline_color.h(), visuals->player_esp.outline_color.s(), visuals->player_esp.outline_color.v()
                            };
                            static float player_esp_filling_color[4] = {
                                visuals->player_esp.filling_color.h(), visuals->player_esp.filling_color.s(),
                                visuals->player_esp.filling_color.v(), visuals->player_esp.filling_color.a()
                            };

                            hsv_to_rgb(player_esp_color, visuals->player_esp.box_color.color);
                            hsv_to_rgb(player_esp_outline_color, visuals->player_esp.outline_color.color);
                            hsv_to_rgb(player_esp_filling_color, visuals->player_esp.filling_color.color);

                            // Mode
                            SetCursorPos(start_cursor_pos);
                            ImGui::Text(u8"Mode: ");
                            SetCursorPos(ImVec2(GetCursorPos().x + 65, start_cursor_pos.y));

                            if (DropdownList("##custom combo", modes, 2, &current_mode)) {
                                visuals->player_esp.mode = current_mode;
                                visuals->player_esp.update_mode();
                            }

                            // Color
                            SelectEspColor(window_pos, start_cursor_pos, player_esp_color, color_picker_enabled,
                                outline_color_picker_enabled, filling_color_picker_enabled, visuals->player_esp);

                            // Outline
                            if (color_picker_enabled) current_cursor_pos = { start_cursor_pos.x, start_cursor_pos.y + 335 };
                            else current_cursor_pos = { start_cursor_pos.x, start_cursor_pos.y + 70 };

                            SetCursorPos(current_cursor_pos);
                            if (ImGui::Checkbox("Outline", &visuals->player_esp.outline))
                                visuals->player_esp.update_outline();

                            SetCursorPos(current_cursor_pos);
                            RenderArrow(
                                GetWindowDrawList(),
                                ImVec2(window_pos.x + GetCursorPosX() + CalcTextSize("Outline").x + 25, window_pos.y + GetCursorPosY() + 5 - GetScrollY()),
                                0xffffffff,
                                outline_color_picker_enabled ? ImGuiDir_Up : ImGuiDir_Down,
                                0.5f
                            );

                            SetCursorPos(ImVec2(GetCursorPosX() + CalcTextSize("Outline").x + 30, GetCursorPosY() + 5));
                            if (ImGui::InvisibleButton("player_esp_outline_color_button", ImVec2(20, 15))
                                && (!color_picker_enabled && !filling_color_picker_enabled))
                                outline_color_picker_enabled = !outline_color_picker_enabled;

                            if (outline_color_picker_enabled) {
                                current_cursor_pos.y += 35;
                                SetCursorPos(current_cursor_pos);

                                if (ImGui::ColorPicker3f(player_esp_outline_color))
                                    visuals->player_esp.update_outline_color();
                            }

                            // Filling
                            if (outline_color_picker_enabled) current_cursor_pos.y += 260;
                            else current_cursor_pos.y += 30;

                            SetCursorPos(current_cursor_pos);
                            if (ImGui::Checkbox("Filling", &visuals->player_esp.filling))
                                visuals->player_esp.update_filling();

                            SetCursorPos(current_cursor_pos);
                            RenderArrow(
                                GetWindowDrawList(),
                                ImVec2(window_pos.x + GetCursorPosX() + CalcTextSize("Filling").x + 25, window_pos.y + GetCursorPosY() + 5 - GetScrollY()),
                                0xffffffff,
                                filling_color_picker_enabled ? ImGuiDir_Up : ImGuiDir_Down,
                                0.5f
                            );

                            SetCursorPos(ImVec2(GetCursorPosX() + CalcTextSize("Filling").x + 30, GetCursorPosY() + 5));
                            if (ImGui::InvisibleButton("player_esp_filling_color_button", ImVec2(20, 15))
                                && (!color_picker_enabled && !outline_color_picker_enabled))
                                filling_color_picker_enabled = !filling_color_picker_enabled;

                            if (filling_color_picker_enabled) {
                                current_cursor_pos.y += 35;
                                SetCursorPos(current_cursor_pos);

                                if (ImGui::ColorPicker4f(player_esp_filling_color))
                                    visuals->player_esp.update_filling_color();
                            }

                            if (filling_color_picker_enabled) enable_checkbox_pos = ImVec2(current_cursor_pos.x, current_cursor_pos.y + 260);
                            else enable_checkbox_pos = ImVec2(current_cursor_pos.x, current_cursor_pos.y + 30);

                            SetCursorPos(enable_checkbox_pos);
                            if (ImGui::Checkbox("Enable", &visuals->player_esp.enabled))
                                visuals->player_esp.update_enabled();
                        }

                        // NameTags
                        ImVec2 renderer_module_name_pos = { 75, 0 };
                        if (player_esp_show) renderer_module_name_pos.y = enable_checkbox_pos.y + segu->FontSize + 30;
                        else renderer_module_name_pos.y = segu->FontSize + 50;

                        renderer_module_name = u8"NameTags";
                        renderer_module_button_size = CalcTextSize(renderer_module_name);

                        SetCursorPos(renderer_module_name_pos);
                        ImGui::Text(renderer_module_name);

                        // Rendering of the arrow next to it
                        SetCursorPos({ renderer_module_name_pos.x + 10, renderer_module_name_pos.y });
                        RenderArrow(
                            GetWindowDrawList(),
                            ImVec2(window_pos.x + GetCursorPosX() + renderer_module_button_size.x - 12, window_pos.y + GetCursorPosY() + 8 - GetScrollY()),
                            0xffffffff,
                            nametags_show ? ImGuiDir_Up : ImGuiDir_Down,
                            0.5f
                        );

                        SetCursorPos(renderer_module_name_pos);
                        if (ImGui::InvisibleButton("nametags_settings", ImVec2(renderer_module_button_size.x + 14, renderer_module_button_size.y)))
                            nametags_show = !nametags_show;

                        if (nametags_show) {
                            if (ImGui::CheckboxWithCustomLabelID("Enable 3", "Enable", &visuals->name_tags.enabled))
                                visuals->name_tags.update_enabled();
                        }

                        // ChestESP
                        renderer_module_name = u8"ChestESP";
                        renderer_module_button_size = CalcTextSize(renderer_module_name);

                        ImGui::SetCursorPos(ImVec2(475 - CalcTextSize(renderer_module_name).x, 0));
                        current_cursor_pos = GetCursorPos();
                        ImGui::Text(renderer_module_name);

                        // Rendering of the arrow next to it
                        SetCursorPos({current_cursor_pos.x + 10, current_cursor_pos.y});
                        RenderArrow(
                            GetWindowDrawList(),
                            ImVec2(window_pos.x + GetCursorPosX() + renderer_module_button_size.x - 12, window_pos.y + GetCursorPosY() + 8 - GetScrollY()),
                            0xffffffff,
                            chest_esp_show ? ImGuiDir_Up : ImGuiDir_Down,
                            0.5f
                        );

                        SetCursorPos(current_cursor_pos);
                        if (ImGui::InvisibleButton("chest_esp_settings", ImVec2(renderer_module_button_size.x + 14, renderer_module_button_size.y))) {
                            player_esp_show = false;
                            chest_esp_show = !chest_esp_show;
                        }

                        if (chest_esp_show) {
                            static const char* chest_esp_current_mode = visuals->chest_esp.mode.c_str();

                            static float chest_esp_color[3] = {
                                visuals->chest_esp.box_color.h(), visuals->chest_esp.box_color.s(), visuals->chest_esp.box_color.v()
                            };
                            static float chest_esp_outline_color[3] = {
                                visuals->chest_esp.outline_color.h(), visuals->chest_esp.outline_color.s(), visuals->chest_esp.outline_color.v()
                            };
                            static float chest_esp_filling_color[4] = {
                                visuals->chest_esp.filling_color.h(), visuals->chest_esp.filling_color.s(),
                                visuals->chest_esp.filling_color.v(), visuals->chest_esp.filling_color.a()
                            };

                            visuals->chest_esp.mode = chest_esp_current_mode;

                            hsv_to_rgb(chest_esp_color, visuals->chest_esp.box_color.color);
                            hsv_to_rgb(chest_esp_outline_color, visuals->chest_esp.outline_color.color);
                            hsv_to_rgb(chest_esp_filling_color, visuals->chest_esp.filling_color.color);

                            current_cursor_pos.x -= 75;
                            current_cursor_pos.y = GetCursorPosY();

                            start_cursor_pos = current_cursor_pos;

                            SetCursorPos(current_cursor_pos);
                            ImGui::Text(u8"Mode: ");

                            SetCursorPos(ImVec2(current_cursor_pos.x + 65, current_cursor_pos.y));
                            if (DropdownList("##custom combo2", modes, 2, &chest_esp_current_mode)) {
                                visuals->chest_esp.mode = chest_esp_current_mode;
                                visuals->chest_esp.update_mode();
                            }

                            // Color
                            SelectEspColor(window_pos, current_cursor_pos, chest_esp_color, chest_esp_color_picker_enabled,
                                chest_esp_outline_color_picker_enabled, chest_esp_filling_color_picker_enabled, visuals->chest_esp);

                            // Outline
                            if (chest_esp_color_picker_enabled) current_cursor_pos = { current_cursor_pos.x, current_cursor_pos.y + 335 };
                            else current_cursor_pos = { current_cursor_pos.x, current_cursor_pos.y + 70 };

                            SetCursorPos(current_cursor_pos);
                            if (ImGui::CheckboxWithCustomLabelID("Outline 2", "Outline", &visuals->chest_esp.outline))
                                visuals->chest_esp.update_outline();

                            SetCursorPos(current_cursor_pos);
                            RenderArrow(
                                GetWindowDrawList(),
                                ImVec2(window_pos.x + GetCursorPosX() + CalcTextSize("Outline").x + 25, window_pos.y + GetCursorPosY() + 5 - GetScrollY()),
                                0xffffffff,
                                chest_esp_outline_color_picker_enabled ? ImGuiDir_Up : ImGuiDir_Down,
                                0.5f
                            );

                            SetCursorPos(ImVec2(GetCursorPosX() + CalcTextSize("Outline").x + 30, GetCursorPosY() + 5));
                            if (ImGui::InvisibleButton("chest_esp_outline_color_button", ImVec2(20, 15))
                                && (!chest_esp_color_picker_enabled && !chest_esp_filling_color_picker_enabled))
                                chest_esp_outline_color_picker_enabled = !chest_esp_outline_color_picker_enabled;

                            if (chest_esp_outline_color_picker_enabled) {
                                current_cursor_pos.y += 35;
                                SetCursorPos(current_cursor_pos);

                                if (ImGui::ColorPicker3f(chest_esp_outline_color))
                                    visuals->chest_esp.update_outline_color();
                            }

                            // Filling
                            if (chest_esp_outline_color_picker_enabled) current_cursor_pos.y += 260;
                            else current_cursor_pos.y += 30;

                            SetCursorPos(current_cursor_pos);
                            if (ImGui::CheckboxWithCustomLabelID("Filling 2", "Filling", &visuals->chest_esp.filling))
                                visuals->chest_esp.update_filling();

                            SetCursorPos(current_cursor_pos);
                            RenderArrow(
                                GetWindowDrawList(),
                                ImVec2(window_pos.x + GetCursorPosX() + CalcTextSize("Filling").x + 25, window_pos.y + GetCursorPosY() + 5 - GetScrollY()),
                                0xffffffff,
                                chest_esp_filling_color_picker_enabled ? ImGuiDir_Up : ImGuiDir_Down,
                                0.5f
                            );

                            SetCursorPos(ImVec2(GetCursorPosX() + CalcTextSize("Filling").x + 30, GetCursorPosY() + 5));
                            if (ImGui::InvisibleButton("chest_esp_filling_color_button", ImVec2(20, 15))
                                && (!chest_esp_color_picker_enabled && !chest_esp_outline_color_picker_enabled))
                                chest_esp_filling_color_picker_enabled = !chest_esp_filling_color_picker_enabled;

                            if (chest_esp_filling_color_picker_enabled) {
                                current_cursor_pos.y += 35;
                                SetCursorPos(current_cursor_pos);

                                if (ImGui::ColorPicker4f(chest_esp_filling_color))
                                    visuals->chest_esp.update_filling_color();
                            }

                            if (chest_esp_filling_color_picker_enabled) enable_checkbox_pos = ImVec2(current_cursor_pos.x, current_cursor_pos.y + 260);
                            else enable_checkbox_pos = ImVec2(current_cursor_pos.x, current_cursor_pos.y + 30);

                            SetCursorPos(enable_checkbox_pos);
                            if (ImGui::CheckboxWithCustomLabelID("Enable 2", "Enable", &visuals->chest_esp.enabled))
                                visuals->chest_esp.update_enabled();
                        }

                        // Chams
                        renderer_module_name_pos = { chest_esp_show ? current_cursor_pos.x + 75 : current_cursor_pos.x, 0 };
                        if (chest_esp_show) renderer_module_name_pos.y = enable_checkbox_pos.y + segu->FontSize + 30;
                        else renderer_module_name_pos.y = segu->FontSize + 50;

                        renderer_module_name = u8"Chams";
                        renderer_module_button_size = CalcTextSize(renderer_module_name);

                        SetCursorPos(renderer_module_name_pos);
                        ImGui::Text(renderer_module_name);

                        // Rendering of the arrow next to it
                        SetCursorPos({ renderer_module_name_pos.x + 10, renderer_module_name_pos.y });
                        RenderArrow(
                            GetWindowDrawList(),
                            ImVec2(window_pos.x + GetCursorPosX() + renderer_module_button_size.x - 12, window_pos.y + GetCursorPosY() + 8 - GetScrollY()),
                            0xffffffff,
                            chams_show ? ImGuiDir_Up : ImGuiDir_Down,
                            0.5f
                        );

                        SetCursorPos(renderer_module_name_pos);
                        if (ImGui::InvisibleButton("chams_settings", ImVec2(renderer_module_button_size.x + 14, renderer_module_button_size.y)))
                            chams_show = !chams_show;

                        if (chams_show) {
                            current_cursor_pos = { renderer_module_name_pos.x - 75, GetCursorPosY() };
                            SetCursorPos(current_cursor_pos);
                            if (ImGui::Checkbox("Players", &visuals->chams.players))
                                visuals->chams.update_players();

                            current_cursor_pos.y += 30;
                            SetCursorPos(current_cursor_pos);
                            if (ImGui::Checkbox("Chests", &visuals->chams.chests))
                                visuals->chams.update_chests();
                        }
                    }

                    ImGui::EndChild();
                    break;
                }
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
    //kogtevran->~Kogtevran();
    noHurtCam->~NoHurtCam();
    velocity->~Velocity();
    visuals->~Visuals();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd) {
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
