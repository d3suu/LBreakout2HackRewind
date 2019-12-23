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
            //hackDrawText("Z - Rainbow! (+score from bricks)", 10, 70, 0x00FFFFFF);
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
            // Find pointers
            long PaddlePhysicalPointerBase = 0x0400000 + 0x0008FE20;
            long PaddlePhysicalPointerSecond;
            ReadProcessMemory(phandle, (LPVOID)PaddlePhysicalPointerBase, &PaddlePhysicalPointerSecond, sizeof(DWORD),0);
            PaddlePhysicalPointerSecond += 0x228;
            long PaddlePhysicalPointerFinal;
            ReadProcessMemory(phandle, (LPVOID)PaddlePhysicalPointerSecond, &PaddlePhysicalPointerFinal, sizeof(DWORD),0);
            PaddlePhysicalPointerFinal += 0x20; // Final pointer

            long PaddleViewPointerBase = 0x0400000 + 0x0006C674;
            long PaddleViewPointerFinal;
            ReadProcessMemory(phandle, (LPVOID)PaddleViewPointerBase, &PaddleViewPointerFinal, sizeof(DWORD),0);
            PaddleViewPointerFinal += 0x20; // Final pointer

            // Write data
            int MaxPaddleSize = 554;
            WriteProcessMemory(phandle, (LPVOID)PaddlePhysicalPointerFinal, &MaxPaddleSize, sizeof(int), NULL);
            WriteProcessMemory(phandle, (LPVOID)PaddleViewPointerFinal, &MaxPaddleSize, sizeof(int), NULL);
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
        // Explosive ball
        if(GetKeyState('C') < 0){
            //cout << "EXPLOSIVE TRIGGER\n";
            // Get the pointer value 1
            long ExplosiveBaseAddressOne = 0x0400000+0x0008DDA0;
            /* Self-learning part:
             - Windows PE starts on "ImageBase" address which is hard coded in
             - executable header. In this case, it's 0x0400000. Now we can add
             - offsets from Cheat Engine.
            */
            DWORD pointA;
            ReadProcessMemory(phandle, (LPVOID)ExplosiveBaseAddressOne, &pointA, sizeof(DWORD),0);
            //cout << "Pointer: 0x" << hex << pointA << dec << endl;
            //cout << "Pointer-end: 0x" << hex << pointA+0x42C4 << dec << endl;
            int status = 0; // Read current status for explosive ball
            ReadProcessMemory(phandle, (LPVOID)(pointA+0x42C4), &status, sizeof(int), 0);
            // Get pointer value 2
            long ExplosiveBaseAddressTwo = 0x0400000+0x0006C67C;
            DWORD pointB;
            ReadProcessMemory(phandle, (LPVOID)ExplosiveBaseAddressTwo, &pointB, sizeof(DWORD),0);

            int setStatus = 0;
            if(status == 0){
                // enable explosive ball
                setStatus = 1;
            }else{
                // disable explosive ball
                setStatus = 0;
            }

            // Write data
            WriteProcessMemory(phandle, (LPVOID)(pointA+0x42C4), &setStatus, sizeof(int), NULL);
            WriteProcessMemory(phandle, (LPVOID)(pointB+0x42C4), &setStatus, sizeof(int), NULL);

            while(GetKeyState('C') < 0){}; // wait for release
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
