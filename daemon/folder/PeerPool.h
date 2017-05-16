/* Copyright (C) 2016 Alexander Shishenko <alex@shishenko.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */
#pragma once
#include "adapters/DiscoveryAdapter.h"
#include "control/FolderParams.h"
#include "util/BandwidthCounter.h"
#include <QObject>
#include <QSet>
#include <QHostAddress>

namespace librevault {

class Peer;
class FolderGroup;
class NodeKey;

class PeerPool : public QObject {
	Q_OBJECT

signals:
	void newValidPeer(Peer* peer);

public:
	PeerPool(const FolderParams& params, DiscoveryAdapter* discovery, NodeKey* node_key, QObject* parent);
	virtual ~PeerPool();

	void handleDiscovered(QPair<QHostAddress, quint16> endpoint);
	void handleIncoming(Peer* peer);

	/* Getters */
	QList<Peer*> peers() const {return remotes_.toList();}
	QList<Peer*> validPeers() const {return remotes_ready_.toList();}

	inline const FolderParams& params() const {return params_;}

private:
	FolderParams params_;
	NodeKey* node_key_;

	DiscoveryGroup* dgroup_;

	BandwidthCounter bandwidth_counter_;

	/* Members */
	QSet<Peer*> remotes_;
	QSet<Peer*> remotes_ready_;

	// Member lookup optimization
	QSet<QByteArray> digests_;
	QSet<QPair<QHostAddress, quint16>> endpoints_;

	bool contains(Peer* peer) const;

private slots:
	void handleHandshake(Peer* peer);
	void handleDisconnected(Peer* peer);
};

} /* namespace librevault */
