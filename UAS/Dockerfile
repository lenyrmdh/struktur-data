FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y g++ \
                       libmysqlclient-dev \
                       mysql-client \
                       make \
                       cmake

WORKDIR /usr/src/app

COPY . .

CMD ["bash"]
