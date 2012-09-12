/****************************************************************************
**   ncw is the nodecast worker, client of the nodecast server
**   Copyright (C) 2010-2011  Frédéric Logier <frederic@logier.org>
**
**   https://github.com/nodecast/ncw
**
**   This program is free software: you can redistribute it and/or modify
**   it under the terms of the GNU Affero General Public License as
**   published by the Free Software Foundation, either version 3 of the
**   License, or (at your option) any later version.
**
**   This program is distributed in the hope that it will be useful,
**   but WITHOUT ANY WARRANTY; without even the implied warranty of
**   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**   GNU Affero General Public License for more details.
**
**   You should have received a copy of the GNU Affero General Public License
**   along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

#ifndef ZEROMQ_H
#define ZEROMQ_H

#include <QFile>
#include <QObject>
#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QCoreApplication>
#include <QSocketNotifier>

#include <boost/cstdint.hpp>
#include <boost/asio.hpp>
#include <zmq.hpp>

#include "mongo/client/dbclient.h"
#include "mongo/bson/bson.h"

#include "service.h"
#include "process.h"
#include "ncw_global.h"

using namespace mongo;
using namespace bson;


enum WorkerType {
    WSERVICE=1,
    WPROCESS=2
};
typedef QMap<QString, WorkerType> StringToEnumMap;

class Zstream : public QObject
{
    Q_OBJECT
public:
    Zstream(zmq::context_t *a_context, QString a_host);
    ~Zstream();

private:
    QSocketNotifier *check_stream;
    zmq::context_t *m_context;
    zmq::socket_t *z_receive;
    zmq::message_t *z_message;
    QMutex *m_mutex;
    QFile *data_stream;

    QString m_host;
    QString m_uuid;

private slots:
    void get_stream(bson::bo payload);
    void stream_payload();
};


class Ztracker : public QObject
{
    Q_OBJECT
public:
    Ztracker(zmq::context_t *a_context, QString a_host, QString a_port);
    ~Ztracker();
    QString m_worker_port;

private:
    zmq::context_t *m_context;
    zmq::socket_t *z_sender;
    zmq::message_t *z_message;
    QMutex *m_mutex;

    QString m_host;
    QString m_port;
    QString m_uuid;

signals:
    void worker_port(QString worker_port, QString worker_uuid);

public slots:
    void init();
    void push_tracker(bson::bo tracker);
};


class Zpayload : public QObject
{
    Q_OBJECT
public:
    Zpayload(zmq::context_t *a_context, ncw_params ncw);
    ~Zpayload();


private:
    QSocketNotifier *check_receive_payload;
    QSocketNotifier *check_pubsub_payload;
    zmq::socket_t *m_socket_worker;
    zmq::socket_t *m_socket_pubsub;
    zmq::message_t *m_message;
    zmq::message_t *m_pubsub_message;

    zmq::context_t *m_context;
    zmq::socket_t *m_receiver;

    QString m_host;
    QString m_port;
    QString m_worker_name;
    QString m_uuid;
    QString m_node_uuid;
    QString m_node_password;
    ncw_params m_ncw;

signals:    
    void payload(bson::bo data);
    void emit_pubsub(string data);
    void emit_launch_worker(ncw_params);

public slots:    
    void init_payload(QString worker_port, QString worker_uuid);
    void receive_payload();
    void push_payload(bson::bo data);
    void pubsub_payload();
};


class Zeromq : public QObject
{
    Q_OBJECT
public:
    Zeromq(ncw_params ncw);
    ~Zeromq();

    Ztracker *tracker;
    Zpayload *payload;
    Zstream *stream;

    // Unix signal handlers.
    static void hupSignalHandler(int unused);
    static void termSignalHandler(int unused);


public slots:
    // Qt signal handlers.
    void handleSigHup();
    void handleSigTerm();

private:
    Process *ncw_process;
    Service *ncw_service;

    QMutex *m_port_mutex;
    zmq::context_t *m_context;
    ncw_params m_ncw;

    static int sighupFd[2];
    static int sigtermFd[2];

    QSocketNotifier *snHup;
    QSocketNotifier *snTerm;
};

#endif // ZEROMQ_H
