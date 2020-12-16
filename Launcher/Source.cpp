#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND hEditIP;
HWND hEditPORT;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"myWindowClass";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"myWindowClass", L"Launcher", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600, 400, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    CreateWindow(L"button", L"Client", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 200, 100, 80, 30, hwnd, (HMENU)10001, hInstance, NULL);
    CreateWindow(L"button", L"Server", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 320, 100, 80, 30, hwnd, (HMENU)10002, hInstance, NULL);

    CreateWindow(L"button", L"OK", WS_CHILD | WS_VISIBLE | BS_USERBUTTON, 200, 300, 80, 30, hwnd, (HMENU)10003, hInstance, NULL);
    CreateWindow(L"button", L"Cancel", WS_CHILD | WS_VISIBLE | BS_USERBUTTON, 300, 300, 80, 30, hwnd, (HMENU)10004, hInstance, NULL);

    hEditIP = CreateWindow(L"Edit", NULL, WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE, 250, 150, 150, 20, hwnd, (HMENU)10005, hInstance, NULL);
    hEditPORT = CreateWindow(L"Edit", NULL, WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE, 250, 200, 150, 20, hwnd, (HMENU)10006, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc; // создаЄм дескриптор ориентации текста на экране
    PAINTSTRUCT psMess; // структура, сод-ща€ информацию о клиентской области (размеры, цвет и тп)
    RECT rectMess; // стр-ра, определ€юща€ размер клиентской области
    COLORREF colorTextMess = RGB(0, 0, 0); // задаЄм цвет текста

    RECT rectIP; // стр-ра, определ€юща€ размер клиентской области
    COLORREF colorTextIP = RGB(255, 0, 0); // задаЄм цвет текста

    RECT rectPORT; // стр-ра, определ€юща€ размер клиентской области
    COLORREF colorTextPORT = RGB(255, 0, 0); // задаЄм цвет текста

    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
        // ≈сли мы нажали на 1-й радиокнопке.
        if (LOWORD(wParam) == 10001)
        {
            SetWindowPos(hEditIP, NULL, 250, 150, 150, 20, SWP_SHOWWINDOW);
            SetWindowPos(hEditPORT, NULL, 250, 200, 150, 20, SWP_SHOWWINDOW);
        }
        // ≈сли мы нажали на 2-й радиокнопке.
        if (LOWORD(wParam) == 10002)
        {
            SetWindowPos(hEditIP, NULL, 250, 150, 150, 20, SWP_HIDEWINDOW);
            SetWindowPos(hEditPORT, NULL, 250, 200, 150, 20, SWP_HIDEWINDOW);
        }

        if (LOWORD(wParam) == 10003)
        {
            TCHAR buff1[1024];
            TCHAR buff2[1024];
            SIZE_T buff1_len = 0;
            SIZE_T buff2_len = 0;

            HANDLE hFile = CreateFileA("data.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_DELETE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

            GetWindowText(hEditIP, buff1, 1024);
            GetWindowText(hEditPORT, buff2, 1024);
            buff1_len = _tcslen(buff1);
            buff2_len = _tcslen(buff2);

            WriteFile(hFile, buff1, buff1_len * 2, &buff1_len, NULL);
            WriteFile(hFile, "\n", 2, NULL, NULL);
            WriteFile(hFile, buff2, buff2_len * 2, &buff2_len, NULL);
            CloseHandle(hFile);
            DestroyWindow(hwnd);
        }

        if (LOWORD(wParam) == 10004)
        {
            DestroyWindow(hwnd);
        }

        break;

    case WM_PAINT: // если нужно нарисовать, то:

        hdc = BeginPaint(hwnd, &psMess); // инициализируем контекст устройства


        GetClientRect(hwnd, &rectMess); // получаем ширину и высоту области дл€ рисовани€
        rectMess.top = 70;
        SetTextColor(hdc, colorTextMess); // устанавливаем цвет контекстного устройства
        DrawText(hdc, L"Please, select who you are:", -1, &rectMess, DT_SINGLELINE | DT_CENTER | DT_TOP); // рисуем текст



        GetClientRect(hwnd, &rectIP); // получаем ширину и высоту области дл€ рисовани€
        rectIP.top = 150;
        rectIP.left = -110;
        SetTextColor(hdc, colorTextIP); // устанавливаем цвет контекстного устройства
        DrawText(hdc, L"IP:", -1, &rectIP, DT_SINGLELINE | DT_CENTER | DT_TOP); // рисуем текст



        GetClientRect(hwnd, &rectPORT); // получаем ширину и высоту области дл€ рисовани€
        rectPORT.top = 200;
        rectPORT.left = 200;
        SetTextColor(hdc, colorTextPORT); // устанавливаем цвет контекстного устройства
        DrawText(hdc, L"PORT:", -1, &rectPORT, DT_SINGLELINE | DT_TOP); // рисуем текст


        EndPaint(hwnd, &psMess); // заканчиваем рисовать
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
