FROM niiknow/openresty:0.1.0

MAINTAINER noogen <friends@niiknow.org>

ENV DS_API="clientapi.brickinc.net" \
    DS_SEO="ds-seo.brickinc.net" \
    DS_CONTENT="cdn.brickinc.net" \
    DS_RESOLVER="8.8.8.8 4.4.4.4;"

ARG LUA_RESTY_AUTO_SSL_VERSION="0.11.0"

RUN \

# Make info file about this build
    printf "Build of brickinc/dsweb, date: %s\n"  `date -u +"%Y-%m-%dT%H:%M:%SZ"` >> /etc/BUILDS/dsweb && \

# begin
    apk add --no-cache --virtual runtime \
    bash \
    coreutils \
    curl \
    diffutils \
    grep \
    openssl \
    nano \
    sed && \

# Even though we install full pkill (via the procps package, which we do for
# "-U" support in our tests), the /usr/bin version that symlinks BusyBox's
# more limited pkill version takes precedence. So manually remove this
# BusyBox symlink to the full pkill version is used.
    if [ -L /usr/bin/pkill ]; then rm /usr/bin/pkill; fi && \

    luarocks install lua-resty-http && \
    luarocks install lua-resty-auto-ssl $LUA_RESTY_AUTO_SSL_VERSION && \

    addgroup -S nginx && \
    adduser -D -S -h /var/cache/nginx -s /sbin/nologin -G nginx nginx && \
    mkdir -p /usr/local/openresty/nginx/ssl && \
    openssl req -new -newkey rsa:2048 -days 3650 -nodes -x509 \
      -subj '/CN=sni-support-required-for-valid-ssl' \
      -keyout /usr/local/openresty/nginx/ssl/resty-auto-ssl-fallback.key \
      -out /usr/local/openresty/nginx/ssl/resty-auto-ssl-fallback.crt && \

    chown -R nginx:nginx /usr/local/openresty/nginx/ssl && \
    chown -R nginx:nginx /usr/local/openresty/nginx/logs/ && \

# link for ease of refs in conf file
    ln -s /usr/local/openresty/nginx/ /etc/nginx && \
    ln -s /usr/local/openresty/nginx/logs/ /var/log/nginx

COPY rootfs/. /

EXPOSE 80 443

VOLUME ["/usr/local/openresty/nginx"]
