# digital store web
- [x] legacy redirects
- [x] prerender redirects
- [x] auto ssl
- [x] custom ssl

## ability to quickly restart and restore from s3
- [x] initialization from s3
- [x] hourly backup/sync to s3

## environments
for app:
```
env DS_API="clientapi.brickinc.net"
env DS_SEO="ds-seo.brickinc.net"
env DS_CONTENT="cdn.brickinc.net"
env DS_RESOLVER="8.8.8.8 4.4.4.4;"
env DS_SITE_TTL=120
```

for backups:
```
env AWS_DEFAULT_REGION=us-west-2;
env AWS_ACCESS_KEY_ID=xxx;
env AWS_SECRET_ACCESS_KEY=yyy;
env AWS_PATH=bucket-name/folder;
```

