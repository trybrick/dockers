# HTTPS server
server {
    listen 80;
    listen 443 ssl;
    server_name custom.com www.custom.com;

    ssl_certificate      	conf.d/custom.com.crt;
    ssl_certificate_key  	conf.d/custom.com.key;

    include              	conf.d/00-dsweb.inc;
    include 				conf.d/00-dsweb-legacy.inc;
}
