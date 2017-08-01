#!/bin/sh

# if aws env are entered, then we can do backup
if [[ "$AWS_PATH" != '' ]]; then
	source /etc/envvars

	# do backup from local to s3
	aws s3 sync "/usr/local/openresty/nginx/" "s3://$AWS_PATH/" --exclude "*logs/*"
fi
