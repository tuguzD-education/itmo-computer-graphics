#pragma once

#ifndef INPUT_DEVICE_HPP_INCLUDED
#define INPUT_DEVICE_HPP_INCLUDED

#include <directxmath.h>

#include <unordered_set>

#include "delegates.hpp"
#include "input_key.hpp"
#include "window.hpp"

namespace computer_graphics {

class InputDevice {
  public:
    struct MouseMoveData {
        DirectX::XMFLOAT2 position;
        DirectX::XMFLOAT2 offset;
        int wheel_delta;
    };

    DECLARE_MULTICAST_DELEGATE(OnMouseMove, const MouseMoveData &);
    DECLARE_MULTICAST_DELEGATE(OnInputKeyUp, InputKey);
    DECLARE_MULTICAST_DELEGATE(OnInputKeyDown, InputKey);

    explicit InputDevice(Window &window);
    ~InputDevice();

    [[nodiscard]] bool IsKeyDown(InputKey key) const;

    [[nodiscard]] const MouseMoveData &GetMouseMoveData() const;
    [[nodiscard]] MouseMoveData &GetMouseMoveData();

    [[nodiscard]] const OnMouseMove &GetOnMouseMove() const;
    [[nodiscard]] OnMouseMove &GetOnMouseMove();

    [[nodiscard]] const OnInputKeyUp &GetOnInputKeyUp() const;
    [[nodiscard]] OnInputKeyUp &GetOnInputKeyUp();

    [[nodiscard]] const OnInputKeyDown &GetOnInputKeyDown() const;
    [[nodiscard]] OnInputKeyDown &GetOnInputKeyDown();

  private:
    friend class Window;

    void OnRawKeyboard(const RAWKEYBOARD &data);
    void OnRawMouse(const RAWMOUSE &data);

    void AddPressedKey(InputKey key);
    void RemovePressedKey(InputKey key);

    Window &window_;
    MouseMoveData mouse_move_data_;
    OnMouseMove on_mouse_move_;
    OnInputKeyDown on_input_key_down_;
    OnInputKeyUp on_input_key_up_;
    std::unordered_set<InputKey> keys_;
};

}  // namespace computer_graphics

#endif  // INPUT_DEVICE_HPP_INCLUDED