# HTTPS server
server {
	listen 443 ssl;
	listen [::]:443 ipv6only=on ssl;
	server_name custom.com www.custom.com;

	ssl_certificate /usr/local/openresty/nginx/conf.d/custom.com.crt;
	ssl_certificate_key /usr/local/openresty/nginx/conf.d/custom.com.key;

	include /usr/local/openresty/nginx/conf.d/zz-dsweb.inc;
	include /usr/local/openresty/nginx/conf.d/zz-dsweb-legacy.inc;
}
