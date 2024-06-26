#pragma once

#ifndef COLLISION_INL_INCLUDED
#define COLLISION_INL_INCLUDED

#pragma push_macro("min")
#undef min
#pragma push_macro("max")
#undef max

#include <range/v3/view/chunk.hpp>
#include <range/v3/view/common.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

#pragma pop_macro("min")
#pragma pop_macro("max")

namespace computer_graphics {

inline TrianglesRange auto MeshCollision::Triangles() const {
    using namespace ranges;

    auto chunks =
        indices_ | views::chunk(3) | views::filter([](auto chunk) { return size(chunk) == 3; });
    auto triangles = chunks | views::transform([=](auto indices) {
        return math::Triangle{
            .point0 = vertices_[indices[0]],
            .point1 = vertices_[indices[1]],
            .point2 = vertices_[indices[2]],
        };
    });
    return triangles;
}

}  // namespace computer_graphics

#endif  // COLLISION_INL_INCLUDED
