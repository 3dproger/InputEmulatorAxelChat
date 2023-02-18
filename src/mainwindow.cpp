#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client(QUrl("ws://127.0.0.1:8355"), [this](const AxelChatClient::Message& message) { onMessageReceived(message); })
{
    ui->setupUi(this);

    connect(&client, &AxelChatClient::stateChanged, this, [this]()
    {
        if (client.isConnected())
        {
            ui->labelState->setText(tr("Connected"));
        }
        else
        {
            ui->labelState->setText(tr("Not connected"));
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onMessageReceived(const AxelChatClient::Message &message)
{
    messagesProcessor.process(message);
}

