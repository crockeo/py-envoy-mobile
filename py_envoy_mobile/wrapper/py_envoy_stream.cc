#include "py_envoy_stream.h"

#include <exception>
#include <iostream>

#include "library/common/main_interface.h"


// TODO: I'm sure that this whole section could be cleaned up by C++ templates, but I haven't done
// templating in far too long.
static void *py_dispatch_on_headers(envoy_headers headers, bool end_stream, void *context) {
  if (context == nullptr) {
    return context;
  }

  StreamCallbacks *callbacks = static_cast<StreamCallbacks *>(context);
  Headers py_headers(headers);
  if (callbacks->on_headers.has_value()) {
    callbacks->stream->parent()->put_thunk([=](Engine& engine) {
      callbacks->on_headers.value()(*callbacks->stream->parent(), *callbacks->stream, py_headers, end_stream);
    });
  }
  return context;
}

static void *py_dispatch_on_data(envoy_data data, bool end_stream, void *context) {
  if (context == nullptr) {
    return context;
  }

  StreamCallbacks *callbacks = static_cast<StreamCallbacks *>(context);
  Data py_data(data);
  if (callbacks->on_data.has_value()) {
    callbacks->stream->parent()->put_thunk([=](Engine& engine) {
      callbacks->on_data.value()(*callbacks->stream->parent(), *callbacks->stream, py_data, end_stream);
    });
  }
  return context;
}

static void *py_dispatch_on_metadata(envoy_headers headers, void *context) {
  if (context == nullptr) {
    return context;
  }

  StreamCallbacks *callbacks = static_cast<StreamCallbacks *>(context);
  Headers py_headers(headers);
  if (callbacks->on_metadata.has_value()) {
    callbacks->stream->parent()->put_thunk([=](Engine& engine) {
      callbacks->on_metadata.value()(*callbacks->stream->parent(), *callbacks->stream, py_headers);
    });
  }
  return context;
}

static void *py_dispatch_on_trailers(envoy_headers headers, void *context) {
  if (context == nullptr) {
    return context;
  }

  StreamCallbacks *callbacks = static_cast<StreamCallbacks *>(context);
  Headers py_headers(headers);
  if (callbacks->on_trailers.has_value()) {
    callbacks->stream->parent()->put_thunk([=](Engine& engine) {
      callbacks->on_trailers.value()(*callbacks->stream->parent(), *callbacks->stream, py_headers);
    });
  }
  return context;
}

static void *py_dispatch_on_error(envoy_error error, void *context) {
  if (context == nullptr) {
    return context;
  }

  StreamCallbacks *callbacks = static_cast<StreamCallbacks *>(context);
  int error_code = error.error_code;
  Data error_message = Data(error.message);
  int attempt_count = error.attempt_count;
  if (callbacks->on_error.has_value()) {
    callbacks->stream->parent()->put_thunk([=](Engine& engine) {
      callbacks->on_error.value()(*callbacks->stream->parent(),
                                  *callbacks->stream,
                                  error_code,
                                  error_message,
                                  attempt_count);
    });
  }
  return context;
}

static void *py_dispatch_on_complete(void *context) {
  if (context == nullptr) {
    return context;
  }

  StreamCallbacks *callbacks = static_cast<StreamCallbacks *>(context);
  if (callbacks->on_complete.has_value()) {
    callbacks->stream->parent()->put_thunk([=](Engine& engine) {
      callbacks->on_complete.value()(engine, *callbacks->stream);
    });
  }
  return context;
}

static void *py_dispatch_on_cancel(void *context) {
  if (context == nullptr) {
    return context;
  }

  StreamCallbacks *callbacks = static_cast<StreamCallbacks *>(context);
  if (callbacks->on_cancel.has_value()) {
    callbacks->stream->parent()->put_thunk([=](Engine& engine) {
      callbacks->on_cancel.value()(engine, *callbacks->stream);
    });
  }
  return context;
}


StreamCallbacks::StreamCallbacks(std::shared_ptr<Stream> stream)
  : stream(stream),
    callbacks {
      .on_headers = &py_dispatch_on_headers,
      .on_data = &py_dispatch_on_data,
      .on_metadata = &py_dispatch_on_metadata,
      .on_trailers = &py_dispatch_on_trailers,
      .on_error = &py_dispatch_on_error,
      .on_complete = &py_dispatch_on_complete,
      .on_cancel = &py_dispatch_on_cancel,
      .context = this,
    } {}

StreamCallbacks& StreamCallbacks::set_on_headers(OnHeadersCallback on_headers) {
  this->on_headers = on_headers;
  return *this;
}

StreamCallbacks& StreamCallbacks::set_on_data(OnDataCallback on_data) {
  this->on_data = on_data;
  return *this;
}

StreamCallbacks& StreamCallbacks::set_on_metadata(OnHeadersLikeCallback on_metadata) {
  this->on_metadata = on_metadata;
  return *this;
}

StreamCallbacks& StreamCallbacks::set_on_trailers(OnHeadersLikeCallback on_trailers) {
  this->on_trailers = on_trailers;
  return *this;
}

StreamCallbacks& StreamCallbacks::set_on_error(OnErrorCallback on_error) {
  this->on_error = on_error;
  return *this;
}

StreamCallbacks& StreamCallbacks::set_on_complete(OnCompleteCallback on_complete) {
  this->on_complete = on_complete;
  return *this;
}

StreamCallbacks& StreamCallbacks::set_on_cancel(OnCompleteCallback on_cancel) {
  this->on_cancel = on_cancel;
  return *this;
}


Stream::Stream(std::shared_ptr<Engine> engine) {
  this->parent_ = engine;
  this->stream_ = init_stream(this->parent_->handle());
}

Stream::~Stream() {
  try {
    this->reset();
  } catch (const std::runtime_error&) {
    // NOTE: we can't throw in a destructor, so we just make a best effort to reset the stream. if
    // it fails then c'est la vie
  }
}

void Stream::start(const StreamCallbacks& callbacks) {
  auto status = start_stream(this->stream_, callbacks.callbacks);
  if (status == ENVOY_FAILURE) {
    throw std::runtime_error("failed to start stream");
  }
}

void Stream::send_headers(const Headers& headers, bool end_stream) {
  auto status = ::send_headers(this->stream_, headers.as_envoy_headers(), end_stream);
  if (status == ENVOY_FAILURE) {
    throw std::runtime_error("failed to send headers");
  }
}

void Stream::send_data(const Data& data, bool end_stream) {
  auto status = ::send_data(this->stream_, data.as_envoy_data(), end_stream);
  if (status == ENVOY_FAILURE) {
    throw std::runtime_error("failed to send data");
  }
}

void Stream::send_metadata(const Headers& metadata) {
  auto status = ::send_metadata(this->stream_, metadata.as_envoy_headers());
  if (status == ENVOY_FAILURE) {
    throw std::runtime_error("failed to send metadata");
  }
}

void Stream::send_trailers(const Headers& trailers) {
  auto status = ::send_trailers(this->stream_, trailers.as_envoy_headers());
  if (status == ENVOY_FAILURE) {
    throw std::runtime_error("failed to send trailers");
  }
}

void Stream::reset() {
  auto status = reset_stream(this->stream_);
  if (status == ENVOY_FAILURE) {
    throw std::runtime_error("failed to reset stream");
  }
}

void Stream::close() {
  this->send_data(Data(""), true);
}

std::shared_ptr<Engine> Stream::parent() const {
  return this->parent_;
}
