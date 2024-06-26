#include "computer_graphics/detail/texture.hpp"

#include <WICTextureLoader.h>

#include <format>

#include "computer_graphics/detail/check_result.hpp"

namespace computer_graphics::detail {

D3DPtr<ID3D11ShaderResourceView> TextureFromFile(
    ID3D11Device& device, ID3D11DeviceContext& device_context, const std::filesystem::path& path) {
    D3DPtr<ID3D11ShaderResourceView> texture;

    const HRESULT result = DirectX::CreateWICTextureFromFile(
        &device, &device_context, path.c_str(), nullptr, &texture);
    CheckResult(result, [&] { return std::format(
        "Failed to create texture from file '{}'", path.generic_string()); });

    return texture;
}

}  // namespace computer_graphics::detail
