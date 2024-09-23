IMAGE_TAG=sdp-base:latest

xhost +local:docker

docker run                           \
    -it                              \
    --rm                             \
    -v $(pwd):/ws                    \
    -w /ws                           \
    -u $(id -u):$(id -g)             \
    --network host                   \
    -e DISPLAY=$DISPLAY              \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    $IMAGE_TAG                       \
    "$@"
