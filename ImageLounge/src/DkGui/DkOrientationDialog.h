/*******************************************************************************************************
 DkOrientationDialog.cpp
 Created on:	20.01.2024

 nomacs is a fast and small image viewer with the capability of synchronizing multiple instances

 Copyright (C) 2011-2015 Markus Diem <markus@nomacs.org>
 Copyright (C) 2011-2015 Stefan Fiel <stefan@nomacs.org>
 Copyright (C) 2011-2015 Florian Kleber <florian@nomacs.org>

 This file is part of nomacs.

 nomacs is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 nomacs is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 *******************************************************************************************************/

#pragma once

#pragma warning(push, 0) // no warnings from includes - begin
#include <QDialog>
#pragma warning(pop) // no warnings from includes - end

// Qt defines
class QLabel;
class QComboBox;

namespace nmc
{

class DkOrientationDialog : public QDialog
{
    Q_OBJECT

public:
    DkOrientationDialog(QWidget *parent = 0, Qt::WindowFlags flags = Qt::WindowFlags());
    ~DkOrientationDialog(){};

    void setImage(const QImage &img, int orientation);
    QImage getProcessedImage();
    int getOrientation() const;

public slots:
    void on_orientationBox_currentIndexChanged(int idx);
    void updatePreview();
    virtual void accept() override;

protected:
    void createLayout();
    QImage processImage(QImage img);
    void getOrientationAngle(int &angle, int &flip, int orientation);
    void resizeEvent(QResizeEvent *re);

    QLabel *mPreviewLabel = 0;
    QComboBox *mOrientationBox = 0;
    QImage mImg;
};

}
