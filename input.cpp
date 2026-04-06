#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>  
#include <vector>

#include "input.h"


bool IsKeyDown(Tasten taste) {
    switch (taste) {
    case MouseRechts:
        return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    case MouseLinks:
        return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    case Key_A: return glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    case Key_B: return glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS;
    case Key_C: return glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
    case Key_D: return glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    case Key_E: return glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
    case Key_F: return glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
    case Key_G: return glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS;
    case Key_H: return glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS;
    case Key_I: return glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS;
    case Key_J: return glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS;
    case Key_K: return glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS;
    case Key_L: return glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS;
    case Key_M: return glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS;
    case Key_N: return glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS;
    case Key_O: return glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS;
    case Key_P: return glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
    case Key_Q: return glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
    case Key_R: return glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
    case Key_S: return glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    case Key_T: return glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS;
    case Key_U: return glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS;
    case Key_V: return glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS;
    case Key_W: return glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    case Key_X: return glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS;
    case Key_Y: return glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS;
    case Key_Z: return glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS;

    case Key_Up:    return glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
    case Key_Down:  return glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
    case Key_Left:  return glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
    case Key_Right: return glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;

    case egal:  return glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS;

    default:
        return false;
    }
}