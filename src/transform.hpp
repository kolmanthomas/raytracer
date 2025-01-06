#pragma once

#include <Eigen/Dense>

namespace transform {

inline Eigen::Matrix4f lookat(const Eigen::Vector3f& from, const Eigen::Vector3f& to)
{
    Eigen::Vector3f forward = (from - to).normalized();
    Eigen::Vector3f right = Eigen::Vector3f::UnitY().cross(forward).normalized(); // Might not work if forward is parallel to up
    Eigen::Vector3f up = forward.cross(right);

    Eigen::Matrix4f result = Eigen::Matrix4f::Identity();
    result(0, 0) = right.x();
    result(1, 0) = right.y();
    result(2, 0) = right.z();
    result(0, 1) = up.x();
    result(1, 1) = up.y();
    result(2, 1) = up.z();
    result(0, 2) = forward.x();
    result(1, 2) = forward.y();
    result(2, 2) = forward.z();
    result(3, 0) = from.x();
    result(3, 1) = from.y();
    result(3, 2) = from.z();
    return result; 
}

inline Eigen::Matrix4f world_to_camera(const Eigen::Vector3f& from, const Eigen::Vector3f& to, const Eigen::Vector3f& up)
{
    Eigen::Vector3f f = (from - to).normalized();
    Eigen::Vector3f s = f.cross(up).normalized(); // Might not work if forward is parallel to up
    Eigen::Vector3f t = s.cross(f);

    Eigen::Matrix4f result = Eigen::Matrix4f::Identity();
    result(0, 0) = s.x();
    result(1, 0) = t.x();
    result(2, 0) = -f.x();
    result(0, 1) = s.y();
    result(1, 1) = t.y();
    result(2, 1) = -f.y();
    result(0, 2) = s.z();
    result(1, 2) = t.z();
    result(2, 2) = -f.z();

    result(0, 3) = -from.x();
    result(1, 3) = -from.y();
    result(2, 3) = -from.z();
    return result; 

}


} // namespace transform
