#ifndef KDL_FRAME_H
#define KDL_FRAME_H

struct kdl_frame {
	struct kdl_vector p;
	struct kdl_rotation M;
};

#endif
