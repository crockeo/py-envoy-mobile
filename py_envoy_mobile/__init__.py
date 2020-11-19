from py_envoy_mobile.wrapper import c_types_wrapper

# build a envoy mobile engine
engine = c_types_wrapper.Engine()
stream = c_types_wrapper.Stream(engine)


def handle_callback(headers: c_types_wrapper.Headers, closed: bool):
    print("hello world")


# NOTE: none of these do anything yet
http_callbacks = c_types_wrapper.HttpCallbacks().set_on_headers(handle_callback)

stream.start(http_callbacks)

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

stream.send_headers(headers, False)

stream.close()
engine.terminate()
