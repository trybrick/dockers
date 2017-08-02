#!/bin/sh

me=`basename "$0"`
echo "[i] DSWEB running: $me"

mkdir -p /var/cache/nginx/site

# purge cache on startup
rm -rf /var/cache/nginx/site/*

chown -R nginx:nginx /var/cache/nginx

# handle each env variables
cd /usr/local/openresty/nginx/conf
sed -i "s/ DS_RESOLVER;/ $DS_RESOLVER;/g" nginx.conf
sed -i "s/ DS_TTL;/ $DS_TTL;/g" nginx.conf
sed -i "s/DS_CONTENT/$DS_CONTENT/g" conf.d/zz-dsweb.inc
sed -i "s/DS_API/$DS_API/g" conf.d/zz-dsweb.inc
sed -i "s/DS_SEO/$DS_SEO/g" conf.d/zz-dsweb.inc

cd /usr/local/openresty/nginx/conf-bak
sed -i "s/ DS_RESOLVER;/ $DS_RESOLVER;/g" nginx.conf
sed -i "s/ DS_TTL;/ $DS_TTL;/g" nginx.conf
sed -i "s/DS_CONTENT/$DS_CONTENT/g" conf.d/zz-dsweb.inc
sed -i "s/DS_API/$DS_API/g" conf.d/zz-dsweb.inc
sed -i "s/DS_SEO/$DS_SEO/g" conf.d/zz-dsweb.inc

# restore backup folder
if [ ! -f /usr/local/openresty/nginx/conf/nginx.conf ]; then
	rsync -a /usr/local/openresty/nginx/conf-bak/ /usr/local/openresty/nginx/conf

	# if folder contain instruction to restore from s3 then
	if [ -f /usr/local/openresty/nginx/conf/s3.restore ]; then
		if [[ "$AWS_PATH" != '' ]]; then
			echo "[i] DSWEB restoring from s3: $AWS_PATH"
			aws s3 sync "s3://$AWS_PATH/" "/usr/local/openresty/conf/" --exclude "*logs/*"
			
			rm -f /usr/local/openresty/nginx/conf/s3.restore
		fi
	fi
fi