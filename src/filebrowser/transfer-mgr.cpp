#include <QDir>
#include <sqlite3.h>
#include <errno.h>
#include <stdio.h>
#include <QDateTime>

#include "utils/file-utils.h"
#include "utils/utils.h"
#include "configurator.h"
#include "account-mgr.h"
#include "seafile-applet.h"
#include "file-browser-requests.h"
#include "tasks.h"
#include "auto-update-mgr.h"
#include "data-cache.h"
#include "data-mgr.h"

#include "transfer-mgr.h"

namespace {

} // namespace

SINGLETON_IMPL(TransferManager)

TransferManager::TransferManager()
{
    current_download_ = NULL;
}

TransferManager::~TransferManager()
{
}

FileDownloadTask * TransferManager::addDownloadTask(const Account& account,
                                                    const QString& repo_id,
                                                    const QString& path,
                                                    const QString& local_path)
{
    FileDownloadTask *task = new FileDownloadTask(account,
                                                  repo_id, path, local_path);
    connect(task, SIGNAL(finished(bool)),
            this, SLOT(onDownloadTaskFinished(bool)));
    if (current_download_) {
        pending_downloads_.enqueue(task);
    } else {
        startDownloadTask(task);
    }
    return task;
}

void TransferManager::onDownloadTaskFinished(bool success)
{
    if (!pending_downloads_.empty()) {
        FileDownloadTask *task = pending_downloads_.dequeue();
        startDownloadTask(task);
    } else {
        current_download_ = NULL;
    }
}

void TransferManager::startDownloadTask(FileDownloadTask *task)
{
    current_download_ = task;
    task->start();
}

QString TransferManager::getDownloadProgress(const QString& repo_id,
                                             const QString& path)
{
    if (current_download_ &&
        current_download_->repoId() == repo_id &&
        current_download_->path() == path) {
        FileNetworkTask::Progress progress = current_download_->progress();
        return progress.toString();
    }
    foreach (FileDownloadTask *task, pending_downloads_) {
        if (task->repoId() == repo_id && task->path() == path) {
            return tr("pending");
        }
    }
    return "";
}
