#include "messagesprocessor.h"

MessagesProcessor::MessagesProcessor(QObject *parent)
    : QObject{parent}
{
    test();
}

void MessagesProcessor::process(const AxelChatClient::Message &message)
{
    const QStringList commands = splitCommands(message.text);
    for (const QString& command : qAsConst(commands))
    {
        processCommand(command);
    }
}

QStringList MessagesProcessor::splitCommands(const QString &text_)
{
    QString text = text_.trimmed();
    if (!text.startsWith('!'))
    {
        return QStringList();
    }

    text = text.mid(1);

    return text.split(' ', Qt::SkipEmptyParts);
}

QString MessagesProcessor::prepareMessageText(const QString &text_)
{
    const QString text = text_.trimmed();
    QString result;

    if (text.startsWith("!"))
    {
        result += "!";

        for (int i = 1; i < text.length(); i++)
        {
            if (text.at(i) != ' ')
            {
                result += text.midRef(i);
                break;
            }
        }
    }
    else
    {
        result = text;
    }

    return result;
}

void MessagesProcessor::test()
{
    test("!R", "!R");
    test("!r", "!r");
    test(" !r ", "!r");
    test("!   ", "!");
    test("   ", "");
    test("!   r", "!r");
}

void MessagesProcessor::test(const QString &input, const QString &expected)
{
    const QString got = prepareMessageText(input);
    if (got != expected)
    {
        qCritical() << Q_FUNC_INFO << "test failed, input =" << input << ", expected =" << expected << ", got =" << got;
    }
}

void MessagesProcessor::processCommand(const QString &command_)
{
    const QString c = command_.trimmed().toLower();
    /*
    // Sega Mega drive emuator
    if (c == "r")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_D, 500);
    }
    else if (c == "l")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_A, 500);
    }
    else if (c == "u")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_W, 500);
    }
    else if (c == "d")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_S, 500);
    }
    else if (c == "start")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_Space, 50);
    }
    else if (c == "mode")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_P, 50);
    }
    else if (c == "a")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_B, 100);
    }
    else if (c == "b")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_N, 100);
    }
    else if (c == "c")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_M, 100);
    }
    else if (c == "x")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_G, 100);
    }
    else if (c == "y")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_H, 100);
    }
    else if (c == "z")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_J, 100);
    }
    */

    // MK4 PC
    if (c == "l")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_A, 500);
    }
    else if (c == "r")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_S, 500);
    }
    else if (c == "u")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_W, 500);
    }
    else if (c == "d")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_Z, 500);
    }
    else if (c == "x") // cross
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_G, 100);
    }
    else if (c == "s") // square
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_T, 100);
    }
    else if (c == "o") // round
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_J, 100);
    }
    else if (c == "t") // triangle
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_U, 100);
    }
    else if (c == "l1")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_H, 100);
    }
    else if (c == "l2")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_B, 100);
    }
    else if (c == "r1")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_Space, 100);
    }
    else if (c == "r2")
    {
        emulateKeyHold(InputEmulatorTypes::Key::Key_M, 100);
    }
}

void MessagesProcessor::emulateKeyHold(const InputEmulatorTypes::Key key, int durationMs)
{
    input.emulateKeyboard(key, true);
    QTimer::singleShot(durationMs, this, [this, key]()
    {
        input.emulateKeyboard(key, false);
    });
}
