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
#include <easy3d/util/logging.h>

/// This example shows how to visualize a model using the calibrated cameras (so the rendering will be offered at
/// the same view as in the images).
/// The camera parameters were recovered using standard computer vision techniques like calibration or SfM.


using namespace easy3d;

int main(int argc, char **argv) {
    // Initialize logging.
    logging::initialize(true);

    const std::string data_dir = DATA_DIR;
    const std::string cloud_file = data_dir + "/pointcloud/pointcloud.ply";

    RealCameraViewer viewer("RealCameraViewer", data_dir, cloud_file);

    viewer.usage_string_ = "------------ Real Camera usage ---------- \n"
                           "'<' or '>': switch views\n"
                           "'H':        show/hide the cameras\n"
                           "----------------------------------------- \n";

    // Run the viewer
    return viewer.run(false);   // the camera will be initialized to the first view when the viewer is initialized
}
