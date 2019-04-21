#include <iostream>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <string>

float FPS = 60.0f;  // frame per second
float interval = 1000.0f / FPS; // update interval

enum SnakerDireCtion{UP ,RIGHT ,DOWN ,LEFT};

struct SnakerState{
    int m_iPosx = 0, m_iPosy = 0;
    int m_iLength = 0;
    SnakerDireCtion m_dir = SnakerDireCtion::UP;
}g_DefaultSnakerState;

// define once game state
class Game{
public:
    int m_iScore;
    int m_iPlayerNum; // always 1
    float m_time; // Game time
    SnakerState m_snakerState;
    Game():
    m_iScore(0), 
    m_iPlayerNum(0) {}
    void elapse(){
        static int s_fCurTime = GetTickCount();
        static int s_fLastTime = s_fCurTime;
        s_fCurTime = GetTickCount();
        this->tick( (s_fCurTime - s_fLastTime) / 1000.0f);
        s_fLastTime = s_fCurTime;
        //std::cout << "s_fCurTime=" << s_fCurTime << std::endl;
        //std::cout << "s_fLastTime=" << s_fLastTime << std::endl;
    }
    void tick(float time) {
        static int last_update = (int)floor(m_time);
        m_time += time * 5;
        if(m_time - last_update < 1.0f) return;
        last_update = (int)floor(m_time);
        switch(m_snakerState.m_dir) {
            case SnakerDireCtion::UP:
                if(m_snakerState.m_iPosy<19)
                    m_snakerState.m_iPosy++;
                break;
            case SnakerDireCtion::DOWN:
                if(m_snakerState.m_iPosy>0)
                    m_snakerState.m_iPosy--;
                break;
            case SnakerDireCtion::RIGHT:
                if(m_snakerState.m_iPosx<19)
                    m_snakerState.m_iPosx++;
                break;
            case SnakerDireCtion::LEFT:
                if(m_snakerState.m_iPosx>0)
                    m_snakerState.m_iPosx--;
                break;
            default:
                break;
        }
        this->show_state();
    }
    void show_state() {
        std::cout << "(" << m_snakerState.m_iPosx << ", " << 
        m_snakerState.m_iPosy << ")" << std::endl;
    }
    void keyControl(KEY_EVENT_RECORD keyEvent) {
        
        switch(keyEvent.uChar.AsciiChar) {
            case 'w':
                if(m_snakerState.m_dir != SnakerDireCtion::DOWN) {
                    m_snakerState.m_dir = SnakerDireCtion::UP;
                }
                break;
            case 's':
                if(m_snakerState.m_dir != SnakerDireCtion::UP) {
                    m_snakerState.m_dir = SnakerDireCtion::DOWN;
                }
                break;
            case 'a':
                if(m_snakerState.m_dir != SnakerDireCtion::RIGHT) {
                    m_snakerState.m_dir = SnakerDireCtion::LEFT;
                }
                break;
            case 'd':
                if(m_snakerState.m_dir != SnakerDireCtion::LEFT) {
                    m_snakerState.m_dir = SnakerDireCtion::RIGHT;
                }
                break;
            default:
                break;
        }
    }
};

Game oneGame;

int initConsole() {
    system("mode con cols=5 lines=5  ");
    CHAR_INFO ch[80];
    memset(ch, '-', sizeof(ch));
    SMALL_RECT srctWriteRect;
    srctWriteRect.Top = 0;
    srctWriteRect.Left = 0;
    srctWriteRect.Bottom = 0;
    srctWriteRect.Right = 79;
    COORD coordBufSize;
    COORD coordBufCoord;
    coordBufSize.Y = 1;
    coordBufSize.X = 80;
    coordBufCoord.X = 0;
    coordBufCoord.Y = 0;
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    BOOL fSuccess;
    HANDLE hNewScreenBuffer = CreateConsoleScreenBuffer( 
       GENERIC_READ |           // read/write access 
       GENERIC_WRITE, 
       FILE_SHARE_READ | 
       FILE_SHARE_WRITE,        // shared 
       NULL,                    // default security attributes 
       CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE 
       NULL);
    if (! SetConsoleActiveScreenBuffer(hNewScreenBuffer) ) 
    {
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError()); 
        return 1;
    }

    fSuccess = WriteConsoleOutput(
        hNewScreenBuffer,
        ch,
        coordBufSize,
        coordBufCoord,
        &srctWriteRect);
    srctWriteRect.Top++;
    srctWriteRect.Bottom++;
    if (!fSuccess) {
        printf("ReadConsoleOutput failed - (%d)\n", GetLastError()); 
        return 1;
    }
    return 0;
}

