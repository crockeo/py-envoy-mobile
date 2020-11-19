from py_envoy_mobile.wrapper import c_types_wrapper


def on_engine_running():
    print("on_engine_running")


def on_exit():
    print("on_exit")


def main(config: str, debug_level: str):
    engine = c_types_wrapper.Engine()

    engine_callbacks = c_types_wrapper.EngineCallbacks().set_on_engine_running(on_engine_running).set_on_exit(on_exit)

    # TODO: figure out why this causes an exception from envoy not being able to lock a mutex
    engine.run(engine_callbacks, config, debug_level)
    # TODO: figure out why this halts the program...and stops the exception
    # engine.terminate()


if __name__ == "__main__":
    # TODO: instead of passing in a malformed template use config_template in the main_interface.h
    # params:
    #   - virtual_clusters
    #   - platform_filter_chain
    #   - dns_refresh_rate_seconds
    #   - dns_failure_refresh_rate_seconds_base
    #   - dns_failure_refresh_rate_seconds_max
    #   - connect_timeout_seconds
    #   - stats_domain
    #   - stats_flush_interval_seconds
    #   - app_id
    #   - app_version
    #   - device_os
    with open("py_envoy_mobile/envoy_config.yaml", "r") as f:
        config = f.read()
    main(config, "debug")
