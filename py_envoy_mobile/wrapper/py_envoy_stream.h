#pragma once

#include <memory>
#include <optional>

#include "py_envoy_data.h"
#include "py_envoy_engine.h"
#include "py_envoy_headers.h"


struct Stream;


using OnHeadersCallback = std::function<void (std::shared_ptr<Engine>, std::shared_ptr<Stream>, std::unique_ptr<Headers>)>;
using OnDataCallback = std::function<void (std::shared_ptr<Engine>, std::shared_ptr<Stream>, std::unique_ptr<Data>)>;
using OnErrorCallback = std::function<void (std::shared_ptr<Engine>, std::shared_ptr<Stream>, int error_code)>;
using OnCompleteCallback = std::function<void (std::shared_ptr<Engine>, std::shared_ptr<Stream>)>;


struct StreamCallbacks {
  StreamCallbacks(std::shared_ptr<Engine> engine, std::shared_ptr<Stream> stream);

  StreamCallbacks& set_on_headers(OnHeadersCallback on_headers);
  StreamCallbacks& set_on_data(OnDataCallback on_data);
  StreamCallbacks& set_on_metadata(OnHeadersCallback on_metadata);
  StreamCallbacks& set_on_trailers(OnHeadersCallback on_trailers);
  StreamCallbacks& set_on_error(OnErrorCallback on_error);
  StreamCallbacks& set_on_complete(OnCompleteCallback on_complete);
  StreamCallbacks& set_on_cancel(OnCompleteCallback on_cancel);

  std::shared_ptr<Engine> engine;
  std::shared_ptr<Stream> stream;

  std::optional<OnHeadersCallback> on_headers;
  std::optional<OnDataCallback> on_data;
  std::optional<OnHeadersCallback> on_metadata;
  std::optional<OnHeadersCallback> on_trailers;
  std::optional<OnErrorCallback> on_error;
  std::optional<OnCompleteCallback> on_complete;
  std::optional<OnCompleteCallback> on_cancel;
};


struct Stream {
  Stream(std::shared_ptr<Engine> engine);
  ~Stream();

  // we want to disallow copying the stream, as it manages the envoy_stream_t internally
  Stream(const Stream&&) = delete;
  Stream& operator=(const Stream&&) = delete;

  void start(std::unique_ptr<StreamCallbacks> callbacks);
  void send_headers(std::unique_ptr<Headers> headers, bool end_stream);
  void send_data(std::unique_ptr<Data> data, bool end_stream);
  void send_metadata(std::unique_ptr<Headers> metadata);
  void send_trailers(std::unique_ptr<Headers> trailers);
  void reset();
  void close();

  std::shared_ptr<Engine> engine;
  envoy_stream_t stream;
};