void ReadInput() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); 
    INPUT_RECORD irInBuf[128]; 
    DWORD cNumRead;
    if(! PeekConsoleInput(
        hStdin,
        irInBuf,
        128,
        &cNumRead)){
            std::cout << "ReadConsoleInput error" << std::endl;
        }
    if (cNumRead) {
        FlushConsoleInputBuffer(hStdin);
    }
    for (int i = 0; i < cNumRead; i++) 
    {
        switch(irInBuf[i].EventType) 
        { 
            case KEY_EVENT: // keyboard input 
                oneGame.keyControl(irInBuf[i].Event.KeyEvent);
                break;
            default: 
                break; 
        } 
    }
}

HANDLE hNewScreenBuffer[2];

// 40 x 40 size
int initRender() {
    SMALL_RECT srctWindow;
    srctWindow.Top = 0;
    srctWindow.Left = 0;
    srctWindow.Bottom = 30;
    srctWindow.Right = 30;
    for(int i = 0;i < 2; ++i){
        hNewScreenBuffer[i] = CreateConsoleScreenBuffer( 
        GENERIC_READ |           // read/write access 
        GENERIC_WRITE, 
        FILE_SHARE_READ | 
        FILE_SHARE_WRITE,        // shared 
        NULL,                    // default security attributes 
        CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE 
        NULL);
        SetConsoleWindowInfo(hNewScreenBuffer[i], TRUE, &srctWindow);
        if (! SetConsoleActiveScreenBuffer(hNewScreenBuffer[i]) ) 
        {
            printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError()); 
            return 1;
        }
    }
    return 0;
}

int RenderConsole(Game oneGame) {
    static int bufferCount = 0;
    CHAR_INFO buf[20 * 20];
    memset(buf, '-', sizeof(buf));
    SMALL_RECT srctWriteRect;
    buf[oneGame.m_snakerState.m_iPosx  + (19 - oneGame.m_snakerState.m_iPosy) * 20].Char.AsciiChar = CHAR("-");
    srctWriteRect.Top = 0;
    srctWriteRect.Left = 0;
    srctWriteRect.Bottom = 19;
    srctWriteRect.Right = 19;
    COORD coordBufSize, coordBufCoord;
    coordBufSize.X = 20;
    coordBufSize.Y = 20;
    coordBufCoord.X = 0;
    coordBufCoord.Y = 0;
    bool fSuccess = false;
    DWORD iCharWritten;
    fSuccess = WriteConsoleOutput(
        hNewScreenBuffer[bufferCount],
        buf,
        coordBufSize,
        coordBufCoord,
        &srctWriteRect);
    if (!fSuccess) {
        printf("WriteConsoleOutput failed - (%d)\n", GetLastError()); 
        return 1;
    }

    if (! SetConsoleActiveScreenBuffer(hNewScreenBuffer[bufferCount]) ) 
    {
       printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError()); 
       return 1;
    }

    bufferCount++;
    bufferCount%=2;
    return 0;
}

int main(){
    int count = 0;
    std::cout << time(0) << std::endl;
    HWND hWnd = GetConsoleWindow();
    std::cout << hWnd << std::endl;
    
    std::cout << "interval=" << interval << std::endl;
    initRender();
    while(true){
        Sleep(interval);
        ReadInput();
        RenderConsole(oneGame);
        //std::cout << count++ << std::endl;
        oneGame.elapse();
    }
    std::cout << time(0) << std::endl;
    return 0;
}
