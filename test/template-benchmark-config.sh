#
# ======================= BENCHMARK CONFIGURATION STEP ========================
#
# Before you can run benchmarks, you need to configure what is to be
# benchmarked.
#
# This configuration file is copied from a default template.
# Feel free to edit the defined functions to suit your needs, then exit.
# Once done, this file is saved under '.config/benchmark-config.sh'.
# You can edit it to change the benchmark setup.
#
# =============================================================================
#


# Invoked once before any benchmark case in its own special environment.
# Useful to install synctl on a remote server.
# Stop benchmark execution if return with a non-zero status.
#
install_synctl() {
    # local server='my-remote-server'
    # local path='my-remote=path'

    # rsync -aAHXzcv --exclude=.git --exclude=obj --exclude=bin     \
    # 	  --exclude=.depends --exclude=.depends.d --exclude=.config \
    # 	  ./ "$server":"$path"/

    # ssh "$server" make -C "$path" all

    return 0
}


# Initialize the synctl server.
# The precise location of the server is configuration specific.
#
synctl_init() {
    local path="$(mktemp -u --suffix='.d' 'synctl-server.XXXXXXXXXX')"

    echo "$path" > '.server-path'

    synctl init "$path"
}

# Cleanup the synctl server.
# Should left the file system just like it was before the previous invocation
# of synctl_cleanup().
#
synctl_cleanup() {
    local path="$(cat '.server-path')"

    rm -rf "$path" '.server-name'

    rm '.server-path'
}

# Push a local content.
# This function behaves like `synctl push` except for the following options:
#   --server
#   --command
# The precise location of the destination server is configuration specific.
#
synctl_push() {
    local path="$(cat '.server-path')"

    synctl push "$@" --server="file://$path"
    sync
}

# Pull a remote content.
# This function behaves like `synctl pull` except for the following options:
#   --server
#   --command
# The precise location of the source server is configuration specific.
#
synctl_pull() {
    local path="$(cat '.server-path')"

    synctl pull "$@" --server="file://$path"
    sync
}
