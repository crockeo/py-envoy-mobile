from py_envoy_mobile.wrapper import c_types_wrapper

# build a envoy mobile engine
engine = c_types_wrapper.Engine()

# constructing a stream
stream = c_types_wrapper.Stream(engine)
