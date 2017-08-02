#!/bin/sh

me=`basename "$0"`
echo "[i] DSWEB running: $me"

mkdir -p /var/cache/nginx/site

# purge cache on startup
rm -rf /var/cache/nginx/site/*

chown -R nginx:nginx /var/cache/nginx

# handle each env variables
sed -i "s/ DS_RESOLVER;/ $DS_RESOLVER;/g" /usr/local/openresty/nginx/conf/nginx.conf
sed -i "s/DS_TTL/$DS_TTL/g" /usr/local/openresty/nginx/conf/nginx.conf
sed -i "s/DS_CONTENT/$DS_CONTENT/g" /usr/local/openresty/nginx/conf.d/zz-dsweb.inc
sed -i "s/DS_API/$DS_API/g" /usr/local/openresty/nginx/conf.d/zz-dsweb.inc
sed -i "s/DS_SEO/$DS_SEO/g" /usr/local/openresty/nginx/conf.d/zz-dsweb.inc

if [[ "$AWS_PATH" != '' ]]; then
	# if sync to s3, download from s3 to local folder - only if folder does not exists
	# this mean that, can manually create this folder to prevent/lock s3 restore
	if [ ! -d /usr/local/openresty/s3/ ]; then
		echo "[i] DSWEB restoring from s3: $AWS_PATH"
		mkdir -p "/usr/local/openresty/s3/"

		aws s3 sync "s3://$AWS_PATH/"  "/usr/local/openresty/s3/" --exclude "*logs/*"
		# if s3 source is valid, then overwrite local
		if [ -f /usr/local/openresty/s3/conf/nginx.conf ]; then
			rsync -a "/usr/local/openresty/s3/" "/usr/local/openresty/nginx/"
		fi
	fi
fi
