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

#ifndef OMNI_UI_ABSTRACTINPUTWIDGET_H_
#define OMNI_UI_ABSTRACTINPUTWIDGET_H_

#include <QLayout>
#include <omni/ui/mixin/Editor.h>
#include "Widget.h"

namespace omni {
  namespace ui {
    /// Input widget with an editor widget
    class OMNIWIDGET_EXPORT AbstractInputWidget :
      public Widget,
      protected mixin::Editor<AbstractInputWidget, QWidget>{
      Q_OBJECT

      public:
        typedef mixin::Editor<AbstractInputWidget, QWidget>mixin_editor_type;
        AbstractInputWidget(QWidget * = nullptr);
        virtual ~AbstractInputWidget() {}

      public slots:
        /// Set editor visible
        virtual void setEditorVisible(bool);

        /// Show editor
        void         showEditor();

        /// Hide editor
        void         hideEditor();

      signals:
        /// Value changed event
        void         valueChanged();

      protected:
        /// Show editor on double click
        virtual void mouseDoubleClickEvent(QMouseEvent *);

        /// Hide editor and set focus on click
        virtual void mousePressEvent(QMouseEvent *);

        /// Show editor when enter key is pressed
        virtual void keyPressEvent(QKeyEvent *);

        /// Draw slider handle
        void         drawHandle(QPainter& _p,
                                double _pos,
                                double _heightFactor = 1.0) const;

        void drawTrack(QPainter& _p,
                       double _pos) const;

        void drawTrack(QPainter& _p,
                       double _left,
                       double _right) const;

        void init();

        QLabel *valueLabel_ = nullptr;
    };
  }
}

#endif /* OMNI_UI_ABSTRACTINPUTWIDGET_H_ */
