#include "computer_graphics/window.hpp"

#include <computer_graphics/detail/string_api_set.hpp>
#include <computer_graphics/input_device.hpp>
#include <stdexcept>
#include <vector>

namespace computer_graphics {

Window::Window(std::string_view name, LONG width, LONG height, HINSTANCE instance_handle)
    : input_device_{}, is_destroyed_{} {
    instance_handle = (instance_handle != nullptr) ? instance_handle : GetModuleHandle(nullptr);

    std::basic_string<TCHAR> t_name = detail::MultiByteToTChar(CP_UTF8, 0, name);
    LPCTSTR c_name = t_name.c_str();

    auto icon = LoadIcon(nullptr, IDI_WINLOGO);
    WNDCLASSEX wc{
        .cbSize = sizeof(decltype(wc)),
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = WndProc,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = instance_handle,
        .hIcon = icon,
        .hCursor = LoadCursor(nullptr, IDC_ARROW),
        .hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)),
        .lpszMenuName = nullptr,
        .lpszClassName = c_name,
        .hIconSm = icon,
    };
    RegisterClassEx(&wc);

    RECT rect = {
        .right = width,
        .bottom = height,
    };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    handle_ = CreateWindowEx(
        WS_EX_APPWINDOW, c_name, c_name,
        WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME,
        (GetSystemMetrics(SM_CXSCREEN) - width) / 2, (GetSystemMetrics(SM_CYSCREEN) - height) / 2,
        rect.right - rect.left, rect.bottom - rect.top,
        nullptr, nullptr, instance_handle, this);
    ShowWindow(handle_, SW_SHOW);
    SetForegroundWindow(handle_);
    SetFocus(handle_);
    ShowCursor(true);
}

bool Window::ErrorBox(LPCTSTR text, LPCTSTR caption, UINT type) {
    return MessageBox(handle_, text, caption, type);
}

HWND Window::GetRawHandle() const {
    return handle_;
}

HINSTANCE Window::GetRawInstanceHandle() const {
    return (HINSTANCE)GetWindowLongPtr(handle_, GWLP_HINSTANCE);
}

RECT Window::GetRect() const {
    RECT rect{};
    GetWindowRect(handle_, &rect);
    return rect;
}

auto Window::GetDimensions() const -> Dimensions {
    RECT rect = GetRect();
    return {
        .width = rect.right - rect.left,
        .height = rect.bottom - rect.top,
    };
}

RECT Window::GetClientRect() const {
    RECT rect{};
    ::GetClientRect(handle_, &rect);
    return rect;
}

auto Window::GetClientDimensions() const -> Dimensions {
    RECT rect = GetClientRect();
    return {
        .width = rect.right - rect.left,
        .height = rect.bottom - rect.top,
    };
}

bool Window::IsDestroyed() const {
    return is_destroyed_;
}

bool Window::SetTitle(std::string_view title) {
    std::basic_string<TCHAR> t_title = detail::MultiByteToTChar(CP_UTF8, 0, title);
    LPCTSTR c_title = t_title.c_str();
    return SetWindowText(handle_, c_title);
}

void Window::ProcessQueueMessages() {
    MSG msg;
    while (PeekMessage(&msg, handle_, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Window::Destroy() {
    if (IsDestroyed()) {
        return;
    }
    DestroyWindow(handle_);
}

LRESULT CALLBACK Window::WndProc(HWND h_wnd, UINT u_message, WPARAM w_param, LPARAM l_param) {
    Window *window;
    if (u_message == WM_NCCREATE) {
        auto create_struct = reinterpret_cast<LPCREATESTRUCT>(l_param);
        window = static_cast<Window *>(create_struct->lpCreateParams);
        SetWindowLongPtr(h_wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    } else {
        window = reinterpret_cast<Window *>(GetWindowLongPtr(h_wnd, GWLP_USERDATA));
    }
    if (!window) {
        return DefWindowProc(h_wnd, u_message, w_param, l_param);
    }

    switch (u_message) {
        case WM_INPUT: {
            InputDevice *input_device = window->input_device_;
            if (input_device == nullptr) {
                break;
            }

            static std::vector<BYTE> lpb;

            UINT size;
            auto h_raw_input = (HRAWINPUT)l_param;
            UINT result = ::GetRawInputData(h_raw_input, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
            if (result) {
                throw std::runtime_error{"Failed to obtain raw input"};
            }

            lpb.reserve(size);
            result = ::GetRawInputData(h_raw_input, RID_INPUT, lpb.data(), &size, sizeof(RAWINPUTHEADER));
            if (result != size) {
                throw std::runtime_error{"Failed to obtain raw input: GetRawInputData does not return correct size"};
            }

            auto raw_input = (RAWINPUT *)lpb.data();
            switch (raw_input->header.dwType) {
                case RIM_TYPEKEYBOARD: {
                    input_device->OnRawKeyboard(raw_input->data.keyboard);
                    break;
                }
                case RIM_TYPEMOUSE: {
                    input_device->OnRawMouse(raw_input->data.mouse);
                    break;
                }
                default: {
                    break;
                }
            }

            lpb.clear();
            break;
        }
        case WM_CLOSE: {
            DestroyWindow(h_wnd);
            break;
        }
        case WM_DESTROY: {
            window->is_destroyed_ = true;
            break;
        }
        default: {
            return DefWindowProc(h_wnd, u_message, w_param, l_param);
        }
    }
    return 0;
}

}  // namespace computer_graphics