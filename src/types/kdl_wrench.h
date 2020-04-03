#ifndef KDL_WRENCH_H
#define KDL_WRENCH_H

struct kdl_wrench {
	struct kdl_vector force;
	struct kdl_vector torque;
};

#endif
