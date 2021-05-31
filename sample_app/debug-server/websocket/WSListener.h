//
// Created by Lewis weaver on 5/30/2021.
//
#pragma once

#ifndef SAMPLE_APP_WSLISTENER_H
#define SAMPLE_APP_WSLISTENER_H

#include "oatpp-websocket/ConnectionHandler.hpp"
#include "oatpp-websocket/WebSocket.hpp"

#include "../MessageInterface.h"

namespace qdb {
/**
 * WebSocket listener listens on incoming WebSocket events.
 */
class RemoteConnection : public oatpp::websocket::WebSocket::Listener {
 public:
  RemoteConnection(std::shared_ptr<MessageCommandInterface> commandInterface) : commandInterface_(commandInterface) {}

  /**
   * Called on "ping" frame.
   */
  void onPing(const WebSocket& socket, const oatpp::String& message) override;

  /**
   * Called on "pong" frame
   */
  void onPong(const WebSocket& socket, const oatpp::String& message) override;

  /**
   * Called on "close" frame
   */
  void onClose(const WebSocket& socket, v_uint16 code, const oatpp::String& message) override;

  /**
   * Called on each message frame. After the last message will be called once-again with size == 0 to designate end of the message.
   */
  void readMessage(const WebSocket& socket, v_uint8 opcode, p_char8 data, oatpp::v_io_size size) override;

 private:

  void handleCommandMessage(const WebSocket& socket, const oatpp::String& message);

  static constexpr const char* TAG = "Server_WSListener";

  /**
   * Buffer for messages. Needed for multi-frame messages.
   */
  oatpp::data::stream::ChunkedBuffer messageBuffer_;
  std::shared_ptr<MessageCommandInterface> commandInterface_;
};

/**
 * Listener on new WebSocket connections.
 */
class WSInstanceListener : public oatpp::websocket::ConnectionHandler::SocketInstanceListener {
 private:
  static constexpr const char* TAG = "Server_WSInstanceListener";

 public:
   explicit WSInstanceListener(std::shared_ptr<MessageCommandInterface> commandInterface)
      : commandInterface_(commandInterface) {}
  /**
   * Counter for connected clients.
   */
  static std::atomic<v_int32> SOCKETS;

 public:
  /**
   *  Called when socket is created
   */
  void onAfterCreate(const oatpp::websocket::WebSocket& socket, const std::shared_ptr<const ParameterMap>& params) override;

  /**
   *  Called before socket instance is destroyed.
   */
  void onBeforeDestroy(const oatpp::websocket::WebSocket& socket) override;

 private:
  std::shared_ptr<MessageCommandInterface> commandInterface_;
};
}// namespace qdb
#endif// SAMPLE_APP_WSLISTENER_HWSLISTENER_H
