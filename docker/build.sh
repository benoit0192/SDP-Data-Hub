DOCKER_DIR=$(dirname "$0")
IMAGE_TAG=sdp-base:latest
CONTEXT_DIR=$DOCKER_DIR/.

echo $CONTEXT_DIR
docker build                            \
    -f $DOCKER_DIR/sdp-base.Dockerfile  \
    -t $IMAGE_TAG                       \
    $CONTEXT_DIR
