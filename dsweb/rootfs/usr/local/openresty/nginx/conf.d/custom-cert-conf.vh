# HTTPS server
server {
  listen 443 ssl;
  listen [::]:443 ipv6only=on ssl;
  server_name custom.com www.custom.com;

  ssl_certificate /etc/nginx/conf.d/custom.com.crt;
  ssl_certificate_key /etc/nginx/conf.d/custom.com.key;

  include conf.d/zz-dsweb.inc;
  include conf.d/zz-dsweb-legacy.inc;
}
