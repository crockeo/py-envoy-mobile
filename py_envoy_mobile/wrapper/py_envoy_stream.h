#pragma once

#include <memory>
#include <optional>

#include "py_envoy_data.h"
#include "py_envoy_engine.h"
#include "py_envoy_headers.h"


class Stream;

using OnHeadersCallback = std::function<void (Engine&, Stream&, std::unique_ptr<Headers>, bool)>;
using OnHeadersLikeCallback = std::function<void (Engine&, Stream&, std::unique_ptr<Headers>)>;
using OnDataCallback = std::function<void (Engine&, Stream&, std::unique_ptr<Data>, bool)>;
using OnErrorCallback = std::function<void (Engine&, Stream&, int error_code)>;
using OnCompleteCallback = std::function<void (Engine&, Stream&)>;

struct StreamCallbacks {
  StreamCallbacks(std::shared_ptr<Stream> stream);

  StreamCallbacks& set_on_headers(OnHeadersCallback on_headers);
  StreamCallbacks& set_on_data(OnDataCallback on_data);
  StreamCallbacks& set_on_metadata(OnHeadersLikeCallback on_metadata);
  StreamCallbacks& set_on_trailers(OnHeadersLikeCallback on_trailers);
  StreamCallbacks& set_on_error(OnErrorCallback on_error);
  StreamCallbacks& set_on_complete(OnCompleteCallback on_complete);
  StreamCallbacks& set_on_cancel(OnCompleteCallback on_cancel);

  std::optional<OnHeadersCallback> on_headers;
  std::optional<OnDataCallback> on_data;
  std::optional<OnHeadersLikeCallback> on_metadata;
  std::optional<OnHeadersLikeCallback> on_trailers;
  std::optional<OnErrorCallback> on_error;
  std::optional<OnCompleteCallback> on_complete;
  std::optional<OnCompleteCallback> on_cancel;

  std::shared_ptr<Stream> stream;
  envoy_http_callbacks callbacks;
};

class Stream {
public:
  Stream(std::shared_ptr<Engine> engine);
  ~Stream();

  // we want to disallow copying the stream, as it manages the envoy_stream_t internally
  Stream(const Stream&&) = delete;
  Stream& operator=(const Stream&&) = delete;

  void start(const StreamCallbacks& callbacks);
  void send_headers(const Headers& headers, bool end_stream);
  void send_data(const Data& data, bool end_stream);
  void send_metadata(const Headers& metadata);
  void send_trailers(const Headers& trailers);
  void reset();
  void close();

  std::shared_ptr<Engine> parent() const;

private:
  std::shared_ptr<Engine> parent_;
  envoy_stream_t stream_;
};
