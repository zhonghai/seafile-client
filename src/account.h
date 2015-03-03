#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QUrl>
#include <QString>
#include <QMetaType>

class Account {
public:
    QUrl serverUrl;
    QString username;
    QString token;
    bool isPro;
    qint64 lastVisited;

    Account() : isPro(false) {}
    Account(QUrl serverUrl, QString username, QString token, qint64 lastVisited=0)
        : serverUrl(serverUrl),
          username(username),
          token(token),
          isPro(false),
          lastVisited(lastVisited) {}

    Account(const Account &rhs)
      : serverUrl(rhs.serverUrl),
        username(rhs.username),
        token(rhs.token),
        isPro(rhs.isPro),
        lastVisited(rhs.lastVisited)
    {
    }

    bool operator==(const Account& rhs) const {
        return serverUrl == rhs.serverUrl
            && username == rhs.username
            && token == rhs.token;
    }

    bool operator!=(const Account& rhs) const {
        return !(*this == rhs);
    }

    bool isValid() const {
        return token.length() > 0;
    }

    QUrl getAbsoluteUrl(const QString& relativeUrl) const;
    QString getSignature() const;
};

Q_DECLARE_METATYPE(Account)

#endif // ACCOUNT_H
