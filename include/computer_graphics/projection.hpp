#pragma once

#ifndef PROJECTION_HPP_INCLUDED
#define PROJECTION_HPP_INCLUDED

#include <numbers>

#include "math.hpp"
#include "transform.hpp"

namespace computer_graphics {

class Projection {
  public:
    virtual ~Projection();

    [[nodiscard]] virtual math::Matrix4x4 ToMatrix(
        float width, float height, float near_plane, float far_plane) const = 0;
    [[nodiscard]] virtual math::Frustum ToFrustum(
        const Transform& transform, float width, float height, float near_plane, float far_plane) const;
};

class OrthographicProjection : public Projection {
  public:
    explicit OrthographicProjection(math::Vector2 units = math::Vector2{2.0f});

    [[nodiscard]] math::Vector2 Units() const;
    bool Units(math::Vector2 units);

    [[nodiscard]] math::Matrix4x4 ToMatrix(
        float width, float height, float near_plane, float far_plane) const override;

  private:
    math::Vector2 units_;
};

class PerspectiveProjection : public Projection {
  public:
    explicit PerspectiveProjection(float horizontal_fov = std::numbers::pi_v<float> / 2.0f);

    [[nodiscard]] float HorizontalFOV() const;
    bool HorizontalFOV(float horizontal_fov);

    [[nodiscard]] math::Matrix4x4 ToMatrix(
        float width, float height, float near_plane, float far_plane) const override;

  private:
    float horizontal_fov_;
};

}  // namespace computer_graphics

#endif  // PROJECTION_HPP_INCLUDED
