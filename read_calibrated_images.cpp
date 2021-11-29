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

#include <easy3d/util/string.h>
#include <easy3d/util/file_system.h>


using namespace easy3d;

bool RealCamera::read_calibrated_images(const std::string& dir) {
    const std::string cam_dir = dir + "/cams";
    std::vector<std::string> cam_files;
    file_system::get_directory_entries(cam_dir, cam_files, false);
    std::sort(cam_files.begin(), cam_files.end());

    if (!views_.empty())
        views_.clear();

    for (const auto& entry : cam_files) {
        const std::string file_name = cam_dir + "/" + entry;
        std::ifstream input(file_name.c_str());
        if (input.fail()) {
            std::cerr << "could not open file \'" << file_name << "\'" << std::endl;
            continue;
        }

        std::string dummy;
        CameraPara cam;
        input >> dummy >> cam.Rt >> dummy >> cam.K;
        cam.image_file = entry;
        string::replace(cam.image_file, "_cam.txt", ".jpg");
        cam.image_file = dir + "/images/" + cam.image_file;
        views_.push_back(cam);
    }

	return views_.size() > 0;
}
