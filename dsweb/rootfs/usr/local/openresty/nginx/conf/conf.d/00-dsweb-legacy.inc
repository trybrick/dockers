# detect non-www host
set $not_www_host 0;

if ($host !~ "^www") {
	set $not_www_host 1;
}

# exclude brickinc domain from non-www host
if ($http_host ~ (.*)\.brickinc\.net ) {
	set $not_www_host 0;
}

if ($not_www_host = 1) {
	return 301 $scheme://www.$host$request_uri; 
}

# expresslane redirects
location ~* ^/default.aspx(.*) {
	return 301 $scheme://$host/;
}

location ~* ^/shop/week {
	return 301 $scheme://$host/circular;
}

location ~* ^/shop/coupon {
	return 301 $scheme://$host/coupons;
}

location ~* ^/shop/managelist {
	return 301 $scheme://$host/mylist;
}

location ~* ^/recipes/(.*) {
	return 301 $scheme://$host/recipecenter;
}
