#ifndef _PORT_GROUP_H
#define _PORT_GROUP_H

#include "port.h"
#include <QHostAddress>
#include <QTcpSocket>

#include "../common/protocol.pb.h"
#include "pbrpcchannel.h"

/* TODO
HIGH
MED
LOW
- Allow hostnames in addition to IP Address as "server address"
*/

#define DEFAULT_SERVER_PORT		7878

class PortGroup : public QObject {
	Q_OBJECT

private:
	quint32			mPortGroupId;
	static quint32	mPortGroupAllocId;
	QString			mUserAlias;			// user defined
#if 0 // PB
	QTcpSocket		*mpSocket;
	QHostAddress	mServerAddress;
	quint16			mServerPort;
#endif
	PbRpcChannel						*rpcChannel;
	::google::protobuf::RpcController	*rpcController;
	OstProto::OstService::Stub			*serviceStub;
public: // FIXME(HIGH): member access
	QList<Port>		mPorts;

public:
	PortGroup(QHostAddress ip = QHostAddress::LocalHost, 
		quint16 port = DEFAULT_SERVER_PORT); 
	~PortGroup();

	void connectToHost() { rpcChannel->establish(); }
	void connectToHost(QHostAddress ip, quint16 port) 
		{ rpcChannel->establish(ip, port); }
	void disconnectFromHost() { rpcChannel->tearDown(); }

	int numPorts() const { return mPorts.size(); }
	quint32 id() const { return mPortGroupId; } 

	const QString& userAlias() const { return mUserAlias; } 
	void setUserAlias(QString alias) { mUserAlias = alias; };

	const QHostAddress& serverAddress() const 
		{ return rpcChannel->serverAddress(); } 
	quint16 serverPort() const 
		{ return rpcChannel->serverPort(); } 
	QAbstractSocket::SocketState state() const
		{ return rpcChannel->state(); }	

	void processPortIdList(OstProto::PortIdList *portIdList);
	void processPortConfigList(OstProto::PortConfigList *portConfigList);

signals:
	void portGroupDataChanged(PortGroup* portGroup);
	void portListAboutToBeChanged(quint32 portGroupId);
	void portListChanged(quint32 portGroupId);

private slots:
	void on_rpcChannel_stateChanged();
	void on_rpcChannel_connected();
	void on_rpcChannel_disconnected();
	void on_rpcChannel_error(QAbstractSocket::SocketError socketError);
#if 0 // PB
	void on_rpcChannel_when_dataAvail();
#endif

private:
#if 0 // PB
	void ProcessCapabilityInfo(const char *msg, qint32 size);
	void ProcessMsg(const char *msg, quint32 size);
#endif
};

#endif
