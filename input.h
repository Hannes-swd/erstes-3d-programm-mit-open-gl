#pragma once
extern GLFWwindow* window;

enum Tasten
{
    MouseRechts,
    MouseLinks,
    egal,

    Key_A, Key_B, Key_C, Key_D, Key_E, Key_F, Key_G, Key_H, Key_I, Key_J, Key_K, Key_L, Key_M,
    Key_N, Key_O, Key_P, Key_Q, Key_R, Key_S, Key_T, Key_U, Key_V, Key_W, Key_X, Key_Y, Key_Z
};
bool IsKeyDown(Tasten taste);