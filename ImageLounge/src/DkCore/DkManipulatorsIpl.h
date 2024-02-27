/*******************************************************************************************************
 nomacs is a fast and small image viewer with the capability of synchronizing multiple instances

 Copyright (C) 2011-2016 Markus Diem <markus@nomacs.org>
 Copyright (C) 2011-2016 Stefan Fiel <stefan@nomacs.org>
 Copyright (C) 2011-2016 Florian Kleber <florian@nomacs.org>

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

 related links:
 [1] https://nomacs.org/
 [2] https://github.com/nomacs/
 [3] http://download.nomacs.org
 *******************************************************************************************************/

#pragma once

#include "DkManipulators.h"

#pragma warning(push, 0) // no warnings from includes
#include <QAction>
#pragma warning(pop)

#pragma warning(disable : 4251) // TODO: remove

#ifndef DllCoreExport
#ifdef DK_CORE_DLL_EXPORT
#define DllCoreExport Q_DECL_EXPORT
#elif DK_DLL_IMPORT
#define DllCoreExport Q_DECL_IMPORT
#else
#define DllCoreExport Q_DECL_IMPORT
#endif
#endif

// Qt defines

namespace nmc
{

// nomacs defines

class DkGrayScaleManipulator : public DkBaseManipulator
{
public:
    DkGrayScaleManipulator(QAction *action = 0);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;
};

class DkAutoAdjustManipulator : public DkBaseManipulator
{
public:
    DkAutoAdjustManipulator(QAction *action = 0);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;
};

class DkNormalizeManipulator : public DkBaseManipulator
{
public:
    DkNormalizeManipulator(QAction *action = 0);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;
};

class DkInvertManipulator : public DkBaseManipulator
{
public:
    DkInvertManipulator(QAction *action = 0);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;
};

class DkFlipHManipulator : public DkBaseManipulator
{
public:
    DkFlipHManipulator(QAction *action = 0);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;
};

class DkFlipVManipulator : public DkBaseManipulator
{
public:
    DkFlipVManipulator(QAction *action = 0);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;
};

class DkRotateCWManipulator : public DkBaseManipulator
{
public:
    DkRotateCWManipulator(QAction *action = 0);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;
};

class DkRotateCCWManipulator : public DkBaseManipulator
{
public:
    DkRotateCCWManipulator(QAction *action = 0);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;
};

class DkRotate180Manipulator : public DkBaseManipulator
{
public:
    DkRotate180Manipulator(QAction *action = 0);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;
};

// Extended --------------------------------------------------------------------
class DllCoreExport DkTinyPlanetManipulator : public DkBaseManipulatorExt
{
public:
    DkTinyPlanetManipulator(QAction *action);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;

    void applyDefault() override;

    void setSize(int size);
    int size() const;

    void setAngle(int angle);
    int angle() const;

    void setInverted(bool invert);
    bool inverted() const;

private:
    int mSizeDefault = 30;
    int mAngleDefault = 0;
    bool mInvertedDefault = false;

    int mSize = mSizeDefault;
    int mAngle = mAngleDefault;
    bool mInverted = mInvertedDefault;
};

class DllCoreExport DkColorManipulator : public DkBaseManipulatorExt
{
public:
    DkColorManipulator(QAction *action);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;

    void applyDefault() override;

    void setColor(const QColor &col);
    QColor color() const;

private:
    QColor mColorDefault = Qt::white;

    QColor mColor = mColorDefault;
};

class DllCoreExport DkBlurManipulator : public DkBaseManipulatorExt
{
public:
    DkBlurManipulator(QAction *action);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;

    void applyDefault() override;

    void setSigma(int sigma);
    int sigma() const;

private:
    int mSigmaDefault = 5;

    int mSigma = mSigmaDefault;
};

class DllCoreExport DkUnsharpMaskManipulator : public DkBaseManipulatorExt
{
public:
    DkUnsharpMaskManipulator(QAction *action);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;

    void applyDefault() override;

    void setSigma(int sigma);
    int sigma() const;

    void setAmount(int amount);
    int amount() const;

private:
    int mSigmaDefault = 30;
    int mAmountDefault = 5;

    int mSigma = mSigmaDefault;
    int mAmount = mAmountDefault;
};

class DllCoreExport DkRotateManipulator : public DkBaseManipulatorExt
{
public:
    DkRotateManipulator(QAction *action);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;

    void applyDefault() override;

    void setAngle(int angle);
    int angle() const;

private:
    int mAngleDefault = 0;

    int mAngle = mAngleDefault;
};

class DllCoreExport DkResizeManipulator : public DkBaseManipulatorExt
{
public:
    DkResizeManipulator(QAction *action);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;

    void applyDefault() override;

    void setScaleFactor(double sf);
    double scaleFactor() const;

    void setInterpolation(int ipl);
    int interpolation() const;

    void setCorrectGamma(bool ug);
    bool correctGamma() const;

private:
    double mScaleFactorDefault = 1.0;
    int mInterpolationDefault = 1;
    bool mCorrectGammaDefault = false;

    double mScaleFactor = mScaleFactorDefault;
    int mInterpolation = mInterpolationDefault;
    bool mCorrectGamma = mCorrectGammaDefault;
};

class DllCoreExport DkThresholdManipulator : public DkBaseManipulatorExt
{
public:
    DkThresholdManipulator(QAction *action);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;

    void applyDefault() override;

    void setThreshold(int thr);
    int threshold() const;

    void setColor(bool col);
    bool color() const;

private:
    int mThresholdDefault = 128;
    bool mColorDefault = false;

    int mThreshold = mThresholdDefault;
    bool mColor = mColorDefault;
};

class DllCoreExport DkHueManipulator : public DkBaseManipulatorExt
{
public:
    DkHueManipulator(QAction *action);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;

    void applyDefault() override;

    void setHue(int hue);
    int hue() const;

    void setSaturation(int sat);
    int saturation() const;

    void setValue(int val);
    int value() const;

private:
    int mHueDefault = 0;
    int mSatDefault = 0;
    int mValueDefault = 0;

    int mHue = mHueDefault;
    int mSat = mSatDefault;
    int mValue = mValueDefault;
};

class DllCoreExport DkExposureManipulator : public DkBaseManipulatorExt
{
public:
    DkExposureManipulator(QAction *action);

    QImage apply(const QImage &img) const override;
    QString errorMessage() const override;

    void applyDefault() override;

    void setExposure(double exposure);
    double exposure() const;

    void setOffset(double offset);
    double offset() const;

    void setGamma(double gamma);
    double gamma() const;

private:
    double mExposureDefault = 0.0;
    double mOffsetDefault = 0.0;
    double mGammaDefault = 1.0;

    double mExposure = mExposureDefault;
    double mOffset = mOffsetDefault;
    double mGamma = mGammaDefault;
};
}