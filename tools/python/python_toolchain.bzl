def _exec_python_tool_impl(ctx):
    python_exec_runtime = (
        ctx.toolchains["@rules_python//python:exec_tools_toolchain_type"].
            exec_tools.exec_interpreter[platform_common.ToolchainInfo].py3_runtime
    )

    output = ctx.actions.declare_file(ctx.label.name)
    ctx.actions.symlink(
        output = output,
        target_file = python_exec_runtime.interpreter,
        is_executable = True,
    )

    return [
        DefaultInfo(
            executable = output,
            files = depset([output]),
            runfiles = ctx.runfiles(transitive_files = python_exec_runtime.files),
        ),
    ]

exec_python_tool = rule(
    implementation = _exec_python_tool_impl,
    executable = True,
    toolchains = [
        "@rules_python//python:exec_tools_toolchain_type",
    ],
)
