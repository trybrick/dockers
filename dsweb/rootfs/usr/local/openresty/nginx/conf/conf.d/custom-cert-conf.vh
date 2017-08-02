# HTTPS server
server {
    listen 80;
	listen 443 ssl;
	server_name custom.com www.custom.com;

    error_page  404              /404.html;

    # redirect server error pages to the static page /50x.html
    #
    error_page  500 502 503 504  /50x.html;

	ssl_certificate /usr/local/openresty/nginx/conf/conf.d/custom.com.crt;
	ssl_certificate_key /usr/local/openresty/nginx/conf/conf.d/custom.com.key;

	include /usr/local/openresty/nginx/conf/conf.d/00-dsweb.inc;
	include /usr/local/openresty/nginx/conf/conf.d/00-dsweb-legacy.inc;
}
