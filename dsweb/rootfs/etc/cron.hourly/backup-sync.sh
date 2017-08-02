#!/bin/sh

source /etc/envvars

# if aws env are entered, then we can do backup
if [[ "$AWS_PATH" != '' ]]; then
	# do backup from local to s3
	aws s3 sync "/usr/local/openresty/nginx/conf/" "s3://$AWS_PATH/" --exclude "*logs/*"
fi
