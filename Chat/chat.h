#pragma once

#include <QWidget>

#include <QLabel>
#include <QTextEdit>
#include <QLineEdit >
#include <QPushButton>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

#include <QUdpSocket>

#include <QByteArray>
#include <QDataStream>
#include <QDateTime>

#include <QCloseEvent>

class Chat : public QWidget {
    Q_OBJECT
public:
    Chat(QWidget* parent = nullptr, quint16 port = 2424);
    ~Chat();

    void start();
    void setConnect();
    void closeEvent (QCloseEvent *event);
private:
    QLabel* plbl;
    QTextEdit* ptxt;
    QLineEdit* pline;
    QPushButton* pbut;

    QGroupBox* pgb;
    QHBoxLayout* phl;
    QVBoxLayout* pvl;

    QString name;
    quint16 port;

    QUdpSocket* pudpsMessage;
private slots:
    void slotName();
    void slotSend();
    void slotRead();
};
