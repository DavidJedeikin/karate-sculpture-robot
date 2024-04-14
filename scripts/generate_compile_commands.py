import os

Import("env")


# Callback
def after_build(source, target, env):
    env.Execute("pio run -t compiledb")
    env.Execute(
        "ln -sf .pio/build/adafruit_feather_nrf52832/compile_commands.json compile_commands.json"
    )


# include toolchain paths
env.Replace(COMPILATIONDB_INCLUDE_TOOLCHAIN=True)

# override compilation DB path
env.Replace(COMPILATIONDB_PATH=os.path.join("$BUILD_DIR", "compile_commands.json"))

env.AddPostAction("$BUILD_DIR/${PROGNAME}.elf", after_build)
