#include "point_cloud_boundary.h"

#include <easy3d/core/principal_axes.h>
#include <easy3d/kdtree/kdtree_search_eth.h>
#include <easy3d/util/logging.h>

#include <algorithm>
#include <functional>


namespace easy3d {


    std::vector<PointCloud::Vertex>
    PointCloudBoundary::apply(PointCloud *cloud, unsigned int k /* = 30 */, float eigen_ratio /* = 2.5f */) {
        std::vector<PointCloud::Vertex> results;

        if (cloud == nullptr || cloud->vertices_size() == 0) {
            LOG(WARNING) << "empty point cloud";
            return results;
        }

        LOG(INFO) << "building kd-tree ...";

        auto kdtree = new KdTreeSearch_ETH;
        kdtree->begin();
        kdtree->add_point_cloud(cloud);
        kdtree->end();

        std::vector<vec3> &points = cloud->points();
        for (auto v : cloud->vertices()) {
            const vec3 &p = cloud->position(v);

            std::vector<int> neighbors;
            kdtree->find_closest_k_points(p, k, neighbors);
            if (neighbors.size() < k) {
                LOG_N_TIMES(5, WARNING) << "could not collect enough neighbors (" << k << " requested but " << neighbors.size() << " found)";
                continue;
            }

            PrincipalAxes<3, float> axes;

            axes.begin();
            for (unsigned int j = 0; j < neighbors.size(); ++j) {
                int idx = neighbors[j];
                axes.add_point(points[idx]);
            }
            axes.end();

            std::vector<double> eigen_values;
            for (int j = 0; j < 3; ++j)
                eigen_values.push_back(axes.eigen_value(j));

            // test if eigen values are sorted in descending order (PrincipalAxes3 does so).
            // if not, sort them descending order.
            bool sorted = (eigen_values[0] >= eigen_values[1]) && (eigen_values[1] >= eigen_values[2]);
            if (!sorted)
                std::sort(eigen_values.begin(), eigen_values.end(), std::greater<double>());

            // for boundaries
            if (eigen_values[1] / eigen_values[0] < 1.0 / eigen_ratio)
                results.push_back(v);
        }

        return results;
    }

}