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

#ifndef EASY3D_TUTORIAL_REAL_CAMERA_H
#define EASY3D_TUTORIAL_REAL_CAMERA_H

#include <easy3d/viewer/viewer.h>

// This RealCamera class visualizes a model from the view point
// given the camera's intrinsic and extrinsic parameters. These
// camera parameters can (usually) be recovered using camera
// calibration or structure from motion techniques.

namespace easy3d {
    class Camera;
    class Texture;
    class LinesDrawable;
    class PointsDrawable;
}

class RealCamera : public easy3d::Viewer
{
public:
    RealCamera(const std::string& title,
               const std::string& image_data_dir,
               const std::string& cloud_file);
    
protected:
    void init() override;
    bool key_press_event(int key, int modifiers) override;
    
private:
    struct CameraPara {
        easy3d::mat4  Rt;
        easy3d::mat3  K;
        std::string image_file;
    };
    std::vector<CameraPara> views_;
    int current_view_;
    
    bool read_calibrated_images(const std::string& dir);
    
    // K [R T] -> easy3d camera representation
    bool KRT_to_camera(int view_index, easy3d::Camera* c);
    void load_image(int view_index);
    void switch_to_view(int view_index);

    void update_cameras_drawable();

    void post_draw() override;

private:
    easy3d::Texture* texture_;
    easy3d::LinesDrawable* cameras_drwable_;

    // point cloud boundaries
    easy3d::PointsDrawable* cloud_boundaries_;
    easy3d::LinesDrawable* cloud_lines_;
};


#endif // EASY3D_TUTORIAL_REAL_CAMERA_H
