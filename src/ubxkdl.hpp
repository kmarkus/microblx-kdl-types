#include <ubx/ubx.h>
#include <kdl/frames.hpp>

gen_class_accessors(KDLVector, KDL::Vector, struct kdl_vector);
gen_class_accessors(KDLRotation, KDL::Rotation, struct kdl_rotation);
gen_class_accessors(KDLFrame, KDL::Frame, struct kdl_frame);
gen_class_accessors(KDLTwist, KDL::Twist, struct kdl_twist);
gen_class_accessors(KDLWrench, KDL::Wrench, struct kdl_wrench);
