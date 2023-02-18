#ifndef AXELCHATCLIENT_H
#define AXELCHATCLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QTimer>

class AxelChatClient : public QObject
{
    Q_OBJECT
public:
    struct Author
    {
        QString name;
        QString id;
        QString serviceId;
    };

    struct Message
    {
        Author author;
        QString text;
    };

    explicit AxelChatClient(const QUrl& address, std::function<void(const Message& message)> messageCallback, QObject *parent = nullptr);
    bool isConnected() const;

signals:
    void stateChanged();

private slots:
    void onReceived(const QString& text);

private:
    const QUrl address;

    QWebSocket socket;
    QTimer timerConnect;
    QTimer timerCheckConnection;

    std::function<void(const Message& message)> messageCallback = nullptr;
};

#endif // AXELCHATCLIENT_H
