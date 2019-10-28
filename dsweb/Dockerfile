FROM niiknow/openresty:0.2.1

LABEL maintainer="noogen <friends@niiknow.org>"

ENV DS_ENV="prd" \
    AWS_DEFAULT_REGION="us-west-2" \
    HOME="/root" \
    USER="root"
USER root
WORKDIR /root
ARG LUA_RESTY_AUTO_SSL_VERSION="0.13.1"
RUN cd /tmp \
    printf "Build of brickinc/dsweb, date: %s\n"  `date -u +"%Y-%m-%dT%H:%M:%SZ"` >> /etc/BUILDS/zz-dsweb && \
    apk add --no-cache \
      bash \
      coreutils \
      curl \
      diffutils \
      grep \
      openssl \
      nano \
      less \
      python3 \
      rsync \
      sed && \
    pip3 install --upgrade pip && \
    pip3 install awscli && \
    if [ -L /usr/bin/pkill ]; then rm /usr/bin/pkill; fi && \
    luarocks install lua-resty-http && \
    luarocks install lua-resty-auto-ssl $LUA_RESTY_AUTO_SSL_VERSION && \
    addgroup -S nginx && \
    adduser -D -S -h /var/cache/nginx -s /sbin/nologin -G nginx nginx && \
    mkdir -p /usr/local/openresty/nginx/conf/ssl && \
    openssl req -new -newkey rsa:2048 -days 3650 -nodes -x509 \
      -subj '/CN=sni-support-required-for-valid-ssl' \
      -keyout /usr/local/openresty/nginx/conf/ssl/resty-auto-ssl-fallback.key \
      -out /usr/local/openresty/nginx/conf/ssl/resty-auto-ssl-fallback.crt && \
    openssl dhparam -out /usr/local/openresty/nginx/conf/ssl/dhparam.pem 2048 && \
    chown -R nginx:nginx /usr/local/openresty/nginx/conf/ssl && \
    chown -R nginx:nginx /usr/local/openresty/nginx/logs/ && \
    ln -s /usr/local/openresty/nginx/logs/ /var/log/nginx && \
    rm -rf /var/cache/apk/*
COPY rootfs/. /
RUN rsync -a /usr/local/openresty/nginx/conf/ /usr/local/openresty/nginx/conf-bak
EXPOSE 80 443
VOLUME ["/usr/local/openresty/nginx/conf"]
