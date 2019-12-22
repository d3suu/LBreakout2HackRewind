#include <iostream>
#include "windows.h"

using namespace std;
int privileges();
void hackDrawText(LPCSTR message, int FromLeft, int FromTop, int Color);
HWND publicHWND;

int main()
{
    cout << "Hello world!" << endl;
    int p = privileges();
    if(p!=0)
        return 1; // No privileges

    HWND hwnd;
    hwnd = FindWindow(0, "LBreakout2");
    if(hwnd==NULL)
        return 2; // Window not found, or error
    publicHWND = hwnd;
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS,0,pid);

    // Memory Write Test
    /*
    long testAddrA = 0x04F5B384;
    long testAddrB = 0x06B40CE4;
    int status = 1;
    WriteProcessMemory(phandle, (LPVOID)testAddrA, &status, sizeof(int), NULL);
    WriteProcessMemory(phandle, (LPVOID)testAddrB, &status, sizeof(int), NULL);
    */

    // Button 'S' test
    /*
    while(1){
        if(GetKeyState('S') < 0){
            cout << "yes\n";
        }else{
            cout << "no\n";
        }
    }
    */

    // Write text test
    /*
    while(1){
        RECT rect;
        HDC wdc = GetWindowDC(hwnd);
        GetClientRect(hwnd, &rect);
        SetTextColor(wdc, 0x00FFFFFF);
        SetBkMode(wdc, TRANSPARENT);
        rect.left=40;
        rect.top=100;
        LPCSTR message = "TEST123";
        DrawText(wdc, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP );
        DeleteDC(wdc);
    }
    */


    // Hack loop
    bool pauseState = true;
    while(1){
        if(pauseState == true){
            // Draw text
            hackDrawText("LBreakout2 test hack by d3suu 2019", 10, 50, 0x00FFFFFF);
            hackDrawText("Z - Rainbow! (+score from bricks)", 10, 70, 0x00FFFFFF);
            hackDrawText("X - Energy Balls (penetrate bricks)", 10, 90, 0x00FFFFFF);
            hackDrawText("C - Explosive Balls", 10, 110, 0x00FFFFFF);
            hackDrawText("V - Extra Life", 10, 130, 0x00FFFFFF);
            hackDrawText("B - Maximum paddle", 10, 150, 0x00FFFFFF);
            hackDrawText("N - +10k score", 10, 170, 0x00FFFFFF);

            // Check keys
            if(GetKeyState('P') < 0){ // Pause key
                //cout << "TRIGGER1\n";
                pauseState = false;
                while(GetKeyState('P') < 0){}; // wait for key release
            }
        }
        // Check pause key state
        if(GetKeyState('P') < 0){
            //cout << "TRIGGER2\n";
            pauseState = true;
            while(GetKeyState('P') < 0){}; // wait for key release
        }
        // Check Rainbow Key (Something is not right with adresses, it does not work for now)
        if(GetKeyState('Z') < 0){
            cout << "RAINBOW TRIGGER\n";
            // Check for current Rainbow status
            int status = 0;
            int setStatus = 0;
            long RainbowAddrA = 0x04F643DC;
            long RainbowAddrB = 0x0697A42C;
            long RainbowAddrC = 0x0697A544;
            long RainbowAddrD = 0x0697A8DC;
            ReadProcessMemory(phandle, (LPVOID)RainbowAddrA, &status, sizeof(int),0);
            if(status == 0){
                // Enable rainbow
                setStatus = 1;
            }else{
                // Disable rainbow
                setStatus = 0;
            }
            // Write data
            WriteProcessMemory(phandle, (LPVOID)RainbowAddrA, &setStatus, sizeof(int), NULL);
            WriteProcessMemory(phandle, (LPVOID)RainbowAddrB, &setStatus, sizeof(int), NULL);
            WriteProcessMemory(phandle, (LPVOID)RainbowAddrC, &setStatus, sizeof(int), NULL);
            WriteProcessMemory(phandle, (LPVOID)RainbowAddrD, &setStatus, sizeof(int), NULL);
            while(GetKeyState('Z') < 0){}; // wait for release
        }
        // Extra life
        if(GetKeyState('V') < 0){
            int status = 0;
            long LifeAddr = 0x0048EC20;
            ReadProcessMemory(phandle, (LPVOID)LifeAddr, &status, sizeof(int),0);
            status += 1;
            // Write data
            WriteProcessMemory(phandle, (LPVOID)LifeAddr, &status, sizeof(int), NULL);
            while(GetKeyState('V') < 0){}; // wait for release
        }
        // Maximum paddle
        if(GetKeyState('B') < 0){
            long PaddleAddrA = 0x052EB388;
            long PaddleAddrB = 0x047B9BE8;
            // Write data
            int status = 550;
            WriteProcessMemory(phandle, (LPVOID)PaddleAddrA, &status, sizeof(int), NULL);
            WriteProcessMemory(phandle, (LPVOID)PaddleAddrB, &status, sizeof(int), NULL);
            while(GetKeyState('B') < 0){}; // wait for release
        }
        // +10k Score
        if(GetKeyState('N') < 0){
            int status = 0;
            long ScoreAddr = 0x0048EC24;
            ReadProcessMemory(phandle, (LPVOID)ScoreAddr, &status, sizeof(int),0);
            status += 10000;
            // Write data
            WriteProcessMemory(phandle, (LPVOID)ScoreAddr, &status, sizeof(int), NULL);
            while(GetKeyState('N') < 0){}; // wait for release
        }
    }


    CloseHandle(phandle);

    return 0;
}

void hackDrawText(LPCSTR message, int FromLeft, int FromTop, int Color){
    RECT rect;
    HDC wdc = GetWindowDC(publicHWND);
    GetClientRect(publicHWND, &rect);
    SetTextColor(wdc, Color);
    SetBkMode(wdc, TRANSPARENT);
    rect.left=FromLeft;
    rect.top=FromTop;
    DrawText(wdc, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP );
    DeleteDC(wdc);
}

int privileges(){
    HANDLE Token;
    TOKEN_PRIVILEGES tp;
    if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &Token)){
        LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
        tp.PrivilegeCount = 1;
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(Token, 0, &tp, sizeof(tp), NULL, NULL);
        return 0;
    }else{
        return 1;
    }
}
