/********************************************************************
 * Copyright (C) 2015 Liangliang Nan <liangliang.nan@gmail.com>
 * https://3d.bk.tudelft.nl/liangliang/
 *
 * This file is part of Easy3D. If it is useful in your research/work,
 * I would be grateful if you show your appreciation by citing it:
 * ------------------------------------------------------------------
 *      Liangliang Nan.
 *      Easy3D: a lightweight, easy-to-use, and efficient C++ library
 *      for processing and rendering 3D data.
 *      Journal of Open Source Software, 6(64), 3255, 2021.
 * ------------------------------------------------------------------
 *
 * Easy3D is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 3
 * as published by the Free Software Foundation.
 *
 * Easy3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 ********************************************************************/

#include "viewer.h"
#include <easy3d/fileio/resources.h>
#include <easy3d/util/logging.h>


// This example shows how to
//        - render a model from the view point given the camera's intrinsic and
//        extrinsic parameters. The camera parameters were recovered using
//        commonly used computer vision techniques (i.e., calibration, or SfM).

using namespace easy3d;

int main(int argc, char **argv) {
    // Initialize logging.
    logging::initialize(true);

    // the bundler file (We use only the camera intrinsic and extrinsic parameters).
    const std::string bundler_file = "/Users/lnan/Documents/Projects/Impossible/data";
    // the point cloud file.
    const std::string cloud_file = "/Users/lnan/Documents/Projects/Impossible/data/PointCloud/pointcloud_COLMAP-cleaned.ply";

    RealCamera viewer("Tutorial_205_RealCamera", bundler_file, cloud_file);

    viewer.usage_func_ = []() -> std::string {
        return ("------------ Real Camera usage ---------- \n"
                "Press 'Space' to switch views\n"
                "Press 'H' to show/hide the cameras\n"
                "----------------------------------------- \n");
    };

    // Run the viewer
    return viewer.run(false); // the camera will be initialized to the first view when the viewer is initialized
}
