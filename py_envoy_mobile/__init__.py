from py_envoy_mobile.wrapper import c_types_wrapper

# build a envoy mobile engine
engine = c_types_wrapper.Engine()
stream = c_types_wrapper.Stream(engine)

# NOTE: none of these do anything yet
http_callbacks = (
    c_types_wrapper.HttpCallbacks()
    .set_on_headers()
    .set_on_data()
    .set_on_metadata()
    .set_on_trailers()
    .set_on_error()
    .set_on_complete()
    .set_on_cancel()
)

headers = (
    c_types_wrapper.Headers()
    .set_header(
        c_types_wrapper.Data("Content-Type"),
        c_types_wrapper.Data("application/json"),
    )
    .set_header(
        c_types_wrapper.Data("Accept"),
        c_types_wrapper.Data("application/json"),
    )
)

stream.close()
engine.terminate()
