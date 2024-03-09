/*******************************************************************************************************
 DkLayout.cpp
 Created on:	09.03.2024

 nomacs is a fast and small image viewer with the capability of synchronizing multiple instances

 Copyright (C) 2011-2013 Markus Diem <markus@nomacs.org>
 Copyright (C) 2011-2013 Stefan Fiel <stefan@nomacs.org>
 Copyright (C) 2011-2013 Florian Kleber <florian@nomacs.org>

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

#include "DkLayout.h"

#pragma warning(push, 0) // no warnings from includes - begin
#include <QWidget>
#pragma warning(pop) // no warnings from includes - end

namespace nmc
{

// DkFlowLayout --------------------------------------------------------------------
DkFlowLayout::DkFlowLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), mHSpace(hSpacing), mVSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

DkFlowLayout::DkFlowLayout(int margin, int hSpacing, int vSpacing)
    : mHSpace(hSpacing), mVSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

DkFlowLayout::~DkFlowLayout()
{
    QLayoutItem *item;
    while ((item = takeAt(0)) != nullptr) {
        delete item;
    }
}

void DkFlowLayout::addItem(QLayoutItem *item)
{
    mItemList.append(item);
}

int DkFlowLayout::horizontalSpacing() const
{
    if (mHSpace >= 0) {
        return mHSpace;
    } else {
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

int DkFlowLayout::verticalSpacing() const
{
    if (mVSpace >= 0) {
        return mVSpace;
    } else {
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}

int DkFlowLayout::count() const
{
    return mItemList.size();
}

QLayoutItem *DkFlowLayout::itemAt(int index) const
{
    return mItemList.value(index);
}

QLayoutItem *DkFlowLayout::takeAt(int index)
{
    if (index >= 0 && index < mItemList.size()) {
        return mItemList.takeAt(index);
    }

    return nullptr;
}

Qt::Orientations DkFlowLayout::expandingDirections() const
{
    return { };
}

bool DkFlowLayout::hasHeightForWidth() const
{
    return true;
}

int DkFlowLayout::heightForWidth(int width) const
{
    return doLayout(QRect(0, 0, width, 0), true);
}

void DkFlowLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize DkFlowLayout::sizeHint() const
{
    return minimumSize();
}

QSize DkFlowLayout::minimumSize() const
{
    QSize size;
    for (const QLayoutItem *item : qAsConst(mItemList)) {
        size = size.expandedTo(item->minimumSize());
    }

    const QMargins margins = contentsMargins();
    size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());

    return size;
}

int DkFlowLayout::doLayout(const QRect &rect, bool testOnly) const
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(left, top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;

    for (QLayoutItem *item : qAsConst(mItemList)) {
        const QWidget *wid = item->widget();
        int spaceX = horizontalSpacing();
        if (spaceX == -1) {
            spaceX = wid->style()->layoutSpacing(
                QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
        }

        int spaceY = verticalSpacing();
        if (spaceY == -1) {
            spaceY = wid->style()->layoutSpacing(
                QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
        }

        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        if (!testOnly) {
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
        }

        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }

    return y + lineHeight - rect.y() + bottom;
}

int DkFlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject *parent = this->parent();
    if (!parent) {
        return -1;
    } else if (parent->isWidgetType()) {
        QWidget *pw = static_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(pm, nullptr, pw);
    } else {
        return static_cast<QLayout *>(parent)->spacing();
    }
}

}
