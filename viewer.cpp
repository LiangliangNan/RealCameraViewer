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

#include <easy3d/core/point_cloud.h>
#include <easy3d/renderer/drawable_points.h>
#include <easy3d/renderer/drawable_lines.h>
#include <easy3d/renderer/camera.h>
#include <easy3d/renderer/manipulated_camera_frame.h>
#include <easy3d/renderer/texture_manager.h>
#include <easy3d/renderer/shapes.h>
#include <easy3d/renderer/renderer.h>
#include <easy3d/util/string.h>
#include <easy3d/util/file_system.h>

#include <3rd_party/glfw/include/GLFW/glfw3.h>	// for the KEYs


using namespace easy3d;


RealCamera::RealCamera(const std::string& title,
                       const std::string& image_data_dir,
                       const std::string& cloud_file)
    : Viewer(title, 4, 3, 2, false, false)
    , current_view_(0)
    , texture_(nullptr)
    , cameras_drwable_(nullptr)
{
    if (add_model(cloud_file)) {
        auto drawable = current_model()->renderer()->get_points_drawable("vertices");
        drawable->set_point_size(5.0f);

        // Read the camera parameters from the bundler file.
        if (read_calibrated_images(image_data_dir))
            update_cameras_drawable();
        else
            std::cerr << "Error: failed load bundler file." << std::endl;

        camera()->setUpVector(vec3(0, 1, 0));
        camera()->setViewDirection(vec3(0, 0, -1));
        fit_screen();
    }
    else
        std::cerr << "Error: failed load point cloud." << std::endl;
}


void RealCamera::init() {
    Viewer::init();

    if (views_.size() > 0)
        switch_to_view(0);
}


bool RealCamera::key_press_event(int key, int modifiers) {
    if (key == GLFW_KEY_COMMA && modifiers == 0) {
        if (!views_.empty()) {
            current_view_ = (current_view_ - 1 + views_.size()) % views_.size();
            switch_to_view(current_view_);
        }
        return true;
    } else if (key == GLFW_KEY_PERIOD && modifiers == 0) {
        if (!views_.empty()) {
            current_view_ = (current_view_ + 1) % views_.size();
            switch_to_view(current_view_);
        }
        return true;
    }
    else if (key == GLFW_KEY_H) {
        if (cameras_drwable_) {
            cameras_drwable_->set_visible(!cameras_drwable_->is_visible());
            update();
        }
        return true;
    }
    else
        return Viewer::key_press_event(key, modifiers);
}


void RealCamera::load_image(int view_index) {
    if (view_index < 0 || view_index >= views_.size()) {
        LOG(ERROR) << "invalid view index: " << view_index << "(available views: " << views_.size() << ")";
        return;
    }

    const std::string image_file = views_[view_index].image_file;
    if (file_system::is_file(image_file)) {
        texture_ = TextureManager::request(image_file);
        update();
    }
    else
        LOG(ERROR) << "image not found: " << image_file << std::endl;
}


bool RealCamera::KRT_to_camera(int view_index, Camera* c) {
    if (view_index < 0 || view_index >= views_.size()) {
        std::cerr << "Error: invalid view index (" << view_index << ")" << std::endl;
        return false;
    }

    const CameraPara& cam = views_[view_index];
    mat34 M(1.0f);
    mat34 proj = cam.K * M * cam.Rt;
    c->set_from_calibration(proj);
	return true;
}


void RealCamera::switch_to_view(int view_index) {
    if (KRT_to_camera(current_view_, camera())) {
        load_image(view_index);

        std::cout << "----- view: " << current_view_ << std::endl;
        set_title("RealCamera: View_" + std::to_string(current_view_));
        const CameraPara &c = views_[current_view_];

        int w = static_cast<int>(c.K(0, 2) * 2.0f);
        int h = static_cast<int>(c.K(1, 2) * 2.0f);

        // make sure the aspect ratio (actual size does not matter)
        resize(w * 0.4, h * 0.4);
    }
}


void RealCamera::update_cameras_drawable()
{
    if (!cameras_drwable_) {
        cameras_drwable_ = new LinesDrawable("cameras");
        add_drawable(cameras_drwable_); // add the camera drawables to the viewer
        cameras_drwable_->set_uniform_coloring(vec4(0, 0, 1, 1.0f));
        cameras_drwable_->set_line_width(2.0f);
        cameras_drwable_->set_visible(false); // default is hidden
    }

    std::vector<vec3> vertices;
    for (std::size_t i = 0; i < views_.size(); ++i) {
        Camera c;
        KRT_to_camera(i, &c);
        int w = static_cast<int>(views_[i].K(0, 2) * 2.0f);
        int h = static_cast<int>(views_[i].K(1, 2) * 2.0f);
        std::vector<vec3> points;
        shapes::create_camera(points, c.sceneRadius() * 0.1f, c.fieldOfView(), static_cast<float>(h)/w);
        const mat4& m = c.frame()->worldMatrix();
        for (auto& p : points)
            vertices.push_back(m * p);
    }
    cameras_drwable_->update_vertex_buffer(vertices);
}


void RealCamera::post_draw() {
    Viewer::post_draw();

    if (texture_ == nullptr)
        return;

    int w = width() * dpi_scaling();
    int h = height() * dpi_scaling();

    int tex_w = texture_->width();
    int tex_h = texture_->height();
    float image_as = tex_w / static_cast<float>(tex_h);
    float viewer_as = width() / static_cast<float>(height());
    if (image_as < viewer_as) {// thin
        tex_h = static_cast<int>(height() * 0.3f);
        tex_w = static_cast<int>(tex_h * image_as);
    }
    else {
        tex_w = static_cast<int>(width() * 0.3f);
        tex_h = static_cast<int>(tex_w / image_as);
    }

    const Rect quad(20 * dpi_scaling(), (20 + tex_w) * dpi_scaling(), 40 * dpi_scaling(), (40 + tex_h) * dpi_scaling());
    shapes::draw_quad_filled(quad, texture_->id(), w, h, -0.9f);
    shapes::draw_quad_wire(quad, vec4(1.0f, 0.0f, 0.0f, 1.0f), w, h, -0.99f);
}