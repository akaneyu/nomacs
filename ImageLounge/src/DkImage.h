/*******************************************************************************************************
 DkImage.h
 Created on:	21.04.2011
 
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

#pragma once

#include <QWidget>
#include <QImageWriter>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QFile>
#include <QSettings>
#include <QImageReader>
#include <QDir>
#include <QThread>
#include <QBuffer>
#include <QStringBuilder>
#include <QDebug>
#include <QMutex>
#include <QFileIconProvider>
#include <QStringList>
#include <QMessageBox>
#include <QDirIterator>
#include <QProgressDialog>
#include <QReadLocker>
#include <QWriteLocker>
#include <QReadWriteLock>
#include <QTimer>
#include <QMovie>
#include <QByteArray>
#include <QCoreApplication>
#include <QApplication>

// opencv
#ifdef WITH_OPENCV

#ifdef WIN32
#pragma warning(disable: 4996)
#endif

#include <libraw/libraw.h>
#ifdef DISABLE_LANCZOS // opencv 2.1.0 is used, does not have opencv2 includes
	#include "opencv/cv.h"
#else
	#include "opencv2/core/core.hpp"
	#include "opencv2/imgproc/imgproc.hpp"
#endif

using namespace cv;
#endif


#include <set>

#ifndef DllExport
#ifdef DK_DLL_EXPORT
#define DllExport Q_DECL_EXPORT
#elif DK_DLL_IMPORT
#define DllExport Q_DECL_IMPORT
#else
#define DllExport
#endif
#endif

// TODO: ifdef
//#include <ShObjIdl.h>
//#include <ShlObj.h>
//#include <Windows.h>

// my classes
//#include "DkNoMacs.h"
#include "DkImageStorage.h"
#include "DkBasicLoader.h"
#include "DkTimer.h"
#include "DkSettings.h"
#include "DkThumbs.h"
#include "DkMetaData.h"
#include "DkBasicLoader.h"
#include "DkImageContainer.h"

#ifdef Q_WS_X11
	typedef  unsigned char byte;
#endif

#ifdef WITH_LIBTIFF
	#ifdef WIN32
		#include "tif_config.h"	
	#endif

	#ifdef Q_WS_MAC
		#define uint64 uint64_hack_
		#define int64 int64_hack_
	#endif // Q_WS_MAC

	#include "tiffio.h"

	#ifdef Q_WS_MAC
		#undef uint64
		#undef int64
	#endif // Q_WS_MAC
#endif


namespace nmc {

/**
 * This class is a basic image loader class.
 * It takes care of the file watches for the current folder,
 * holds the currently displayed image,
 * calls the load routines
 * and saves the image or the image metadata.
 **/ 
class DllExport DkImageLoader : public QObject {

	Q_OBJECT

public:

	enum cacheProps {
		cache_force_load = 0,
		cache_default = 1,
		cache_disable_update,
		cache_props_end
	};

	DkImageLoader(QFileInfo file = QFileInfo());

	virtual ~DkImageLoader();

	//static QString saveFilter;		// for system close dialog
	//static QString openFilter;		// for system  open dialog
	static QStringList fileFilters;	// just the filters
	static QStringList openFilters;	// for open dialog
	static QStringList saveFilters;	// for close dialog

	QStringList ignoreKeywords;
	QStringList keywords;
	QStringList folderKeywords;		// are deleted if a new folder is opened

	static bool isValid(const QFileInfo& fileInfo);
	//static int locateFile(QFileInfo& fileInfo, QDir* dir = 0);
	static QStringList getFoldersRecursive(QDir dir);
	QFileInfoList updateSubFolders(QDir rootDir);

	// deprecated
	static QStringList getFilteredFileList(QDir dir, QStringList ignoreKeywords = QStringList(), QStringList keywords = QStringList(), QStringList folderKeywords = QStringList());
	QFileInfoList getFilteredFileInfoList(const QDir& dir, QStringList ignoreKeywords = QStringList(), QStringList keywords = QStringList(), QStringList folderKeywords = QStringList());

	//static DkMetaData imgMetaData;	// static class so that the metadata is only loaded once (performance)

	void rotateImage(double angle);
	void saveFile(QFileInfo filename, QImage saveImg = QImage(), QString fileFilter = "", int compression = -1);
	void setFile(QFileInfo& filename);
	QSharedPointer<DkImageContainerT> getCurrentImage() const;
	QFileInfo file() const;
	QStringList getFileNames();
	QVector<QSharedPointer<DkImageContainerT> > getImages();
	void nextFile(bool silent = false);
	void previousFile(bool silent = false);
	void firstFile();
	void lastFile();
	void loadFileAt(int idx);
	void clearPath();
	void clearFileWatcher();
	QString getCurrentFilter();
	QDir getDir() const;
	QDir getSaveDir();
	void loadLastDir();
	void setDir(QDir& dir);
	void setSaveDir(QDir& dir);
	QSharedPointer<DkImageContainerT> setImage(QImage img, QFileInfo editFile = QFileInfo());
	//QImage loadThumb(QFileInfo& file, bool silent = false);
	bool hasFile();
	bool hasMovie();
	//bool isCached(QFileInfo& file);
	//void updateCacheIndex();
	QString fileName();
	//QFileInfo getChangedFileInfo(int skipIdx, bool silent = false, bool searchFile = true);		//deprecated
	QSharedPointer<DkImageContainerT> getSkippedImage(int skipIdx, bool silent = false, bool searchFile = true);
	//static QStringList sort(const QStringList& files, const QDir& dir);	// deprecated
	void sort();
	QVector<QSharedPointer<DkImageContainerT> > sort(const QVector<QSharedPointer<DkImageContainerT> >& images, const QDir& dir);
	QSharedPointer<DkImageContainerT> findOrCreateFile(const QFileInfo& file) const;
	QSharedPointer<DkImageContainerT> findFile(const QFileInfo& file) const;
	int findFileIdx(const QFileInfo& file, const QVector<QSharedPointer<DkImageContainerT> >& images) const;
	void setCurrentImage(QSharedPointer<DkImageContainerT> newImg);
	bool loadDir(QFileInfo newFile, bool scanRecursive = true);
	bool loadDir(QDir newDir, bool scanRecursive = true);
	
