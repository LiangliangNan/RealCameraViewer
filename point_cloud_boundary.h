

#ifndef _ALGORITHM_POINT_SET_NORMALS_H_
#define _ALGORITHM_POINT_SET_NORMALS_H_

#include <easy3d/core/point_cloud.h>

#include <vector>

namespace easy3d {

    class PointCloud;

    class PointCloudBoundary {
    public:
        // returns the indices of the boundary points
        static std::vector<PointCloud::Vertex> apply(PointCloud *cloud, unsigned int k = 30, float eigen_ratio = 2.5f);
    };

}

#endif