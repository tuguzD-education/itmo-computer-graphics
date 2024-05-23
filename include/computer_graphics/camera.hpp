#pragma once

#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include <numbers>
#include <variant>

#include "scene_component.hpp"

namespace computer_graphics {

struct PerspectiveCameraProjectionType {
    float horizontal_fov = std::numbers::pi_v<float> / 2.0f;
};

struct OrthographicCameraProjectionType {
    float orthographic_units = 2.0f;
};

using CameraProjectionType = std::variant<PerspectiveCameraProjectionType, OrthographicCameraProjectionType>;

class Camera final : public SceneComponent {
  public:
    struct Initializer : SceneComponent::Initializer {
        CameraProjectionType projection_type = OrthographicCameraProjectionType{};
        float width = 0.0f;
        float height = 0.0f;
        float near_plane = 0.1f;
        float far_plane = 100.0f;

        Initializer &ProjectionType(CameraProjectionType projection_type);
        Initializer &Width(float width);
        Initializer &Height(float height);
        Initializer &NearPlane(float near_plane);
        Initializer &FarPlane(float far_plane);
    };

    explicit Camera(class Game &game, const Initializer &initializer = {});

    [[nodiscard]] const CameraProjectionType &ProjectionType() const;
    [[nodiscard]] CameraProjectionType &ProjectionType();

    [[nodiscard]] float Width() const;
    bool Width(float width);

    [[nodiscard]] float Height() const;
    bool Height(float height);

    [[nodiscard]] float NearPlane() const;
    bool NearPlane(float near_plane);

    [[nodiscard]] float FarPlane() const;
    bool FarPlane(float far_plane);

    [[nodiscard]] float AspectRatio() const;
    [[nodiscard]] float InverseAspectRatio() const;

    [[nodiscard]] math::Matrix4x4 View() const;
    [[nodiscard]] math::Matrix4x4 Projection() const;

    [[nodiscard]] math::Frustum Frustum() const;

  private:
    float width_;
    float height_;
    float near_plane_;
    float far_plane_;
    CameraProjectionType projection_type_;
};

}  // namespace computer_graphics

#endif  // CAMERA_HPP_INCLUDED