#pragma once

#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include "game.fwd"
#include "detail/d3d_ptr.hpp"

#include "component.hpp"
#include "window.hpp"
#include "timer.hpp"

#include <d3d11.h>
#include <wrl/client.h>

#include <vector>
#include <memory>

namespace computer_graphics {

class Game {
  public:
    explicit Game(Window &window);

    void Run();

  private:
    friend class Component;

    void InitializeDevice();
    void InitializeSwapChain(const Window &window);
    void InitializeRenderTargetView();

    void Draw();

    Timer timer_;
    Window &window_;

    LONG initial_width_;
    LONG initial_height_;

    std::vector<std::unique_ptr<Component>> components_;

    detail::D3DPtr<ID3D11RenderTargetView> render_target_view_;
    detail::D3DPtr<IDXGISwapChain> swap_chain_;
    detail::D3DPtr<ID3D11DeviceContext> device_context_;
    detail::D3DPtr<ID3D11Device> device_;
};

}

#endif //GAME_HPP_INCLUDED
