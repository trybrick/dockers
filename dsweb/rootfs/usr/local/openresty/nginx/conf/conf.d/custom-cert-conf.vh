# HTTPS server
server {
    listen 80;
    listen 443 ssl;
    server_name custom.com www.custom.com;

    ssl_certificate /usr/local/openresty/nginx/conf/conf.d/custom.com.crt;
    ssl_certificate_key /usr/local/openresty/nginx/conf/conf.d/custom.com.key;

    include /usr/local/openresty/nginx/conf/conf.d/00-dsweb.inc;
    include /usr/local/openresty/nginx/conf/conf.d/00-dsweb-legacy.inc;
}
