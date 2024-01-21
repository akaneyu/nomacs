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

#include "DkOrientationDialog.h"

#include "DkImageStorage.h"

#pragma warning(push, 0) // no warnings from includes - begin
#include <QComboBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#pragma warning(pop) // no warnings from includes - end

namespace nmc
{

// DkOrientationDialog --------------------------------------------------------------------
DkOrientationDialog::DkOrientationDialog(QWidget *parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
{
    setWindowTitle(tr("Set EXIF Orientation"));
    createLayout();

    QMetaObject::connectSlotsByName(this);
}

void DkOrientationDialog::setImage(const QImage &img, int orientation)
{
    int angle = 0;
    int flip = 0;
    getOrientationAngle(angle, flip, orientation);

    mImg = img;
    mImg = DkImage::rotateImage(mImg, -angle);
    if (flip == -1) {
        mImg = mImg.mirrored(true, false);
    }

    auto orientationIndex = orientation - 1;
    if (orientationIndex < 0 || orientationIndex >= mOrientationBox->count()) {
        orientationIndex = 0;
    }

    mOrientationBox->setCurrentIndex(orientationIndex);

    updatePreview();
}

QImage DkOrientationDialog::getProcessedImage()
{
    return processImage(mImg);
}

int DkOrientationDialog::getOrientation() const
{
    return mOrientationBox->currentIndex() + 1;
}

void DkOrientationDialog::on_orientationBox_currentIndexChanged(int)
{
    updatePreview();
}

void DkOrientationDialog::updatePreview()
{
    if (mImg.isNull())
        return;

    QImage img = processImage(mImg);

    img = img.scaled(mPreviewLabel->size(), Qt::KeepAspectRatio, Qt::FastTransformation);
    mPreviewLabel->setPixmap(QPixmap::fromImage(img));
}

void DkOrientationDialog::accept()
{
    QDialog::accept();
}

void DkOrientationDialog::createLayout()
{
    // preview
    mPreviewLabel = new QLabel(this);
    mPreviewLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mPreviewLabel->setMinimumHeight(100);

    QWidget *mainBoxes = new QWidget(this);
    QGridLayout *gridLayout = new QGridLayout(mainBoxes);

    // orientation
    mOrientationBox = new QComboBox();
    QStringList orientationList;
    orientationList.append(tr("top, left"));
    orientationList.append(tr("top, right"));
    orientationList.append(tr("bottom, right"));
    orientationList.append(tr("bottom, left"));
    orientationList.append(tr("left, top"));
    orientationList.append(tr("right, top"));
    orientationList.append(tr("right, bottom"));
    orientationList.append(tr("left, bottom"));
    mOrientationBox->addItems(orientationList);
    mOrientationBox->setObjectName("orientationBox");
    mOrientationBox->setCurrentIndex(0);

    gridLayout->addWidget(mOrientationBox, 0, 0);

    // notice
    QLabel *noticeText = new QLabel(tr(
            "NOTE: The EXIF orientation will be LOST if you apply additional adjustments."));
    noticeText->setWordWrap(true);

    // bottom buttons
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    buttons->button(QDialogButtonBox::Ok)->setText(tr("&OK"));
    buttons->button(QDialogButtonBox::Cancel)->setText(tr("&Cancel"));
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    QGridLayout *layout = new QGridLayout(this);
    layout->setColumnStretch(0, 1);
    layout->addWidget(mPreviewLabel, 0, 0);
    layout->addWidget(mainBoxes, 1, 0);
    layout->addWidget(noticeText, 2, 0);
    layout->addWidget(buttons, 3, 0);

    adjustSize();
    resize(400, 500);
}

QImage DkOrientationDialog::processImage(QImage img)
{
    auto orientation = mOrientationBox->currentIndex() + 1;

    int angle = 0;
    int flip = 0;
    getOrientationAngle(angle, flip, orientation);

    QImage newImg = img;
    if (flip == -1) {
        newImg = newImg.mirrored(true, false);
    }
    newImg = DkImage::rotateImage(newImg, angle);

    return newImg;
}

void DkOrientationDialog::resizeEvent(QResizeEvent *re)
{
    updatePreview();
    QDialog::resizeEvent(re);
}

void DkOrientationDialog::getOrientationAngle(int &angle, int &flip, int orientation)
{
    switch (orientation) {
    case 1:
        angle = 0;
        flip = 1;
        break;
    case 2:
        angle = 0;
        flip = -1;
        break;
    case 3:
        angle = 180;
        flip = 1;
        break;
    case 4:
        angle = 180;
        flip = -1;
        break;
    case 5:
        angle = 90;
        flip = -1;
        break;
    case 6:
        angle = 90;
        flip = 1;
        break;
    case 7:
        angle = -90;
        flip = -1;
        break;
    case 8:
        angle = -90;
        flip = 1;
        break;
    default:
        angle = 0;
        flip = 1;
        break;
    }
}

}