	static void initFileFilters();	// add special file filters

	bool hasImage();
	bool isEdited() const;
	int numFiles() const;
	QImage getImage();
	bool dirtyTiff();

	static bool restoreFile(const QFileInfo &fileInfo);

signals:
	void folderFiltersChanged(QStringList filters);
	void updateImageSignal();
	void updateInfoSignalDelayed(QString msg, bool start = false, int timeDelayed = 700);
	void updateSpinnerSignalDelayed(bool start = false, int timeDelayed = 700);
	void updateFileSignal(QFileInfo file, QSize s = QSize(), bool edited = false, QString attr = QString());
	//void updateDirSignal(QFileInfo file, int force = DkThumbsLoader::not_forced);
	void newErrorDialog(QString msg, QString title = "Error");
	void fileNotLoadedSignal(QFileInfo file);
	void setPlayer(bool play);
	void updateFileWatcherSignal(QFileInfo file);

	// new signals
	void imageUpdatedSignal(QSharedPointer<DkImageContainerT> image);
	void imageLoadedSignal(QSharedPointer<DkImageContainerT> image, bool loaded = true);
	void showInfoSignal(QString msg, int time = 3000, int position = 0);
	void errorDialogSignal(const QString&);
	void updateDirSignal(QVector<QSharedPointer<DkImageContainerT> > images);

public slots:
	//QImage changeFileFast(int skipIdx, QFileInfo& fileInfo, bool silent = false);
	void changeFile(int skipIdx, bool silent = false, int force = cache_default);
	//void fileChanged(const QString& path);
	void directoryChanged(const QString& path = QString());
	//void saveFileSilentIntern(QFileInfo file, QImage saveImg = QImage());
	void saveFileIntern(QFileInfo filename, QImage saveImg = QImage(), QString fileFilter = "", int compression = -1);
	//void load(QFileInfo file, bool silent = false, int cacheState = cache_default);	// deprecated
	void load(QSharedPointer<DkImageContainerT> image = QSharedPointer<DkImageContainerT>(), bool silent = false);
	void load(const QFileInfo& file, bool silent = false);
	//virtual bool loadFile(QFileInfo file, bool silent = false, int cacheState = cache_default);
	//void saveRating(int rating);
	void deleteFile();
	QFileInfo saveTempFile(QImage img, QString name = "img", QString fileExt = ".png", bool force = false, bool threaded = true);
	void setFolderFilters(QStringList filters);
	QStringList getFolderFilters();
	void updateFileWatcher(QFileInfo filePath);
	void disableFileWatcher();

	// new slots
	void imageLoaded(bool loaded = false);
	void imageSaved(QFileInfo file, bool saved = true);
	void unloadFile();
	void reloadImage();

protected:

	//DkBasicLoader basicLoader;
	//DkCacher* cacher;

	QTimer delayedUpdateTimer;
	bool timerBlockedUpdate;
	
	//QFileInfo editFile;
	//QFileInfo lastFileLoaded;
	QDir dir;
	QDir saveDir;
	QFileSystemWatcher *watcher;
	QFileSystemWatcher *dirWatcher;
	QStringList subFolders;
	//QStringList files;
	QVector<QSharedPointer<DkImageContainerT > > images;
	QSharedPointer<DkImageContainerT > currentImage;
	QSharedPointer<DkImageContainerT > lastImageLoaded;
	bool folderUpdated;
	//int cFileIdx; // deprecated?
	int tmpFileIdx; // deprecated?

	// functions
	void updateCacher(QSharedPointer<DkImageContainerT> imgC);
	int getNextFolderIdx(int folderIdx);
	int getPrevFolderIdx(int folderIdx);
	//void saveFileSilentThreaded(QFileInfo file, QImage img = QImage());
	void updateHistory();
	void sendFileSignal();
	QString getTitleAttributeString();
	void createImages(const QFileInfoList& files);
};

// deprecated
class DkColorLoader : public QThread {
	Q_OBJECT

public:
	DkColorLoader(QVector<QSharedPointer<DkImageContainerT> > images);
	~DkColorLoader() {};

	void stop();
	void run();

	const QVector<QColor>& getColors() const {
		return cols;
	};

	const QVector<int>& getIndexes() const {
		return indexes;
	};

	int maxFiles() const {
		return maxThumbs;
	};

	QString getFilename(int idx) const {

		if (idx < 0 || idx >= images.size())
			return QString("");

		return images.at(idx)->file().fileName();
	}

signals:
	void updateSignal(const QVector<QColor>& cols, const QVector<int>& indexes);

protected:
	void init();
	void loadThumbs();
	void loadColor(int fileIdx);
	QColor computeColor(QImage& thumb);

	QVector<QSharedPointer<DkImageContainerT> > images;
	QVector<QColor> cols;
	QVector<int> indexes;
	bool isActive;
	bool paused;
	QMutex mutex;
	int maxThumbs;
};

};
