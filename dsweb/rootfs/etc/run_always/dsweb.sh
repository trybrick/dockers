#!/bin/sh

me=`basename "$0"`
echo "[i] DSWEB running: $me"

mkdir -p /var/cache/nginx/site

# purge cache on startup
rm -rf /var/cache/nginx/site/*

chown -R nginx:nginx /var/cache/nginx

# restore backup folder
if [ ! -f /usr/local/openresty/nginx/conf/nginx.conf ]; then
	rsync -a /usr/local/openresty/nginx/conf-bak/ /usr/local/openresty/nginx/conf/

	# handle each env variables
	sed -i "s/ DS_RESOLVER;/ $DS_RESOLVER;/g" /usr/local/openresty/nginx/conf/nginx.conf
	sed -i "s/DS_TTL/$DS_TTL/g" /usr/local/openresty/nginx/conf/nginx.conf
	sed -i "s/DS_CONTENT/$DS_CONTENT/g" /usr/local/openresty/nginx/conf/conf.d/zz-dsweb.inc
	sed -i "s/DS_API/$DS_API/g" /usr/local/openresty/nginx/conf/conf.d/zz-dsweb.inc
	sed -i "s/DS_SEO/$DS_SEO/g" /usr/local/openresty/nginx/conf/conf.d/zz-dsweb.inc

	# if folder contain instruction to restore from s3 then
	if [ -f /usr/local/openresty/nginx/conf/s3.restore ]; then
		if [[ "$AWS_PATH" != '' ]]; then
			echo "[i] DSWEB restoring from s3: $AWS_PATH"
			aws s3 sync "s3://$AWS_PATH/" "/usr/local/openresty/conf/" --exclude "*logs/*"
			
			rm -f /usr/local/openresty/nginx/conf/s3.restore
		fi
	fi
fi