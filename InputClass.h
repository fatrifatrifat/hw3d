#pragma once
#include <Windows.h>

class Input {
public:
    Input() { memset(keys, 0, sizeof(keys)); }

    void Update() {
        for (int i = 0; i < 256; i++)
            keys[i] = GetAsyncKeyState(i) & 0x8000;
    }

    bool IsKeyPressed(int key) const { return keys[key]; }

private:
    bool keys[256];
};