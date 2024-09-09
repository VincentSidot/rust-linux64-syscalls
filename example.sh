#!/bin/bash

SCRIPT_DIR="$(dirname $0)"

function show_usage {
    echo "Usage: $0 [example] [options] -- [arguments]"
    echo "Options:"
    echo "  --help    | -h               : Show help"
    echo "  --list    | -l               : List all examples"
    echo "  --release | -r               : Build in release mode"
}

function list_examples {
    echo "Available examples:"
    for example in $(ls $SCRIPT_DIR/examples); do
        echo "  $example"
    done
}

# Parse arguments
function parse_args {
    while [[ $# -gt 0 ]]; do
        case $1 in
            --help|-h)
                show_usage
                exit 0
                ;;
            --list|-l)
                list_examples
                exit 0
                ;;
            --release|-r)
                CARGO_ARGS="--release"
                ;;
            *)
                EXAMPLE=$1
                shift
                break
                ;;
        esac
        shift
    done
    ARGS=$@
}

parse_args $@
cd $SCRIPT_DIR/examples/$EXAMPLE
cargo run $CARGO_ARGS -- $ARGS