#include "chat.h"

Chat::Chat(QWidget* parent, quint16 port) : QWidget(parent), port(port) {
    setWindowTitle("Chat");

    plbl = new QLabel("Chat", this);
    ptxt = new QTextEdit(this);
    pline = new QLineEdit(this);
    pbut = new QPushButton("->", this);

    pudpsMessage = new QUdpSocket(this);

    pgb = new QGroupBox;
    phl = new QHBoxLayout;
    pvl = new QVBoxLayout(this);

    phl->addWidget(pline);
    phl->addWidget(pbut);

    pgb->setLayout(phl);

    pvl->addWidget(plbl);
    pvl->addWidget(ptxt);
    pvl->addWidget(pgb);

    plbl->setAlignment(Qt::AlignCenter);
    ptxt->setReadOnly(true);

    setGeometry(150, 150, 350, 300);
    start();
}

Chat::~Chat() {
    delete plbl;
    delete ptxt;
    delete pline;
    delete pbut;

    delete pgb;
    delete phl;
    delete pvl;

    pudpsMessage->close();
    delete pudpsMessage;
}

void Chat::start() {
    ptxt->append("Введите свой никнейм: ");

    connect(pbut, SIGNAL(clicked()), this, SLOT(slotName()));
    connect(pline, SIGNAL(returnPressed()), this, SLOT(slotName()));
}

void Chat::setConnect() {
    if (pudpsMessage->bind(QHostAddress::Any, port, QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress)) {
        disconnect(pbut, SIGNAL(clicked()), this, SLOT(slotName()));
        disconnect(pline, SIGNAL(returnPressed()), this, SLOT(slotName()));

        connect(pbut, SIGNAL(clicked()), this, SLOT(slotSend()));
        connect(pline, SIGNAL(returnPressed()), this, SLOT(slotSend()));
        connect(pudpsMessage, SIGNAL(readyRead()), this, SLOT(slotRead()));

        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_3);

        out << QDateTime::currentDateTime().toString().split(' ')[3] + ": " + name + " присоединился к чату.";
        pudpsMessage->writeDatagram(data, QHostAddress::Broadcast, port);
    }
    else {
        ptxt->setText("Ошибка: Порт для общения занят сторонней программой, запрещяющей прослушивать его сторонним приложениям!");
    }
}

void Chat::slotName() {
    name = pline->text();
    ptxt->insertPlainText(name);
    pline->clear();

    setConnect();
}

void Chat::slotSend() {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    QString message = name + " (" + QDateTime::currentDateTime().toString().split(' ')[3] + "): " + pline->text();

    out << message;
    pudpsMessage->writeDatagram(data, QHostAddress::Broadcast, port);

    pline->clear();
}

void Chat::slotRead() {
    QByteArray data;

    do {
        data.resize(pudpsMessage->pendingDatagramSize());
        pudpsMessage->readDatagram(data.data(), data.size());
    } while(pudpsMessage->hasPendingDatagrams());

    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    QString message;
    in >> message;

    ptxt->append(message);
}

void Chat::closeEvent (QCloseEvent *event) {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    out << QDateTime::currentDateTime().toString().split(' ')[3] + ": " + name + " вышел из чата.";
    pudpsMessage->writeDatagram(data, QHostAddress::Broadcast, port);
}
