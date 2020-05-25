#ifndef UBXKDL_HPP
#define UBXKDL_HPP

#include <ubx/ubx.h>
#include <kdl/frames.hpp>

def_class_accessors(KDLVector, KDL::Vector, struct kdl_vector);
def_class_accessors(KDLRotation, KDL::Rotation, struct kdl_rotation);
def_class_accessors(KDLFrame, KDL::Frame, struct kdl_frame);
def_class_accessors(KDLTwist, KDL::Twist, struct kdl_twist);
def_class_accessors(KDLWrench, KDL::Wrench, struct kdl_wrench);

#endif
