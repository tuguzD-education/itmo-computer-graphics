#include "phobos.hpp"

#include <computer_graphics/game.hpp>

namespace detail {

computer_graphics::GeometricPrimitiveComponent& CreatePhobosMesh(
    computer_graphics::Game& game, const computer_graphics::SceneComponent* parent) {
    computer_graphics::GeometricPrimitiveComponent::Initializer initializer{
        .primitive_arguments =
            computer_graphics::TeapotGeometricPrimitiveArguments{
                .size = 0.1f,
            },
        .color = computer_graphics::math::colors::linear::PaleVioletRed.v,
    };
    initializer.Parent(parent);
    return game.AddComponent<computer_graphics::GeometricPrimitiveComponent>(initializer);
}

}  // namespace detail

Phobos::Phobos(computer_graphics::Game& game, const Initializer& initializer)
    : SceneComponent(game, initializer), mesh_{detail::CreatePhobosMesh(Game(), this)} {}

const computer_graphics::GeometricPrimitiveComponent& Phobos::Mesh() const {
    return mesh_;
}

computer_graphics::GeometricPrimitiveComponent& Phobos::Mesh() {
    return mesh_;
}

void Phobos::Draw(const computer_graphics::Camera* camera) {
    SceneComponent::Draw(camera);

    const auto collision_primitive = CollisionPrimitive();
    const auto& sphere = collision_primitive.Primitive();
    Game().DebugDraw().DrawSphere(sphere);
}

bool Phobos::Intersects(const computer_graphics::Collision& other) const {
    return CollisionPrimitive().Intersects(other);
}

bool Phobos::Intersects(const computer_graphics::math::Ray& ray, float& dist) const {
    return CollisionPrimitive().Intersects(ray, dist);
}

computer_graphics::SphereCollision Phobos::CollisionPrimitive() const {
    auto [position, rotation, scale] = mesh_.get().WorldTransform();

    const computer_graphics::math::Sphere sphere{position, 0.1f};
    return computer_graphics::SphereCollision{sphere};
}
