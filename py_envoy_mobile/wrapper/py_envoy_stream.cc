#include "py_envoy_stream.h"


StreamCallbacks::StreamCallbacks(std::shared_ptr<Engine> engine, std::shared_ptr<Stream> stream) {
  this->engine = engine;
  this->stream = stream;
}

StreamCallbacks& StreamCallbacks::set_on_headers(OnHeadersCallback on_headers) {
  return *this;
}

StreamCallbacks& StreamCallbacks::set_on_data(OnDataCallback on_data) {
  return *this;
}

StreamCallbacks& StreamCallbacks::set_on_metadata(OnHeadersCallback on_metadata) {
  return *this;
}

StreamCallbacks& StreamCallbacks::set_on_trailers(OnHeadersCallback on_trailers) {
  return *this;
}

StreamCallbacks& StreamCallbacks::set_on_error(OnErrorCallback on_error) {
  return *this;
}

StreamCallbacks& StreamCallbacks::set_on_complete(OnCompleteCallback on_complete) {
  return *this;
}

StreamCallbacks& StreamCallbacks::set_on_cancel(OnCompleteCallback on_cancel) {
  return *this;
}


Stream::Stream(std::shared_ptr<Engine> engine) {
  this->engine = engine;
  // TODO: init stream
}

Stream::~Stream() {
  // TODO: close stream
}

void Stream::start(std::unique_ptr<StreamCallbacks> callbacks) {
}

void Stream::send_headers(std::unique_ptr<Headers> headers, bool end_stream) {
}

void Stream::send_data(std::unique_ptr<Data> data, bool end_stream) {
}

void Stream::send_metadata(std::unique_ptr<Headers> metadata) {
}

void Stream::send_trailers(std::unique_ptr<Headers> trailers) {
}

void Stream::reset() {
}

void Stream::close() {
}
