#pragma once

#ifndef TRIANGLECOMPONENT_HPP_INCLUDED
#define TRIANGLECOMPONENT_HPP_INCLUDED

#include "detail/d3d_ptr.hpp"

#include "component.hpp"

#include <directxmath.h>
#include <wrl/client.h>

#include <span>

namespace computer_graphics {

class TriangleComponent : public Component {
  public:
    struct Vertex {
      DirectX::XMFLOAT4 position;
      DirectX::XMFLOAT4 color;
    };
    using Index = int;

    explicit TriangleComponent(
        Game &game,
        std::span<Vertex> vertices,
        std::span<Index> indices
    );

    void Update(float delta_time) override;
    void Draw() override;

  private:
    void InitializeVertexShader();
    void InitializeIndexShader();
    void InitializeInputLayout();
    void InitializeRasterizerState();
    void InitializeVertexBuffer(std::span<Vertex> vertices);
    void InitializeIndexBuffer(std::span<Index> indices);

    detail::D3DPtr<ID3D11RasterizerState> rasterizer_state_;
    detail::D3DPtr<ID3D11InputLayout> input_layout_;

    detail::D3DPtr<ID3D11Buffer> index_buffer_;
    detail::D3DPtr<ID3D11PixelShader> index_shader_;
    detail::D3DPtr<ID3DBlob> index_byte_code_;

    detail::D3DPtr<ID3D11Buffer> vertex_buffer_;
    detail::D3DPtr<ID3D11VertexShader> vertex_shader_;
    detail::D3DPtr<ID3DBlob> vertex_byte_code_;
};

}

#endif //TRIANGLECOMPONENT_HPP_INCLUDED
