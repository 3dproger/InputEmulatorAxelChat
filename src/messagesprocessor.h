#ifndef MESSAGESPROCESSOR_H
#define MESSAGESPROCESSOR_H

#include "axelchatclient.h"
#include "inputemulator.h"
#include <QObject>

class MessagesProcessor : public QObject
{
    Q_OBJECT
public:
    explicit MessagesProcessor(QObject *parent = nullptr);
    void process(const AxelChatClient::Message& message);

signals:
private:
    static QStringList splitCommands(const QString& text);
    static QString prepareMessageText(const QString& text);
    static void test();
    static void test(const QString& input, const QString& expected);

    void processCommand(const QString& command);

    void emulateKeyHold(const InputEmulatorTypes::Key key, int durationMs);

    InputEmulator input;
};

#endif // MESSAGESPROCESSOR_H
