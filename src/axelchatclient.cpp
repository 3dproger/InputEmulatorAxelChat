#include "axelchatclient.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

AxelChatClient::AxelChatClient(const QUrl& address_, std::function<void(const Message& message)> messageCallback_, QObject *parent)
    : QObject{parent}
    , address(address_)
    , messageCallback(messageCallback_)
{
    QObject::connect(&socket, &QWebSocket::stateChanged, this, [this](QAbstractSocket::SocketState state)
    {
        Q_UNUSED(state)
        //qDebug() << Q_FUNC_INFO << "socket state changed:" << state;
        emit stateChanged();
    });

    QObject::connect(&socket, &QWebSocket::connected, this, [this]()
    {
        //qDebug() << Q_FUNC_INFO << "socket connected";
        timerCheckConnection.start();
        emit stateChanged();
    });

    QObject::connect(&socket, &QWebSocket::disconnected, this, [this]()
    {
        //qDebug() << Q_FUNC_INFO << "socket disconnected";
        emit stateChanged();
    });

    QObject::connect(&socket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, [this](QAbstractSocket::SocketError error_)
    {
        qDebug() << Q_FUNC_INFO << "socket error:" << error_ << ":" << socket.errorString();
        emit stateChanged();
    });

    QObject::connect(&socket, &QWebSocket::textMessageReceived, this, &AxelChatClient::onReceived);

    connect(&timerConnect, &QTimer::timeout, this, [this]()
    {
        if (socket.state() == QAbstractSocket::SocketState::ConnectedState)
        {
            return;
        }

        socket.close();
        socket.open(address);
    });
    timerConnect.setInterval(3000);
    timerConnect.start();

    connect(&timerCheckConnection, &QTimer::timeout, this, [this]()
    {
        if (socket.state() == QAbstractSocket::SocketState::UnconnectedState)
        {
            return;
        }

        qWarning() << Q_FUNC_INFO << "socket timeout, close";
        socket.close();
    });
    timerCheckConnection.setInterval(4000);
    timerCheckConnection.setSingleShot(true);
}

bool AxelChatClient::isConnected() const
{
    return socket.state() == QAbstractSocket::SocketState::ConnectedState;
}

void AxelChatClient::onReceived(const QString &text)
{
    timerCheckConnection.start();
    //qDebug("\n" + text.toUtf8() + "\n");

    const QJsonObject root = QJsonDocument::fromJson(text.toUtf8()).object();
    const QJsonObject data = root.value("data").toObject();
    const QString type = root.value("type").toString();

    if (type == "hello")
    {
        //TODO
    }
    else if (type == "state")
    {
        //TODO
    }
    else if (type == "messages")
    {
        const QJsonArray jsonMessages = data.value("messages").toArray();
        for (const QJsonValue& v : qAsConst(jsonMessages))
        {
            const QJsonObject jsonMessage = v.toObject();
            const QJsonObject jsonAuthor = jsonMessage.value("author").toObject();

            Message message;

            message.author.name = jsonAuthor.value("name").toString();
            message.author.serviceId = jsonAuthor.value("serviceId").toString();

            const QJsonArray jsonContents = jsonMessage.value("contents").toArray();
            for (const QJsonValue& v : qAsConst(jsonContents))
            {
                const QJsonObject jsonContent = v.toObject();
                const QJsonObject data = jsonContent.value("data").toObject();
                const QString type = jsonContent.value("type").toString();
                if (type == "text")
                {
                    message.text += data.value("text").toString();
                }
                else
                {
                    qWarning() << Q_FUNC_INFO << "unknown content type" << type;
                }
            }

            if (!message.text.isEmpty())
            {
                if (messageCallback)
                {
                    messageCallback(message);
                }
            }
        }
    }
    else
    {
        qWarning() << Q_FUNC_INFO << "unknown message type" << type;
    }
}
