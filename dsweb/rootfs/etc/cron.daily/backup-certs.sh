#!/bin/sh

# sync to s3, log to tmp, cleanup tmp
backup_dest=$(sed 's/\/*$//' <<< "$1")
backup_dest=$(sed 's/^\/*//' <<< "$backup_dest")
backup_date=$(date +%Y%m)
backup_date2=$(expr 999999 - $(date +%Y%m))
backup_sort=$backup_date2"_"$backup_date

# this assume that the environment variables are available
aws s3 sync "/backup/" "s3://$backup_dest/$backup_sort/" --storage-class STANDARD_IA --exclude="*" --include "*.tar" >> /tmp/aws-backup."$(date +%F_%R)".log
