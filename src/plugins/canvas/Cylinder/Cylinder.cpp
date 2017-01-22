/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Cylinder.h"
#include "CylinderWidget.h"

#include <omni/ui/CanvasParameters.h>

namespace omni
{
  namespace canvas
  {
    Cylinder::Cylinder()
    {
    }

    Cylinder::~Cylinder()
    {
    }

    void Cylinder::draw() const {
      glPushMatrix();
      withCurrentContext([this](QOpenGLFunctions& _)
      {
        // Scale offset
        float r = diameter_ * 0.5;
        glScalef(r, r, height_);
        vbo_.bindAndDraw();
      });
      glPopMatrix();
    }

    float Cylinder::diameter() const {
      return diameter_;
    }

    void Cylinder::setDiameter(float _diameter) {
      diameter_ = _diameter;
    }

    float Cylinder::height() const {
      return height_;
    }

    void Cylinder::setHeight(float _height) {
      height_ = _height;
    }

    float Cylinder::beginAngle() const {
      return beginAngle_;
    }

    void Cylinder::setBeginAngle(float _beginAngle) {
      beginAngle_ = _beginAngle;
      update();
    }

    float Cylinder::endAngle() const {
      return endAngle_;
    }

    void Cylinder::setEndAngle(float _endAngle) {
      endAngle_ = _endAngle;
      update();
    }

    bool Cylinder::topCap() const {
      return topCap_;
    }

    void Cylinder::setTopCap(bool _topCap) {
      topCap_ = _topCap;
      update();
    }

    bool Cylinder::bottomCap() const {
      return bottomCap_;
    }

    void Cylinder::setBottomCap(bool _bottomCap) {
      bottomCap_ = _bottomCap;
      update();
    }

    void Cylinder::update() {
      if (beginAngle_ > endAngle_) {
        std::swap(beginAngle_,endAngle_);
      }

      primaryContextSwitch([&](QOpenGLFunctions& _) {
        vertices_.clear();
        indices_.clear();
        vertices_.reserve(2 * segments_);
        indices_.reserve(6 * segments_);

        auto getPos = [&](size_t i) -> QVector2D {
          float tx = float(i) / segments_;
          float _m   = (tx * (endAngle_ - beginAngle_) + beginAngle_) / 360.0;
          _m *= 2.0 * M_PI;
          return QVector2D(cos(_m),sin(_m));
        };

        size_t startIndex = 0;
        auto addTriangle = [&](size_t a, size_t b, size_t c) {
          indices_.push_back(startIndex + a);
          indices_.push_back(startIndex + b);
          indices_.push_back(startIndex + c);
        };

        startIndex = vertices_.size();
        for (size_t i = 0; i <= segments_; ++i)
        {
          float tx = float(i) / segments_;
          auto pos = getPos(i);
          float x = pos.x(), y = pos.y();

          QVector3D n(x,y,0.0);
          vertices_.emplace_back(QVector3D(x,y,1.0),n,QVector2D(tx,1.0));
          vertices_.emplace_back(n,n,QVector2D(tx,0.0));
        }

        for (size_t i = 0; i <= segments_; ++i)
        {
            size_t idx0 = i;
            size_t idx1 = (i + 1);
            /// Top triangle
            addTriangle(2 * idx0, 2 * idx0 + 1, 2*idx1);

            /// Bottom triangle
            addTriangle(2 * idx0 + 1, 2* idx1 + 1, 2 * idx1);
        }

        vbo_.buffer(vertices_, indices_);
        vertices_.clear();
        indices_.clear();
      });
    }

    ui::CanvasParameters* Cylinder::widget() {
        return ui::makeWidget<ui::canvas::Cylinder>(this);
    }
  }
}