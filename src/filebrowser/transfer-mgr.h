#ifndef SEAFILE_CLIENT_FILE_BROWSER_TRANSFER_MANAGER_H
#define SEAFILE_CLIENT_FILE_BROWSER_TRANSFER_MANAGER_H

#include <QObject>
#include <QQueue>

#include "api/api-error.h"
#include "seaf-dirent.h"
#include "utils/singleton.h"


template<typename Key> class QQueue;

class QThread;

class Account;
class SeafileApiRequest;
class GetDirentsRequest;
class DirentsCache;
class FileCacheDB;
class FileUploadTask;
class FileDownloadTask;
class FileNetworkTask;

/**
 * TransferManager manages all upload/download tasks.
 *
 */
class TransferManager : public QObject {
    SINGLETON_DEFINE(TransferManager)
    Q_OBJECT
public:
    TransferManager();
    ~TransferManager();

    FileDownloadTask *addDownloadTask(const Account& account,
                                      const QString& repo_id,
                                      const QString& path,
                                      const QString& local_path);

    QString getDownloadProgress(const QString& repo_id,
                                const QString& path);

private slots:
    void onDownloadTaskFinished(bool success);

private:
    void startDownloadTask(FileDownloadTask *task);

    FileDownloadTask *current_download_;
    QQueue<FileDownloadTask *> pending_downloads_;
};


#endif // SEAFILE_CLIENT_FILE_BROWSER_TRANSFER_MANAGER_H
