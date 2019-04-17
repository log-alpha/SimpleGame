#include <iostream>
#include <windows.h>
#include <time.h>
#include <math.h>

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
        m_time += time;
        if(m_time - last_update < 1.0f) return;
        last_update = (int)floor(m_time);
        switch(m_snakerState.m_dir) {
            case SnakerDireCtion::UP:
                m_snakerState.m_iPosy++;
                break;
            case SnakerDireCtion::DOWN:
                m_snakerState.m_iPosy--;
                break;
            case SnakerDireCtion::RIGHT:
                m_snakerState.m_iPosx++;
                break;
            case SnakerDireCtion::LEFT:
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
};

int main(){
    int count = 0;
    std::cout << time(0) << std::endl;
    HWND hWnd = GetConsoleWindow();
    std::cout << hWnd << std::endl;
    Game oneGame;
    std::cout << "interval=" << interval << std::endl;
    while(true){
        Sleep(interval);
        //std::cout << count++ << std::endl;
        oneGame.elapse();
    }
    std::cout << time(0) << std::endl;
    return 0;
}
